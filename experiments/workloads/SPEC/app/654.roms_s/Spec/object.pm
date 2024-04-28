$benchnum  = '654';
$benchname = 'roms_s';
$exename   = 'sroms';
$benchlang = 'F';
@base_exe  = ($exename);

$bench_fppflags = '-w -m literal-single.pm -m c-comment.pm -I. -DBENCHMARK -DNestedGrids=1 -DNO_GETTIMEOFDAY';
$bench_fflags = '-I.';

$bench_cflags = '-I.';

$reltol = 1.0e-7;
$abstol = 1.0e-7;

$sources = '554.roms_r';

%workloads = (
    'test'     => [ [ '554.roms_r' ] ],
    'train'    => [ [ '554.roms_r' ] ],
    'refspeed' => [ [ '554.roms_r' ] ],
    'refrate'  => [ [ '554.roms_r' ] ],
);

sub _calculate_dimensions {
    my ($threads) = @_;
    my ($NT, $NI, $NJ, $NX, $NJO);

    return undef unless ($threads > 0);
    $NT = $NI = $threads;
    $NX = $NJ = $NJO = 1;

    if ( $NT >= 15 ) {
        # Look for "nice" factors 32 or 16
        foreach my $factor (32, 16) {
            if ( $NT % $factor == 0 ) {
                $NJ = $factor;
                $NI = $NT / $factor;
                last;
            }
        }
        if ( $NI == $threads ) {
            # Not 32 or 16, so look for factors 2,3,5,7 in NT until remainder < 15
            while ( $NT > 14 ) {
                $NJO = $NJ;
                $NX = undef;
                foreach my $factor (2, 3, 5, 7) {
                    if ( $NT % $factor == 0 ) {
                        $NX = $factor;
                        last;
                    }
                }
                $NX = 1 unless defined($NX);
                $NJ = $NJ * $NX;
                $NT = $NT / $NX;
                $NI = $NT;
                if ( $NJ == $NJO ) {
                    $NI = $NT;
                    $NT = 1;
                } else {
                    # Max NJ is 14 so stop finding factor here, set NJ = as big as possible
                    if ( $NJ > 14 ) {
                        if ( $NX > $NJO ) {
                            $NJ = $NX;
                            $NI = $NJO * $NT;
                        } else {
                            $NJ = $NJO;
                            $NI = $NX * $NT;
                        }
                        $NT = 1;
                    }
                }
            }
            # Found NI and NJ, use NJ the larger one if below 15
            if ( $NI > $NJ && $NI < 15 ) {
                ($NI, $NJ) = ($NJ, $NI);
            }
        }
    } else {
        # For NT 1..14 use NJ=NT, NI=1 (adjusted below)
        $NJ = $NT;
        $NI = 1;
    }

    # Adjust NI to minimum 4
    while( $NI && $NI < 4 ) {
        $NI *= 2;
    }

    return wantarray ? ($NI, $NJ) : ($NI * $NJ);
}

sub check_threads {
    my ($me) = @_;

    my $threads = $me->{'threads'};

    my ($NI, $NJ) = _calculate_dimensions($threads);
    if ( !defined($NI) || $NI <= 0 ) {
        ::Log(0, "ERROR: ".$me->name." cannot calculate grid dimensions for $threads threads\n");
        return 1;
    }

    return 0;
}

use IO::File;
sub pre_run {
    my ($me, @paths) = @_;
    my $NTHREADS = $me->{'threads'};
    $NTHREADS += 0;
    if ( $NTHREADS == 0 ) {
        ::Log(0, "ERROR: In pre_run() for ".$me->name.", cannot run with $NTHREADS threads\n");
        return 1;
    }

    my ($NI, $NJ) = _calculate_dimensions($NTHREADS);
    if ( !defined($NI) || $NI <= 0 ) {
        ::Log(0, "ERROR: In pre_run() for ".$me->name.", cannot calculate dimensions for $NTHREADS threads\n");
        return 1;
    }
    ::Log(150, $me->name.": Prerun decomposition for $NTHREADS threads: NtileI=$NI NtileJ=$NJ\n");

    # Fix up all *.x files
    for my $path (@paths) {
        for my $file ($me->input_files_base) {
            next unless $file =~ s#\.x$##o;
            my $fullpath = ::jp($path, $file);
            my $ifh = new IO::File '<'.$fullpath.'.x';
            if (!defined($ifh)) {
                ::Log(0, "ERROR: In pre_run() for $name, ${fullpath}.x could not be opened for reading: $!\n");
                return 1;
            }
            my $ofh = new IO::File '>'.$fullpath;
            if (!defined($ofh)) {
                ::Log(0, "ERROR: In pre_run() for $name, $fullpath could not be opened for writing: $!\n");
                return 1;
            }
            while(defined(my $line = <$ifh>)) {
                $line =~ s/^(\s+NtileI ==)\s+\d+/$1 $NI/;
                $line =~ s/^(\s+NtileJ ==)\s+\d+/$1 $NJ/;
                $ofh->print($line);
            }
            $ofh->close();
            $ifh->close();
        }
    }
    return 0;
}

sub invoke {
    my ($me) = @_;
    my $name = $me->name;
    my @rc;

    my $exe = $me->exe_file;
    foreach my $input ($me->input_files_base) {
        next if $input =~ /varinfo.dat/;
        my $outname = $input;
        $outname =~ s/\.in.x$//;
        push @rc, {
            'command' => $me->exe_file,
            'args'    => [ ],
            'output'  => "$outname.log",
            'error'   => "$outname.err",
            'input'   => "$outname.in",
        };
    }
    return @rc;
}

1;

# Editor settings: (please leave this at the end of the file)
# vim: set filetype=perl syntax=perl shiftwidth=4 tabstop=8 expandtab nosmarttab:
