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

noinst_PROGRAMS = cmml-parse cmml-write \
		  cmml-seek-clip cmml-seek-offset cmml-seek-utc \
		  cmml-uri-file
CMMLDIR = src
INCLUDES = -I${top_srcdir}/$(CMMLDIR)
LIBS = -lexpat -lm -L${top_srcdir}/$(CMMLDIR)
CMML_LIBS = ../$(CMMLDIR)/libcmml.la

# cmml-parse:
cmml_parse_SOURCES = cmml-parse.c
cmml_parse_LDADD = ${CMML_LIBS}

# cmml-write:
cmml_write_SOURCES = cmml-write.c
cmml_write_LDADD = ${CMML_LIBS}

# cmml-seek-clip:
cmml_seek_clip_SOURCES = cmml-seek-clip.c
cmml_seek_clip_LDADD = ${CMML_LIBS}

# cmml-seek-offset:
cmml_seek_offset_SOURCES = cmml-seek-offset.c
cmml_seek_offset_LDADD = ${CMML_LIBS}

# cmml-seek-utc:
cmml_seek_utc_SOURCES = cmml-seek-utc.c
cmml_seek_utc_LDADD = ${CMML_LIBS}

# cmml-uri-file:
cmml_uri_file_SOURCES = cmml-uri-file.c
cmml_uri_file_LDADD = ${CMML_LIBS}

