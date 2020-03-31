# script permettant de modifier les coordonnées des POINTS d'une configuration de manière uniforme
# par exemple :
#
# perl modify_cfg.pl default.cfg "x+10" > new_file.cfg
#
# ajoute 10 unités à la coordonée x pour tous les POINTS dans le fichier
# default.cfg

use strict;
use warnings;

my $file = 'default.cfg';
my $cmd = "x+10";
my $axe = "x";
my $incr = "+";
my $nb = "10";

if ($ARGV[0]){
	$file   = $ARGV[0];
	}

if ($ARGV[1]){
	$cmd   = $ARGV[1];
        if( $cmd =~ /([xyz]{1})([+-]{1})(\d+)/ ) {
                $axe = $1;
                $incr = $2;
                $nb = $3;
                #                print "Action : axe : '${axe}' incr : '${incr}' nb : '${nb}'\n";
                }
	}

open my $info, $file or die "Could not open $file: $!";

while( my $line = <$info>)  {
    if ( $line =~ /^\s*POINT\s*:\s*([0-9]+)\s*([0-9]+)\s*([0-9]+)\s*$/) {
            my ($ptx, $pty, $ptz) = ($1, $2, $3);
            #        print " ($axe eq \"x\") :". ($axe eq "x")."\n" ;

            if ($axe eq "x") {
                    eval( '$ptx ='.$ptx.$incr.$nb.';' );
            }
            if ($axe eq "y") {
                    eval( '$pty ='.$pty.$incr.$nb.';' );
            }
            if ($axe eq "z") {
                    eval( '$ptz ='.$ptz.$incr.$nb.';' );
            }
        my $myline = "POINT : $ptx $pty $ptz\n";

        print $myline;
    } else {
        print $line;
    }
}

close $info;
