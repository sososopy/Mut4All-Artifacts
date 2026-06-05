#!/bin/bash

csmith_path="csmith/build/bin/csmith"
csmith_flags=""

base_output_folder="results"

start_ts=$(date +%s)

time_i=0
time_max=25
file_i=1

while true
do
    current_ts=$(date +%s)

    elapsed_time=$((current_ts - start_ts))
    threshold=$((time_i * 10))

    if [ "$elapsed_time" -ge $threshold ]; then
        echo "elapsed time: $elapsed_time"
        echo "threshold: $threshold"
        time_i=$((time_i + 1))
        if [ "$time_i" -ge "$time_max" ]; then
            break
        fi
        output_folder="$base_output_folder/$time_i"
        mkdir -p "$output_folder"
        echo "$time_i:start at $(date -d @$current_ts "+%F %T") output to $output_folder"
    fi

    (ulimit -St 50; $csmith_path $csmith_flags > "$output_folder/$file_i.c")
    file_i=$((file_i + 1))

done
