instrumentor_path=/home/ubuntu/TyMut-OOPSLA25-Artifacts-main/fuzzer/build/bin/mutation-instrument

base_output_folder="results"

start_ts=$(date +%s)

time_i=0
time_max=25
file_i=1

while true
do
    for i in $(seq 1 52); do

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

        echo "$i"
        target_file="$output_folder/$file_i-$i.cpp"
        cp seed/$file_i.cpp $target_file
        ls -l $target_file
        (ulimit -St 50; $instrumentor_path $target_file --mutator=$i --sourcedir=$output_folder/ -- )
        
        if cmp -s "seed/$file_i.cpp" "$target_file"; then
            echo "Files are identical. Deleting $target_file"
            rm "$target_file"
        else
            echo "Files are different. Keeping $target_file"
        fi
    done
    
    file_i=$((file_i + 1))

done