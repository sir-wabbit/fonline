# FOnline open source

This is an open source implementation of the Fallout Online released under GPL. 
It was originally developed by Oleg Mareskin, Fodev and TeamX teams and released under a GPL license. This is a cleaned up (ver 9000 warnings and errors fixed) and slightly modified version of the original.

As far as I know, the original repository is no longer accessible, but I still have a copy of it, if anyone is interested.

# Roadmap (from the original repository)

Here `-` means that the task is not started, `~` means that the task is half-finished, and `✓` means that the task is essentially done.

<table>
   <thead>
      <tr>
         <td><strong>#</strong></td>
         <td><strong>Task</strong></td>
         <td><strong>State</strong></td>
      </tr>
   </thead>
   <tbody>
      <tr><td>1</td><td>Transitions from one map to another</td><td>~</td></tr>
      <tr><td>2</td><td>Map of Den with all NPCs</td><td>~</td></tr>
      <tr><td>3</td><td>NPC dialogues</td><td>✓</td></tr>
      <tr><td>4</td><td>Killable mobs on map</td><td>&ndash;</td></tr>
      <tr><td>5</td><td>Combat system</td><td>~</td></tr>
      <tr><td>6</td><td>In-game objects</td><td>~</td></tr>
      <tr><td>7</td><td>Quests</td><td>&ndash;</td></tr>
      <tr><td>8</td><td>Character creation</td><td>✓</td></tr>
      <tr><td>9</td><td>Music and sounds</td><td>✓</td></tr>
      <tr><td>10</td><td>Opening/Closing doors</td><td>&ndash;</td></tr>
   </tbody>
</table>

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
