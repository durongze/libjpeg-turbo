# Makefile for Independent JPEG Group's software

# This makefile is for DJGPP (Delorie's GNU C port on MS-DOS), v2.0 or later.
# Thanks to Frank J. Donahoe for this version.

# Read installation instructions before saying "make" !!

# The name of your C compiler:
CC= gcc

# You may need to adjust these cc options:
CFLAGS= -g -Wall -I. 

# Generally, we recommend defining any configuration symbols in jconfig.h,
# NOT via -D switches here.

# Link-time cc options:
LDFLAGS= 

# To link any special libraries, add the necessary -l commands here.
LDLIBS= 

# Put here the object file name for the correct system-dependent memory
# manager file.  For DJGPP this is usually jmemnobs.o, but you could
# use jmemname.o if you want to use named temp files instead of swap space.
SYSDEPMEM= jmemnobs.o

# miscellaneous OS-dependent stuff
# linker
LN= $(CC)
# file deletion command
RM= rm
# library (.a) file creation command
AR= ar rc
# second step in .a creation (use "touch" if not needed)
AR2= ranlib

# End of configurable options.

JSIMDSOURCES=jccolext-avx2.asm  jcgray-avx2.asm    jchuff-sse2.asm    jdcolext-avx2.asm  jdmerge-avx2.asm \
			 jdsample-avx2.asm  jfdctint-avx2.asm  jidctint-avx2.asm  jquanti-avx2.asm \
			 jccolext-sse2.asm  jcgray-sse2.asm    jcphuff-sse2.asm   jdcolext-sse2.asm  jdmerge-sse2.asm \
			 jdsample-sse2.asm  jfdctint-sse2.asm  jidctint-sse2.asm  jquanti-sse2.asm \
			 jccolor-avx2.asm   jcgryext-avx2.asm  jcsample-avx2.asm  jdcolor-avx2.asm   jdmrgext-avx2.asm \
			 jfdctflt-sse.asm   jidctflt-sse2.asm  jidctred-sse2.asm  \
			 jccolor-sse2.asm   jcgryext-sse2.asm  jcsample-sse2.asm  jdcolor-sse2.asm   jdmrgext-sse2.asm \
			 jfdctfst-sse2.asm  jidctfst-sse2.asm  jquantf-sse2.asm   jsimdcpu.asm

JSIMDOBJS=$(JSIMDSOURCES:%.asm=%.o)

JSIMDSRC=jsimd_none.c
JSIMDOBJ=jsimd_none.o

# source files: JPEG library proper
LIBSOURCES= jaricom.c jcapimin.c jcapistd.c jcarith.c jccoefct.c jccolor.c \
        jcdctmgr.c jchuff.c jcinit.c jcmainct.c jcmarker.c jcmaster.c \
        jcomapi.c jcparam.c jcprepct.c jcsample.c jctrans.c jdapimin.c \
        jdapistd.c jdarith.c jdatadst.c jdatasrc.c jdcoefct.c jdcolor.c \
        jddctmgr.c jdhuff.c jdinput.c jdmainct.c jdmarker.c jdmaster.c \
        jdmerge.c jdpostct.c jdsample.c jdtrans.c jerror.c jfdctflt.c \
        jfdctfst.c jfdctint.c jidctflt.c jidctfst.c jidctint.c jquant1.c \
        jquant2.c jutils.c jmemmgr.c 
# memmgr back ends: compile only one of these into a working library
SYSDEPSOURCES= jmemansi.c jmemname.c jmemnobs.c jmemdos.c jmemmac.c
# source files: cjpeg/djpeg/jpegtran applications, also rdjpgcom/wrjpgcom
APPSOURCES= cjpeg.c djpeg.c jpegtran.c rdjpgcom.c wrjpgcom.c cdjpeg.c \
        rdcolmap.c rdswitch.c transupp.c rdppm.c wrppm.c rdgif.c wrgif.c \
        rdtarga.c wrtarga.c rdbmp.c wrbmp.c rdrle.c wrrle.c
SOURCES= $(LIBSOURCES) $(SYSDEPSOURCES) $(APPSOURCES)
# files included by source files
INCLUDES= jdct.h jerror.h jinclude.h jmemsys.h jmorecfg.h jpegint.h \
        jpeglib.h jversion.h cdjpeg.h cderror.h transupp.h
# documentation, test, and support files
DOCS= README install.txt usage.txt cjpeg.1 djpeg.1 jpegtran.1 rdjpgcom.1 \
        wrjpgcom.1 wizard.txt example.c libjpeg.txt structure.txt \
        coderules.txt filelist.txt change.log
