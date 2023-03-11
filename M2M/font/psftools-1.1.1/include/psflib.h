/*
    psftools: Manipulate console fonts in the .PSF (v2) format
    Copyright (C) 2002, 2021  John Elliott <seasip.webmaster@gmail.com>

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

/* This file prototypes functions to load and save fonts in the PSF format 
 * (versions 1 and 2) and the BSD vfont format (versions 1 and 2) */

#ifndef PSFLIB_H_INCLUDED

#define PSFLIB_H_INCLUDED

#define PSF_MAGIC 0x864ab572L	/* PSF file magic number */
#define PSF_MAGIC0     0x72
#define PSF_MAGIC1     0xb5
#define PSF_MAGIC2     0x4a
#define PSF_MAGIC3     0x86


#ifndef PSF1_MAGIC
#define PSF1_MAGIC 0x0436       /* PSF file magic number */

#define VFONT_MAGIC_V1 0436	/* Berkeley vfont magic (note 0436 vs 0x436!) */
#define VFONT_MAGIC_V2 0x68a45b27L /* Magic for proposed vfont v2 file */

#define PSF_E_OK        ( 0)    /* OK */
#define PSF_E_NOMEM     (-1)    /* Out of memory */
#define PSF_E_NOTIMPL   (-2)    /* Unsupported PSF file format */
#define PSF_E_NOTPSF    (-3)    /* Attempt to load a non-PSF file */
#define PSF_E_ERRNO     (-4)    /* Error in fread() or fwrite() */
#define PSF_E_EMPTY     (-5)    /* Attempt to save an empty file */
#define PSF_E_ASCII     (-6)    /* Not a Unicode PSF */
#define PSF_E_UNICODE   (-7)    /* Failed to load Unicode directory */
#define PSF_E_V2        (-8)    /* PSF is in version 2 */
#define PSF_E_NOTFOUND  (-9)	/* Code point not found */
#define PSF_E_BANNED    (-10)	/* Invalid Unicode code point */
#define PSF_E_PARSE     (-11)   /* Invalid Unicode sequence string */
#define PSF_E_RANGE	(-12)	/* Character index out of range */

typedef unsigned char  psf_byte;
typedef unsigned short psf_word;
typedef unsigned long  psf_dword;

typedef int psf_errno_t;


/* In-memory representation of a .PSF unicode directory entry. Rather than
 * malloc these one at a time, we malloc them in blocks and keep a linked
 * list of the blocks allocated (psf_unicode_buffer).
 *
 * The entries themselves also take the form of a linked list; there will
 * be 256 or 512 chains for characters, and one for free blocks.
 */

typedef struct psfu_entry
{
	struct psfu_entry *psfu_next;
	psf_dword psfu_token;	
} psf_unicode_dirent;

#define PSF_ENTRIES_PER_BUFFER 32	/* Can be tuned */

typedef struct psfu_buffer
{	
	struct psfu_buffer *psfb_next;
	psf_unicode_dirent psfb_dirents[PSF_ENTRIES_PER_BUFFER];
} psf_unicode_buffer;

#endif

typedef struct 
{
	psf_unicode_dirent **psf_dirents_used;
	psf_unicode_dirent *psf_dirents_free;
	psf_unicode_buffer *psf_dirents_buffer;
	size_t psf_dirents_nused;
	size_t psf_dirents_nfree;	/* total malloced is always used+free */
} PSF_UNICODE_DIR;

/* In-memory representation of a .PSF file (v1 or v2) */
typedef struct
{
	psf_dword psf_magic;
	psf_dword psf_version;
	psf_dword psf_hdrlen;	/* Size of header */
	psf_dword psf_flags;
	psf_dword psf_length;	/* Number of characters */	
	psf_dword psf_charlen;	/* Length of a character bitmap */
	psf_dword psf_height;	/* Height of a glyph */
	psf_dword psf_width;	/* Width of a glyph */	
	psf_byte *psf_data;	/* Font bitmaps */
	PSF_UNICODE_DIR psf_dir;
} PSF_FILE;

/* In-memory representation of a Berkeley vfont file (v1 or v2) */
typedef struct vfont_dispatch
{
        psf_dword       vfd_addr;    /* Offset of bitmap in font bitmaps */
        psf_dword       vfd_nbytes;  /* Length of bitmap, 0 if none */
        signed short    vfd_up;	     /* Rows above baseline */
	signed short	vfd_down;    /* Rows below baseline */
				     /* up+down gives bitmap height */
	signed short 	vfd_left;    /* Columns left of baseline point */
	signed short    vfd_right;   /* Columns right of baseline point */
				     /* left+down gives bitmap width */
/* Negative values in up or down / left or right are permitted so long as 
   the totals remain positive. They mean that the baseline point is outside 
   the character. */
        signed short    vfd_width;   /* Logical width, used by troff */
} VFONT_DISPATCH;

