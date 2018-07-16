#!/bin/sh
if [ $# -ne 1 ]; then
    echo "Please specify search algorithm name as argument."
    echo "algorithm name: hill_climbing, ga, random"
    exit 1
fi
if [ $1 != "hill_climbing" -a $1 != "ga" -a $1 != "random" ]; then
    echo "Please specify the correct search algorithm name as argument."
    echo "algorithm name: hill_climbing, ga, random"
    exit 1
fi
git checkout -b search
cat .gitignore_search > .gitignore
git add .gitignore
git add target_code
cd src
if [ $1 = "hill_climbing" ]; then
    python hill_climbing_main.py
elif [ $1 = "ga" ]; then
    python ga_main.py
elif [ $1 = "random" ]; then
    python random_search_main.py
fi
cd ../
git reset HEAD .gitignore
git reset HEAD target_code
git checkout master
git checkout .gitignore
git checkout target_code
git branch -D search
