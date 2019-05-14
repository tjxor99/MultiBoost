#!/bin/bash
# cd /mnt/c/Users/ATIV/Desktop/MultiBoost/build
# cmake ..
# make all
# 
# 
dataset=$1
c=$2
alpha=$3
# 
train_epochs=10000
if [ "$dataset" == "mnist" ]; then
	train_epochs=30000
fi
# 
DIRECTORY="newest_$dataset"
if [ ! -d "$DIRECTORY" ]; then
	mkdir $DIRECTORY
fi
# 
train_file=$dataset
train_file+="Train.arff"
test_file=$dataset
test_file+="Test.arff"

target_file="AFTL_c"
target_file+=$c
target_file+="_alpha"
target_file+=$alpha
target_file+=".dta"


echo "Running AFTL for dataset $dataset with Params c = $c, alpha = $alpha, init = 1"
./multiboost --fileformat arff --traintest "../data/$train_file" "../data/$test_file" $train_epochs --verbose 1 --learnertype BanditSingleStumpLearner --outputinfo "./$DIRECTORY/$target_file" --banditalgo AFTL --updaterule logedge --c $c --alpha $alpha