typedef struct vfont
{
	psf_dword vf_magic;	/* Magic number VFONT_MAGIC */
	psf_dword vf_version;	/* Version number (currently 0) */
	psf_dword vf_hdrlen;	/* Length of header (10 for v1, 32 for v2) */
	psf_dword vf_flags;	/* Bit 0 set for Unicode (v2 only) */
	int       vf_be;	/* Big-endian format? (v1 only) */
	psf_dword vf_length;	/* Count of characters */
	psf_dword vf_size;	/* Total # bytes of bitmaps */
	psf_dword vf_maxx;	/* Maximum horizontal glyph size */
	psf_dword vf_maxy;	/* Maximum vertical glyph size */
	psf_dword vf_xtend;	/* Extension (v1 only) */
	PSF_UNICODE_DIR vf_dir; /* Unicode directory */
	VFONT_DISPATCH *vf_dispatch; /* Character definitions */
	unsigned char *vf_bitmap;    /* Character bitmaps */
} VFONT; 



typedef struct array
{
        psf_byte *data;
        unsigned long len;
} PSFIO_ARRAY;


/* Stream I/O support */
typedef struct psfio
{
        PSF_FILE *psf;
	VFONT *vfont;

        int (*readfunc )(struct psfio *io);
        int (*writefunc)(struct psfio *io, psf_byte b);
        union
        {
                FILE *fp;
                PSFIO_ARRAY arr;
		void *general;
        } data;
} PSFIO;

typedef struct psf_mapping
{
	char *psfm_name;
	psf_dword  psfm_count;
	psf_dword *psfm_tokens[0];
} PSF_MAPPING;

typedef struct psf_mapping256
{
	char *psfm_name;
	psf_dword  psfm_count;
	psf_dword *psfm_tokens[256];
} PSF_MAPPING256;

typedef struct psf_mapping512
{
	char *psfm_name;
	psf_dword  psfm_count;
	psf_dword *psfm_tokens[512];
} PSF_MAPPING512;

/* Initialise the structure as empty */
void psf_file_new(PSF_FILE *f);
/* Initialise the structure for a new font*/
psf_errno_t psf_file_create(PSF_FILE *f, psf_dword width, psf_dword height,
				psf_dword nchars, psf_byte unicode);
/* Add Unicode directory to a font */
psf_errno_t psf_file_create_unicode(PSF_FILE *f);

/* Initialise Unicode directory */
void psf_dir_init(PSF_UNICODE_DIR *dir);

/* Add an entry to the Unicode directory */
psf_errno_t psf_unicode_add(PSF_FILE *f, psf_word nchar, psf_dword token);
/* Add a chain to the Unicode directory from the map */
psf_errno_t psf_unicode_addmap(PSF_FILE *f, psf_word destchar, 
				PSF_MAPPING *m, psf_word srcchar);

/* Helper method: Populate the entire Unicode directory in one hit */
psf_errno_t psf_unicode_addall(PSF_FILE *f, PSF_MAPPING *m, unsigned first, unsigned last);

/* Remove an entry from the Unicode directory */
psf_errno_t psf_unicode_delete(PSF_FILE *f, psf_word nchar, psf_dword token);
/* Find a token in the Unicode directory */
psf_errno_t psf_unicode_lookup(PSF_FILE *f, psf_dword token, psf_dword *nchar);

/* Find a token in the Unicode directory, based on chain slot in mapping m */
psf_errno_t psf_unicode_lookupmap(PSF_FILE *f, PSF_MAPPING *m, psf_word slot, psf_dword *nchar, psf_dword *found);
/* Is 'token' a character that can't go in the Unicode mapping table? */
psf_errno_t psf_unicode_banned(psf_dword token);
/* Convert a Unicode chain to a string */
psf_errno_t psf_unicode_to_string(psf_unicode_dirent *chain, char **str);
/* ... and back */
psf_errno_t psf_unicode_from_string(PSF_FILE *f, psf_word nchar, const char *str);


/* Allocate and initialise a psf_unicode_buffer */
psf_unicode_buffer *psf_malloc_unicode_buffer(void);

/* Load a PSF from memory */
psf_errno_t psf_memory_read(PSF_FILE *f, psf_byte *data, unsigned len);
/* Save a PSF to memory */
psf_errno_t psf_memory_write(PSF_FILE *f, psf_byte *data, unsigned len);
/* Load a PSF from a stream 
 * nb: If the file pointer is important to you, you have to save it. */
