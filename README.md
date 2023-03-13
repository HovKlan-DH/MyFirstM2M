MiSTer2MEGA65 Demo Core
=======================

This project is taken directly from
https://github.com/sy2002/MiSTer2MEGA65/, and my purpose here is solely to familiarize myself with how the menu structure works.
All guidance is very well-documented within the parent project and it is
nicely structured, so please look in there for the details.

The MiSTer2MEGA65 project is done by **sy2002** and **MJoergen** and I have only tweaked their great work for the menu part (visible when pressing the HELP button) - I have no idea what the rest is doing ;-)

I have listed below all my changes and how I "run it".

Font changes:
* Manually modified but it is heavily based on "Anikki-16x16"
* Changed a few characters to correspond to real ASCII characters
* Realigned all characters for better readability
* Changed some borders and added a new horizontal "snake" line

I have done changes only to these files:
* CORE\vhdl\config.vhd
* CORE\vhdl\globals.vhd
* M2M\font\Anikki-16x16-m2m.txt
* M2M\rom\sysdef.asm

Files and what they do:
* **config.vhd**
  - Menu list items (headlines, options and separators)
  - Menu size in characters is defined in OPTM_DX (excluding frame)
  - If changing menu list items, then update OPTM_ITEMS and OPTM_GROUPS, and make
sure OPTM_SIZE corresponds with the actual number of lines in menu (all
lines, but excluding frame)
* **globals.vhd**
  - This is where the font file (ROM) is set
* **Anikki-16x16-m2m.txt**
  - Font file containing all 256 characters, defined pixel-by-pixel
  - The text file should be converted to a PSF file and then to a C-include file (.h)
* **sysdef.asm**
  - Menu items and colors
  - Characters used for frame
  - Keyboard delay for repeat is M2M$TYPEMATIC_DLY

Steps I do to build a new bitstream:
* Modify \build_new_bitstream.sh if you want to change core name from
"MYFirstM2M V2" (this is visible in the MEGA65 core selector)
* Modify \CORE\vhdl\config.vhd for menu
* Modify the font file \M2M\font\Anikki-16x16-m2m.txt
* Run \M2M\font\build_anika.sh to rebuild font
* Run \build_new_bitstream.sh to build new bitstream - this will result in a **COR file** I can use on the MEGA65 to flash a new slot (1-7)
  - In Vivado then click the "Generate Bitstream"
  - When it states "Bitstream generation has already completed and is up-to-date. Re-run anyway?" then "Cancel" and go to the "synth_1" in the "Design Runs" tab and "Reset Runs". Then click the "Generate Bitstream" again

Requirements:
* Vivado 2022.2 installed - I have my setup in "Subsystem for Linux" in Windows

This is how the "Help" looks like:
![Help](http://howto.dk/MiSTer2MEGA65-Color-Schema/help.jpg)

And this is the menu:
![Help](http://howto.dk/MiSTer2MEGA65-Color-Schema/menu.jpg)

MiSTer2MEGA65-Color-Schema
==========================

I have made a web color schema picker, and it is available here, http://howto.dk/MiSTer2MEGA65-Color-Schema/
