#!/usr/bin/perl 
##########################################################################
# from LA-UR-04-2506
#
# mcnp_pstudy - perform MCNP parameter studies
#        - this version substitutes variable values by values 
#          read from the file set_variables.dat  fcg 1/10/2007
#
#        - FB Brown, 6/2003, revised 2/2004
#
#    References:  LA-UR-04-0499,  LA-UR-04-2506
#
# Variable definition lines:
#
#     c @@@  symbol = val
#     c @@@  symbol = val1 val2 ... valn
#     c @@@  symbol = normal  n  ave sig
#     c @@@  symbol = uniform n  low high
#     c @@@  symbol = equidistant n low high --- not yet implemented !!!
#     c @@@  symbol = repeat n
#     c @@@  symbol = ( expression )
#     c @@@  constraint = ( logical-expression )
#
#   notes:
#     (1) For values, strings with embedded whitespace not allowed
#     (2) Be careful about length of lines after substitution
#     (3) "expression" may include arithmetic, previously defined 
#         symbols, parentheses. The outer set of parenthese is
#         REQUIRED. The result must be a single scalar value.
#     (4) "constraint" is a literal. The logical-expression may
#         contain previously defined symbols, parentheses, logical
#         operators, & constants. The outer set of parentheses
#         is REQUIRED. The result must be true or false.
#     (5) "repeat n" creates a list of integers 1..n. This is
#         convenient for creating a dummy variable for the 
#         purpose of repeating a calculation
#     (6) The counts "n" can be integers or a previously defined
#         symbol which is a single integer. Other parameters
#         (ave sig low high val1..valn) can be defined in terms
#         of any previously-defined symbol.
#     (7) Any of the "c @@@" lines can be continued by ending the
#         line with a backslash character. The continuation lines
#         should start with "c @@@".
#
# If -setup is present, job files will be created in a filetree as
# follows:
#            $JOBDIR/$CASE$N/$JOB
#
#       (1) $JOBDIR is created if it does not exist
#       (2) $N has up to $CASENUM digits, padded on the left with '0'
# 
# pstudy command line options: 
#
#      Note:  these can also be embedded in the MCNP input file,
#             on lines beginning with    'c @@@ OPTIONS =   '
#
#   -jobdir d      = use directory 'd' as the $JOBDIR. Can be absolute
#                    or relative. Created if doesn't exist
#   -case   c      = use 'c' for $CASE in creating case names.
#   -casenum n     = use 'n' digits appended to the case name,
#                    eg, 001, 002, ... for n=3
#   -job  j        = the mcnp input file created in each directory 
#                    will be called 'j'.  Default is 'inp'.
#   -i  infile     = the template input file (with substitution commands
#                    for pstudy) is file 'infile'
#   -log logfile   = use 'logfile' for saving the summary results
#   -mcnp  m       = 'm' is used as the run command. 'm' may be
#                    the location of mcnp or may be a string of 
#                    commands, such as  'mpirun -n 4 ~fbrown/bin/mcnp'
#   -mcnp_opts m   = 'm' is appended to the mcnp run line. May be a
#                    series of options, eg,  'o=outx  tasks 4'
#   -bsub_opts b   = 'b' is the set of options for a bsub command.
#                    Only used if the -submit option is invoked.
#   -outer
#   -inner         = If -outer is selected (the default), then jobs are 
#                    created for all possible combinations of the 
#                    parameters handled by pstudy. If -inner is selected,
#                    the various parameters are substituted serially.
#                    For -outer, the number of jobs is the product of
#                    the number of times each parameter specified. For
#                    -inner, the number of jobs is the max of the times
#                    any parameter is specified.
#   -debug
#   -verbose       = print extra output.
#
#   -setup         = setup all mcnp input files. (Don't run or submit
#                    unless -submit or -run is specified.)
#   -submit        = submit the jobs to LSF using bsub.
#   -resubmit      = resubmit any jobs which did not run to completion
#                    and produce a mctal file, to LSF using bsub.
#   -run           = run the jobs now, on the current machine.
#   -collect       = collect results from each job & average them.
#   -avgonly       = only print the average results
#   -symlink 'files...'
#                  = create symbolic links in each case directory to the
#                    listed files. Can use Unix wildcards in filenames.
#                    Separate files in list by a blank. Must use double
#                    or single quotes around list if more than 1 file.
# 
# Typical usage:
#
#   mcnp_pstudy.pl -i godiva   \
#      -mcnp 'mpirun -np 4 /usr/local/codes/mcnp/IRIX64/mcnp.mpi' \
#      -mcnp_opts 'tasks 4' -bsub_opts '-n 4 -W 10'   -setup -submit
#
#   mcnp_pstudy.pl -i godiva  -setup
#   mcnp_pstudy.pl -i godiva  -run  -collect
# 
#
# Sample input file
#
#      Godiva (title line)
#      c
#      c @@@ # godiva problem with varying density & radius
#      c @@@ OPTIONS =  -outer
#      c @@@ OPTIONS =  -log gd.txt  
#      c @@@ OPTIONS =  -jobdir /Users/fbrown/GD2
#      c
#      c @@@  DEN = 18.  18.74   19.
#      c @@@  RAD = 8.   8.741  9.
#      c
#      c below is normal MCNP input, with parameters
#      c
#      1    1  -DEN  -1
#      2    0           1
#       
#      1    so RAD
#       
#      kcode 1000 1.0 10 40
#      ksrc  0. 0. 0.
#      imp:n 1 0
#      m1    92235 -94.73  92238 -5.27
#      prdmp 2j 1 99999999
#
#   Using the default -outer, 9 cases will be set up & run --
#   all combinations of DEN & RAD.  If -inner is used, then only 3
#   cases would be run --   18. & 8.,  18.74 & 8.741,  19. & 9.
#
##########################################################################
#### srand(12345);         ### for repeatability in testing

