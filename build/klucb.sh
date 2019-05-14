#!/bin/bash
# cd /mnt/c/Users/ATIV/Desktop/MultiBoost/build
# cmake ..
# make all
# 
# 
dataset=$1
train_epochs=$2
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

echo "Running KL-UCB for dataset $dataset"
./multiboost --fileformat arff --traintest "../data/$train_file" "../data/$test_file" $train_epochs --verbose 1 --learnertype BanditSingleStumpLearner --outputinfo "./$DIRECTORY/klucb.dta" --banditalgo KLUCB --updaterule logedge