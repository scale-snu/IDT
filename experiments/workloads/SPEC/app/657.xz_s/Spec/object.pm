$benchnum  = '657';
$benchname = 'xz_s';
$exename   = 'xz_s';
$benchlang = 'C';
@base_exe  = ($exename);

$calctol = 0;

use Config;
$bench_flags  = '-DSPEC_AUTO_BYTEORDER=0x'.$Config{'byteorder'};
$bench_flags .= ' -DHAVE_CONFIG_H=1 -DSPEC_MEM_IO -DSPEC_XZ';
$bench_flags .= ' '.join(' ', qw(
    -I.
    -Ispec_mem_io
    -Isha-2
    -Icommon
    -Iliblzma/api
    -Iliblzma/lzma
    -Iliblzma/common
    -Iliblzma/check
    -Iliblzma/simple
    -Iliblzma/delta
    -Iliblzma/lz
    -Iliblzma/rangecoder
    ));

$sources = '557.xz_r';

%workloads = (
    'test'     => [ [ '557.xz_r' ] ],
    'train'    => [ [ '557.xz_r' ] ],
    'refspeed' => [ [ '557.xz_r' ] ],
    'refrate'  => [ [ '557.xz_r' ] ],
);

use File::Basename;

sub invoke {
    my ($me) = @_;
    my @rc;

    my @temp = main::read_file('control');
    my $exe = $me->exe_file;

    foreach my $args (@temp) {
        next if $args =~ m/^\s*(#|$)/;
        my ($name, $size, $sum, $min, $max, @levels) = split(/\s+/, $args);
        @levels = grep { /^\de?/ } @levels;
        next if $size <= 0 || $sum eq '' || @levels == 0;
        my $leveltext = join('_', @levels);
        push @rc, (
            {
                'command' => $exe,
                'args'    => [ "${name}.xz", $size, $sum, $min, $max, @levels ],
                'output'  => "$name-$size-$leveltext.out",
                'error'   => "$name-$size-$leveltext.err",
            }
        );
    }
    return @rc;
}

1;

# Editor settings: (please leave this at the end of the file)
# vim: set filetype=perl syntax=perl shiftwidth=4 tabstop=8 expandtab nosmarttab:
