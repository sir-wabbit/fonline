# FOnline open source

This is an open source implementation of the Fallout Online. It was originally developed by Oleg Mareskin, Fodev and TeamX teams and released under GPL. This is a cleaned up and slightly refactored version of the original.

As far as I know, the original repository is no longer accessible, but I still have a copy of it, if anyone is interested.

# Roadmap (from the original repository)

Here `-` means that the task is not started, `~` means that the task is half-finished, and `✓` means that the task is essentially done.


|\# | Task                              | State     |
|:-:|-----------------------------------|:---------:|
|1  |Transitions from one map to another|**~**      |
|2  |Map of Den with all NPCs           |**~**      |
|3  |NPC dialogues                      |✓          |
|4  |Killable mobs on map               |**&ndash;**|
|5  |Combat system                      |**~**      |
|6  |In-game objects                    |**~**      |
|7  |Quests                             |**&ndash;**|
|8  |Character creation                 |✓          |
|9  |Music and sounds                   |✓          |
|10 |Opening/Closing doors              |**&ndash;**|

# Changelog

* **Fixed** over 9000 compilation warnings and errors.
* **Extracted** LZSS from client into a separate module.
* **Extracted** ACMDecompressor from client into a separate module.
* **Extracted** FileManager from client **and** server into a separate module.
* **Extracted** logging methods into a FOnlineCommon module and made them UTF-8 friendly.
* **Added** a new IniFile library and replaced Windows-specific `.ini` related calls.
* **Achieved** server compilation on Linux.

# How to build on Linux:
   
    mkdir build
    cmake ..
    make

# How to build on Windows:
Use cmake-gui or premake.
