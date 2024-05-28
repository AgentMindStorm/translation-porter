#include <string>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <cstdio>
#include <vector>
#include "json.hpp"
using json = nlohmann::json;

int readConfigFile(const std::string& input_filename,std::vector<std::string> &java_vector,std::vector<std::string> &bedrock_vector);
int expandIdentifier(std::string base_identifier,std::vector<std::string> &identifier_list,const std::vector<std::string> &expansion_list);

int main(int argc, char* argv[]) {

    //UTF-8 Setup

    // Set console code page to UTF-8 so console known how to interpret string data
    SetConsoleOutputCP(CP_UTF8);

    // Enable buffering to prevent VS from chopping up UTF-8 byte sequences
    setvbuf(stdout, nullptr, _IOFBF, 1000);

//////
//////  INPUT PROCESSING
//////

    //Validate input arguments
    //<program.exe> <s/m/c/n> <java_identifier> <bedrock_identifier> <prefix> <suffix>
    // s/m/c/n == single/multiple/classic_color/new_color
    // VAR converts to the multiple definitions or the colors in identifiers
    // SECTION converts to § in prefix and suffix
    // NULL indicates to exclude that argument, for the optional args
    if (argc < 4) {
        std::cerr << "Usage: (required) ./translation_translator <s/m/c/n> <base_java_identifier> <base_bedrock_identifier> (optional) <prefix> <suffix> <sort_override>" << std::endl;
        return -1;
    }
    std::cout << "User-Defined Configuration: " << std::endl;

    //Expansion type
    char expansion_type = *argv[1];
    if (expansion_type == 's') {
        std::cout << "Expansion Type: Single" << std::endl;
    }
    else if (expansion_type == 'm') {
        std::cout << "Expansion Type: Multiple; reads from \"multiple.txt\"." << std::endl;
    }
    else if (expansion_type == 'c') {
        std::cout << "Expansion Type: 16 Colors (Original Names); reads from \"colors_classic.txt\"." << std::endl;
    }
    else if (expansion_type == 'n') {
        std::cout << "Expansion Type: 16 Colors (New Names); reads from \"colors_new.txt\"." << std::endl;
    }
    else {
        std::cerr << "Expansion type not recognized. Valid characters are s (single), m (multiple), c (classic color), or n (new color)." << std::endl;
        return -10;
    }

    //Java identifier
    std::string base_java_identifier = argv[2]; //Used to construct real identifiers
    std::cout << "Base Java Identifier: " << base_java_identifier << std::endl;

    //Bedrock identifier
    std::string base_bedrock_identifier = argv[3]; //Used to construct real identifiers
    std::cout << "Base Bedrock Identifier: " << base_bedrock_identifier << std::endl;

    //Prefix
    std::string prefix;
    if (argc > 4) {
        prefix = argv[4];
        size_t section_location = prefix.find("SECTION");
        if (prefix == "NULL") {
            prefix = "";
        }
        else if (section_location != std::string::npos) {
            prefix.erase(section_location,7);
            prefix.insert(section_location,"§");
        }
    }
    if (prefix.empty()) {
        std::cout << "No Prefix" << std::endl;
    }
    else {
        std::cout << "Prefix (Replaced SECTION with §): " << prefix << std::endl;
    }

    //Suffix
    std::string suffix;
    if (argc > 5) {
        suffix = argv[5];
        size_t section_location = suffix.find("SECTION");
        if (suffix == "NULL") {
            suffix = "";
        }
        else if (section_location != std::string::npos) {
            suffix.erase(section_location,7);
            suffix.insert(section_location,"§");
        }
    }
    if (suffix.empty()) {
        std::cout << "No Suffix" << std::endl;
    }
    else {
        std::cout << "Suffix (Replaced SECTION with §): " << suffix << std::endl;
    }

    //Sort override (start of alphabetical comparisons)
    std::string sort_override;
    bool sort_override_enabled = false;
    if (argc > 6) {
        sort_override = argv[6];
        if (sort_override == "NULL") {
            sort_override = "";
        }
    }
    if (sort_override.empty()) {
        std::cout << "No Sort Override" << std::endl << std::endl;
    }
    else {
        sort_override_enabled = true;
        std::cout << "Start Sort At: " << sort_override << std::endl << std::endl;
    }

//////
//////  CONFIG FILES
//////

    //Language list
    std::vector<std::string> java_language, bedrock_language;
    if (readConfigFile("languages.txt",java_language,bedrock_language) != 0) {
        std::cerr << "Aborted. Failed to read languages.txt." << std::endl;
        return -2;
    }

    //Identifier expansion
    std::vector<std::string> java_identifier, bedrock_identifier;
    if (expansion_type == 's') { //Single line, no expansion
        java_identifier.push_back(base_java_identifier); //Use same system for single identifier to be as easy as possible
        bedrock_identifier.push_back(base_bedrock_identifier);
    }
    //Multiple or colors expansions
    else {
        std::vector<std::string> java_multiple, bedrock_multiple;
        std::string config_file = "multiple.txt";
        if (expansion_type == 'c') {
            config_file = "colors_classic.txt";
        }
        else if (expansion_type == 'n'){
            config_file = "colors_new.txt";
        }
        if (readConfigFile(config_file, java_multiple, bedrock_multiple) != 0) {
            std::cerr << "Aborted. Failed to read " << config_file << "." << std::endl;
            return -3;
        }
        if (expandIdentifier(base_java_identifier, java_identifier, java_multiple) != 0) {
            std::cerr << "Failed to expand list of Java identifiers." << std::endl;
            return -4;
        }
        if (expandIdentifier(base_bedrock_identifier, bedrock_identifier, bedrock_multiple) != 0) {
            std::cerr << "Failed to expand list of Bedrock identifiers." << std::endl;
            return -5;
        }
    }

//////
//////  READ JAVA DEFINITIONS
//////

    //Iterate through every language
    for (int i = 0; i < bedrock_language.size(); i++) {

        std::ifstream fin("lang_java/" + java_language.at(i) + ".json", std::ios_base::app);
        if (fin.fail()) {
            std::cerr << "Failed to open lang_java/" + java_language.at(i) + ".json." << std::endl;
            return -6;
        }
        std::cout << "Opened " + java_language.at(i) + ".json..." << std::endl;

        //Read all necessary defs, store in vector
        json java_json = json::parse(fin);
        std::string current_string;
        std::vector<std::string> definition;
        bool identifier_found = false; //For error checking

        for (const auto & j : java_identifier) {
            if (java_json.contains(j)) {
                current_string = java_json[j];
                current_string.insert(0,prefix);
                current_string += suffix;
                definition.push_back(current_string);
                identifier_found = true;
            }
            else {
                std::cerr << "Failed to find " << j << " in " << java_language.at(i) << ".json." << std::endl;
                definition.emplace_back("NULL");
            }
        }
        fin.close();
        std::cout << "Finished reading " + java_language.at(i) + ".json..." << std::endl;

        //Check any identifier was found in file
        if (!identifier_found) {
            std::cerr << "Aborted; no matching definitions found." << std::endl;
            return -7;
        }

        //Check for size error
        if (java_identifier.size() != definition.size()) {
            std::cerr << "Aborted; Java identifiers and definitions desynchronized." << std::endl;
            return -8;
        }

//////
//////  WRITE BEDROCK DEFINITIONS
//////

        //Append output definitions
        std::ifstream copyin("lang_bedrock/" + bedrock_language.at(i) + ".lang");
        if (copyin.fail()) {
            std::cerr << "Failed to open lang_bedrock/" + bedrock_language.at(i) + ".lang." << std::endl;
            return -9;
        }
        std::cout << "Opened existing " + bedrock_language.at(i) + ".lang..." << std::endl;

        //Find correct insertion location in lang file
        std::string clean_identifier = base_bedrock_identifier;
        std::string alphabetical_identifier, after_first_period;
        if (!sort_override_enabled) {
            if (expansion_type != 's') { //Remove VAR part of string
                clean_identifier = base_bedrock_identifier.substr(0, base_bedrock_identifier.find("VAR"));
            }
            size_t first_period = clean_identifier.find('.');
            if (first_period != std::string::npos) {
                alphabetical_identifier = clean_identifier.substr(0,first_period);
            }
            else {
                alphabetical_identifier = clean_identifier;
            }
        }
        else {
            alphabetical_identifier = sort_override;
        }

        //Unmodified text handling
        std::string current_line, trimmed_line;
        std::vector<std::string> pre_insertion_file, post_insertion_file;
        bool insert_end = false;

        //Find similar structure to base identifier
        while (!copyin.eof() && alphabetical_identifier != trimmed_line) {
            getline(copyin, current_line, '\n');
            pre_insertion_file.push_back(current_line); //Build file
            if (current_line.size() > alphabetical_identifier.size()) { //Trim current line to match alphabetical identifier
                trimmed_line = current_line.substr(0, alphabetical_identifier.size());
            }
            else {
                trimmed_line = current_line;
            }
            //Check for duplicate definitions
            for (const auto & j : bedrock_identifier) {
                if (j == current_line.substr(0,current_line.find('='))) {
                    std::cerr << "Duplicate definition found in " << bedrock_language.at(i) << " on line " << pre_insertion_file.size() << "." << std::endl;
                }
            }
        }
        //Start alphabetical search through current location
        while (!copyin.eof() && current_line < bedrock_identifier.at(0) && !(sort_override_enabled && current_line.empty())) { //In sort override, an empty line stops alpha search
            getline(copyin, current_line, '\n');
            pre_insertion_file.push_back(current_line); //Continue to build file

            //Check for duplicate definitions
            for (const auto & j : bedrock_identifier) {
                if (j == current_line.substr(0,current_line.find('='))) {
                    std::cerr << "Duplicate definition found in " << bedrock_language.at(i) << " on line " << pre_insertion_file.size() << "." << std::endl;
                }
            }
        }
        //Sometimes, no similar definition is found
        if (copyin.eof()) {
            if (!sort_override_enabled) {
                std::cerr << "No similar identifiers found; inserting new lines at end of file." << std::endl;
            }
            else {
                std::cerr << "No existing identifiers found matching sort override \"" << sort_override << "\"; inserting new lines at end of file." << std::endl;
            }
            insert_end = true;
        }
        //Otherwise, save the rest of the file
        else {
            while (!copyin.eof()) { //Finish reading file
                getline(copyin, current_line, '\n');
                post_insertion_file.push_back(current_line);

                //Check for duplicate definitions
                for (const auto & j : bedrock_identifier) {
                    if (j == current_line.substr(0,current_line.find('='))) {
                        std::cerr << "Duplicate definition found in " << bedrock_language.at(i) << " on line " << pre_insertion_file.size() + post_insertion_file.size() << "." << std::endl;
                    }
                }
            }
        }
        copyin.close();
        //Remove trailing whitespace
        if (!post_insertion_file.empty()) {
            while (post_insertion_file.back().empty() && !post_insertion_file.empty()) {
                post_insertion_file.pop_back();
            }
        }

        //Insert output definitions
        std::ofstream fout("lang_bedrock/" + bedrock_language.at(i) + ".lang");
        if (fout.fail()) {
            std::cerr << "Failed to open lang_bedrock/" + bedrock_language.at(i) + ".lang." << std::endl;
            return -10;
        }
        std::cout << "Overwrote " + bedrock_language.at(i) + ".lang..." << std::endl;

        //Print pre-insertion until lines the program is adding
        for (int j = 0; j < pre_insertion_file.size() - 1; j++) {
            fout << pre_insertion_file.at(j) << std::endl;
        }

        //Add extra space for end insertion
        if (insert_end) {
            fout << pre_insertion_file.at(pre_insertion_file.size() - 1) << std::endl;
        }

        //Print new lines to Bedrock output file
        for (int k = 0; k < definition.size(); k++) {
            if (definition.at(k) != "NULL") {
                fout << bedrock_identifier.at(k) << "=" << definition.at(k);
                if (bedrock_language.at(i) != "en_US") {
                    fout << "\t#" << std::endl;
                }
                else {
                    fout << std::endl;
                }
            }
            else {
                std::cerr << "Skipped missing definition " << java_identifier.at(k) << " -> " << bedrock_identifier.at(k) << " (Java->Bedrock identifier)." << std::endl;
            }
        }

        //Print remaining lines
        if (!insert_end) {
            fout << pre_insertion_file.at(pre_insertion_file.size() - 1) << std::endl;
            if (!post_insertion_file.empty()) {
                for (const auto &l: post_insertion_file) {
                    fout << l << std::endl;
                }
            }
        }

        fout.close();
        std::cout << "Finished writing to " + bedrock_language.at(i) + ".lang..." << std::endl << std::endl;
    }

    std::cout << "Task completed successfully!" << std::endl;
}

