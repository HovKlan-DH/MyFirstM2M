/*
    psftools: Manipulate console fonts in the .PSF format
    Copyright (C) 2002, 2007  John Elliott

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

/* Get/set pixel in a character bitmap */
psf_errno_t psf_get_pixel(PSF_FILE *f, psf_dword ch, psf_dword x, psf_dword y, psf_byte *pix)
{
	psf_byte * b;

	if (!f->psf_data) return PSF_E_EMPTY;
	*pix = 0;
	if (ch >= f->psf_length || x >= f->psf_width || y >= f->psf_height) 
		return PSF_E_RANGE;

	b = &f->psf_data[ch * f->psf_charlen];	
	b += (y * ((f->psf_width + 7)/8)) + (x/8);

	*pix = *b & (0x80 >> (x & 7));	
	return PSF_E_OK;
}


psf_errno_t psf_set_pixel(PSF_FILE *f, psf_dword ch, psf_dword x, psf_dword y, psf_byte pix)
{
	psf_byte * b;

	if (!f->psf_data) return PSF_E_EMPTY;
	if (ch >= f->psf_length || x >= f->psf_width || y >= f->psf_height) 
		return PSF_E_RANGE;

	b = &f->psf_data[ch * f->psf_charlen];	
	b += (y * ((f->psf_width + 7)/8)) + (x/8);

	if (pix) *b |= (0x80 >> (x & 7));	
	else     *b &= ~(0x80 >> (x & 7));
	return PSF_E_OK;
}


psf_errno_t vfont_get_pixel(VFONT *f, psf_dword ch, psf_dword x, psf_dword y, psf_byte *pix)
{
	psf_byte * b;
	psf_dword w;	/* Character width, pixels */
	psf_errno_t err;

	if (!f->vf_bitmap || !f->vf_dispatch) return PSF_E_EMPTY;
	*pix = 0;
	err = vfont_get_charsize(f, ch, &w, NULL); 
	if (err) return err;

	/* Character has no bitmap */
	if (!f->vf_dispatch[ch].vfd_nbytes) return PSF_E_EMPTY;

	b = &f->vf_bitmap[f->vf_dispatch[ch].vfd_addr];

	b += (y * ((w + 7)/8)) + (x/8);

	*pix = *b & (0x80 >> (x & 7));	
	return PSF_E_OK;
}


psf_errno_t vfont_set_pixel(VFONT *f, psf_dword ch, psf_dword x, psf_dword y, psf_byte pix)
{
	psf_byte * b;
	psf_dword w;	/* Character width, pixels */
	psf_errno_t err;

	if (!f->vf_bitmap || !f->vf_dispatch) return PSF_E_EMPTY;
	err = vfont_get_charsize(f, ch, &w, NULL); 
	if (err) return err;

	/* Character has no bitmap */
	if (!f->vf_dispatch[ch].vfd_nbytes) return PSF_E_EMPTY;

	b = &f->vf_bitmap[f->vf_dispatch[ch].vfd_addr];

	b += (y * (w + 7)/8) + (x/8);
	
	if (pix) *b |= (0x80 >> (x & 7));	
	else     *b &= ~(0x80 >> (x & 7));
	return PSF_E_OK;
}




psf_errno_t vfont_get_charsize(VFONT *f, psf_dword ch, psf_dword *x, psf_dword *y)
{
	if (!f->vf_dispatch) return PSF_E_EMPTY;
	if (ch >= f->vf_length) return PSF_E_RANGE;

	if (x) *x = f->vf_dispatch[ch].vfd_left + f->vf_dispatch[ch].vfd_right;
	if (y) *y = f->vf_dispatch[ch].vfd_up   + f->vf_dispatch[ch].vfd_down;
	return PSF_E_OK;	
}

psf_errno_t vfont_get_cellsize(VFONT *f, psf_dword ch, psf_dword *x, psf_dword *y)
{
	if (!f->vf_dispatch) return PSF_E_EMPTY;
	if (ch >= f->vf_length) return PSF_E_RANGE;

	*x = f->vf_dispatch[ch].vfd_left + f->vf_dispatch[ch].vfd_right;
	*y = f->vf_dispatch[ch].vfd_up   + f->vf_dispatch[ch].vfd_down;

	/* If the baseline point is outside the cell, expand the cell 
	  until it's in there */
	if (f->vf_dispatch[ch].vfd_left < 0)  *x -= f->vf_dispatch[ch].vfd_left;
	if (f->vf_dispatch[ch].vfd_right < 0) *x -= f->vf_dispatch[ch].vfd_right;
	if (f->vf_dispatch[ch].vfd_up < 0)    *y -= f->vf_dispatch[ch].vfd_up;
	if (f->vf_dispatch[ch].vfd_down < 0)  *y -= f->vf_dispatch[ch].vfd_down;

	return PSF_E_OK;	
}



/* Get max dimensions of a character bitmap */
psf_errno_t vfont_get_max_charsize(VFONT *f, psf_dword *cx, psf_dword *cy)
{
	psf_dword maxx = 0, maxy = 0, x, y;
	psf_dword ch;

	if (!f->vf_dispatch) return PSF_E_EMPTY;
	for (ch = 0; ch < f->vf_length; ch++)
	{
		x = f->vf_dispatch[ch].vfd_left + f->vf_dispatch[ch].vfd_right;
		y = f->vf_dispatch[ch].vfd_up   + f->vf_dispatch[ch].vfd_down;
		
		if (x > maxx) maxx = x;
		if (y > maxy) maxy = y;	
	}
	if (cx) *cx = maxx;
	if (cy) *cy = maxy;
	return PSF_E_OK;
}


/* Get max dimensions of a character cell */
psf_errno_t vfont_get_max_cellsize(VFONT *f, psf_dword *cell_w, 
	psf_dword *cell_h, psf_dword *base_x, psf_dword *base_y)
{
	signed int left, right, up, down;
	signed int maxleft = 0, maxright = 0, maxup = 0, maxdown = 0;
	psf_dword ch;
	/* Start the base point at (0,0) */
	signed int xb = 0, yb = 0;

	if (!f->vf_dispatch) return PSF_E_EMPTY;

	/* Find out what the furthest extents of left / right / up / down are
	 * (distance from basepoint to edge of character) */
	for (ch = 0; ch < f->vf_length; ch++)
	{
		left  = f->vf_dispatch[ch].vfd_left;
		right = f->vf_dispatch[ch].vfd_right;
		up    = f->vf_dispatch[ch].vfd_up;
		down  = f->vf_dispatch[ch].vfd_down;

		if (left  > maxleft ) maxleft  = left;
		if (right > maxright) maxright = right;
		if (up    > maxup)    maxup    = up;
		if (down  > maxdown)  maxdown  = right;
	}
/* Now make sure that basepoint is placed such that all characters fit in
 * the cell */
	for (ch = 0; ch < f->vf_length; ch++)
	{
		left  = f->vf_dispatch[ch].vfd_left;
		up    = f->vf_dispatch[ch].vfd_up;

/* If character won't fit in top / left, move the basepoint down and right */
		if (left > xb) xb = left;
		if (up   > yb) yb = up;
	}
	if (cell_w) *cell_w = maxleft + maxright;
	if (cell_h) *cell_h = maxup + maxdown;
	if (base_x) *base_x = xb;
	if (base_y) *base_y = yb;
	return PSF_E_OK;
}

