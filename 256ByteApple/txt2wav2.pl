#!/usr/bin/perl

# convert a file from txt into wav
# shifty death effect done by noah vawter 6/13/2001

if($#ARGV<0){
   print " usage: txt2wav <filename>\n";
   print "generates a .wav file from your .txt file\n";
   die;
}


# determine length of input file
# also keep track of highest and lowest value in file
$innem=$ARGV[0];
open(SRC,"$innem") || die "can't open $innem\n";
$max=-999999;
$min=999999;
while(<SRC>){
    $len++;
    if($_>$max){$max=$_;}
    if($_<$min){$min=$_;}
}
print "input length= $len\n";

# auto-normalize will save lots of time :) 
$midpoint=($max+$min)/2;
if( ($max-midpoint) eq 0){$max++;$midpoint=($max-$min)/2;}  # protection
$scalefactor=32767/($max-$midpoint);
print "max=$max, min=$min\n";
print "midpoint=$midpoint, scalefactor=$scalefactor\n";


# do output, change filename to .wav
$_=$ARGV[0];
s/\.txt$//;   # eliminate .txt at the end if its there
s/\.csv$//;   # eliminate .csv at the end if its there
$_.=".wav";   # append .wav to your filename
$outnem=$_;

$samplen=$len;   # used in prep_output function
$samplerate=11025;   # TEMP TEMP TEM PTEM PTE TPMET PMTEPMT E shuld 44100
$samplerate=8000;   # TEMP TEMP TEM PTEM PTE TPMET PMTEPMT E shuld 44100
prep_output();   # setup wave file

# copy data from txt into new file
seek SRC,0,0;
for($i=0;$i<$len;$i++){
    $_=<SRC>;

#    $output=$_;    # if no auto-normalize
    $output=int(  ($_-$midpoint)*$scalefactor);
    print DST pack("s1",$output);
}

# write a simple wave file header
sub prep_output{
    # output file preparation
    open(DST,">$outnem") || die "couldn't write to $outnem\n";
    select(DST);

    # 16-bit, Mono 44.1KHz Wave Header generation
    $F2=2*$samplen;  # 2 bytes per sample
    $F1=0x24+$F2;      # that's just the len's of headers.

    @MyHeader=pack("A4V1A4A4V1v1v1V1V1v1v1A4V1",
		   "RIFF",$F1,"WAVE","fmt ",0x10,0x1,0x1,
		   $samplerate,2*$samplerate,0x2,16,"data",$F2);

    print @MyHeader;
    select(STDOUT);
}