//Builds list identifiers using expansion words and stores in identifier_list
int expandIdentifier(std::string base_identifier,std::vector<std::string> &identifier_list,const std::vector<std::string> &expansion_list) {

    size_t insertion_point = base_identifier.find("VAR");
    if (insertion_point == std::string::npos) {
        std::cerr << "Text \"VAR\" not found in input string." << std::endl;
        return -1;
    }
    base_identifier.erase(insertion_point,3);

    std::string current_identifier;
    for (const auto & i : expansion_list) { //Iterate through full expansion list
        current_identifier = base_identifier;
        current_identifier.insert(insertion_point,i); //Insert expansion into string
        identifier_list.push_back(current_identifier); //Add to vector
    }

    std::cout << "Created the following identifiers:" << std::endl;
    for (const auto & i : identifier_list) {
        std::cout << i << std::endl;
    }
    std::cout << std::endl;

    return 0;
}

//Reads config file (in two-column Java/Bedrock format) into provided vectors
int readConfigFile(const std::string& input_filename,std::vector<std::string> &java_vector,std::vector<std::string> &bedrock_vector) {

    //Open config file
    std::ifstream fin(input_filename);
    if (fin.fail()) {
        std::cerr << "Failed to open " << input_filename << "." << std::endl;
        return -1;
    }
    std::cout << "Opened " << input_filename << "..." << std::endl;

    std::string input;
    int cycle = 0; //For alternating Bedrock and Java
    while (!fin.eof()) {
        fin >> input;
        if (cycle % 2 == 0) {
            java_vector.push_back(input);
        }
        else {
            bedrock_vector.push_back(input); //Push every other word to Bedrock
        }
        cycle++;
    }
    fin.close();
    std::cout << "Closed " << input_filename << "..." << std::endl << std::endl;

    //Error checker
    if (java_vector.size() != bedrock_vector.size()) {
        std::cerr << "Input file " << input_filename << "does not have an equal number of Java and Bedrock definitions, or file was empty." << std::endl;
        return -2;
    }

    return 0;
}