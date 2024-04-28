$benchnum  = '631';
$benchname = 'deepsjeng_s';
$exename   = 'deepsjeng_s';
$benchlang = 'CXX';
@base_exe  = ($exename);

$obiwan = 1;

$bench_flags = '-DBIG_MEMORY';

$sources = '531.deepsjeng_r';

sub invoke {
    my ($me) = @_;
    my $name;
    my @rc;

    my $exe = $me->exe_file;
    for ($me->input_files_base) {
        if (($name) = m/(.*).txt$/) {
            push @rc, {
                'command' => $exe,
                'args'    => [ "$name.txt" ],
                'output'  => "$name.out",
                'error'   => "$name.err",
            };
        }
    }
    return @rc;
}

1;

# Editor settings: (please leave this at the end of the file)
# vim: set filetype=perl syntax=perl shiftwidth=4 tabstop=8 expandtab nosmarttab:
