#!/bin/bash

perl ~/pstudy.pl -i deck0 -setup

homedir=$(pwd)

for number in {1..5}
do
    cd $homedir
    cd case00$number

#    grep "pstudy A+B=1" inp| while read -r line ; do
#    echo "Processing $line   " $number

    echo '#! /bin/bash' >> job2.$number
    echo '#SBATCH --job-name=pstudy.'$number >> job2.$number
    echo '#SBATCH --partition=verylong' >> job2.$number
    echo '#SBATCH --nodes=1-8' >> job2.$number
    echo '#SBATCH --exclusive' >> job2.$number
    echo '#SBATCH --exclude=r1n29' >> job2.$number
    echo ' ' >> job2.$number
    echo 'mpirun mcnpx i=inp' >> job2.$number
#    sleep 1
    sbatch job2.$number
    
    cd ..
#done
# grep -v m $homedir/case00$number/job2.$number |while read -r line ; do
#  touch $homedir/case00$number/outp
# done

 done


