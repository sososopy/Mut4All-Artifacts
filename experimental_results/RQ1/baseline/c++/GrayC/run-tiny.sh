#!/bin/bash
build=/home/ubuntu/baseline/grayc/GrayC-issta-2023/ISSTA-2023-AE/fuzzer/work_dir/copy_1/llvm-csmith-1/llvm-fuzzer-build
setA=/home/ubuntu/baseline/grayc/GrayC-issta-2023/ISSTA-2023-AE/fuzzer/work_dir/copy_1/setA/
setB=/home/ubuntu/baseline/grayc/GrayC-issta-2023/ISSTA-2023-AE/fuzzer/work_dir/copy_1/setB
seeds=/home/ubuntu/baseline/grayc/GrayC-issta-2023/ISSTA-2023-AE/fuzzer/seeds/seeds_out1.txt

# Set the coprus to be as in the expriments
cd $build/../../ ; rm -rf $setA $setB; cp -r setA-Controlle-experiments-data-12-Nov-21 $setA 

# Start the running for 24 Hours
curr_ts=`date -d '+0 day +0 hour +0 minutes' '+%F %T'`
end_ts=`date -d '+0 day +0 hour +15 minutes' '+%F %T'`
echo "Starts GrayC Fuzzer (Aggressive mode) $curr_ts and will end at $end_ts"
end_total=`date -d "$end_ts" +%s`

# Use this script to run GrayC for 24 Hours, or any other fuzzer that is LibFuzzer based
cd $build
rm -rf $build/fuzz* $build/tar_prev_* $build/independent-grayc/ independent-grayc/crash-*
mkdir $build/fuzzer-output-directory-prev/
while IFS= read -r seed ; do
  timestamp=`date +"%m-%d-%y-%T"`
  echo "Running GrayC Fuzzer (Aggressive mode) with seed $seed $timestamp"
  (bin/clang-fuzzer $setA -detect_leaks=0 -max_len=1000000 -rss_limit_mb=4096 -create_missing_dirs=1 -ignore_crashes=1 -ignore_timeouts=1 -ignore_ooms=1 -seed=$seed -timeout=50 -max_total_time=86400)

  # Keep the data and prepare for re-run
  fdupes -dN $setA # Remove duplicates
  tar -czvf $build/fuzzed-files-$seed.tar.gz $build/fuzzer-output-directory/ > tar_prev_$seed.txt 2>&1

  for i in $build/fuzzer-output-directory/*; do
    echo "Copy to PREV. $i"
    cp "$i" $build/fuzzer-output-directory-prev/
  done

  # Check if we need to exit
  curr_ts=`date -d '+0 day +0 hour +0 minutes' '+%F %T'` # Update current
  curr_total=`date -d "$curr_ts" +%s`
  if [ "$curr_total" -ge "$end_total" ] ; then
    echo "DONE: bin/grayc-aggressive $setA ... "
    exit
  fi
done < "$seeds"
echo "DONE: bin/grayc-aggressive $setA ... "

python3.10 extract_res.py $build/fuzzer-output-directory-prev/ 30