MKFILES= configure Makefile.in makefile.ansi makefile.unix makefile.b32 \
        makefile.bcc makefile.mc6 makefile.dj makefile.wat makefile.vc \
        makefile.vs makejdsw.vc6 makeadsw.vc6 makejdep.vc6 makejdsp.vc6 \
        makejmak.vc6 makecdep.vc6 makecdsp.vc6 makecmak.vc6 makeddep.vc6 \
        makeddsp.vc6 makedmak.vc6 maketdep.vc6 maketdsp.vc6 maketmak.vc6 \
        makerdep.vc6 makerdsp.vc6 makermak.vc6 makewdep.vc6 makewdsp.vc6 \
        makewmak.vc6 makejsln.v15 makeasln.v15 makejvcx.v15 makejfil.v15 \
        makecvcx.v15 makecfil.v15 makedvcx.v15 makedfil.v15 maketvcx.v15 \
        maketfil.v15 makervcx.v15 makerfil.v15 makewvcx.v15 makewfil.v15 \
        makeproj.mac makcjpeg.st makdjpeg.st makljpeg.st maktjpeg.st \
        makefile.manx makefile.sas makefile.mms makefile.vms makvms.opt
CONFIGFILES= jconfig.cfg jconfig.bcc jconfig.mc6 jconfig.dj jconfig.wat \
        jconfig.vc jconfig.mac jconfig.st jconfig.manx jconfig.sas \
        jconfig.vms
CONFIGUREFILES= config.guess config.sub install-sh ltmain.sh depcomp \
        missing ar-lib
OTHERFILES= jconfig.txt ckconfig.c jmemdosa.asm libjpeg.map libjpeg.pc.in
TESTFILES= testorig.jpg testimg.ppm testimg.bmp testimg.jpg testprog.jpg \
        testimgp.jpg
DISTFILES= $(DOCS) $(MKFILES) $(CONFIGFILES) $(SOURCES) $(INCLUDES) \
        $(CONFIGUREFILES) $(OTHERFILES) $(TESTFILES)
# library object files common to compression and decompression
COMOBJECTS= jaricom.o jcomapi.o jutils.o jerror.o jmemmgr.o \
            rdppm.o rdgif.o rdtarga.o rdbmp.o \
            wrppm.o wrgif.o wrtarga.o wrbmp.o \
			jcicc.o jcphuff.o jdicc.o jdphuff.o jidctred.o \
			turbojpeg.o jdatadst-tj.o jdatasrc-tj.o \
			transupp.o libyuv.o \
            $(SYSDEPMEM) $(JSIMDOBJ)
# compression library object files
CLIBOBJECTS= jcapimin.o jcapistd.o jcarith.o jctrans.o jcparam.o \
        jdatadst.o jcinit.o jcmaster.o jcmarker.o jcmainct.o jcprepct.o \
        jccoefct.o jccolor.o jcsample.o jchuff.o jcdctmgr.o jfdctfst.o \
        jfdctflt.o jfdctint.o
# decompression library object files
DLIBOBJECTS= jdapimin.o jdapistd.o jdarith.o jdtrans.o jdatasrc.o \
        jdmaster.o jdinput.o jdmarker.o jdhuff.o jdmainct.o \
        jdcoefct.o jdpostct.o jddctmgr.o jidctfst.o jidctflt.o \
        jidctint.o jdsample.o jdcolor.o jquant1.o jquant2.o jdmerge.o
# These objectfiles are included in libjpeg.a
LIBOBJECTS= $(CLIBOBJECTS) $(DLIBOBJECTS) $(COMOBJECTS)
# object files for sample applications (excluding library files)
COBJECTS= cjpeg.o rdswitch.o \
        cdjpeg.o
DOBJECTS= djpeg.o rdcolmap.o \
        cdjpeg.o
TROBJECTS= jpegtran.o rdswitch.o cdjpeg.o transupp.o


all: libjpeg.a cjpeg.exe djpeg.exe jpegtran.exe rdjpgcom.exe wrjpgcom.exe tjexample.exe

libjpeg.a: $(LIBOBJECTS)
	$(AR) libjpeg.a  $(LIBOBJECTS) 

cjpeg.exe: $(COBJECTS) $(LIBOBJECTS)
	$(LN) $(LDFLAGS) -o cjpeg.exe $(LIBOBJECTS) $(COBJECTS) $(LDLIBS)

djpeg.exe: $(DOBJECTS) libjpeg.a
	$(LN) $(LDFLAGS) -o djpeg.exe $(DOBJECTS) libjpeg.a $(LDLIBS)

jpegtran.exe: $(TROBJECTS) libjpeg.a
	$(LN) $(LDFLAGS) -o jpegtran.exe $(TROBJECTS) libjpeg.a $(LDLIBS)

