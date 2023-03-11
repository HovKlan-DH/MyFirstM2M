/*
    psftools: Manipulate console fonts in the .PSF format
    Copyright (C) 2002, 2007, 2021  John Elliott <seasip.webmaster@gmail.com>

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
#include <stdlib.h>
#include <string.h>
#include "psfio.h"

static psf_errno_t resize_dir(PSF_UNICODE_DIR *f, int srccount, int dstcount)
{
	int nc;
	psf_unicode_dirent **de, *ude, *ude2;

/* v1.0.1: Resize the unicode directory to match the font. Firstly, 
 * allocate a new set of pointers with the right number of characters. */
	de = malloc(dstcount * sizeof(psf_unicode_dirent *));
	if (!de)
	{
		return PSF_E_NOMEM;
	}
/* Secondly, copy across the pointers we're going to keep */
	for (nc = 0; nc < dstcount; nc++)
	{
		if (nc < srccount)
		{
			de[nc] = f->psf_dirents_used[nc];
		}
		else
		{
			de[nc] = NULL;
		}
	}
/* Thirdly, free the ones we aren't.  */
	for (; nc < srccount; nc++)
	{
		ude = f->psf_dirents_used[nc]; 	
		while (ude)
		{
/* Move entry to the free list, and adjust counts appropriately */
			ude2 = ude->psfu_next;
			ude->psfu_next = f->psf_dirents_free;
			f->psf_dirents_free = ude;
			--f->psf_dirents_nused;
			++f->psf_dirents_nfree;
			ude = ude2;
		}
	}
/* Finally, swap in the new list */
	free(f->psf_dirents_used);
	f->psf_dirents_used = de;
	return PSF_E_OK;
}

/* Force a PSF file to be v1 */
psf_errno_t psf_force_v1(PSF_FILE *f)
{
	psf_byte *ndata;
	int nc, srccount, dstcount, y, wb;
	psf_errno_t err;

	if (f->psf_magic == PSF1_MAGIC) return PSF_E_OK;
	if (!f->psf_data || !f->psf_height) return PSF_E_EMPTY;

	dstcount = 256;
	if (f->psf_length > 256) dstcount = 512;

	srccount = f->psf_length;
	/* dstcount = number of chars in PSF1 file */
	/* srccount = number of chars in PSF2 file */

	ndata = malloc(dstcount * f->psf_height);
	if (!ndata) return PSF_E_NOMEM;
	memset(ndata, 0, dstcount * f->psf_height);

	if (psf_is_unicode(f))	/* It's unicode */
	{
		err = resize_dir(&f->psf_dir, srccount, dstcount);
		if (err) 
		{
			free(ndata);
			return err;
		}
	}
/* Now force the characters to be 1 byte wide */
	wb = (f->psf_width + 7) / 8;
/* Copy such character bitmaps as fit */
	for (nc = 0; nc < srccount; nc++)
	{
		if (nc >= dstcount) break;
		for (y = 0; y < f->psf_height; y++)
		{
			ndata[f->psf_height * nc + y] =
				f->psf_data[f->psf_charlen * nc + y * wb]; 
		}
	}
/* Zero any extra character bitmaps */
	for (; nc < dstcount; nc++)
	{
		for (y = 0; y < f->psf_height; y++)
		{
			ndata[f->psf_height * nc + y] = 0;
		}
	}
/* Swap in the new font data, and tamper with the header */
	free(f->psf_data);
	f->psf_data = ndata;
	f->psf_magic = PSF1_MAGIC;
	f->psf_version = 0;
	f->psf_hdrlen = 4;
	f->psf_length = dstcount;
	f->psf_charlen = f->psf_height;
	f->psf_width = 8;

	return PSF_E_OK;
}



psf_errno_t psf_force_v2(PSF_FILE *f)
{
	f->psf_magic = PSF_MAGIC;
	f->psf_version = 0;
	f->psf_hdrlen = 32;
	return PSF_E_OK;
}


/* Force a VFONT file to be v1 */
psf_errno_t vfont_force_v1(VFONT *f)
{
	psf_byte *ndata;
	int nc, srccount, dstcount;
	psf_errno_t err;
	size_t required;
	size_t offset;

	if (f->vf_magic == VFONT_MAGIC_V1) return PSF_E_OK;
	if (!f->vf_bitmap || !f->vf_dispatch) return PSF_E_EMPTY;

	dstcount = 256;
	srccount = f->vf_length;
	/* dstcount = number of chars in VFONT1 file */
	/* srccount = number of chars in VFONT2 file */
	required = 0;
	for (nc = 0; nc < srccount; nc++)
	{
		if (nc >= dstcount) break;

		required += f->vf_dispatch[nc].vfd_nbytes;
	}

	ndata = malloc(required);
	if (!ndata) return PSF_E_NOMEM;
	memset(ndata, 0, required);

	if (vfont_is_unicode(f))	/* It's unicode */
	{
		err = resize_dir(&f->vf_dir, srccount, dstcount);
		if (err) 
		{
			free(ndata);
			return err;
		}
	}

	/* Ensure there are exactly 256 characters */
	if (srccount != dstcount)
	{
		VFONT_DISPATCH *ndisp = realloc(f->vf_dispatch, 
					  dstcount * sizeof(VFONT_DISPATCH));
		if (!ndisp) 
		{
			free(ndata);
			return PSF_E_NOMEM;
		}
		f->vf_length = dstcount;
	}

/* Copy the character bitmaps */
	offset = 0;
	for (nc = 0; nc < srccount; nc++)
	{
		if (nc >= dstcount) break;
		memcpy(ndata + offset, 
			f->vf_bitmap + f->vf_dispatch[nc].vfd_addr,
			f->vf_dispatch[nc].vfd_nbytes);
		f->vf_dispatch[nc].vfd_addr = offset;
		offset += f->vf_dispatch[nc].vfd_nbytes;
	}
/* Zero any extra characters */
	for (; nc < dstcount; nc++)
	{
		memset(&f->vf_dispatch[nc], 0, sizeof(VFONT_DISPATCH)); 
	}
/* Swap in the new font data, and tamper with the header */
	free(f->vf_bitmap);
	f->vf_bitmap = ndata;
	f->vf_magic = VFONT_MAGIC_V1;
	f->vf_version = 0;
	f->vf_hdrlen = 10;
	f->vf_length = dstcount;

	return PSF_E_OK;
}




psf_errno_t vfont_force_v2(VFONT *f)
{
	f->vf_magic = VFONT_MAGIC_V2;
	f->vf_version = 0;
	f->vf_hdrlen = 32;
	return PSF_E_OK;
}

