#!/bin/sh
branch_name=`git rev-parse --abbrev-ref HEAD`
git checkout target_code
git reset HEAD .gitignore
git reset HEAD target_code
git checkout master
git checkout .gitignore
git checkout target_code
git branch -D $branch_name
