#!/bin/bash
cd /mnt/c/Users/ATIV/Desktop/MultiBoost/build
cmake ..
make all

dataset=$1
# 
train_epochs=10000
if [ "$dataset" == "mnist" ]; then
	train_epochs=30000
fi
# 


# TS
# bash ./ts.sh $dataset $train_epochs

# KL-UCB, assume delta = 0.1
# bash ./klucb.sh $dataset $train_epochs


# EXP3++
# Hyperparmaeter _c
# bash ./exp3pp.sh $dataset 0.015625 $train_epochs
# bash ./exp3pp.sh $dataset 0.03125 $train_epochs
# bash ./exp3pp.sh $dataset 0.125 $train_epochs
# bash ./exp3pp.sh $dataset 0.25 $train_epochs

# BGE
# bash ./bge.sh $dataset 0.25 $train_epochs
# bash ./bge.sh $dataset 0.5 $train_epochs
# bash ./bge.sh $dataset 0.75 $train_epochs

# EXP3P, default params
# bash ./exp3p.sh $dataset $train_epochs

# AFTL
# Hyperparameters in order of c, alpha
# bash ./aftl.sh $dataset 0.2 0.7 $train_epochs
# bash ./aftl.sh $dataset 0.3 0.5 $train_epochs
# bash ./aftl.sh $dataset 1 0.5 $train_epochs
# bash ./aftl.sh $dataset 2 0.4 $train_epochs
# bash ./aftl.sh $dataset 3 0.3 $train_epochs

# ABE
# Hyperparameters in order of c, alpha
bash ./abe.sh $dataset 0.2 0.7 $train_epochs
bash ./abe.sh $dataset 0.3 0.5 $train_epochs
bash ./abe.sh $dataset 1 0.5 $train_epochs
bash ./abe.sh $dataset 2 0.4 $train_epochs
bash ./abe.sh $dataset 3 0.3 $train_epochs