psf_errno_t psf_file_read  (PSF_FILE *f, FILE *fp);
/* Write PSF to stream */
psf_errno_t psf_file_write (PSF_FILE *f, FILE *fp);
/* Free any memory associated with a PSF file */
void psf_file_delete (PSF_FILE *f);
/* Remove any unicode directory from a PSF file */
void psf_file_delete_unicode(PSF_FILE *f);
/* Expand error number returned by other routines, to string */
char *psf_error_string(psf_errno_t err);

/* Find an ASCII->Unicode mapping */
PSF_MAPPING *psf_find_mapping(char *name);
void psf_list_mappings(FILE *fp);

/* Get/set pixel in a character bitmap */
psf_errno_t psf_get_pixel(PSF_FILE *f, psf_dword ch, psf_dword x, psf_dword y, psf_byte *pixel);
psf_errno_t psf_set_pixel(PSF_FILE *f, psf_dword ch, psf_dword x, psf_dword y, psf_byte pixel);

/* Input/output from/to generic streams */
psf_errno_t psf_read (PSFIO *io);
psf_errno_t psf_write(PSFIO *io);

psf_errno_t psf_write_dir(PSFIO *io, PSF_UNICODE_DIR *dir, int nchars, int magic);

/* Force a PSF file to be v1 or v2 */
psf_errno_t psf_force_v1(PSF_FILE *f);
psf_errno_t psf_force_v2(PSF_FILE *f);

/* vfont access functions */
/* Initialise the structure as empty */
void vfont_new(VFONT *f);
/* Initialise the structure for a new font*/
psf_errno_t vfont_create(VFONT *f, psf_dword nchars, psf_dword bitmap_size,
			psf_byte unicode);
/* Reallocate the size of the bitmap */
psf_errno_t vfont_realloc(VFONT *f, psf_dword bitmap_size);

/* Add Unicode directory to a font */
psf_errno_t vfont_create_unicode(VFONT *f);
/* Free any memory associated with a VFONT */
void vfont_delete (VFONT *f);
/* Remove any unicode directory from a VFONT */
void vfont_delete_unicode(VFONT *f);

/* Add an entry to the Unicode directory */
psf_errno_t vfont_unicode_add(VFONT *f, psf_word nchar, psf_dword token);
/* Remove an entry from the Unicode directory */
psf_errno_t vfont_unicode_delete(VFONT *f, psf_word nchar, psf_dword token);
/* Find a token in the Unicode directory */
psf_errno_t vfont_unicode_lookup(VFONT *f, psf_dword token, psf_dword *nchar);
psf_errno_t vfont_unicode_from_string(VFONT *f, psf_word nchar, const char *str);

psf_errno_t vfont_force_v1(VFONT *f);
psf_errno_t vfont_force_v2(VFONT *f);

psf_errno_t vfont_read  (VFONT *f, FILE *fp);
psf_errno_t vfont_write (VFONT *f, FILE *fp);
psf_errno_t vfont_mem_read  (VFONT *f, psf_byte *data, unsigned len);
psf_errno_t vfont_mem_write (VFONT *f, psf_byte *data, unsigned len);

psf_errno_t vfont_get_pixel(VFONT *f, psf_dword ch, psf_dword x, psf_dword y, psf_byte *pixel);
psf_errno_t vfont_set_pixel(VFONT *f, psf_dword ch, psf_dword x, psf_dword y, psf_byte pixel);
/* Get dimensions of a character bitmap */
psf_errno_t vfont_get_charsize(VFONT *f, psf_dword ch, psf_dword *x, psf_dword *y);
/* Get dimensions of a character cell */
psf_errno_t vfont_get_cellsize(VFONT *f, psf_dword ch, psf_dword *x, psf_dword *y);

/* Get max dimensions of a character bitmap */
psf_errno_t vfont_get_max_charsize(VFONT *f, psf_dword *x, psf_dword *y);
/* Get max dimensions of a character cell */
psf_errno_t vfont_get_max_cellsize(VFONT *f, 
		psf_dword *cell_w, psf_dword *cell_h,
		psf_dword *base_x, psf_dword *base_y);

#define vfont_is_unicode(f) (( VFONT_MAGIC_V2 == (f)->vf_magic ) && \
			       ((f)->vf_flags & 1))
#define vfont_count_chars(f) ( (f)->vf_length )

#define psf_is_unicode(f)  ( ((f)->psf_flags) & 1 )
#define psf_count_chars(f) ( (f)->psf_length )
#include "psfutils.h"

#endif /* PSFLIB_H_INCLUDED */

