#!/bin/bash
cd /mnt/c/Users/ATIV/Desktop/MultiBoost/build
cmake ..
make all

dataset=$1

# TS
bash ./ts.sh $dataset

# KL-UCB, assume delta = 0.1
bash ./klucb.sh $dataset


# EXP3++
# Hyperparmaeter _c
bash ./exp3pp.sh $dataset 0.015625 
bash ./exp3pp.sh $dataset 0.03125
bash ./exp3pp.sh $dataset 0.125 
bash ./exp3pp.sh $dataset 0.25

# BGE
bash ./bge.sh $dataset 0.25
bash ./bge.sh $dataset 0.5
bash ./bge.sh $dataset 0.75

# EXP3P, default params
bash ./exp3p.sh $dataset

# AFTL
# Hyperparameters in order of c, alpha
bash ./aftl.sh $dataset 0.2 0.7
bash ./aftl.sh $dataset 0.3 0.5
bash ./aftl.sh $dataset 1 0.5
bash ./aftl.sh $dataset 2 0.4
bash ./aftl.sh $dataset 3 0.3

# ABE
# Hyperparameters in order of c, alpha
bash ./abe.sh $dataset 0.2 0.7
bash ./abe.sh $dataset 0.3 0.5
bash ./abe.sh $dataset 1 0.5
bash ./abe.sh $dataset 2 0.4
bash ./abe.sh $dataset 3 0.3
