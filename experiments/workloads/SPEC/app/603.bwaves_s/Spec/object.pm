$benchnum  = '603';
$benchname = 'bwaves_s';
$exename   = 'speed_bwaves';
$benchlang = 'F';
@base_exe  = ($exename);

$reltol = {
    'bwaves_1.out'    => 0.015,
    'bwaves_2.out'    => 0.015,
    'bwaves_3.out'    => 0.015,
    'bwaves_4.out'    => 0.015,
    'test1_dqnrm.log' => 0.001,
    'test2_dqnrm.log' => 0.001,
    'train1_dqnrm.log' => 0.001,
    'train2_dqnrm.log' => 0.001,
    'refspeed1_dqnrm.log' => 0.001,
    'refspeed2_dqnrm.log' => 0.001,
    'refrate1_dqnrm.log' => 0.001,
    'refrate2_dqnrm.log' => 0.001,
    'refrate3_dqnrm.log' => 0.001,
    'refrate4_dqnrm.log' => 0.001,
    'default' => undef,
};

$abstol = {
    'bwaves_1.out'   => 1.0e-16,
    'bwaves_2.out'   => 1.0e-16,
    'bwaves_3.out'   => 1.0e-16,
    'bwaves_4.out'   => 1.0e-16,
    'test1_resid.log' => 1.0e-6,
    'test2_resid.log' => 1.0e-6,
    'train1_resid.log' => 1.0e-6,
    'train2_resid.log' => 1.0e-6,
    'refspeed1_resid.log' => 1.0e-6,
    'refspeed2_resid.log' => 1.0e-6,
    'refrate1_resid.log' => 1.0e-6,
    'refrate2_resid.log' => 1.0e-6,
    'refrate3_resid.log' => 1.0e-6,
    'refrate4_resid.log' => 1.0e-6,
    'default' => undef,
};

$sources = '503.bwaves_r';

%workloads = (
    'test'     => [ [ '503.bwaves_r' ] ],
    'train'    => [ [ '503.bwaves_r' ] ],
    'refspeed' => [ [ '503.bwaves_r' ] ],
    'refrate'  => [ [ '503.bwaves_r' ] ],
);

sub invoke {
    my ($me) = @_;
    my (@rc);

    my @temp = main::read_file('control');
    my $exe = $me->exe_file;

    for my $line (@temp) {
        my ($dirname) = split(/\s+/, $line);
        next if $dirname =~ m/^\s*(?:#|$)/;
        push @rc, {
            'command' => $exe,
            'args'    => [ "$dirname" ],
            'input'   => "$dirname.in",
            'output'  => "$dirname.out",
            'error'   => "$dirname.err",
        };
    }
    return @rc;
}

1;

# Editor settings: (please leave this at the end of the file)
# vim: set filetype=perl syntax=perl shiftwidth=4 tabstop=8 expandtab nosmarttab:
