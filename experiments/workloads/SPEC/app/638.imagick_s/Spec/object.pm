$benchnum  = '638';
$benchname = 'imagick_s';
$exename   = 'imagick_s';
$benchlang = 'C';
@base_exe  = qw(imagick_s imagevalidate_638);

$reltol = 0.01;

$need_math = 'yes';

%workloads = (
    'test'       => [ [ '538.imagick_r' ] ],
    'train'      => [ [ '538.imagick_r' ] ],
    'refspeed'   => [ [ '538.imagick_r' ] ],
    'refrate'    => [ [ '538.imagick_r' ] ],
);

$sources = '538.imagick_r';

$bench_flags = '-I.';

use File::Basename;

sub compare_commands {
    my ($me) = @_;
    my @rc = getruns($me, qr/^\s*imagevalidate/);
    my @badexe = grep { $_->{'notfound'} ne '' } @rc;

    if (@badexe) {
        die "Unknown executable '$badexe->{'notfound'}' encountered in compare_commands() in ". __FILE__ ."\n";
    }

    # Fix up the names of the imagevalidate file arguments
    my ($comparefiles) = $me->compare_files_hash;
    foreach my $run (@rc) {
        my $file = basename(pop(@{$run->{'args'}}), '.tga');
        my $origfile = $comparefiles->{$file.'_expected.tga'} || "COULD_NOT_LOCATE_${file}_expected.tga_FILE";
        push @{$run->{'args'}}, $file.'_output.tga', $origfile;
    }

    return @rc;
}

sub invoke {
    my ($me) = @_;
    my @rc = getruns($me, qr/^convert/);
    my @badexe = grep { $_->{'notfound'} ne '' } @rc;

    if (@badexe) {
        die "Unknown executable '$badexe->{'notfound'}' encountered in invoke() in ". __FILE__ ."\n";
    }
    return @rc;
}

sub getruns {
    my ($me, $filter) = @_;
    my @rc;

    my @runs = grep { m{$filter} } main::read_file('control');
    my %exes = ();
    foreach my $exe ($me->exe_files) {
        my $baseexe = basename($exe);
        $baseexe =~ s/_(?:base|peak).*//;
        $baseexe =~ s/^openmp_//;
        $baseexe =~ s/_[rs]$//;
        $baseexe =~ s/_638$//;
        $baseexe =~ s/imagick_//;
        $baseexe =~ s/imagick/convert/;
        $exes{$baseexe} = $exe;
    }

    foreach my $run (@runs) {
        my ($exe, $output, $error, @args) = split(/\s+/, $run);
        if (exists ($exes{$exe})) {
            push @rc, {
                'command' => $exes{$exe},
                'args'    => [ @args ],
                'output'  => "$output",
                'error'   => "$error",
            };
        } else {
            # This should never happen
            return { 'notfound' => $exe };
        }
    }
    return @rc;
}

1;

# Editor settings: (please leave this at the end of the file)
# vim: set filetype=perl syntax=perl shiftwidth=4 tabstop=8 expandtab nosmarttab:
