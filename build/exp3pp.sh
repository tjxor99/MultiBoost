#!/bin/bash
# cd /mnt/c/Users/ATIV/Desktop/MultiBoost/build
# cmake ..
# make all
# 
# 
dataset=$1
c=$2
train_epochs=$3
# 
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

target_file="exp3pp_c"
target_file+=$c
target_file+=".dta"

echo "Running EXP3++ for dataset $dataset with c = $c"
./multiboost --fileformat arff --traintest "../data/$train_file" "../data/$test_file" $train_epochs --verbose 1 --learnertype BanditSingleStumpLearner --outputinfo "./$DIRECTORY/$target_file" --banditalgo EXP3pp --updaterule logedge --c $c