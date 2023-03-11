
ZIP=psft1_10.zip

TARGETS=psf2inc.exe psf2pbms.exe psf2raw.exe psf2wof.exe psf2xbm.exe \
	psf2zx.exe psfs2cpi.exe  psf2txt.exe psf2bsd.exe psf2wyse.exe \
	psfxform.exe psfmerge.exe psfjoin.exe psf2bbc.exe psfs2mda.exe \
	mda2psf.exe raw2psf.exe zx2psf.exe psfpages.exe psf2bdf.exe \
	wof2psf.exe txt2psf.exe wyse2psf.exe bbc2psf.exe \
	psf2ams.exe ams2psf.exe \
	cpi2psf.exe cpicomp.exe cpidcomp.exe \
	fon2fnts.exe fnts2fon.exe fnt2psf.exe psf2fnt.exe \
	loadpsf.exe page2cp2.exe 

MANPAGES=cpi2psf.man   psf2bdf.man   psf2xbm.man   raw2psf.man \
	 cpicomp.man   psf2fnt.man   psf2zx.man	   txt2psf.man \
	 cpidcomp.man  psf2inc.man   psfjoin.man   wof2psf.man \
	 wyse2psf.man  psf2bbc.man   bbc2psf.man   psfs2mda.man \
	 fnt2psf.man   psf2pbms.man  psfmerge.man  zx2psf.man \
	 fnts2fon.man  psf2raw.man   psfpages.man  psf2bsd.man \
	 fon2fnts.man  psf2txt.man   psfs2cpi.man  psf2wyse.man \
	 mda2psf.man   psf2wof.man   psfxform.man  psf2ams.man \
	 ams2psf.man

CPI2PSF_OBJECTS=cpi2psf.obj cnvshell.obj 
PSFS2CPI_OBJECTS=psfs2cpi.obj cnvmulti.obj
PSFS2MDA_OBJECTS=psfs2mda.obj cnvmulti.obj
CPILIB_OBJECTS=cpialloc.obj cpiload.obj cpisave.obj cpicrush.obj cpibloat.obj

FON2FNTS_OBJ1=mz.obj ne.obj pe.obj nerootd.obj neroote.obj nesubd.obj nesube.obj
FON2FNTS_OBJ2=peresd.obj perese.obj resdir.obj
FON2FNTS_OBJECTS=$(FON2FNTS_OBJ1) $(FON2FNTS_OBJ2)

PSFLIB_1= psferror.obj psfio.obj    psflib.obj   psfpix.obj   psfucs.obj
PSFLIB_2= psfucs2.obj  psfutil1.obj psfutil2.obj psfutil3.obj psfver.obj
PSFLIB_3= mappings.obj
RM = xdel /n

PSFLIB_OBJECTS=$(PSFLIB_1) $(PSFLIB_2) $(PSFLIB_3) 

CFLAGS=-Bl -I. -I../include -D__MSDOS__
CC=pacc

all:	$(TARGETS)

manpages:	$(MANPAGES)

