/*
    psftools: Manipulate console fonts in the .PSF format
    Copyright (C) 2005, 2020  John Elliott <seasip.webmaster@gmail.com>

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
#include <stdio.h>
#include "cnvshell.h"
#include "psflib.h"

/* Convert a PSF file to a text format */

char *cnv_progname = "VFONT2TXT";


char *cnv_set_option(int ddash, char *variable, char *value)
    {
    return "This program does not have options.";
    }

char *cnv_help(void)
    {
    static char buf[150];

    sprintf(buf, "Syntax: %s vfont_file txt_file\n", cnv_progname);
    return buf;
    }


int render_character(FILE *fpout, VFONT *vfont, int ch)
{
	VFONT_DISPATCH *disp = &vfont->vf_dispatch[ch];
	int char_w = disp->vfd_left + disp->vfd_right;
	int char_h = disp->vfd_up + disp->vfd_down;
	int y, x, rv;
	psf_byte pix;
	char *draw;

	fprintf(fpout, "Left: %d\n", disp->vfd_left);
	fprintf(fpout, "Right: %d\n", disp->vfd_right);
	fprintf(fpout, "Up: %d\n", disp->vfd_up);
	fprintf(fpout, "Down: %d\n", disp->vfd_down);
	fprintf(fpout, "Width: %d\n", disp->vfd_width);
		
	for (y = 0; y < char_h; y++)
	{
		if (y == 0) fprintf(fpout, "Bitmap: ");
		else	    fprintf(fpout, "        ");

		for (x = 0; x < char_w; x++)
		{
			draw = "-#";

			if (y == disp->vfd_up && x == disp->vfd_left) 
			{
				draw = "O@";
			}
			else if (y == disp->vfd_up)
			{
				draw = "=#";
			}
			else if (x == disp->vfd_left)
			{
				draw = "|#";
			}

			rv = vfont_get_pixel(vfont, ch, x, y, &pix);
			if (rv) break;
			if (pix) fputc(draw[1], fpout);
			else	 fputc(draw[0], fpout);
		}
		if (rv) break;
		if (y < char_h - 1) fprintf(fpout, " \\");
		fputc('\n', fpout);
	}
	return 0;
}



char *cnv_execute(FILE *fpin, FILE *fpout)
{	
	int rv;
	VFONT vfont;
	psf_dword ch;
	char *ucs;

	vfont_new(&vfont);
	rv = vfont_read(&vfont, fpin);	
	if (rv != PSF_E_OK) return psf_error_string(rv);

	fprintf(fpout, "%%VFONT2\n");
	fprintf(fpout, "Version: %ld\n", vfont.vf_version);
	fprintf(fpout, "Flags: %ld\n", vfont.vf_flags);
	fprintf(fpout, "BitmapLength: %ld\n", vfont.vf_size);
	fprintf(fpout, "Length: %ld\n", vfont.vf_length);
	fprintf(fpout, "MaxWidth: %ld\n", vfont.vf_maxx);
	fprintf(fpout, "MaxHeight: %ld\n", vfont.vf_maxy);
	fprintf(fpout, "Extend: %ld\n", vfont.vf_xtend);
	rv = 0;
	for (ch = 0; ch < vfont.vf_length; ch++)
	{
		fprintf(fpout, "%%\n// Character %ld\n", ch);
		if (!vfont.vf_dispatch[ch].vfd_nbytes)
		{
			continue;
		}
		render_character(fpout, &vfont, ch);
		if (vfont_is_unicode(&vfont))
		{
			rv = psf_unicode_to_string(vfont.vf_dir.psf_dirents_used[ch], 
					&ucs);
			if (!rv)
			{
				fprintf(fpout, "Unicode: %s\n", ucs);
				free(ucs);
			}

		}
		if (rv) break;
	}

	vfont_delete(&vfont);
	return 0;
}

