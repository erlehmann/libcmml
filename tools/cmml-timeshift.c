/* Copyright (C) 2005 CSIRO Australia

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   
   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
   
   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
   
   - Neither the name of the CSIRO nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.
   
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
   PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE ORGANISATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>

#ifndef WIN32
#include <unistd.h>
#endif

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif

#include <cmml.h>

/**
 * \file
 * cmml-timeshift:
 * parses a CMML instance document and applies a timeshift to
 * all the clip tags.
 *
\verbatim
Usage: cmml-timeshift [options] filename
Apply a timeshift to all the clip tags.
 
Possible options:
  -s offset, --sec offset
                 Specify the offset in seconds to add to clip tags
  -o filename, --output filename
                 Specify the output filename. The file is written
                 to standard output by default.
  -h, --help     Display this help information
  -v, --version  Display version information
\endverbatim
 *
 */

/** the size of the print buffer */
#define BUFSIZE 100000

/*
#define DEBUG
*/

/**
 * outfile: defines FILE pointer to print output to
 */
static FILE * outfile;

/**
 * secs:    number of seconds to offset the clip start/end times
 */
static double secs;

/**
 * PrintUsage: prints out help on how to use this program
 *
 * \param prog the program's name
 */
static void
PrintUsage(char *prog) {
  fprintf(stderr, "Usage: %s [options] filename\n", prog);
  fprintf(stderr, "Apply a negative or positive time offset (in seconds) to a cmml file.\n\n");
  fprintf(stderr, "Possible options:\n");
#ifdef HAVE_GETOPT_LONG
  fprintf(stderr, "  -s offset, --sec offset\n");
  fprintf(stderr, "                 Specify the offset in seconds to add to clips\n");
  fprintf(stderr, "  -o filename, --output filename\n");
  fprintf(stderr, "                 Specify the output filename. The file is written\n");
  fprintf(stderr, "                 to standard output by default.\n");
  fprintf(stderr, "  -h, --help     Display this help information\n");
  fprintf(stderr, "  -v, --version  Display version information\n");
#else
  fprintf(stderr, "  -s offset\n");
  fprintf(stderr, "                 Specify the offset in seconds to add to clips\n");
  fprintf(stderr, "  -o filename\n");
  fprintf(stderr, "                 Specify the output filename. The file is written\n");
  fprintf(stderr, "                 to standard output by default.\n");
  fprintf(stderr, "  -h             Display this help information\n");
  fprintf(stderr, "  -v             Display version information\n");
#endif
  fprintf(stderr, "\nPlease report bugs to <libcmml-devel@cmis.csiro.au>.\n");
  exit(1);
}

/**
 * read_stream: the callback for a stream element
 *
 * \param cmml the CMML* handle in use
 * \param stream the stream element's content represented in a
 *        CMML_Stream*
 * \param user_data user defined data
 *
 * \returns 0 on success, 1 on error
 */
static int
read_stream (CMML * cmml, const CMML_Stream * stream, void * user_data) {
  char buf[BUFSIZE];
  CMML_Error * err;
  if ((err = cmml_get_last_error(cmml)) != NULL) {
    cmml_error_snprint(buf, BUFSIZE, err, cmml);
    fprintf(stderr, "cmml-timeshift: Parsing stream tag %s\n", buf);
    fprintf(stderr, "cmml-timeshift: Non-recoverable error\n");
    return -1;
  } else {
    /* print stream */
    cmml_stream_pretty_snprint (buf, BUFSIZE, (CMML_Stream *) stream);
    fprintf(outfile, "%s\n", buf);
  }
  return 0;
}

/**
 * read_head: the callback for a head element
 *
 * \param cmml the CMML* handle in use
 * \param head the head element's content represented in a
 *        CMML_Head*
 * \param user_data user defined data
 *
 * \returns 0 on success, 1 on error
 */
static int
read_head (CMML * cmml, const CMML_Head * head, void * user_data) {
  char buf[BUFSIZE];
  CMML_Error * err;
  if ((err = cmml_get_last_error(cmml)) != NULL) {
    cmml_error_snprint(buf, BUFSIZE, err, cmml);
    fprintf(stderr, "cmml-timeshift: Parsing head tag %s\n", buf);
    fprintf(stderr, "cmml-timeshift: Non-recoverable error\n");
    return -1;
  } else {
    cmml_head_pretty_snprint (buf, BUFSIZE, (CMML_Head *) head);
    fprintf(outfile, "%s\n", buf);
  }
  return 0;
}

/**
 * read_clip: the callback for a clip element
 *
 * \param cmml the CMML* handle in use
 * \param clip the clip element's content represented in a
 *        CMML_Clip*
 * \param user_data user defined data
 *
 * \returns 0 on success, 1 on error
 */
