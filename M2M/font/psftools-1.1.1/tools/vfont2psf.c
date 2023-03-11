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

/* Convert a Berkeley vfont to PSF. */


static int psf1 = 0;		/* Output file format */
static int psf2 = 0;		/* Output file format */
static int align = 0;		/* Align all character bitmaps? */

static char helpbuf[2048];

/* Program name */
char *cnv_progname = "VFONT2PSF";

/* ddash = 1 if option started with a double-dash; else 0 */
/* Return NULL if OK, else error string */
char *cnv_set_option(int ddash, char *variable, char *value)
{
	if (!stricmp(variable, "psf1"))  { psf1 = 1; return NULL; }
	if (!stricmp(variable, "psf2"))  { psf2 = 1; return NULL; }
	if (!stricmp(variable, "align")) { align = 1; return NULL; }
	if (strlen(variable) > 2000) variable[2000] = 0;
	sprintf(helpbuf, "Unknown option: %s\n", variable);
	return helpbuf;
}


/* Return help string */
char *cnv_help(void)
{
	sprintf(helpbuf, "Syntax: %s vfont psffile { options }\n\n", 
			cnv_progname);
	strcat (helpbuf, "Options: \n"
		    "        --psf1          - Output in PSF version 1 format.\n"
		    "        --psf2          - Output in PSF version 2 format (default).\n"
		    "        --align         - Align characters based on their basepoints.\n"
		);
	return helpbuf;
}


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



/* Do the conversion */
char *cnv_execute(FILE *infile, FILE *outfile)
{	
	int rv, x0, y0;
	PSF_FILE psf;
	VFONT vfont;
	psf_dword cell_w, cell_h, char_w, char_h, ch, basex = 0, basey = 0;

	/* Load the vfont */
	vfont_new(&vfont);
	rv = vfont_read(&vfont, infile);	
	if (rv != PSF_E_OK) return psf_error_string(rv);

	/* Work out how big each character needs to be */
	if (align) 
	{
		vfont_get_max_cellsize(&vfont, &cell_w, &cell_h,
			&basex, &basey);
/*		fprintf(stderr, "Max cell size = %ld x %ld base=(%ld,%ld)\n", 
			cell_w, cell_h, basex, basey); */
	}
	else	   
	{
		vfont_get_max_charsize(&vfont, &cell_w, &cell_h);
/*
		fprintf(stderr, "Max char size = %ld x %ld\n", cell_w, cell_h);	
*/
	}

	if (psf1)
	{
		if (cell_w > 8)
		{
			cell_w = 8;
			fprintf(stderr, "Characters with width exceeding 8 "
					"will be truncated.\n");
		}
	}

	psf_file_new(&psf);
	rv = psf_file_create(&psf, cell_w, cell_h, vfont.vf_length, 
		vfont_is_unicode(&vfont));

	if (rv == PSF_E_OK)
	{
		for (ch = 0; ch < vfont.vf_length; ch++)
		{
			if (!vfont.vf_dispatch[ch].vfd_nbytes)
				continue;	/* Missing character */

			vfont_get_charsize(&vfont, ch, &char_w, &char_h);
			if (align)
			{
/* (x0, y0) is the basepoint. Find the top left corner of the character */
				x0 = basex - vfont.vf_dispatch[ch].vfd_left;
				y0 = basey - vfont.vf_dispatch[ch].vfd_up;
				copydots(&vfont, &psf, ch, 0, 0, char_w, char_h,
					x0, y0); 
			}
			else
			{
				copydots(&vfont, &psf, ch, 0, 0, char_w, char_h,
					0, 0); 
			}
			
			if (psf_is_unicode(&psf))
			{
				psf_unicode_dirent *ude;

				for (ude = vfont.vf_dir.psf_dirents_used[ch]; 
					ude != NULL; ude = ude->psfu_next)
				{
					psf_unicode_add(&psf, ch, ude->psfu_token);
				}
			}
		}	
		if (psf2) psf_force_v2(&psf);
		if (psf1) psf_force_v1(&psf);
		rv = psf_file_write(&psf, outfile);
	}
	psf_file_delete(&psf);
	vfont_delete(&vfont);
	
	if (rv != PSF_E_OK)
	{
		return psf_error_string(rv);
	}
	return NULL;
}

