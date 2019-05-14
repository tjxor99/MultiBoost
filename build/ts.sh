#!/bin/bash
# cd /mnt/c/Users/ATIV/Desktop/MultiBoost/build
# cmake ..
# make all
# 
# 
dataset=$1
# 
train_epochs=10000
if [ "$dataset" == "mnist" ]; then
	train_epochs=30000
fi
# 
DIRECTORY="newest_$dataset"
if [ ! -d "$DIRECTORY" ]; then
	# echo "Making Directory $DIRECTORY ..."
	mkdir $DIRECTORY
fi
# 
train_file=$dataset
train_file+="Train.arff"
test_file=$dataset
test_file+="Test.arff"
target_file="ts.dta"

echo "Running TS for dataset $dataset"
./multiboost --fileformat arff --traintest "../data/$train_file" "../data/$test_file" $train_epochs --verbose 1 --learnertype BanditSingleStumpLearner --outputinfo "./$DIRECTORY/$target_file" --banditalgo TS --updaterule logedge