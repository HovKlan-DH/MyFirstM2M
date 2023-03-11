/*
    psftools: Manipulate console fonts in the .PSF format
    Copyright (C) 2002, 2007, 2021  John Elliott

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
#include "psfio.h"


/* Initialise a structure as empty */
void vfont_new(VFONT *f)
{
	f->vf_magic   = VFONT_MAGIC_V2;
	f->vf_version = 0;
	f->vf_hdrlen  = 32;
	f->vf_flags   = 0;
	f->vf_length  = 0;	
	f->vf_size    = 0;
	f->vf_maxx    = 0;
	f->vf_maxy    = 0;
	f->vf_xtend   = 0;
	f->vf_dispatch = NULL;
	f->vf_bitmap   = NULL;
	
	psf_dir_init(&f->vf_dir);
}


/* Initialise the structure for a new font */

psf_errno_t vfont_create(VFONT *f, psf_dword nchars, 
				psf_dword bitmap_size, psf_byte unicode)
{
	vfont_delete(f);

	f->vf_dispatch = calloc(nchars, sizeof(VFONT_DISPATCH));
	if (!f->vf_dispatch) return PSF_E_NOMEM;
	
	memset(f->vf_dispatch, 0, nchars * sizeof(VFONT_DISPATCH));
	if (bitmap_size)
	{
		f->vf_bitmap = malloc(bitmap_size);
		if (!f->vf_bitmap)
		{
			free(f->vf_dispatch);
			f->vf_dispatch = NULL;
			return PSF_E_NOMEM;
		}
		memset(f->vf_bitmap, 0, bitmap_size);
	}
	f->vf_length = nchars;
	f->vf_size = bitmap_size;
	if (unicode) 
	{
		psf_errno_t err = vfont_create_unicode(f);
		if (err) return err;
		f->vf_flags |= 1;
	}
	return PSF_E_OK;
}

psf_errno_t vfont_realloc(VFONT *f, psf_dword bitmap_size)
{
	unsigned char *bits = realloc(f->vf_bitmap, bitmap_size);

	if (!bits) return PSF_E_NOMEM;

	f->vf_bitmap = bits;
	f->vf_size = bitmap_size;
	return PSF_E_OK;
}

static int vfont_read_ucs_dir(PSFIO *io, int cchar)
{
	psf_byte  utf8;
	psf_dword ucs32;
	int err;

	if (psfio_get_byte(io, &utf8)) return PSF_E_UNICODE;
	while (utf8 != 0xFF)
	{
		if (utf8 == 0xFE) ucs32 = 0xFFFE;
		else if (psfio_get_utf8(io, utf8, &ucs32)) return PSF_E_UNICODE;
 		err = vfont_unicode_add(io->vfont, cchar, ucs32);
	/* Drop banned characters silently */
		if (err == PSF_E_BANNED) err = PSF_E_OK;
   		if (err != PSF_E_OK) return err;
		if (psfio_get_byte(io, &utf8)) return PSF_E_UNICODE;
	}	
	return PSF_E_OK;
}


static psf_word bswap(psf_word w)
{
	return ((w >> 8) & 0xFF) | ((w & 0xFF) << 8);
}

