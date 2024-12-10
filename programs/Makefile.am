# Process w/ automake.  Or, autoreconf; make #
AUTOMAKE_OPTIONS = foreign

INCLUDES = -I. -I$(srcdir) -I$(prefix)/include -I$(prefix)/toolkit/include -DPROG_VERSION=\"$(PACKAGE)-$(VERSION)\"

LOCAL_LIB = ../.libs/libgv_utils.a
LDADD = @LIBS@ $(LOCAL_LIB) 

bin_PROGRAMS =  construct_time_in_seconds \
                get_gv_radar_site_info \
                get_site_info_from_raw_file \
                get_grid_origin

bin_SCRIPTS = $(perl_scripts)

perl_scripts = gv_utilities.pl

$(perl_scripts): Makefile
	@echo -n "Checking file $@ ... "
	@cp $@ $@.in
	@sed -e 's/\$PROG_VERSION[ ]*=.*/$PROG_VERSION = \"$(PACKAGE)-$(VERSION)\";/' \
		-e 's&#\!.*perl&#\! $(perlbin)&' \
		-e 's&(\@INC,.*&(\@INC, \"$(bindir)\"\)\;&' \
    < $@.in > $@.new
	@if cmp -s $@ $@.new; then \
       rm $@.new; \
       echo "$@ remains untouched."; \
    else \
       mv $@.new $@; \
       echo "substitutions made in $@."; \
    fi
	@rm -f $@.in


EXTRA_DIST = $(bin_SCRIPTS)

