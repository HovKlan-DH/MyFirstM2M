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

/* Convert a PSF to a Berkeley vfont. */


static int version = 1;		/* Output file format */
static int big_endian = 0;	/* When writing v1, use big-endian? */

static char helpbuf[2048];

static int basepoint_x = 0, basepoint_y = 0, have_basepoint = 0;

/* Program name */
char *cnv_progname = "PSF2VFONT";

/* ddash = 1 if option started with a double-dash; else 0 */
/* Return NULL if OK, else error string */
char *cnv_set_option(int ddash, char *variable, char *value)
{
	if (!stricmp(variable, "v1")) { version = 1; return NULL; }
	if (!stricmp(variable, "v2")) { version = 2; return NULL; }
	if (!stricmp(variable, "be") || !stricmp(variable, "big-endian")) 
		{ big_endian = 1; return NULL; }
	if (!stricmp(variable, "le") || !stricmp(variable, "little-endian")) 
		{ big_endian = 0; return NULL; }
	if (!stricmp(variable, "basepoint"))
	{
		if (value == NULL || sscanf(value, "%d,%d", 
			&basepoint_x, &basepoint_y) != 2)
		{
			return "The -basepoint option must be followed by two numbers.";
		}
		have_basepoint = 1;	
		return NULL;
	}

	if (strlen(variable) > 2000) variable[2000] = 0;
	sprintf(helpbuf, "Unknown option: %s\n", variable);
	return helpbuf;
}


/* Return help string */
char *cnv_help(void)
{
	sprintf(helpbuf, "Syntax: %s psffile vfont { options }\n\n", 
			cnv_progname);
	strcat (helpbuf, "Options: \n"
		    "        --v1            - Output in vfont version 1 format (default).\n"
		    "        --v2            - Output in proposed vfont version 2 format.\n"
		    "        --big-endian    - Output a big-endian file (v1 only).\n"
		    "        --basepoint=x,y - Set basepoint of characters (default is the\n"
                    "                          bottom-left corner of capital A)");
	return helpbuf;
}



/* Do the conversion */
char *cnv_execute(FILE *infile, FILE *outfile)
{	
	int rv, x;
	PSF_FILE psf;
	VFONT vfont;
	psf_dword bitmap_size;

	if (big_endian && version == 2)
	{
		return "A version 2 vfont cannot be big-endian";
	}
	psf_file_new(&psf);
	rv = psf_file_read(&psf, infile);	
	if (rv != PSF_E_OK) return psf_error_string(rv);

	bitmap_size = psf.psf_charlen * psf.psf_length;
	
	vfont_new(&vfont);
	rv = vfont_create(&vfont, psf.psf_length, bitmap_size,
		(psf_is_unicode(&psf) && 2 == version));
	if (version == 2)
	{
		vfont.vf_magic = VFONT_MAGIC_V2;
		vfont.vf_be    = 0;
	}
	else
	{
		vfont.vf_magic = VFONT_MAGIC_V1;
		vfont.vf_be    = big_endian;
	}
	if (rv == PSF_E_OK)
	{
/* Find the basepoint. Start by assuming it's in the bottom left-hand corner */
		signed short up = psf.psf_height;
		signed short down = 0;
		signed short left = 0;
		signed short right = psf.psf_width;

		if (have_basepoint)
		{
			left  = basepoint_x;
			right = psf.psf_width - basepoint_x;
			up    = basepoint_y;
			down  = psf.psf_height - basepoint_y;
		}
		else if (psf.psf_length >= 'A')
		{
			int y, x;
			psf_byte pixel;		
	
			for (y = psf.psf_height - 1; y >= 0; y--)		
			{
				for (x = 0; x < psf.psf_width; x++)
				{
					if (!psf_get_pixel(&psf, 'A', x, y, &pixel) && pixel != 0)
					{	/* x, y locate bottom corner */
						up = y; 
						down = psf.psf_height - y;							left = x;
						right = psf.psf_width - x;
						goto found;
					}
				}
			}
			found: ;	/* Break out of double loop */
		}
		if (!have_basepoint)
		{
			fprintf(stderr, "Basepoint set to (%d,%d)\n", left, up);
		}

		for (x = 0; x < psf.psf_length; x++)
		{
			vfont.vf_dispatch[x].vfd_addr   = psf.psf_charlen * x;
			vfont.vf_dispatch[x].vfd_nbytes = psf.psf_charlen;
			vfont.vf_dispatch[x].vfd_up = up;
			vfont.vf_dispatch[x].vfd_down = down;
			vfont.vf_dispatch[x].vfd_left = left;
			vfont.vf_dispatch[x].vfd_right = right;
			vfont.vf_dispatch[x].vfd_width = psf.psf_width;
			if (version == 2 && psf_is_unicode(&psf))
			{
				psf_unicode_dirent *ude;

				for (ude = psf.psf_dir.psf_dirents_used[x]; 
					ude != NULL; ude = ude->psfu_next)
				{
					vfont_unicode_add(&vfont, x, ude->psfu_token);
				}
			}
		}	
		memcpy(vfont.vf_bitmap, psf.psf_data, 
			psf.psf_length * psf.psf_charlen);
		rv = vfont_write(&vfont, outfile);
	}
	psf_file_delete(&psf);
	vfont_delete(&vfont);
	
	if (rv != PSF_E_OK)
	{
		return psf_error_string(rv);
	}
	return NULL;
}