/* Load a VFONT from memory or disc*/
psf_errno_t vfont_io_read(PSFIO *io)
{
        psf_byte *ndata;
	VFONT_DISPATCH *disp;
        int err, cchar;
	int vfontver = 0;
	int big_endian = 0;
	psf_dword magic;
	psf_dword version, headersize, flags, nchars;
	psf_dword bitmap_size, max_w, max_h, xtend = 0;
	psf_dword n;

	/* Check the magic number. */
	err = psfio_get_dword(io, &magic); if (err) return err;

	if      (magic == VFONT_MAGIC_V2)
	{
		vfontver = 2;
	}
	else if ((magic & 0xFFFF) == VFONT_MAGIC_V1)
	{
		vfontver = 1;
		bitmap_size = (magic >> 16);
		magic &= 0xFFFF;
	}
	else if ((bswap(magic) & 0xFFFF) == VFONT_MAGIC_V1)
	{
		vfontver = 1;
		big_endian = 1;
		bitmap_size = bswap(magic >> 16);
		magic &= 0xFFFF;
	}
        else return PSF_E_NOTPSF;

	if (vfontver == 1)
	{
		psf_word w; 

		/* Max.w */
		if (psfio_get_word(io, &w)) return PSF_E_NOTPSF;
		if (big_endian) w = bswap(w);
		max_w = w;
		/* Max.h */
		if (psfio_get_word(io, &w)) return PSF_E_NOTPSF;
		if (big_endian) w = bswap(w);
		max_h = w;
		/* Xtend */
		if (psfio_get_word(io, &w)) return PSF_E_NOTPSF;
		if (big_endian) w = bswap(w);
		xtend = w;
		headersize = 10;
		flags = 0;
		nchars = 256;
		version = 0;
	}
	else
	{
		if (psfio_get_dword(io, &version))     return PSF_E_NOTPSF;
		if (version > 0) return PSF_E_NOTIMPL;
		if (psfio_get_dword(io, &headersize))  return PSF_E_NOTPSF;
		if (psfio_get_dword(io, &flags))       return PSF_E_NOTPSF;
		if (psfio_get_dword(io, &nchars))      return PSF_E_NOTPSF;
		if (psfio_get_dword(io, &bitmap_size)) return PSF_E_NOTPSF;
		if (psfio_get_dword(io, &max_h))       return PSF_E_NOTPSF;
		if (psfio_get_dword(io, &max_w))       return PSF_E_NOTPSF;
	}
	/* Header read. Allocate dispatch structures and bitmap */
	disp = calloc(nchars, sizeof(VFONT_DISPATCH));
	if (!disp) return PSF_E_NOMEM;
	
	memset(disp, 0, nchars * sizeof(VFONT_DISPATCH));
	ndata = malloc(bitmap_size);
        if (!ndata) 
	{
		free(disp);
		return PSF_E_NOMEM;
	}
        vfont_delete(io->vfont);

	/* OK. Font data allocated. Set up the headers */

	io->vfont->vf_magic    = magic;
	io->vfont->vf_version  = version;
	io->vfont->vf_hdrlen   = headersize;
	io->vfont->vf_flags    = flags;
	io->vfont->vf_be       = big_endian;
	io->vfont->vf_length   = nchars;
	io->vfont->vf_size     = bitmap_size;
	io->vfont->vf_maxx     = max_w;
	io->vfont->vf_maxy     = max_h;
	io->vfont->vf_xtend    = xtend;
        io->vfont->vf_dispatch = disp;
        io->vfont->vf_bitmap   = ndata;

	/* Skip any extra header bits */
	while (headersize > 32)
	{
		err = psfio_get_byte(io, ndata);
		--headersize;
	}
	/* Load the dispatch structures */
	for (n = 0; n < nchars; n++)
	{
		if (vfontver == 1)
		{
			psf_word w; 
			signed char ch;

			/* Read in: Bitmap offset */
			if (psfio_get_word(io, &w)) return PSF_E_NOTPSF;
			if (big_endian) w = bswap(w);
			disp[n].vfd_addr = w;
			/* Length of bitmap */
			if (psfio_get_word(io, &w)) return PSF_E_NOTPSF;
			if (big_endian) w = bswap(w);
			disp[n].vfd_nbytes = w;
			/* Row / column counts */
			if (psfio_get_byte(io, (psf_byte *)&ch)) return PSF_E_NOTPSF;
			disp[n].vfd_up = ch;
			if (psfio_get_byte(io, (psf_byte *)&ch)) return PSF_E_NOTPSF;
			disp[n].vfd_down = ch;
			if (psfio_get_byte(io, (psf_byte *)&ch)) return PSF_E_NOTPSF;
			disp[n].vfd_left = ch;
			if (psfio_get_byte(io, (psf_byte *)&ch)) return PSF_E_NOTPSF;
			disp[n].vfd_right = ch;
			/* Logical width */
			if (psfio_get_word(io, &w)) return PSF_E_NOTPSF;
			if (big_endian) w = bswap(w);
			disp[n].vfd_width = w;
		}
		else	/* v2 */
		{
			if (psfio_get_dword(io, &disp[n].vfd_addr)) return PSF_E_NOTPSF;
			if (psfio_get_dword(io, &disp[n].vfd_nbytes)) return PSF_E_NOTPSF;
			if (psfio_get_word(io, (psf_word *)&disp[n].vfd_up)) return PSF_E_NOTPSF;
			if (psfio_get_word(io, (psf_word *)&disp[n].vfd_down)) return PSF_E_NOTPSF;
			if (psfio_get_word(io, (psf_word *)&disp[n].vfd_left)) return PSF_E_NOTPSF;
			if (psfio_get_word(io, (psf_word *)&disp[n].vfd_right)) return PSF_E_NOTPSF;
			if (psfio_get_word(io, (psf_word *)&disp[n].vfd_width)) return PSF_E_NOTPSF;
		}
	}
	/* Load bitmap */
	err = psfio_get_bytes(io, ndata, bitmap_size);
	if (err) return PSF_E_ERRNO;
        if (!vfont_is_unicode(io->vfont)) return PSF_E_OK;

	err = vfont_create_unicode(io->vfont); 
	if (err != PSF_E_OK) return err; 
        cchar = 0;

        while (cchar < nchars)
        {
		err = vfont_read_ucs_dir(io, cchar);
		if (err) return err;
        	++cchar; 
        }
	/* Recalculate MaxX and MaxY */
	vfont_get_max_charsize(io->vfont, &io->vfont->vf_maxx, 
		 			  &io->vfont->vf_maxy);
        return PSF_E_OK;
}	

