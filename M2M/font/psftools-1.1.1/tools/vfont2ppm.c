/*
    psftools: Manipulate console fonts in the .PSF format
    Copyright (C) 2003, 2021  John Elliott <seasip.webmaster@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include "cnvshell.h"
#include "psflib.h"

/* Convert a Berkeley vfont to PPM. */


static int align = 0;		/* Align all character bitmaps? */
static int across = 32;

static char helpbuf[2048];

/* Program name */
char *cnv_progname = "VFONT2PPM";

/* ddash = 1 if option started with a double-dash; else 0 */
/* Return NULL if OK, else error string */
char *cnv_set_option(int ddash, char *variable, char *value)
{
	if (!stricmp(variable, "align")) { align = 1; return NULL; }
	if (!stricmp(variable, "across")) 
	{
		if (!value || !atoi(value))
		{
                        return "The 'across' value may not be zero or omitted.";
		}
		across = atoi(value);
		return NULL;
	}
	if (strlen(variable) > 2000) variable[2000] = 0;
	sprintf(helpbuf, "Unknown option: %s\n", variable);
	return helpbuf;
}


/* Return help string */
char *cnv_help(void)
{
	sprintf(helpbuf, "Syntax: %s vfont ppmfile { options }\n\n", 
			cnv_progname);
	strcat (helpbuf, "Options: \n"
		    "        --across=val    - Set the number of characters across the bitmap.\n"
		    "        --align         - Align characters based on their basepoints.\n"
		);
	return helpbuf;
}


#if 0
/* A naive pixel-by-pixel blit */
static void copydots(VFONT *src, PSF_FILE *dst, psf_dword ch,
			psf_dword xs, psf_dword ys, psf_dword w, psf_dword h,
			psf_dword xd, psf_dword yd)
{
	psf_dword x, y;
	psf_byte pixel;

	for (y = 0; y < h; y++)
		for (x = 0; x < w; x++)
	{
		if (vfont_get_pixel(src, ch, xs + x, ys + y, &pixel))
			pixel = 1;
		psf_set_pixel(dst, ch, xd + x, yd + y, pixel);
	}
}
#endif



