#!/bin/sh
git checkout -b search
cat .gitignore_search > .gitignore
git add .gitignore
git add target_code
cd src
if [ $1 = "hill_climbing" ]; then
    python hill_climbing_main.py
elif [ $1 = "ga" ]; then
    python ga_main.py
elif [ $1 = "random"]; then
    python random_search_main.py
fi
cd ../
git checkout .
git reset HEAD target_code
git checkout master
git reset HEAD --hard
git branch -D search
