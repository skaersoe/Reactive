#!/bin/bash

perl ~/pstudy.pl -i deck_pstudy_v1 -setup

homedir=$(pwd)

#for number in {1..858} all of it. Below is a test subset
for number in {1..9}
do
    cd $homedir
    cd case00$number
    echo '#! /bin/bash' >> job2.$number
    echo '#SBATCH --job-name=Zirc_U.'$number >> job2.$number
    echo '#SBATCH --partition=long' >> job2.$number
    echo '#SBATCH --nodes=1' >> job2.$number
    echo '#SBATCH --exclusive' >> job2.$number
    echo '#SBATCH --exclude=r1n29' >> job2.$number
    echo ' ' >> job2.$number
    echo 'mpirun mcnpx i=inp' >> job2.$number

    sbatch job2.$number

    cd ..
done
for number in {10..99}
do
    cd $homedir
    cd case0$number
    echo '#! /bin/bash' >> job2.$number
    echo '#SBATCH --job-name=Zirc_U.'$number >> job2.$number
    echo '#SBATCH --partition=long' >> job2.$number
    echo '#SBATCH --nodes=1' >> job2.$number
    echo '#SBATCH --exclusive' >> job2.$number
    echo '#SBATCH --exclude=r1n29' >> job2.$number
    echo ' ' >> job2.$number
    echo 'mpirun mcnpx i=inp' >> job2.$number

    sbatch job2.$number

    cd ..
done

for number in {100..858}
do
    cd $homedir
    cd case$number
    echo '#! /bin/bash' >> job2.$number
    echo '#SBATCH --job-name=Zirc_U.'$number >> job2.$number
    echo '#SBATCH --partition=long' >> job2.$number
    echo '#SBATCH --nodes=1' >> job2.$number
    echo '#SBATCH --exclusive' >> job2.$number
    echo '#SBATCH --exclude=r1n29' >> job2.$number
    echo ' ' >> job2.$number
    echo 'mpirun mcnpx i=inp' >> job2.$number

    sbatch job2.$number

    cd ..
done