static int
read_clip (CMML * cmml, const CMML_Clip * clip, void * user_data) {
  char buf[BUFSIZE];
  CMML_Error * err;
  CMML_Clip * lclip;
  double newsecs;

  if ((err = cmml_get_last_error(cmml)) != NULL) {
    cmml_error_snprint(buf, BUFSIZE, err, cmml);
    fprintf(stderr, "cmml-timeshift: Parsing clip %s\n", buf);
    fprintf(stderr, "cmml-timeshift: Skipping clip\n\n");
    return -1;
  } else {
    lclip = cmml_clip_clone((CMML_Clip *)clip);

    /* shift clip's start time by secs */
    newsecs = cmml_sec_parse(lclip->start_time->tstr) + secs;
    cmml_time_free(lclip->start_time);
    lclip->start_time = cmml_time_new_secs (newsecs);

    /* check end time, too */
    if (lclip->end_time != NULL) {
      newsecs = cmml_sec_parse(lclip->end_time->tstr) + secs;
      cmml_time_free(lclip->end_time);
      lclip->end_time = cmml_time_new_secs (newsecs);
    }

    /* print clip if beyond start time */
    if (lclip->start_time->t.sec >= 0.0) {
      cmml_clip_pretty_snprint (buf, BUFSIZE, lclip);
      fprintf(outfile, "%s\n", buf);
    }

    cmml_clip_destroy(lclip);
  }

  return 0;
}

/**
 * main function of cmml-timeshift, which opens the CMML file, seeks to
 * any given offsets, registers the callbacks, and then steps through
 * the file in chunks of BUFSIZE size, during which the callbacks get
 * activated as the relevant elements get parsed.
 */
int main(int argc, char *argv[])
{
  char *pathfile = NULL;
  int i;
  char buf[BUFSIZE];
  CMML * doc;
  CMML_Error * err;
  CMML_Preamble * pre;
  long n = 0;
  char * outfilename = NULL;
  buf[0]='\0';

  /* default outfile to stdout and secs to 0 */
  outfile = stdout;
  secs = 0;

  while (1) {
    char * optstring = "hvos:";

#ifdef HAVE_GETOPT_LONG
    static struct option long_options[] = {
      {"help",no_argument,0,'h'},
      {"version",no_argument,0,'v'},
      {"output", required_argument,0,'o'},
      {"offset", required_argument,0,'s'},
      {0,0,0,0}
    };

    i = getopt_long(argc, argv, optstring, long_options, NULL);
#else
    i = getopt(argc, argv, optstring);
#endif

    if (i == -1) break;
    if (i == ':') PrintUsage(argv[0]);

    switch (i) {
    case 'h': /* help */
      PrintUsage(argv[0]);
      break;
    case 'v': /* version */
      fprintf(stdout, "cmml-timeshift version " VERSION "\n");
      fprintf(stdout, "# cmml-timeshift, Copyright (C) 2005 CSIRO Australia www.csiro.au ; www.annodex.net\n");
      break;
    case 'o': /* output */
      outfilename = optarg;
      break;
    case 's': /* offset */
      /* parse seconds specification */
      secs = atof(optarg);
      break;
    default:
      break;
    }
  }

  /* more arguments that were not parsed */
  if (optind > argc) {
    PrintUsage(argv[0]);
  }

  /* open output file */
  if (outfilename != NULL) {
    if ((outfile = fopen (outfilename, "wb")) == NULL) {
      fprintf (stderr, "%s: %s: Error opening %s for writing\n", 
	       argv[0], pathfile, outfilename);
      exit (1);
    }
  }

  /* no filename given? */
  if (optind == argc) {
    pathfile = "-";
  } else {
    pathfile = argv[optind++];
  }

  /* try open file for parsing and setup cmml parsing */
  errno=0;
  if (strcmp (pathfile, "-") == 0) {
    doc = cmml_new (stdin);
  } else {
    doc = cmml_open (pathfile);
  }

  if (doc == NULL) {
    if (errno == 0) {
      fprintf(stderr, "%s: %s: CMML error opening file\n", argv[0], pathfile);
    } else {
      fprintf(stderr, "%s: %s: %s\n", argv[0], pathfile, strerror(errno));
    }
    PrintUsage(argv[0]);
  }

  /* turn on sloppy parsing */
  cmml_set_sloppy(doc, 1);

  /* print preamble */
  pre = cmml_get_preamble(doc);
  cmml_preamble_snprint(buf, BUFSIZE, pre);
  fprintf(outfile, "%s\n", buf);

  /* register callbacks */
  cmml_set_read_callbacks (doc, read_stream, read_head, read_clip, NULL);

  /* read document frame-wise and check against CMML.dtd */
  while ((n = cmml_read (doc, BUFSIZE)) > 0) {
    /* if error reading, print and exit */
    if ((err = cmml_get_last_error(doc)) != NULL && err->type != CMML_EOF) {
      char *filename;
      filename = (strrchr(pathfile, '/') == NULL ? pathfile
		  : strrchr(pathfile, '/')+1);
      cmml_error_snprint(buf, BUFSIZE, err, doc);
      fprintf (stderr, "%s:%s\n", filename, buf);
      goto cleanup;
    }
  }

  err = cmml_get_last_error(doc);
  if (err!=NULL && err->type != CMML_EOF) {
      char *filename;
      filename = (strrchr(pathfile, '/') == NULL ? pathfile
		  : strrchr(pathfile, '/')+1);
      cmml_error_snprint(buf, BUFSIZE, err, doc);
      fprintf (stderr, "%s:%s\n", filename, buf);
      goto cleanup;
  }

  /* write end tag */
  fprintf(outfile, "</cmml>\n");
  
 cleanup:
  /* clean up */
  cmml_close(doc);

  return 0;
}