# kbat: TMath::Trig added
use Math::Trig;

##### environment info
$USER = $ENV{'USER'};
$OS   = `uname`;     chomp($OS);
$HOST = `hostname`;  chomp($HOST);
$WORK = `pwd`;       chomp($WORK);

    # special treatment for Bluemountain scratch
    if( $OS=~/IRIX/ && $HOST=~/^b\d+/ && $WORK=~/^\/scratch\// ) {
      $WORK = "/n/$HOST$WORK";
    }


##### global options - defaults
$JOBDIR    = '.';
$CASE      = 'case';
$CASENUM   = 3;
$JOB       = 'inp';
$IN        = '-';
$LOG       = 'log.txt';
#$MCNP      = "/Users/fbrown/mcnp/MCNP6_DEV/mcnp6/Source/src/mcnp6";
$MCNP      = "mcnpx";

if ($OS=~/CYGWIN/ ) {
    $MCNP = "mcnpx.exe";
}

# die("*** change the line above to the location of MCNP on your system\n");
$MCNP_OPTS = '';
$BSUB_OPTS = '';
$SETUP     = '';
$SUBMIT    = '';
$RESUBMIT  = '';
$RUN       = '';
$COLLECT   = '';
$AVGONLY   = '';
$DEBUG     = '';
$OUTER     = 'yes';
undef @SYMLINKS;

##### get global options from command-line args
  &get_global_opts( @ARGV );

##### parse the input to pick up substitution parameters & tally identifiers
  &parse_input;
##### replaces symbol values by values read from set_variables.dat
  &replace_sym;
##### 
  &parse_tally_info;
  &parse_symlink_info;

