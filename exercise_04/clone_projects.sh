#!/usr/bin/bash

targetdir="/media/jascha/530abec2-0ac0-4858-bf4b-242cc3dfc37d/git-logs/"


# Clone all repos

filename="$1"
counter=0

while read -r line
do
    counter=$((counter+1))
    echo "##################################### Now we process project #$counter ###############################################"
    echo "The time is: "
    date
    echo "###############################################################################################################"
    name="$line"
    echo "Name read from file - $name"

    github="https://www.github.com/"
    git=".git"
    gitlink=$github$name$git

    clearname=${name//[\/]/_}
    echo clearname: $clearname

    git clone $gitlink $clearname

    cd $clearname

    ## MASTER-BRANCH

    defmaster="false";

    branches=`git branch -a`
    for item in ${branches:2}
    do

      branch=${item//[\/]/$}
      if [ "$branch" == "master" ]; then
        defmaster = "true";
      fi

      if [ "$item" == "->" ]; then
        continue
      fi
      if [ "$defmaster" == "true" ]; then
        if [ "$item" == "origin/master" ]; then
          continue
        fi
      fi
      if [ "$item" == "remotes/origin/HEAD" ]; then
        continue
      fi

      git checkout $item
      targetfile=$targetdir$clearname"§"$branch".log"
      echo "Item: $item"
      echo "Targetfile: $targetfile"

      git config merge.renameLimit 999999
      git config diff.renameLimit 999999

      git log --date=iso --numstat > $targetfile

    done

    cd ..

    rm -rf $clearname

done < "$filename"