/* Write PSF to file */
int vfont_io_write (PSFIO *io)
{
	unsigned nchars, n;
	int err;
	int big_endian = 0;

	/* Empty VFONT */
	if (!io->vfont->vf_bitmap || !io->vfont->vf_dispatch) return PSF_E_EMPTY;
	/* Calculate MaxX and MaxY */
	vfont_get_max_charsize(io->vfont, &io->vfont->vf_maxx, 
		 			  &io->vfont->vf_maxy);

	/* Write a v1 header and character table */
	if (io->vfont->vf_magic == VFONT_MAGIC_V1)
	{
		big_endian = io->vfont->vf_be;
		nchars = 256;

		psfio_put_word(io, big_endian ? bswap(io->vfont->vf_magic) : io->vfont->vf_magic);
		psfio_put_word(io, big_endian ? bswap(io->vfont->vf_size)  : io->vfont->vf_size);
		psfio_put_word(io, big_endian ? bswap(io->vfont->vf_maxx)  : io->vfont->vf_maxx);
		psfio_put_word(io, big_endian ? bswap(io->vfont->vf_maxy)  : io->vfont->vf_maxy);
		psfio_put_word(io, big_endian ? bswap(io->vfont->vf_xtend) : io->vfont->vf_xtend);

		for (n = 0; n < 256; n++)
		{
/* For out-of-range characters, write blank records */
			if (n >= io->vfont->vf_length)
			{
				psfio_put_word(io, 0);
				psfio_put_word(io, 0);
				psfio_put_byte(io, 0);
				psfio_put_byte(io, 0);
				psfio_put_byte(io, 0);
				psfio_put_byte(io, 0);
				psfio_put_word(io, 0);
				continue;
			}
			psfio_put_word(io, big_endian ? bswap(io->vfont->vf_dispatch[n].vfd_addr) : 
							io->vfont->vf_dispatch[n].vfd_addr);
			psfio_put_word(io, big_endian ? bswap(io->vfont->vf_dispatch[n].vfd_nbytes) : 
							io->vfont->vf_dispatch[n].vfd_nbytes);
			psfio_put_byte(io, io->vfont->vf_dispatch[n].vfd_up);
			psfio_put_byte(io, io->vfont->vf_dispatch[n].vfd_down);
			psfio_put_byte(io, io->vfont->vf_dispatch[n].vfd_left);
			psfio_put_byte(io, io->vfont->vf_dispatch[n].vfd_right);
			psfio_put_word(io, big_endian ? bswap(io->vfont->vf_dispatch[n].vfd_width) : 
							io->vfont->vf_dispatch[n].vfd_width);
		}
	}
	else
	{
	/* Write a v2 header and character table */
		psfio_put_dword(io, io->vfont->vf_magic);
		psfio_put_dword(io, io->vfont->vf_version);
		psfio_put_dword(io, io->vfont->vf_hdrlen);
		psfio_put_dword(io, io->vfont->vf_flags);
		psfio_put_dword(io, io->vfont->vf_length);
		psfio_put_dword(io, io->vfont->vf_size);
		psfio_put_dword(io, io->vfont->vf_maxy);
		psfio_put_dword(io, io->vfont->vf_maxx);
		nchars = io->vfont->vf_length;
		for (n = 0; n < nchars; n++)
		{
			psfio_put_dword(io, io->vfont->vf_dispatch[n].vfd_addr);
			psfio_put_dword(io, io->vfont->vf_dispatch[n].vfd_nbytes);
			psfio_put_word(io, io->vfont->vf_dispatch[n].vfd_up);
			psfio_put_word(io, io->vfont->vf_dispatch[n].vfd_down);
			psfio_put_word(io, io->vfont->vf_dispatch[n].vfd_left);
			psfio_put_word(io, io->vfont->vf_dispatch[n].vfd_right);
			psfio_put_word(io, io->vfont->vf_dispatch[n].vfd_width);
		}
	}
	/* Write the bitmap */
	if (psfio_put_bytes(io, io->vfont->vf_bitmap, io->vfont->vf_size)) return PSF_E_ERRNO;

	/* Write Unicode directory */

	if (vfont_is_unicode(io->vfont))
	{
		err = psf_write_dir(io, &io->vfont->vf_dir, io->vfont->vf_length, io->vfont->vf_magic);
		if (err) return err;
	}

	return PSF_E_OK;
}



/* Free any memory associated with a PSF file */
void vfont_delete(VFONT *f)
{
	vfont_delete_unicode(f);
	if (f->vf_dispatch) free(f->vf_dispatch);
	if (f->vf_bitmap)   free(f->vf_bitmap);
	vfont_new(f);	/* Reset to empty */
}

