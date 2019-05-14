#!/bin/bash
# cd /mnt/c/Users/ATIV/Desktop/MultiBoost/build
# cmake ..
# make all
# 
# 
dataset=$1
c=$2
alpha=$3
train_epochs=$4


DIRECTORY="newest_$dataset"
if [ ! -d "$DIRECTORY" ]; then
	mkdir $DIRECTORY
fi
# 
train_file=$dataset
train_file+="Train.arff"
test_file=$dataset
test_file+="Test.arff"

target_file="ABE_c"
target_file+=$c
target_file+="_alpha"
target_file+=$alpha
target_file+=".dta"

echo "Running ABE for dataset $dataset with params c = $c, alpha = $alpha"
./multiboost --fileformat arff --traintest "../data/$train_file" "../data/$test_file" $train_epochs --verbose 1 --learnertype BanditSingleStumpLearner --outputinfo "./$DIRECTORY/$target_file" --banditalgo ABE --updaterule logedge --c $c --alpha $alpha