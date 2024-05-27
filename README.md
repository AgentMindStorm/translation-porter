Ports a user-defined set of identifiers from Java Edition to Bedrock Edition. Features:
 - User-defined language pairs, allowing cross-language porting (You could port Pirate Speak on Java to en_US on Bedrock)
 - User-defined identifier pairs, for identifiers which do not match on both platforms
 - Expandable identifier sets, used when porting colors, potion effects, or other multi-identifier definitions
 - Built-in color set expansion for porting all 16 variants of colored definitions at once
 - Simple prefix and suffix support for inserting or appending a string on each definition
 
WARNING: Since Minecraft's language files are UTF-8 encoded, they do not play nice with Windows PowerShell by default. Run the following command to configure PowerShell correctly:

$PSDefaultParameterValues['Out-File:Encoding'] = 'utf8'

Without this command, output files will be created using ANSI instead of UTF-8, breaking languages with non-ASCII characters.

This program MUST be run from the command line or PowerShell with 3-5 arguments! (Prefix and suffix are optional.)

Usage:

./translation_porter.exe <s/m/c> <java_identifier> <bedrock_identifier> <prefix> <suffix>

Example:

./translation_porter.exe m effect.minecraft.VAR effect.VAR SECTIONc NULL

s/m/c means Single, Multiple, or Color. Single ports a single definition from Java to Bedrock. Multiple ports a set of definitions defined in multiple.txt. Color ports a set of definitions specific to the 16 colors of many Minecraft blocks.

To use the Multiple setting, the "multiple.txt" file must be configured by the user. The "colors.txt" and "languages.txt" files use the same format. The Java expansions are in the left column, while the Bedrock expansions are in the right column, separated by one space.

The identifiers may take any form without spaces. They are the left side of the language definition in the files, which does not change between languages (the "key"). To define which part of the identifier should be replaced by the multiple or color expansion, write VAR.

Example:

In Color mode, the identifier "block.minecraft.banner.bricks.VAR" expands into 16 identifiers such as "block.minecraft.banner.bricks.black", "block.minecraft.banner.bricks.blue", "block.minecraft.banner.bricks.brown", and all the rest.

In Multiple mode, the identifier "effect.minecraft.VAR" expands into a user-defined set of identifiers such as "effect.minecraft.infested", "effect.minecraft.oozing", and  "effect.minecraft.weaving".

Expansions support the two-column format for some identifiers which differ between Java and Bedrock, such as "light_gray" on Java becoming "silver" on Bedrock.

A prefix and suffix may be added during a port. These must be the same for all ported identifiers in one execution.

To easily write the section code (§) symbol for color formatting in a prefix or suffix, write SECTION.

Example:

"SECTIONe" outputs as "§e". See a full list of formatting codes here: https://minecraft.wiki/w/Formatting_codes

Input files are located in the lang_java folder. The files provided with this program are from 24w20a. Use https://github.com/shivamCode0/minecraft-asset-extractor/tree/main to extract the latest language files.

Output files are located in the lang_bedrock folder. Newly ported definitions are automatically appended to any existing file in the lang_bedrock folder, allowing easy future expansion.