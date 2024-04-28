$benchnum  = '607';
$benchname = 'cactuBSSN_s';
$exename   = 'cactuBSSN_s';
$benchlang = 'CXX,C,F';
@base_exe  = ($exename);

$reltol = { 'default' => undef };
$abstol = {
    'gxx.xl'  => 0.0000000000005,
    'gxy.xl'  => 0.0000000000005,
    'default' => undef,
};
$floatcompare = 1;
$need_math = 'yes';

%workloads = (
    'test'       => [ [ '507.cactuBSSN_r' ] ],
    'train'      => [ [ '507.cactuBSSN_r' ] ],
    'refspeed'   => [ [ '507.cactuBSSN_r' ] ],
    'refrate'    => [ [ '507.cactuBSSN_r' ] ],
);

$sources = '507.cactuBSSN_r';

$bench_cflags   = '-Iinclude -DCCODE';
$bench_cxxflags = $bench_cflags.' -DCCTK_DISABLE_RESTRICT=1';
$bench_fflags   = '-Iinclude -DFCODE';

sub invoke {
    my ($me) = @_;
    my (@rc);

    my @temp = main::read_file('control');
    my $exe = $me->exe_file;

    foreach my $input (@temp) {
        chomp($input);
        $input =~ /(.*).par/;
        my $name = $1;
        push (@rc, { 'command' => $exe,
                'args'    => [ $input ],
                'output'  => "$name.out",
                'error'   => "$name.err",
            });
    }
    return @rc;
}

1;

# Editor settings: (please leave this at the end of the file)
# vim: set filetype=perl syntax=perl shiftwidth=4 tabstop=8 expandtab nosmarttab:
