#! /usr/bin/perl
#-*-perl-*-
#
# gv_utilities.pl
#       Perl's common code for gv.
#
# Requires:
#  construct_time_in_seconds
#  get_radar_name_from_file
#  $GVS_DATA_PATH/gv_radar_site_info.data
#
#--------------------------------------------------------------------------
#  By:
#       Thuy Nguyen
#       Science Systems and Applications, Inc. (SSAI)
#       NASA/TRMM Office
#       nguyen@trmm.gsfc.nasa.gov
#       July 15, 1997
#
###########################################################################
#
require  "timelocal.pl";
@path = split(/\//, $0);
$this_prog = $path[$#path];
$curr_dir = `pwd`;
chop $curr_dir;    # Remove \r
$curr_dir = $curr_dir."/";
$save_curr_dir = $curr_dir;

$SIG{'INT'} = 'doexit_handler';
$SIG{'KILL'} = 'doexit_handler';
$SIG{'STOP'} = 'doexit_handler';
$SIG{'SEGV'} = 'doexit_handler';
$SIG{'ILL'} = 'doexit_handler';
$SIG{'FPE'} = 'doexit_handler';

# Define exit codes;
# Note: 1 is reserved for application
$OUT_OF_SPACE_CODE = 2;
$SEVERE_ERROR_CODE = -1;
$MINOR_ERROR_CODE = -3;
$INTER_CODE = -2;
$SUCCESS_CODE = 0;

######################################################################
#                                                                    #
#                          ignore_catched_signals                              #
#                                                                    #
######################################################################
sub ignore_catched_signals {

	$SIG{'INT'} = 'ignore';
	$SIG{'KILL'} = 'ignore';
	$SIG{'STOP'} = 'ignore';
	$SIG{'SEGV'} = 'ignore';
	$SIG{'ILL'} = 'ignore';
	$SIG{'FPE'} = 'ignore';
}

######################################################################
#                                                                    #
#                          interrupted                               #
#                                                                    #
######################################################################
sub interrupted {
	local($stat) = @_;
	return 1 if ($stat == $INTER_CODE || $stat ==254);
	return 0;
}

######################################################################
#                                                                    #
#                               is_severe_error                      #
#                                                                    #
######################################################################
sub is_severe_error {
	local($stat) = @_;
	return 1 if ($stat == $SEVERE_ERROR_CODE || $stat ==255);
	return 0;
}

######################################################################
#                                                                    #
#                               out_of_space                         #
#                                                                    #
######################################################################
sub out_of_space {
	local($stat) = @_;
	return 1 if ($stat == $OUT_OF_SPACE_CODE);
	return 0;
}

######################################################################
#                                                                    #
#                              is_minor_error                        #
#                                                                    #
######################################################################
sub is_minor_error {
	local($stat) = @_;
	return 1 if ($stat == $MINOR_ERROR_CODE || $stat ==253);
	return 0;
}

######################################################################
#                                                                    #
#                       get_site_name_from_uf_file                   #
#                                                                    #
######################################################################
sub get_site_name_from_uf_file {
	local($uf_file) = @_;
	# Get site name from the uf file.
	return "unknown" if $uf_file eq "";
	open(SITE, "get_radar_name_from_file NON_HDF $uf_file|") || 
		return "unknown";
	local(@lines) = <SITE>;
	$lines[0] =~ s/\n$//;
    close(SITE);
	return $lines[0];
}

######################################################################
#                                                                    #
#                          construct_time                            #
#                                                                    #
######################################################################
sub construct_time {
	local($hr, $min, $sec, $mon, $day, $yr, 
		  *date_str, *time_str, *time_seconds) = @_;
	# Create date_str: mm/dd/yyy
    #        time_str: hh:mm:ss
    #        time_seconds: time in seconds.

	local($time_seconds) = &date_time2seconds($hr, $min, $sec, $mon, $day, $yr);
	#Change 2 digit year to 4 digit year */
	if ($yr < 1000) {
		if ($yr > 60) {
			$yr += 1900;
		}
		else {
			$yr += 2000;
		}
	}
	$date_str = sprintf("%2.2d/%2.2d/%4.4d", $mon, $day,$yr);
	$time_str = sprintf("%2.2d:%2.2d:%2.2d", $hr, $min,$sec);

}


######################################################################
#                                                                    #
#                  get_date_from_granule_product_filename            #
#                                                                    #
######################################################################
sub get_date_from_granule_product_filename {
	local($fname, *mon, *day, *yr) = @_;
	# fname: [pathname]4-char.yymmdd.granule#.site.ver#.HDF[.gz|.Z]
	# Return yr in 4-digit.
	return if ($fname eq "");
	($yr, $mon, $day) = $fname =~ /.*\w\w\w\w\.(\d\d)(\d\d)(\d\d)\..+HDF.*/;
	#Change 2 digit year to 4 digit year */

	if ($yr < 100) {
		if ($yr > 60) {
			$yr += 1900;
		}
		else {
			$yr += 2000;
		}
	}
}


######################################################################
#                                                                    #
#               get_time_from_singlevos_product_filename             #
#                                                                    #
######################################################################
sub get_time_from_singlevos_product_filename {
	local($vos_ascii_fname, *file_date_str, *file_time_str, 
		  *file_time_secs) = @_;
	# Get time from the file name and store it in string and seconds forms.
	# Filename: 2A53.<yymmdd>.<granule#>.<site>.<ver#>.<hhmm>.ascii

	return 0 if $vos_ascii_fname eq "";
	local($yr, $mon, $day, $hr, $min) = $vos_ascii_fname =~ /.*[23]A5.\.(\d\d)(\d\d)(\d\d).+(\d\d)(\d\d).+/;
	local($sec) = 0;
	
	do construct_time($hr, $min, $sec, $mon, $day,
					  $yr, *file_date_str, *file_time_str, *file_time_secs);

} # get_time_from_singlevos_product_filename

######################################################################
#                                                                    #
#                          date2julian_day                           #
#                                                                    #
######################################################################
sub date2julian_day {
	local($mon, $day, $yr, *jday) = @_;
	# Convert date to julian day.
	# yr is a 4-digit number.
	return if ($yr < 1900 || $mon < 1 || $day < 1);
	local(@days_nonleap_year) =  (0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365);
	local (@days_leap_year) = (0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366);
	
	local(@days_list) = @days_nonleap_year;
	# Set to leap year list if it's a leap year.
	@days_list = @days_leap_year if (($yr / 4) == 0);
	local($i);
	$jday = $day + $days_list[$mon - 1];
}

######################################################################
#                                                                    #
#                         date_time2seconds                          #
#                                                                    #
######################################################################
sub date_time2seconds {
	# Return time in seconds.
    local($hr, $min, $sec, $mon, $day, $yr) = @_;

	local($cmd) = "construct_time_in_seconds $hr $min $sec $mon $day $yr|";
	open(TIME_SECS, $cmd) || return 0;
	$time_seconds = <TIME_SECS>;
	close(TIME_SECS);
	chop $time_seconds if $time_seconds =~ /\n|\r/;

	return $time_seconds;
}

######################################################################
#                                                                    #
#                        date_time_strs2seconds                      #
#                                                                    #
######################################################################
sub date_time_strs2seconds {
	# Return time in seconds where
    # date_str: yyyy-mm-dd
	# time_str: hh:mm:ss[.sss] -- [.sss] will be discarded.
    # 
	local($date_str, $time_str) = @_;

	local($yr, $mon, $day) = $date_str =~ /(\d+)\-(\d+)\-(\d+)/;
	local($hr, $min, $sec) = $time_str =~ /(\d+):(\d+):(\d+)/;
	local($time_sec) = &date_time2seconds($hr, $min, $sec, $mon, $day, $yr);
	return $time_sec;
}

######################################################################
#                                                                    #
#                        date_time_strs2seconds2                     #
#                                                                    #
######################################################################
sub date_time_strs2seconds2 {
	# Return time in seconds where
    # date_str: m[m]-d[d]-yy[yy]
	# time_str: hh:mm[:ss.sss] -- [:ss.sss] will be discarded.
    # 
	local($date_str, $time_str) = @_;

	local($mon, $day, $yr) = $date_str =~ /(\d+)\-(\d+)\-(\d+)/;
	local($hr, $min) = $time_str =~ /(\d+):(\d+)/;
	local($sec) = 0;
	local($time_sec) = &date_time2seconds($hr, $min, $sec, $mon, $day, $yr);
	return $time_sec;
}

sub time_seconds2date_time_strs {
	# Construct date_str: mm/dd/yyyy
    #           time_str: hh:mm:ss

	local($time_sec, *date_str, *time_str) = @_;
	local($sec, $min, $hr, $day, $mon, $yr) = localtime($time_sec);
	$mon++;
	$yr += 1900;
	$date_str = sprintf("%2.2d/%2.2d/%4.4d", $mon, $day,$yr);
	$time_str = sprintf("%2.2d:%2.2d:%2.2d",$hr,$min,$sec);
}

######################################################################
#                                                                    #
#                        get_current_date_time                       #
#                                                                    #
######################################################################
sub get_current_date_time {
	local(*date_str, *time_str) = @_;

	# Get current date time.
    # date: mm/dd/yyyy
    # time: hh:mm:ss
	time_seconds2date_time_strs(time, *date_str, *time_str);

} 

######################################################################
#                                                                    #
#                        change2absolute_filename                      #
#                                                                    #
######################################################################
sub change2absolute_filename {
	local($pref_dir, $fname) = @_;

	# Note: hp, sgi, linux understand: /usr/data/../data/gvs.dat
	return $fname if ($fname eq "");
	# Remove leading blank for fname
	$fname =~ s/^\s+//;
	# Remove trailing blank for pref dir
	$pref_dir =~ s/\s+$//;
	# Change  './[fname]' to $pref_dir/[fname]
	$fname =~ s/(^\.\/)/$pref_dir\//;
	# Change fname to $pref_dir/fname

    $fname = $pref_dir."/".$fname if ($fname =~ /^[^\/]/);
	return $fname;
}

######################################################################
#                                                                    #
#                        get_gvs_dir_name                            #
#                                                                    #
######################################################################
sub get_gvs_dir_name {
	local($product, $tape_id) = @_;
	
	return "gvs_" . $product . "_" . $tape_id."/";
}

######################################################################
#                                                                    #
#                        get_radar_site_info_fname                    #
#                                                                    #
######################################################################
sub get_radar_site_info_fname {
	# default is from $GVS_DATA_PATH
	local($data_path) = $ENV{'GVS_DATA_PATH'};
	if ($data_path eq "") { # Pick a default.
		$data_path = "/usr/local/trmm/GVBOX/data";
	}
	return $data_path."/gv_radar_site_info.data";
} # get_radar_site_info_fname


######################################################################
#                                                                    #
#                   gv_utils_get_radar_num_for_site_name             #
#                                                                    #
######################################################################
sub gv_utils_get_radar_num_for_site_name {
	local($tsdis_name) = @_;
	# Find radar num for the specified site from 
    # $GVS_DATA_PATH/gv_radar_site_info.data
	# Return -1 if can't find sitename.

	local($radar_site_fname) = &get_radar_site_info_fname();
	# Line's format (each field is separted with ','): 
    #  TSDIS  WSR88D                        Radar            SPRINT
    #  Name,    ID,   City, State, Country, Name, Lat, Lon, radar_ID

	local($radar_num) = -1;
	open(SITE_FILE, $radar_site_fname) || 
		(print STDERR "$this_prog: Can't open file <$radar_site_fname>.\n" && return -1);
	# Site name is case insentive.
	local($lowercases_tsdis_name = $tsdis_name) =~ y/A-Z/a-z/;
	while (<SITE_FILE>) {
		next if ($_ =~ /^#/);
		$_ =~ s/\n$//;    # REmove \n.
		@line = split(',',$_);
		local($lowercases_tsdis_name2 = $line[0]) =~ y/A-Z/a-z/;
		# Remove leading and trailing blanks
	    $lowercases_tsdis_name2 =~ s/^\s+//;
	    $lowercases_tsdis_name2 =~ s/\s+$//;
		if ($lowercases_tsdis_name eq $lowercases_tsdis_name2) {
			# Get radar_num and remove leading blanks.
			($radar_num = $line[$#line]) =~ s/^\s+//;
			last;
		}
	}
	close(SITE_FILE);
	return $radar_num;
} #get_radar_num_for_site_name



1;