##### get absolute path for $JOBDIR, then create $JOBDIR if needed
  if( $JOBDIR !~ /^\// ) { $JOBDIR = "$WORK/$JOBDIR"; }

      # special treatment for Bluemountain scratch
      if( $OS=~/IRIX/ && $HOST=~/^b\d+/ && $JOBDIR=~/^\/scratch\// ) {
        $JOBDIR = "/n/$HOST$$JOBDIR";
      }

  if( $SETUP && ! -d $JOBDIR ) {
    print "***** creating $JOBDIR\n";
    mkdir $JOBDIR, 0750 || die("***** can't create $JOBDIR\n");
  }

##### open the log file
  open(LOG, ">>$LOG");

##### determine the total number of cases & setup subdir names
  $NSYM   = @SYM;
  if( $OUTER ) {
    $NCASES = 1;
    for $s (@SYM) { $NCASES *= &symcon($data{$s}[2]); }
  }
  else {
    $NCASES = 0;
    for $s (@SYM) {
      if( &symcon($data{$s}[2])>$NCASES ) { $NCASES = &symcon($data{$s}[2]); }
    }
  }
  print "\n***** Total number of cases = $NCASES\n";

  # adjust $CASENUM if needed
  my $n = length("$NCASES");
  if( $n>$CASENUM ) { 
    print "***** CASENUM changed from $CASENUM to $n\n";
    $CASENUM = $n; 
  }

  # set sub-directory names for cases
  my $k;
  for( $k=1; $k<=$NCASES; $k++ ) {
    $CASENAMES[$k] = $CASE . substr( "0000000000" . $k, -$CASENUM );
  }

##### get the case parameters
  if( $SETUP ) {
    $KASE = 0;
    while( &next_case <= $NCASES ) {
      &get_case_params;
    }
  }

##### set up the job files for all cases
  if( $SETUP ) {
    print LOG "\nSETUP\n\n";
    $KASE = 0;
    while( &next_case <= $NCASES ) {
  
      # create the subdir for case
      $CASEDIR = "$JOBDIR/$CASENAMES[$KASE]";
      if( ! -d "$CASEDIR" ) {
        mkdir  "$CASEDIR", 0750 || die("***** can't create case dir $CASEDIR\n");
      }
      $OUT = "$CASEDIR/$JOB";

      # perform substitutions & create job file for case
      &substitute_case_params;

      # create symbolic links in case subdirectory
      foreach $f (keys(%symlink_files)) {
        if( ! -e "$CASEDIR/$f" ) { symlink $symlink_files{$f}, "$CASEDIR/$f"; }
      }

      # logfile entries
      print LOG "$CASENAMES[$KASE]\t$CASEDIR\n";
      for( $k=0; $k<@SYM; $k++) {
        print LOG "\t$SYM[$k]\t$VAL[$KASE][$k]\n";
      }
    }
  }

##### run or submit all cases in order
if( $RUN || $SUBMIT || $RESUBMIT ) {
  $KASE = 0;
  while( &next_case <= $NCASES ) {

    $CASEDIR = "$JOBDIR/$CASENAMES[$KASE]";

    if( $RESUBMIT ) {
      if( -s "$JOBDIR/$CASENAMES[$KASE]/mctal" ) { next; } 
      print "\n\t\t***** resubmit *****\n";
    }

    print "\n=====> Case $KASE\n";
    print   "=====>    dir  = $CASEDIR\n";
    print   "=====>    mcnp = $MCNP\n";
    print   "\n";

    # kbat:
#    system("cd $CASEDIR && pwd && echo 'file = $JOB' > phits.in");
#    system("cp *.phits $CASEDIR");


    if( $RUN ) {
      # run the case
#      system( "cd $CASEDIR
#               rm -fv mct* run* out*
#               $MCNP  i=$JOB  $MCNP_OPTS" );
       system("cd $CASEDIR && $MCNP")
    }
    elsif( $SUBMIT || $RESUBMIT ) {
      # submit job to LSF queuing system
#	system("cd $CASEDIR && echo file = inp > phits.in"); # kbat
      system( "echo bsub -J $CASENAMES[$KASE] $BSUB_OPTS <<__END__
               cd $CASEDIR
               rm -fv mct* run* out*
               $MCNP  i=$JOB  $MCNP_OPTS
__END__
              " );
    }
  }
}

##### collect results
if( $COLLECT ) {
  # collect results & print
  print LOG "\nRESULTS\n\n";
  $KASE = 0;
  while( &next_case <= $NCASES ) {
    $CASEDIR = "$JOBDIR/$CASENAMES[$KASE]";
    # get keff & tallies from mctal file
    &collect_results;
  }
  &print_results;
}


exit;
#-------------------------------------------------------------------------
sub get_global_opts() {
  # scan the arg list for keywords & set global options
  my @args = @_;
  # first, put any strings back together
  { my $k, $j;
    for( $k=0; $k<@args; ) {
      if( $args[$k] =~ /^([\'\"])/ ) {
        $j = $k+1;
        while( $args[$k] !~ /$1$/  && $j<@args ) {
          $args[$k] .= ' '.$args[$j];
          $args[$j] = '--';
          $j++;
        }
        $args[$k] = substr( $args[$k], 1,-1 );
        $k = $j+1;
      }
      else {
        $k++;
      }
    }
  }
  # scan the args & set globals
  my $arg;
  while(   $arg = shift @args  ) {
    if(    $arg eq '-jobdir'    )  { $JOBDIR    = shift @args; }
    elsif( $arg eq '-case'      )  { $CASE      = shift @args; }
    elsif( $arg eq '-casenum'   )  { $CASENUM   = shift @args; }
    elsif( $arg eq '-job'       )  { $JOB       = shift @args; }
    elsif( $arg eq '-i'         )  { $IN        = shift @args; }
    elsif( $arg eq '-log'       )  { $LOG       = shift @args; }
    elsif( $arg eq '-mcnp'      )  { $MCNP      = shift @args; }
    elsif( $arg eq '-mcnp_opts' )  { $MCNP_OPTS = shift @args; }
    elsif( $arg eq '-bsub_opts' )  { $BSUB_OPTS = shift @args; }
    elsif( $arg eq '-symlink'   )  { $SYMLINKS  = shift @args; }
    elsif( $arg eq '-outer'     )  { $OUTER     = 'yes'; }
    elsif( $arg eq '-inner'     )  { $OUTER     = '';    }
    elsif( $arg eq '-debug'     )  { $DEBUG     = 'yes'; }
    elsif( $arg eq '-verbose'   )  { $DEBUG     = 'yes'; }
    elsif( $arg eq '-v'         )  { $DEBUG     = 'yes'; }
    elsif( $arg eq '-setup'     )  { $SETUP     = 'yes'; }
    elsif( $arg eq '-submit'    )  { $SUBMIT    = 'yes'; }
    elsif( $arg eq '-resubmit'  )  { $RESUBMIT  = 'yes'; }
    elsif( $arg eq '-run'       )  { $RUN       = 'yes'; $COLLECT='yes'; }
    elsif( $arg eq '-collect'   )  { $COLLECT   = 'yes'; }
    elsif( $arg eq '-avgonly'   )  { $AVGONLY   = 'yes'; }
    elsif( $arg eq '--'         )  { next; }
    else { die("***** $0 error, bad arg: $arg\n"); }
  }
}
#-------------------------------------------------------------------------
sub parse_input() {
  ##### parse the input file for substituion & control lines
  ##### create:   %data,  @SYM

  my @VALS, $typ, $sym, $num, $ave, $sig, $xmin, $xmax, $s;
  my $nconstraint=0;

# dos2unix @ CYGWIN:
  if ($OS=~/CYGWIN/ ) {
#      print "Converting DOS to Unix: $IN\n";
      system("dos2unix.exe $IN");
  }
  

  ##### open input & output files (default: stdin, stdout)
  open( IN,  "<$IN")  || die("$0: can't open $IN\n");

  while( <IN> ) {

    # only parse lines starting with "c @@@ " 
    if( ! /^c +@@@/i ) { next; }

    # pick up any continuation lines
    while( substr($_,-2,1) eq "\\" ) { $_ = substr($_,0,-2).<IN>; }

    # remove leading 'c @@@ ', commas, extra blanks, tabs, newline
    chop;
    s/c +@@@ */ /gi;
    s/#.*//;
    s/,/ /g   unless  /=\s*\(.*\)\s*$/;
    s/\t/ /g;
    s/\s*=\s*/=/;
    s/ +/ /g;
    s/ $//;
    s/^ //;

    undef @VALS;
    if(    /^options=(.*)/i ) {
      my @opts = split(' ', $1 );
      &get_global_opts( @opts );
    }
    elsif(    /^constraint=\((.*)\)/i ) {
      # constraint condition
      $nconstraint++;
      $typ = 'constraint';
      $sym = 'C-'.$nconstraint;
      $num = 1;
      $val = $1;
      push @SYM, $sym;
      $data{ $sym } = [ $sym, $typ, $num, $val ];
    }
    elsif( /^(\w+)=\((.*)\)/ ) {
      # expression to evaluate
      $typ = 'eval    ';
      $sym = $1;
      $num = 1;
      $val = $2;
      push @SYM, $sym;
      $data{ $sym } = [ $sym, $typ, $num, $val ];
    }
    elsif( /^(\w+)=normal (\S+) (\S+) (\S+)$/i ) {
      # sample from normal PDF
      $typ = 'normal  ';
      $sym = $1;
      $num = &symcon($2);
      $ave = $3;
      $sig = $4;
      push @SYM, $sym;
      $data{ $sym } = [ $sym, $typ, $num, $ave, $sig ];
    }
    elsif( /^(\w+)=uniform (\S+) (\S+) (\S+)$/i ) {
      # sample from uniform PDF
      $typ  = 'uniform ';
      $sym  = $1;
      $num  = &symcon($2);
      $xmin = $3;
      $xmax = $4;
      push @SYM, $sym;
      $data{ $sym } = [ $sym, $typ, $num, $xmin, $xmax ];
    }
    elsif( /^(\w+)=equidistant (\S+) (\S+) (\S+)$/i ) { # not yet implemented!!!
      # equidistant num points between xmin and xmax
      $typ  = 'equidistant ';
      $sym  = $1;
      $num  = &symcon($2);
      $xmin = $3;
      $xmax = $4;
      push @SYM, $sym;
      $data{ $sym } = [ $sym, $typ, $num, $xmin, $xmax ];
    }
    elsif( /^(\w+)=repeat (\S+)$/i ) {
      # repeat n - list of 1..n
      $typ  = 'list    ';
      $sym  = $1;
      $num  = &symcon($2);
      @VALS = (1..$num);
      push @SYM, $sym;
      $data{ $sym } = [ $sym, $typ, $num, @VALS ];
    }
    elsif( /^(\w+)=(\S+)$/ ) {
      # single value, constant
      $typ = 'constant';
      $sym = $1;
      $val = $2;
      $num = 1;
      push @SYM, $sym;
      $data{ $sym } = [ $sym, $typ, $num, $val ];
    }
    elsif( /^(\w+)=(\S+(?: \S+)+)$/ ) {
      # list of values
      $typ  = 'list    ';
      $sym  = $1;
      @VALS = split(' ',$2);
      $num  = @VALS;
      push @SYM, $sym;
      $data{ $sym } = [ $sym, $typ, $num, @VALS ];
    }
  }
  close(IN);

  if( $DEBUG ) {
    print "\n***** Parameter substitution data:\n";
    foreach $s (@SYM) {
      $sym = $data{$s}[0];
      $typ = $data{$s}[1];
      $num = $data{$s}[2];
      if(     $typ =~ /normal/     ) { $n = 2;}
      elsif(  $typ =~ /uniform/    ) { $n = 2; }
      elsif(  $typ =~ /equidistant/) { $n = 2; }
      elsif(  $typ =~ /eval/       ) { $n = 1; }
      elsif(  $typ =~ /constraint/ ) { $n = 1; }
      else                           { $n = $num; }
      print "$sym\t$typ\t$num\t$data{$s}[3]";
      for( $k=1; $k<$n; $k++ ) {
        print "\t$data{$s}[3+$k]";
      }
      print "\n";
    }
  }
}
#-------------------------------------------------------------------------
sub parse_tally_info() {
  ##### scan the input file, looking for tally specs

  open(   IN,  "<$IN")  || die("$0: can't open $IN\n");
  while( <IN> ) {
    if( /^c /i ) { next; }
    if( /^#/   ) { next; }
 
    if( /^\s*\*?(fp?)0*(\d+):?[npe]? /i ) {
      push @TAL_TYP, lc($1);
      push @TAL_NUM, $2;
    }
  }
  close(IN);

  if( $DEBUG ) {
    print "\n***** Tally specs in input file:\n";
    my $k;
    for( $k=0; $k<@TAL_NUM; $k++) {
      print "\t$TAL_NUM[$k]";
    }
    print "\n";
  }
}
#-------------------------------------------------------------------------
sub parse_symlink_info() {
  ##### scan the input file, looking for required symbolic links
  my $arg, $f, $g, @mopts, $in_msg=0;
  undef %symlink_files;

  # add symlinks from -mcnp_opts
  @mopts = split(' ',$MCNP_OPTS);
  foreach $arg (@mopts) {
    if( $arg=~/\S+=(\S+)/ ) { $symlink_files{$1} = "$WORK/$1"; }
  }

  # scan the input file for symlinks needed
  open(   IN,  "<$IN")  || die("$0: can't open $IN\n");
  while( <IN> ) {
    if( /^c /i   ) { next; }
    if( /^#/     ) { next; }
    # parse the message block
    if( /^\s*message:/i     ) { $in_msg=1; }
    if( $in_msg && /^\s*$/  ) { $in_msg=0; }
    if( $in_msg ) {
      foreach $arg (split) {
        if( $arg=~/datapath/i ) { next; }
        if( $arg=~/\S+=(\S+)/ ) { $symlink_files{$1} = "$WORK/$1"; }
      }
    }
    # add dump files
    elsif( /^\s*dump\s+(\S+)/i ) { $symlink_files{$1} = "$WORK/$1"; }
  }
  close(IN);

  # get -symlink files from the command line
  if( $SYMLINKS ) {
    foreach $f (glob($SYMLINKS)) {
      $g = $f;
      $g =~ s/.*\///;  # basename
      if( $f !~ /^\// ) { $f = "$WORK/$f"; }

          # special treatment for Bluemountain scratch
          if( $OS=~/IRIX/ && $HOST=~/^b\d+/ && $f=~/^\/scratch\// ) {
            $f = "/n/$HOST$f";
          }

      $symlink_files{$g} = $f;
    }
  }

  if($DEBUG && %symlink_files) {
    print "\nNeed to create soft links for these files:\n";
    foreach $f (keys(%symlink_files)) { print "\t\t$f\t\t$symlink_files{$f}\n"; }
  }
}
#-------------------------------------------------------------------------
sub next_case() {
  ##### iterator for looping over cases
  my $k, $s;
  if( $KASE==0 || $KASE>$NCASES ) {
    # initialize 
    $KASE = 1;
    for( $k=0; $k<@SYM; $k++) { $INDEX[$k]=0; }
  }
  else {
    # increment the indices for each variable
    $KASE++;
    if( $OUTER ) {
      $INDEX[0] += 1;
      for( $k=0; $k<$NSYM-1; $k++ ) {
        $s = $SYM[$k];
        if( $INDEX[$k]<&symcon($data{$s}[2]) ) { last; }
        $INDEX[$k] = 0;
        $INDEX[$k+1]++;
      }
    }
    else {
      for( $k=0; $k<@SYM; $k++) { $INDEX[$k]= $KASE-1; }
    }
  }
  return $KASE;
}
#-------------------------------------------------------------------------
sub  symcon() {
  # If arg is integer, return it.
  # If not, try 1 level substitution - constant only.
  my $s = $_[0];
  my $j; 
  if( $s =~ /\d+/ ) { return $s; }
  for( $j=0; $j<@SYM; $j++ ) {
    if( $s eq $SYM[$j] ) {
      if( $data{$s}[1] eq 'constant' ) {
        $s = $data{$s}[3];
        if( $s =~ /\d+/ ) { return $s; }
      }
      # error if symbol not already defined constant integer
      die("*** error: can't use non-constant to define control var\n");
    }
  }
  return $s;
}
#-------------------------------------------------------------------------
sub symsub() {
  # make substitutions based on previously defined vars
  my( $last, $expr ) = @_;
  my $j, $s, $v;
  for( $j=0; $j<$last; $j++ ) {
    $s = $SYM[$j];
    $v = $VAL[$KASE][$j];
    $expr =~ s/\b$s\b/$v/g;
  }
  return $expr;
}

#-------------------------------------------------------------------------
sub replace_sym() {
 ###### replace symbols values with values from "set_variables.dat"
    my %variables, @Fld, $key, $sym, $k;
  ##### open input & output files (default: stdin, stdout)
    open( SUB,"set_variables.dat")  || return 0; #die("$0: can't open set_variables.dat\n");
    while(!eof(SUB)) {
	$_ = <SUB>;
	chop;
	@Fld = split(' ',$_,3);
	$variables{$Fld[0]} = $Fld[1];
    }
    foreach $key (keys(%variables)) {
	for($k=0; $k<@SYM; $k++ ) {
	    $sym = $SYM[$k];
	    if($key eq $sym) {
                $data{ $sym } = [];
		$typ = 'constant';
		$num = 1;
		$val= $variables{$key};
		$data{ $sym } = [ $sym, $typ, $num, $val ];
	    }
	}
    }
# kbat begin
  close(SUB);
# kbat end
}
  
  
#-------------------------------------------------------------------------
sub  get_case_params() {
  ##### determine the substitutions for this case

  my $j, $k, $s, $v, $ave, $sig;
  my $sym, $typ, $num, $val, $expr, $xmin, $xmax;
  my $pi = 3.14159265358979323844;

  # determine all the parameters
  STARTOVER: {
    for($k=0; $k<@SYM; $k++ ) {
      $sym = $SYM[$k];
      $typ = $data{$sym}[1];
      $num = &symcon( $data{$sym}[2] );
  
      if(    $typ =~ /constraint/ ) {
        # substitute defined parameters into expr
        $expr = &symsub( $k, $data{$sym}[3] );
        # if expr not true, start over
        if( ! eval( $expr ) ) { goto STARTOVER;}
      }
      elsif( $typ =~ /normal/ ) {
        $ave = &symsub( $k, $data{$sym}[3] );
        $sig = &symsub( $k, $data{$sym}[4] );
        $val = $ave + $sig*sqrt(-2.*log(rand))*sin(2.*$pi*rand);
      }
      elsif( $typ =~ /uniform/ ) {
        $xmin= &symsub( $k, $data{$sym}[3] );
        $xmax= &symsub( $k, $data{$sym}[4] );
        $val = $xmin + ($xmax-$xmin)*rand;
      }
      elsif( $typ =~ /equidistant/ ) { # not yet implemented - how to account for the step number ???
        $xmin= &symsub( $k, $data{$sym}[3] );
        $xmax= &symsub( $k, $data{$sym}[4] );
        $val = $xmin + ($xmax-$xmin)/5*$k; 
      }
      elsif( $typ =~ /constant/ ) {
        $val = &symsub( $k, $data{$sym}[3] );
      }
      elsif( $typ =~ /list/ ) {
        if( $INDEX[$k]<$num ) {
          $val = &symsub( $k, $data{$sym}[3+$INDEX[$k]] );
        }
        else {
          $val = &symsub( $k, $data{$sym}[3+$num-1] );
        }
      }
      elsif( $typ =~ /eval/ ) {
        $expr = &symsub( $k, $data{$sym}[3] );
        $val  = eval( $expr );
      }
      $VAL[$KASE][$k] = $val;
    }
  }

  if( $DEBUG ) {
    # print case info
    print "\n***** Case = $KASE,     Substitutions:\n";
    for( $k=0; $k<$NSYM; $k++ ) {
      $s = $SYM[$k];
      $v = $VAL[$KASE][$k];
      if( $s =~ /^C-\d+$/ ) { next; }
      print "\t\t$s\t$v\n";
    }
  }
}
#-------------------------------------------------------------------------
sub  substitute_case_params() {
  # scan the input file & substitute the case parameters

  my $k, $s, $v;

  print "***** Case = $KASE\t\tCreate file:  $OUT\n";
  open( IN,  "<$IN")  || die("$0: can't open $IN\n");
  open( OUT, ">$OUT") || die("$0: can't open $OUT\n");

  while( <IN> ) {

    # skip lines with leading "c @@@ "
    if( /^c +@@@/i ) { next; }
    # make the substitutions
    for($k=0; $k<@SYM; $k++ ) {
      $s = $SYM[$k];
      $v = $VAL[$KASE][$k];
      if( $s =~ /^C-\d+$/ ) { next; }

      s/\b$s\b/$v/g;
    }
    # if there are embedded new-lines, do each piece
    while( length($_)>0 && index($_,"\n",0)<=length($_)-1 ) {
# print $_
      my $p = substr($_,0,index($_,"\n",0)+1,'');
      # wrap lines at 75 chars, but don't split-up comments
      if( $p !~ /^ {0,4}c /i ) {
        until( length($p)<=75 ) {
          my $k = rindex( $p, ' ', 75 );
          my $j =  index( $p, '$',  0 );
          if( $j>0 && $j<$k ) { $k=$j; }
          print OUT substr($p,0,$k,'     '),"\n";
        }
      }
      print OUT $p;
    }
  }
  close(IN);
  close(OUT);
}
#-------------------------------------------------------------------------
sub collect_results() {

  { ##### combined Keff & std dev for this case
    my @RKPL, @line, $kcode, $NC, $IKZ, $MK, $n, $k;
    undef $kcode;
    open(MCTAL,"<$CASEDIR/mctal");
    while( <MCTAL> ) {
      if( $kcode>0 ) {
        chop;
        s/^ //;
        s/  +/ /g;
        @line = split;
        foreach $k (@line) { push @RKPL, $k; }
      }
      else {
        if( /^kcode *(\d+) *(\d+) *(\d+)/ ) {
          $kcode = 1;
          $NC  = $1;
          $IKZ = $2;
          $MK  = $3;
        }
      }
    }
    close(MCTAL);
    if( @RKPL ) {
      $j = $MK*($NC-1) + 11;
      $KEFF[$KASE] = $RKPL[$j];
      $KSIG[$KASE] = $RKPL[$j+1];
    }
  }

  { ##### get tally results & std-dev for this case

    my $ntal, @talnums;
    my @TALS, @line, $flag, $ntal, $ktal, $k;
    undef $flag;
    open(MCTAL,"<$CASEDIR/mctal");
    while( <MCTAL> ) {
      if( /^ntal *(\d+)/i ) {
        $ntal = $1;
        $flag = 1;
      }
      elsif( /^tally *(\d+)/i ) {
        $ktal = $1;
        $flag = 1;
    }
      elsif( /^vals/i ) {
        $flag = 2;
      }
      elsif( /^tfc/i || /^kcode/i ) {
        $flag = 0;
        if( $DEBUG ) {
          for( $k=0; $k<@TALS; $k+=2 ) {
            print "\ttal\t$ktal\t$TALS[$k]\t$TALS[$k+1]\n";
          }
        }
        for( $k=0; $k<@TALS; $k+=2 ) {
          $TAL_AVE[$KASE][$ktal][$k/2] = $TALS[$k];
          $TAL_SIG[$KASE][$ktal][$k/2] = $TALS[$k+1];
        }
        $TAL_LEN[$ktal] = @TALS / 2;
        undef @TALS;
      }
      elsif( $flag==2 ) {
        chop;
        s/  +/ /g;
        s/^ //;
        @line = split;
        foreach $k (@line) { push @TALS, $k; }
      }
    }
    close(MCTAL);
  }
}
#-------------------------------------------------------------------------
sub print_results() {

  if( @KEFF ) {
    my $k, $ksum, $ksum2 ;
    for( $k=1; $k<=$NCASES; $k++ ) {
      print     "$CASENAMES[$k]\tKEFF\t$KEFF[$k]\tKSIG\t$KSIG[$k]\n";
      print LOG "$CASENAMES[$k]\tKEFF\t$KEFF[$k]\tKSIG\t$KSIG[$k]\n";
      $ksum  += $KEFF[$k];
      $ksum2 += $KEFF[$k]**2;
    }
    # get the average Keff & sig
    if( $NCASES > 1 ) {
      $KEFF_AVE = $ksum / $NCASES;
      $KEFF_SIG = sqrt(($ksum2/$NCASES-$KEFF_AVE**2)/($NCASES-1));
      print     "AVERAGE\tKEFF\t$KEFF_AVE\tKSIG\t$KEFF_SIG\n";
      print LOG "AVERAGE\tKEFF\t$KEFF_AVE\tKSIG\t$KEFF_SIG\n";
    }
  }

  if( @TAL_AVE ) {
    my $kt, $tn, $ttyp;
    for($kt=0; $kt<@TAL_NUM; $kt++) {
      $tn   = $TAL_NUM[$kt];
      $ttyp = $TAL_TYP[$kt];
      print     "tally\t$tn\n";
      print LOG "tally\t$tn\n";
      my $n, $k, $j, @t, @t2;
      undef  @t;
      undef  @t2;
      $n = $TAL_LEN[$tn];
      for( $k=1; $k<=$NCASES; $k++ ) {
        if( ! $AVGONLY ) {
          print     "\t$CASENAMES[$k]\n";
          print LOG "\t$CASENAMES[$k]\n";
        }
        for( $j=0; $j<$n; $j++ ) {
          if( ! $AVGONLY ) {
            if( $ttyp eq "fp" ) {
              printf     "  %11.5e %6.4f",$TAL_AVE[$k][$tn][$j],$TAL_SIG[$k][$tn][$j];
              printf LOG "  %11.5e %6.4f",$TAL_AVE[$k][$tn][$j],$TAL_SIG[$k][$tn][$j];
              if( ($j+1)%4==0 ) { print "\n"; print LOG "\n"; }
            }
            else {
              print     "\t\t$TAL_AVE[$k][$tn][$j]\t$TAL_SIG[$k][$tn][$j]\n";
              print LOG "\t\t$TAL_AVE[$k][$tn][$j]\t$TAL_SIG[$k][$tn][$j]\n";
            }
          }
          $t[$j]  += $TAL_AVE[$k][$tn][$j];
          $t2[$j] += $TAL_AVE[$k][$tn][$j]**2;
        }
        if( ! $AVGONLY ) { print "\n"; print LOG "\n"; }
      }
      print     "\tAVERAGE\n";
      for( $j=0; $j<$n; $j++ ) {
        $t[$j]  = $t[$j] / $NCASES;
        if( $t[$j]==0.0 || $NCASES<=1 || $t2[$j]<0.0 ) {
          $t2[$j] = 1.0;
        }
        else {
          $t2[$j] = abs( sqrt(($t2[$j]/$NCASES-$t[$j]**2)/($NCASES-1)) / $t[$j] );
        }
        if( $ttyp eq "fp" ) {
          printf     "  %11.5e %6.4f",$t[$j],$t2[$j];
          printf LOG "  %11.5e %6.4f",$t[$j],$t2[$j];
          if( ($j+1)%4==0 ) { print "\n"; print LOG "\n"; }
        }
        else {
          printf     "\t\t%11.5e\t%6.4f\n", $t[$j],$t2[$j];
          printf LOG "\t\t%11.5e\t%6.4f\n", $t[$j],$t2[$j];
        }
      }
      print "\n"; print LOG "\n";
    }
  }
}
#-------------------------------------------------------------------------

sub maximum {
    my ($ff, $i, $length);
    my(@args) = @_;
    $length = scalar(@args);
    $ff = $args[0];
    for ($i=1; $i<$length; $i++) {
	if($args[$i] > $ff) {$ff = $args[$i]}
    }
    $ff;
}

