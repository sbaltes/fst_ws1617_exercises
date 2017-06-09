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

    # REMOTE BRANCHES
    branch_output=`git branch -r`

    branches=""
    next_is_default=false
    default_branch=""
    default_branch_latest_commit=""

    # iterate over branch output to determine default branch
    for item in ${branch_output:2}
    do
      # determine default branch
      if [ "$item" == "origin/HEAD" ]; then 
        continue
      fi

      if [ "$item" == "->" ]; then
        next_is_default=true
        continue
      fi

      if [ "$next_is_default" == true ] ; then
        default_branch="$item"
        default_branch_latest_commit=`git log -n 1 $item --pretty=format:"%H"`
        next_is_default=false
        echo "Default branch: $default_branch"
        continue    
      fi

      branches=$branches$item$'\n'
    done

    # write header for branches csv
    branches_csv=$targetdir$clearname"_branches.csv"
    echo $'branch,default_branch,latest_commit,merge_base' > $branches_csv 

    # iterate over branches to save git log, latest commit, and merge-base
    for remote_branch in ${branches}
    do
      branch="${remote_branch/origin\/}"

      # check out current branch and save git log
      if [ "$remote_branch" == "$default_branch" ] ; then
        git checkout $branch
      else
        git checkout -b $branch $remote_branch 
      fi

      targetfile=$targetdir$clearname"§"$branch".log"
      echo "Remote branch: $remote_branch"
      echo "Branch name: $branch"
      echo "Target file: $targetfile"
      
      git config merge.renameLimit 999999
      git config diff.renameLimit 999999

      git log --date=iso --numstat --diff-filter=ADM > $targetfile

      output=$branch
      if [ "$remote_branch" == "$default_branch" ]; then
        output=$output",true"
      else
        output=$output",false"
      fi

      # append latest commit
      latest_commit=`git log -n 1 $remote_branch --pretty=format:"%H"`
      output=$output","$latest_commit

      # append merge-base
      args=$default_branch_latest_commit" "$latest_commit
      merge_base=`git merge-base $args`
      output=$output","$merge_base$'\n'

      echo $output >> $branches_csv
    done

    cd ..

    rm -rf $clearname

done < "$filename"
