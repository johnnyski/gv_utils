# Process w/ automake.  Or, autoreconf; make #
AUTOMAKE_OPTIONS = foreign
SUBDIRS = . doc programs
INCLUDES = -I. -I$(srcdir) -I$(prefix)/include -I$(prefix)/toolkit/include -DPROG_VERSION=\"$(PACKAGE)-$(VERSION)\"

datadir = $(prefix)/data
data_DATA = gv_radar_site_info.data

lib_LTLIBRARIES = libgv_utils.la

libgv_utils_la_LDFLAGS = -version-info 1:17
libgv_utils_la_SOURCES = \
                gv_file.c \
                gv_time.c \
                gv_string.c \
                gv_utilities.c \
                gv_site.c $(headers)

headers = gv_utils.h \
          gv_file.h \
          gv_site.h \
          gv_string.h \
          gv_time.h \
          gv_utilities.h

headersdir = $(prefix)/include
headers_DATA = $(headers)

EXTRA_DIST = CHANGES Copyright GPL $(data_DATA)

