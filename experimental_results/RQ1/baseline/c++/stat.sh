work_dir_list=(CSmith Grayc MetaMut Tymut YARPGen-2.0)
base_dir=$(pwd)

for work_dir in ${work_dir_list[@]}; do
    echo ">>>>>> Processing $work_dir <<<<<<"
    total=0
    for i in $(seq 1 24); do
        cur_dir="$work_dir/results/$i"
        if [ -d "$cur_dir" ]; then
            nums=$(ls | wc -l)
            total=$((total + nums))
        else
            echo "Folder $i does not exist in $work_dir"
            exit 1
        fi
    done
    echo "Total files in $work_dir: $total"
    echo "Average files per hour in $work_dir: $((total / 24))"
    echo "Counting lines of code in $work_dir"
    cloc $work_dir/results
done
