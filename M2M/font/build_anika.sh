#!/bin/bash

# Just to make sure - CLEAN-UP
rm Anikki-16x16-m2m.h*

# Convert text font into PSF
psftools-1.1.1/tools/txt2psf Anikki-16x16-m2m.txt Anikki-16x16-m2m.psf

# Build H-include file from PSF font
psftools-1.1.1/tools/psf2inc Anikki-16x16-m2m.psf Anikki-16x16-m2m.h

# Set a constant/variable
echo "#define FONT_SIZE (256*32)" >> Anikki-16x16-m2m.h.TEMP1
echo "const unsigned char FONT[FONT_SIZE] =" >> Anikki-16x16-m2m.h.TEMP1
echo "{" >> Anikki-16x16-m2m.h.TEMP1

# Remove the first 10 lines
tail -n +10 Anikki-16x16-m2m.h >> Anikki-16x16-m2m.h.TEMP1

# Remove the last two lines and delete from the last comma of the last line
cat Anikki-16x16-m2m.h.TEMP1 | head --lines=-2 | sed '$s/,\s*$//' > Anikki-16x16-m2m.h.TEMP2

# Close the array
echo "};" >> Anikki-16x16-m2m.h.TEMP2

# Move and delete files
mv Anikki-16x16-m2m.h.TEMP2 Anikki-16x16-m2m.h
rm Anikki-16x16-m2m.h.TEMP1

# Compile the C-source
gcc Anikki-16x16-m2m.c -o Anikki-16x16-m2m

# Run the executable and get the ".rom" file
./Anikki-16x16-m2m