zip:	all manpages
	zip -jk $(ZIP) README ../doc/*.txt *.exe *.cp2 *.man ../COPYING


%.man:	../man/%.1
	nroff -c -mandoc $< | col -b > $@

libcpi.lib:	$(CPILIB_OBJECTS)
	libr r $@ $(CPILIB_OBJECTS)

libpsf.lib:	$(PSFLIB_OBJECTS)
	libr r $@ $(PSFLIB_1)
	libr r $@ $(PSFLIB_2)
	libr r $@ $(PSFLIB_3)

fon2fnts.lib:	$(FON2FNTS_OBJECTS)
	libr r $@ $(FON2FNTS_OBJ1)
	libr r $@ $(FON2FNTS_OBJ2)

cpi2psf.exe:	$(CPI2PSF_OBJECTS) libpsf.lib libcpi.lib
	$(CC) $(CFLAGS) $+

cpicomp.exe:	cpicomp.obj cnvshell.obj libpsf.lib libcpi.lib
	$(CC) $(CFLAGS) $+

cpidcomp.exe:	cpidcomp.obj cnvshell.obj libpsf.lib libcpi.lib
	$(CC) $(CFLAGS) $+

psfs2mda.exe:	$(PSFS2MDA_OBJECTS) libpsf.lib
	$(CC) $(CFLAGS) $+

psfs2cpi.exe:	$(PSFS2CPI_OBJECTS) libpsf.lib libcpi.lib
	$(CC) $(CFLAGS) $+

mda2psf.exe:	mda2psf.obj cnvshell.obj libpsf.lib 
	$(CC) $(CFLAGS) $+

psf2mda.exe:	psf2mda.obj cnvshell.obj libpsf.lib 
	$(CC) $(CFLAGS) $+

raw2psf.exe:	raw2psf.obj cnvshell.obj libpsf.lib 
	$(CC) $(CFLAGS) $+

psf2raw.exe:	psf2raw.obj cnvshell.obj libpsf.lib 
	$(CC) $(CFLAGS) $+

wof2psf.exe:	wof2psf.obj cnvshell.obj libpsf.lib 
	$(CC) $(CFLAGS) $+

psf2wof.exe:	psf2wof.obj cnvshell.obj libpsf.lib 
	$(CC) $(CFLAGS) $+

psf2bdf.exe:	psf2bdf.obj cnvshell.obj libpsf.lib 
	$(CC) $(CFLAGS) $+

psf2inc.exe:	psf2inc.obj cnvshell.obj libpsf.lib 
	$(CC) $(CFLAGS) $+

psf2bsd.exe:	psf2bsd.obj cnvshell.obj libpsf.lib 
	$(CC) $(CFLAGS) $+

psf2wyse.exe:	psf2wyse.obj cnvshell.obj libpsf.lib 
	$(CC) $(CFLAGS) $+

psf2bbc.exe:	psf2bbc.obj cnvshell.obj libpsf.lib 
	$(CC) $(CFLAGS) $+

psf2xbm.exe:	psf2xbm.obj cnvshell.obj libpsf.lib 
	$(CC) $(CFLAGS) $+

psfmerge.exe:	psfmerge.obj cnvmulti.obj libpsf.lib 
	$(CC) $(CFLAGS) $+

psfjoin.exe:	psfjoin.obj cnvmulti.obj libpsf.lib 
	$(CC) $(CFLAGS) $+

psfxform.exe:	psfxform.obj cnvshell.obj libpsf.lib 
	$(CC) $(CFLAGS) $+

txt2psf.exe:	txt2psf.obj cnvshell.obj libpsf.lib 
	$(CC) $(CFLAGS) $+

psf2txt.exe:	psf2txt.obj cnvshell.obj libpsf.lib 
	$(CC) $(CFLAGS) $+

psf2pbms.exe:	psf2pbms.obj cnvshell.obj libpsf.lib 
	$(CC) $(CFLAGS) $+

psf2zx.exe:	psf2zx.obj zxflib.obj cnvshell.obj libpsf.lib 
	$(CC) $(CFLAGS) $+

zx2psf.exe:	zx2psf.obj zxflib.obj cnvshell.obj libpsf.lib 
	$(CC) $(CFLAGS) $+

fon2fnts.exe:	fon2fnts.obj fon2fnts.lib
	$(CC) $(CFLAGS) $+

fnts2fon.exe:	fnts2fon.obj cnvmulti.obj mswfntr.obj mswfntw.obj libpsf.lib
	$(CC) $(CFLAGS) $+

fnt2psf.exe:	fnt2psf.obj cnvshell.obj mswfntr.obj libpsf.lib
	$(CC) $(CFLAGS) $+

psf2fnt.exe:	psf2fnt.obj cnvshell.obj mswfntw.obj libpsf.lib
	$(CC) $(CFLAGS) $+

wyse2psf.exe:	wyse2psf.obj cnvshell.obj libpsf.lib
	$(CC) $(CFLAGS) $+

bbc2psf.exe:	bbc2psf.obj cnvshell.obj libpsf.lib
	$(CC) $(CFLAGS) $+

page2cp2.exe:	page2cp2.obj libpsf.lib
	$(CC) $(CFLAGS) $+

loadpsf.exe:	loadpsf.obj load_pc.obj ldshell.obj libpsf.lib
	$(CC) $(CFLAGS) $+

%.exe:	%.obj libpsf.lib 
	$(CC) $(CFLAGS) $< libpsf.lib 

%.obj:	../tools/%.c
	$(CC) -c $(CFLAGS) $<

%.obj:	../lib/%.c
	$(CC) -c $(CFLAGS) $<

%.obj:	../winfonts/%.c
	$(CC) -c $(CFLAGS) $<


clean:	
	$(RM) *.exe
	$(RM) *.obj
	$(RM) *.lib

