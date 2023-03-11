
TARGETS=cpi2psf.exe  psfs2cpi.exe mda2psf.exe  psf2bdf.exe  psf2inc.exe \
	psf2pbms.exe psf2raw.exe  psf2txt.exe  psf2wof.exe  psf2xbm.exe \
	psf2zx.exe   psfjoin.exe  psfmerge.exe psfpages.exe psfxform.exe \
	raw2psf.exe  txt2psf.exe  wof2psf.exe  zx2psf.exe cpicomp.exe \
	cpidcomp.exe page2cp2.exe psfs2mda.exe

CPIDCOMP_OBJECTS=cpidcomp.o cnvshell.o
CPICOMP_OBJECTS=cpicomp.o cnvshell.o
CPI2PSF_OBJECTS=cpi2psf.o cnvshell.o 
PSFS2CPI_OBJECTS=psfs2cpi.o cnvmulti.o
PSFS2MDA_OBJECTS=psfs2mda.o cnvmulti.o
CPILIB_OBJECTS=cpialloc.o cpiload.o cpisave.o cpicrush.o cpibloat.o

PSFLIB_OBJECTS = mappings.o \
	8859-1.o   8859-2.o   8859-3.o   8859-4.o   8859-5.o \
	8859-6.o   8859-7.o   8859-8.o   8859-9.o   8859-10.o \
	8859-13.o  8859-14.o  8859-15.o  8859-16.o  amstrad.o \
	cp1250.o   cp1251.o   cp1252.o   cp1253.o   cp1254.o \
	cp1255.o   cp1256.o   cp1257.o   cp1258.o   cp437.o \
	cp737.o    cp775.o    cp850.o    cp852.o    cp855.o \
	cp857.o    cp860.o    cp861.o    cp862.o    cp863.o \
	cp864.o    cp865.o    cp866.o    cp869.o    cp874.o \
	pcgem.o    cp858.o    cp367.o    bbcmicro.o ppcgreek.o \
	ppc437.o   ppc860.o   ppc865.o \
	psferror.o psfio.o    psflib.o   psfpix.o   psfucs.o \
	psfucs2.o  psfutil1.o psfutil2.o psfutil3.o psfver.o 

CFLAGS=-I. -I../include
CC=gcc
AR = ar rc
RANLIB = ranlib

all:	$(TARGETS)

libcpi.a:	$(CPILIB_OBJECTS)
	$(AR) $@ $(CPILIB_OBJECTS)
	$(RANLIB) $@

libpsf.a:	$(PSFLIB_OBJECTS)
	$(AR) $@ $(PSFLIB_OBJECTS)
	$(RANLIB) $@

cpi2psf.exe:	$(CPI2PSF_OBJECTS) libpsf.a libcpi.a
	$(CC) -o $@ $(CFLAGS) $(CPI2PSF_OBJECTS) -L. -lcpi -lpsf

psfs2cpi.exe:	$(PSFS2CPI_OBJECTS) libpsf.a libcpi.a
	$(CC) -o $@ $(CFLAGS) $(PSFS2CPI_OBJECTS) -L. -lcpi -lpsf

psfs2mda.exe:	$(PSFS2MDA_OBJECTS) libpsf.a
	$(CC) -o $@ $(CFLAGS) $(PSFS2MDA_OBJECTS) -L. -lpsf

cpidcomp.exe:	$(CPIDCOMP_OBJECTS) libpsf.a libcpi.a
	$(CC) -o $@ $(CFLAGS) $(CPIDCOMP_OBJECTS) -L. -lcpi -lpsf

cpicomp.exe:	$(CPICOMP_OBJECTS) libpsf.a libcpi.a
	$(CC) -o $@ $(CFLAGS) $(CPICOMP_OBJECTS) -L. -lcpi -lpsf

page2cp2.exe:	../tools/page2cp2.c 
	$(CC) -o $@ $(CFLAGS) $< -L. -lpsf

psfjoin.exe:	../tools/psfjoin.c cnvmulti.o libpsf.a
	$(CC) -o $@ $(CFLAGS) $< cnvmulti.o -L. -lpsf

psfmerge.exe:	../tools/psfmerge.c cnvmulti.o libpsf.a
	$(CC) -o $@ $(CFLAGS) $< cnvmulti.o -L. -lpsf

psfpages.exe:	../tools/psfpages.c libpsf.a
	$(CC) -o $@ $(CFLAGS) $< -L. -lpsf

zx2psf.exe:	../tools/zx2psf.c cnvshell.o zxflib.o libpsf.a
	$(CC) -o $@ $(CFLAGS) $< cnvshell.o zxflib.o -L. -lpsf

psf2zx.exe:	../tools/psf2zx.c cnvshell.o zxflib.o libpsf.a
	$(CC) -o $@ $(CFLAGS) $< cnvshell.o zxflib.o -L. -lpsf

%.exe:	../tools/%.c cnvshell.o libpsf.a
	$(CC) -o $@ $(CFLAGS) $< cnvshell.o -L. -lpsf

%.o:	../tools/%.c
	$(CC) -c $(CFLAGS) $<

%.o:	../lib/%.c
	$(CC) -c $(CFLAGS) $<