/* Do the conversion */
char *cnv_execute(FILE *infile, FILE *outfile)
{	
	int rv;
	VFONT vfont;
	VFONT_DISPATCH *disp;
	psf_dword cell_w, cell_h, char_w, char_h, ch, basex = 0, basey = 0;
	psf_dword ppm_w, ppm_h, ppm_x, ppm_y, block_w, block_h;
	int rows;
	int pixel, cy, cx, chary, charx, dotx, doty, boxx, boxy, boxw, boxh;
	psf_byte dot;

	/* Load the vfont */
	vfont_new(&vfont);
	rv = vfont_read(&vfont, infile);	
	if (rv != PSF_E_OK) return psf_error_string(rv);

	/* Work out how big each character needs to be */
	if (align) 
	{
		vfont_get_max_cellsize(&vfont, &cell_w, &cell_h,
			&basex, &basey);
	}
	else	   
	{
		vfont_get_max_charsize(&vfont, &cell_w, &cell_h);
	}
	rows = (vfont.vf_length + (across - 1)) / across;

	block_w = 3 + cell_w;
	block_h = 3 + cell_h;
	ppm_w = (across * block_w + 1);
	ppm_h = (rows   * block_h + 1);

	fprintf(outfile, "P6 %lu %lu 255\n", ppm_w, ppm_h);

	for (ppm_y = 0; ppm_y < ppm_h; ppm_y++)
	{
		for (ppm_x = 0; ppm_x < ppm_w; ppm_x++)
		{
			cy = ppm_y % block_h;
			cx = ppm_x % block_w;
			pixel = 0; /* Background */
			ch = (ppm_y / block_h) * across + (ppm_x / block_w);

			/* Character out of range? */
			if (ch >= vfont.vf_length) goto draw; 

			disp = &vfont.vf_dispatch[ch];

			/* If no character, leave as background */
			if (!disp->vfd_nbytes) goto draw;

			/* charx, chary = position to draw character bitmap 
			   within cell */
			if (align)
			{
				charx = basex - disp->vfd_left;
				chary = basey - disp->vfd_up;
			}
			else
			{
				charx = chary = 0;
			}
			char_w = disp->vfd_left + disp->vfd_right;
			char_h = disp->vfd_up   + disp->vfd_down;
			boxx = charx;
			boxy = chary;
			boxw = char_w;
			boxh = char_h;

			if (disp->vfd_left < 0) 
			{
				if (align) boxx += disp->vfd_left;
				else	   charx -= disp->vfd_left;
				boxw -= disp->vfd_left;
			} 
			if (disp->vfd_right < 0) 
			{
				boxw -= disp->vfd_right;
			}
			if (disp->vfd_up < 0) 
			{
				if (align) boxy += disp->vfd_up;
				else	   chary -= disp->vfd_up;
				boxh -= disp->vfd_up;
			}
			if (disp->vfd_down < 0) 
			{
				boxh -= disp->vfd_down;
			}
			dotx = cx - (charx + 2);
			doty = cy - (chary + 2);
 	
			/* Background */	
			if (cx < boxx + 1 || 
			    cy < boxy + 1 || 
			    cx > boxx + boxw + 2 || 
		  	    cy > boxy + boxh + 2) 
			{
				pixel = 0;	
			}
			/* Border */
			else if (cx == boxx + 1 || 
			    cy == boxy + 1 || 
			    cx == boxx + boxw + 2 || 
		  	    cy == boxy + boxh + 2) 
			{
				pixel = 1;	
			}
			else if (dotx >= 0 && dotx < char_w &&
				 doty >= 0 && doty < char_h)
			{
 				/* Character pixel */
				if (!vfont_get_pixel(&vfont, ch, dotx, doty, &dot))
					pixel = dot ? 3 : 2;
			}
			/* Must be within padding area */
			else
			{
				pixel = 4;	
			}
			if (dotx == disp->vfd_left && doty == disp->vfd_up)
			{
				pixel += 10;
			} 
			else if (dotx == disp->vfd_left || doty == disp->vfd_up)
			{
				pixel += 20;
			}
draw:
			switch (pixel)
			{
				/* Normal */
				case 0: fprintf(outfile, "%c%c%c", 
						0xAA, 0xAA, 0xAA); break;
				case 1: fprintf(outfile, "%c%c%c", 
						0x40, 0x40, 0x40); break;
				case 2: fprintf(outfile, "%c%c%c", 
						0xFF, 0xFF, 0xFF); break;
				case 3: fprintf(outfile, "%c%c%c", 
						0x00, 0x00, 0x00); break;
				case 4: fprintf(outfile, "%c%c%c", 
						0xFF, 0xFF, 0x80); break;
				/* Basepoint */
				case 10: fprintf(outfile, "%c%c%c", 
						0xAA, 0x00, 0x00); break;
				case 11: fprintf(outfile, "%c%c%c", 
						0x40, 0x00, 0x00); break;
				case 12: fprintf(outfile, "%c%c%c", 
						0xFF, 0x00, 0x00); break;
				case 13: fprintf(outfile, "%c%c%c", 
						0xC0, 0x00, 0x00); break;
				case 14: fprintf(outfile, "%c%c%c", 
						0xFF, 0x80, 0x80); break;
				/* Baseline */
				case 20: fprintf(outfile, "%c%c%c", 
						0xAA, 0xAA, 0xAA); break;
				case 21: fprintf(outfile, "%c%c%c", 
						0x40, 0x40, 0x40); break;
				case 22: fprintf(outfile, "%c%c%c", 
						0x00, 0xFF, 0x00); break;
				case 23: fprintf(outfile, "%c%c%c", 
						0x00, 0x40, 0x00); break;
				case 24: fprintf(outfile, "%c%c%c", 
						0x80, 0xFF, 0x80); break;
/* Magenta for any colour combination I've missed */
				default: fprintf(outfile, "%c%c%c", 
						0xFF, 0x00, 0xFF); break;

			}
		}
	}

	vfont_delete(&vfont);
	return NULL;
}