rdjpgcom.exe: rdjpgcom.o
	$(LN) $(LDFLAGS) -o rdjpgcom.exe rdjpgcom.o $(LDLIBS)

wrjpgcom.exe: wrjpgcom.o
	$(LN) $(LDFLAGS) -o wrjpgcom.exe wrjpgcom.o $(LDLIBS)

tjexample.exe: tjexample.o
	$(LN) $(LDFLAGS) -o tjexample.exe tjexample.o libjpeg.a $(LDLIBS)

jconfig.h: jconfig.txt
	echo You must prepare a system-dependent jconfig.h file.
	echo Please read the installation directions in install.txt.
	exit 1

clean:
	$(RM) cjpeg.exe
	$(RM) djpeg.exe
	$(RM) jpegtran.exe
	$(RM) rdjpgcom.exe
	$(RM) wrjpgcom.exe
	$(RM) tjexample.exe
	$(RM) libjpeg.a *.o
	$(RM) testout*.* ../*.yuv

test: cjpeg.exe djpeg.exe jpegtran.exe
	$(RM) testout*.*
	./djpeg -dct int -ppm -outfile testout.ppm  testorig.jpg
	./djpeg -dct int -bmp -colors 256 -outfile testout.bmp  testorig.jpg
	./cjpeg -dct int -outfile testout.jpg  testimg.ppm
	./djpeg -dct int -ppm -outfile testoutp.ppm testprog.jpg
	./cjpeg -dct int -progressive -opt -outfile testoutp.jpg testimg.ppm
	./jpegtran -outfile testoutt.jpg testprog.jpg
	fc /b testimg.ppm testout.ppm
	fc /b testimg.bmp testout.bmp
	fc /b testimg.jpg testout.jpg
	fc /b testimg.ppm testoutp.ppm
	fc /b testimgp.jpg testoutp.jpg
	fc /b testorig.jpg testoutt.jpg


jaricom.o: jaricom.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcapimin.o: jcapimin.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcapistd.o: jcapistd.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcarith.o: jcarith.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jccoefct.o: jccoefct.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jccolor.o: jccolor.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcdctmgr.o: jcdctmgr.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jchuff.o: jchuff.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcinit.o: jcinit.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcmainct.o: jcmainct.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcmarker.o: jcmarker.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcmaster.o: jcmaster.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcomapi.o: jcomapi.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcparam.o: jcparam.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcprepct.o: jcprepct.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcsample.o: jcsample.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jctrans.o: jctrans.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdapimin.o: jdapimin.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdapistd.o: jdapistd.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdarith.o: jdarith.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdatadst.o: jdatadst.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h
jdatasrc.o: jdatasrc.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h
jdcoefct.o: jdcoefct.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdcolor.o: jdcolor.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jddctmgr.o: jddctmgr.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jdhuff.o: jdhuff.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdinput.o: jdinput.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdmainct.o: jdmainct.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdmarker.o: jdmarker.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdmaster.o: jdmaster.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdmerge.o: jdmerge.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdpostct.o: jdpostct.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdsample.o: jdsample.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdtrans.o: jdtrans.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jerror.o: jerror.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jversion.h jerror.h
jfdctflt.o: jfdctflt.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jfdctfst.o: jfdctfst.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jfdctint.o: jfdctint.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jidctflt.o: jidctflt.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jidctfst.o: jidctfst.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jidctint.o: jidctint.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jquant1.o: jquant1.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jquant2.o: jquant2.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jutils.o: jutils.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jmemmgr.o: jmemmgr.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jmemsys.h
jmemansi.o: jmemansi.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jmemsys.h
jmemname.o: jmemname.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jmemsys.h
jmemnobs.o: jmemnobs.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jmemsys.h
jmemdos.o: jmemdos.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jmemsys.h
jmemmac.o: jmemmac.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jmemsys.h
cjpeg.o: cjpeg.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h jversion.h
djpeg.o: djpeg.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h jversion.h
jpegtran.o: jpegtran.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h transupp.h jversion.h
rdjpgcom.o: rdjpgcom.c jinclude.h jconfig.h
wrjpgcom.o: wrjpgcom.c jinclude.h jconfig.h
cdjpeg.o: cdjpeg.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
rdcolmap.o: rdcolmap.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
rdswitch.o: rdswitch.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
transupp.o: transupp.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h transupp.h
rdppm.o: rdppm.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
wrppm.o: wrppm.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
rdgif.o: rdgif.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
wrgif.o: wrgif.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
rdtarga.o: rdtarga.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
wrtarga.o: wrtarga.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
rdbmp.o: rdbmp.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
wrbmp.o: wrbmp.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
rdrle.o: rdrle.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
wrrle.o: wrrle.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
