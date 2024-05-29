# Purpose
Ports a user-defined set of identifiers from Minecraft: Java Edition to Bedrock Edition. Features:
 - User-defined language pairs, allowing cross-language porting (You could port Pirate Speak on Java to en_US on Bedrock)
 - User-defined identifier pairs, for identifiers which do not match on both platforms
 - Expandable identifier sets, used when porting colors, potion effects, or other multi-identifier definitions
 - Built-in color set expansion for porting all 16 variants of colored definitions at once
 - Simple prefix and suffix support for inserting or appending a string on each definition
 - Sort override for forcing ported definitions into a specific text block in the file

# Usage
WARNING: Since Minecraft's language files are UTF-8 encoded, they do not play nice with Windows PowerShell by default. Run the following command to configure PowerShell correctly:

    $PSDefaultParameterValues['Out-File:Encoding'] = 'utf8'

Without this command, output files will be created using ANSI instead of UTF-8, breaking languages with non-ASCII characters.

This program MUST be run from the command line or PowerShell with 3-5 arguments! (Prefix, suffix, and sort override are optional.)

    ./translation_porter.exe <s/m/c/n> <java_identifier> <bedrock_identifier> <prefix> <suffix> <sort_override>

### Example:

    ./translation_porter.exe m effect.minecraft.VAR effect.VAR SECTIONc NULL effect.badOmen
    
# Blowup Prevention
In the past, a bug was found which caused infinite file writing unless the program was manually ended. There are no known blowup bugs in the current version, but if you find one, **report it immediately** and include the file and arguments that caused the blowup.

**To manually stop the program while in a command line, press `Ctrl+C`.** You can tell the program is stuck if the output freezes without outputting "Task completed successfully!" for longer than a second.

# Input and Output Files
Input files are placed in the `lang_java` folder next to the executable. Use [minecraft-asset-extractor](https://github.com/shivamCode0/minecraft-asset-extractor/tree/main) to extract the latest language files. Place the language files (such as `en_us.json`) directly into `lang_java`.

Output files are placed in the `lang_bedrock` folder next to the executable. Newly ported definitions are automatically sorted into any existing file (such as `en_US.lang`) in the `lang_bedrock` folder in a roughly alphabetical order (see **Sort Override** below).

The program can be run multiple times with different parameters on the same set of Bedrock files. The `lang_bedrock` files are modified, not overwritten!

This program does not prevent porting duplicate identifiers, but it will print a message when a duplicate identifier is found in the existing Bedrock file.

# Identifier Expansion
`s/m/c/n` means Single, Multiple, Classic Color, or New Color. Single ports a single definition from Java to Bedrock. Multiple ports a set of definitions defined in `multiple.txt`. Both Color options port a set of definitions specific to the 16 colors of many Minecraft blocks.

Classic Color is for blocks which use `lightBlue` and `silver` on Bedrock. New Color is for blocks which use `light_blue` and `light_gray` instead. This differs between each block, so you must check the vanilla LANG files to know which one to use.

All config files are in the same folder as the executable. To use the Multiple setting, the `multiple.txt` file must be configured by the user. The `colors.txt` and `languages.txt` files work as examples for how to write `multiple.txt`. The Java expansions are in the left column, while the Bedrock expansions are in the right column, separated by one space.

The identifiers may take any form without spaces. They are the left side of each line in the JSON and LANG files. The identifier does not change between languages (it is also known as the "translation key").

To define which part of the identifier should be replaced by the multiple or color expansion, write `VAR`.

### Example:

In the Color modes, the identifier `block.minecraft.banner.bricks.VAR` expands into 16 identifiers such as `block.minecraft.banner.bricks.black`, `block.minecraft.banner.bricks.blue`, `block.minecraft.banner.bricks.brown`, and all the rest.

In Multiple mode, the identifier `effect.minecraft.VAR` expands into a user-defined set of identifiers such as `effect.minecraft.infested`, `effect.minecraft.oozing`, and  `effect.minecraft.weaving`.

Expansions support the two-column format for some identifiers which differ between Java and Bedrock, such as `light_gray` on Java becoming `silver` on Bedrock.

# Prefix and Suffix
A prefix and suffix may be added during a port. These must be the same for all ported identifiers in one execution.

To easily write the section code (§) symbol for color formatting in a prefix or suffix, write SECTION.

To indicate no prefix or suffix, write `NULL`.

### Example:

`SECTIONe` outputs as `§e`. [See a full list of formatting codes here.](https://minecraft.wiki/w/Formatting_codes#Color_codes)

# Sort Override
The default sort attempts to match the first word of the Bedrock identifier with another identifier that exists in the file. Once a match is found, an alphabetical sort begins. If a match is not found, the definitions are inserted at the end of the file. This helps existing files with non-alphabetical groupings stay nice.

A sort override forces the alphabetical sort to begin at the specified identifier. It will stop at a double newline if the alphabetical condition is not met to avoid breaking up definitions, as it is assumed the new definitions should be sorted in the same text block as the sort override.

The start of the identifier must be included, but the full identifier does not need to be. The example below clarifies this.

### Example:

Sort override `item.ban` will start the alphabetical sort on `item.banner.globe` or `item.banana`, whichever is first in the file.

To indicate no sort override, you may write `NULL` or not include the argument.

# License
Uses the nlohmann-json header for JSON.

The class is licensed under the MIT License:

Copyright © 2013-2022 Niels Lohmann

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
