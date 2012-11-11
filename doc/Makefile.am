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


AUTOMAKE_OPTIONS = foreign

CLEANFILES = doxygen-build.stamp debug.txt

EXTRA_DIST = Doxyfile.in cmml.dtd \
             cmml_dox.h cmml_2_0.dtd cmml_2_1.dtd \
             cmml-validate.1.sgml.in cmml-validate.1 \
             cmml-fix.1.sgml.in cmml-fix.1 \
             cmml-fortune.6.sgml.in cmml-fortune.6 \
             cmml-timeshift.1.sgml.in cmml-timeshift.1

DOXY_INPUT = ${top_srcdir}/src/cmml.h \
             ${top_srcdir}/tools/cmml-validate.c \
             ${top_srcdir}/tools/cmml-fix.c \
             ${top_srcdir}/tools/cmml-fortune.c \
             $(top_srcdir)/tools/cmml-timeshift.c \
             ${srcdir}/cmml_dox.h


SUBDIRS = .

if HAVE_DOCBOOKTOMAN
man_MANS = cmml-validate.1 cmml-fix.1 cmml-timeshift.1 cmml-fortune.6
endif

# do not delete - make dist needs it
doc_DATA = 

# for make
docdir = ${datadir}/doc/@PACKAGE@
inst_docdir = $(datadir}/doc/@PACKAGE@

# avoid having to use xsltproc:
#%.1: %.1.xml
#	xsltproc /usr/share/sgml/docbook/stylesheet/xsl/nwalsh/manpages/docbook.xsl $<

# another possibility is the use of docbook2man:
#%.1: %.1.sgml
#	docbook2man $< > $@
# but it creates weird stuff in line 3

if HAVE_DOCBOOKTOMAN
%.1: %.1.sgml
	docbook-to-man $< > $@
%.6: %.6.sgml
	docbook-to-man $< > $@
else
%.1: %.1.sgml
%.6: %.6.sgml
endif

if HAVE_DOCBOOK2HTML
%.1.html: %.1.sgml
	-docbook2html $<
	mv index.html $@
%.6.html: %.6.sgml
	-docbook2html $<
	mv index.html $@
else
%.1.html: %.1.sgml
%.6.html: %.6.sgml
endif

html: 
	cmml-validate.1.html
	cmml-fix.1.html
	cmml-timeshift.1.html
	cmml-fortune.6.html

all: doxygen-build.stamp

if HAVE_DOXYGEN
doxygen-build.stamp: Doxyfile ${DOXY_INPUT}
	@echo "*** Running doxygen ***"
	doxygen
	if test -f $(top_srcdir)/doc/debug.txt; then rm -f $(top_srcdir)/doc/debug.txt; fi
	touch doxygen-build.stamp
else
doxygen-build.stamp:
	echo "*** Warning: Doxygen not found; documentation will not be built."
	touch doxygen-build.stamp
endif

# The install targets don't copy whole directories so we have to
# handle 'html/' specially:

# "make dist" -- this seems unnecessary now
dist_docdir = $(distdir)/libcmml/
dist-hook:
	if test -d libcmml; then \
	  mkdir $(dist_docdir); \
	  for dir in libcmml/*; do \
	    b=`basename $$dir`; \
	    if test $$b != "CVS"; then \
	      if test -d $$dir; then \
	        mkdir $(dist_docdir)/$$b; \
	        for f in $$dir/*; do \
	          cp -p $$f $(dist_docdir)/$$b; \
	        done \
	      fi \
	    fi \
	  done \
	fi

# "make install"
install-data-local: doxygen-build.stamp
	$(mkinstalldirs) $(DESTDIR)$(docdir)
	if test -d libcmml; then \
	  for dir in libcmml/*; do \
	    if test -d $$dir; then \
	      b=`basename $$dir`; \
	      $(mkinstalldirs) $(DESTDIR)$(docdir)/$$b; \
	      for f in $$dir/*; do \
	        $(INSTALL_DATA) $$f $(DESTDIR)$(docdir)/$$b; \
	      done \
	    fi \
	  done \
	fi

# "make uninstall" 
uninstall-local:
	if test -d $(DESTDIR)$(docdir); then \
	rm -rf $(DESTDIR)$(docdir); \
	fi

maintainer-clean-local:
	if test -d libcmml; then rm -rf libcmml; fi
	if test -f doxygen-build.stamp; then rm -f doxygen-build.stamp; fi
	if test -f $(top_srcdir)/doc/debug.txt; then rm -f $(top_srcdir)/doc/debug.txt; fi
if HAVE_DOCBOOKTOMAN
	if test -f cmml-validate.1; then rm cmml-validate.1; fi
	if test -f cmml-fix.1; then rm cmml-fix.1; fi
	if test -f cmml-timeshift.1; then rm cmml-timeshift.1; fi
	if test -f cmml-fortune.6; then rm cmml-fortune.6; fi
endif

clean-local:
	if test -d libcmml; then rm -rf libcmml; fi
	if test -f doxygen-build.stamp; then rm -f doxygen-build.stamp; fi
	if test -f $(top_srcdir)/doc/debug.txt; then rm -f $(top_srcdir)/doc/debug.txt; fi
if HAVE_DOCBOOKTOMAN
	if test -f cmml-validate.1; then rm cmml-validate.1; fi
	if test -f cmml-fix.1; then rm cmml-fix.1; fi
	if test -f cmml-timeshift.1; then rm cmml-timeshift.1; fi
	if test -f cmml-fortune.6; then rm cmml-fortune.6; fi
endif
