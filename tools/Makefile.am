# Copyright (C) 2003 CSIRO Australia
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#  
#  - Redistributions of source code must retain the above copyright
#  notice, this list of conditions and the following disclaimer.
#  
#  - Redistributions in binary form must reproduce the above copyright
#  notice, this list of conditions and the following disclaimer in the
#  documentation and/or other materials provided with the distribution.
#  
#  - Neither the name of the CSIRO nor the names of its
#  contributors may be used to endorse or promote products derived from
#  this software without specific prior written permission.
#  
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
#  PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE ORGANISATION OR
#  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

## Process this file with automake to produce Makefile.in

AM_CFLAGS = -Wall -pedantic

bin_PROGRAMS = cmml-validate cmml-fix cmml-fortune cmml-timeshift

CMMLDIR = src
INCLUDES = -I${top_srcdir}/$(CMMLDIR)
LIBS = ${GETOPT_LIBS} ${EXPAT_LIBS} -lm -L${top_srcdir}/${CMMLDIR}
#${GETOPT_LONG_LIBS) 
CMML_LIBS = ../$(CMMLDIR)/libcmml.la

# cmml-validate:
cmml_validate_SOURCES = cmml-validate.c
cmml_validate_LDADD = ${CMML_LIBS}

# cmml-fix:
cmml_fix_SOURCES = cmml-fix.c
cmml_fix_LDADD = ${CMML_LIBS}

# cmml-fortune:
cmml_fortune_SOURCES = cmml-fortune.c
cmml_fortune_LDADD = ${CMML_LIBS}

# cmml-timeshift:
cmml_timeshift_SOURCES = cmml-timeshift.c
cmml_timeshift_LDADD = ${CMML_LIBS}
