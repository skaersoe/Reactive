#!/bin/bash

#perl ~/Reactive/MSR_babysteps/pstudy.pl -i deck_core_pstudy_v1_troels -setup

homedir=$(pwd)

for number in {1..9}
do
    cd $homedir
    cd case00$number

    grep "pstudy A+B=1" inp| while read -r line ; do
    echo "Processing $line   " $number

    echo '#! /bin/bash' >> job2.$number
    echo '#SBATCH --job-name=pstudy.'$number >> job2.$number
    echo '#SBATCH --partition=long' >> job2.$number
    echo '#SBATCH --nodes=1' >> job2.$number
    echo '#SBATCH --exclusive' >> job2.$number
    echo '#SBATCH --exclude=r1n29,r1n30,r1n33,r1n17,r1n38,r1n16' >> job2.$number
    echo ' ' >> job2.$number
    echo 'mpirun mcnpx i=inp' >> job2.$number
    sleep 1
    sbatch job2.$number
    
    cd ..
#done
# grep -v m $homedir/case00$number/job2.$number |while read -r line ; do
#  touch $homedir/case00$number/outp
 done

done

 for number in {10..99}
 do
     cd $homedir
     cd case0$number

     grep  "pstudy A+B=1" inp| while read -r line ; do
     echo "Processing $line   " $number

     echo '#! /bin/bash' >> job2.$number
     echo '#SBATCH --job-name=pstudy.'$number >> job2.$number
     echo '#SBATCH --partition=verylong' >> job2.$number
     echo '#SBATCH --nodes=1' >> job2.$number
     echo '#SBATCH --exclusive' >> job2.$number
     echo '#SBATCH --exclude=r1n29,r1n30,r1n33,r1n17,r1n38,r1n16' >> job2.$number
     echo ' ' >> job2.$number
     echo 'mpirun mcnpx i=inp' >> job2.$number

    sleep 2
    sbatch job2.$number

     cd ..
 done

done
 for number in {100..999}
 do
     cd $homedir
     cd case$number

     grep  "pstudy A+B=1" inp| while read -r line ; do
     echo "Processing $line   " $number

     echo '#! /bin/bash' >> job2.$number
     echo '#SBATCH --job-name=pstudy.'$number >> job2.$number
     echo '#SBATCH --partition=verylong' >> job2.$number
     echo '#SBATCH --nodes=1' >> job2.$number
     echo '#SBATCH --exclusive' >> job2.$number
     echo '#SBATCH --exclude=r1n29,r1n30,r1n33,r1n17,r1n38,r1n16' >> job2.$number
     echo ' ' >> job2.$number
     echo 'mpirun mcnpx i=inp' >> job2.$number

    sleep 2
    sbatch job2.$number

     cd ..
 done

# grep -v "mcnpx" $homedir/case0$number/job2.$number |while read -r line ; do
# touch $homedir/case0$number/outp
#done
done
cd $homedir
