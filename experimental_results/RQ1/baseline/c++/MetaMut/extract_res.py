import sys
import os
from datetime import datetime, timedelta
from shutil import copy

def get_time_from_log(log_file):
    with open(log_file, 'r') as f:
        lines = f.readlines()
        for line in lines:
            time_stamp = line.split("[")[1].split("] DEBUG:")[0].strip()
            return time_stamp

def get_timestamp_seconds(timestamp_str):
    timestamp = datetime.strptime(timestamp_str, "%Y-%m-%d %H:%M:%S.%f")
    timestamp_seconds = timestamp.timestamp()
    return timestamp_seconds
            
def organize_files_by_interval(file_data, interval_seconds=3600):
    file_data.sort(key=lambda x: get_timestamp_seconds(x["timestamp"]))
    
    first_timestamp = get_timestamp_seconds(file_data[0]["timestamp"])
    current_interval_start = first_timestamp
    folder_counter = 1

    print(f"Organizing files into folders based on intervals of {interval_seconds} seconds...")
    print(f"First timestamp: {first_timestamp}, Interval start: {current_interval_start}")
    
    for entry in file_data:
        file_path = entry["file"]
        timestamp_str = entry["timestamp"]
        timestamp = get_timestamp_seconds(timestamp_str)

        if timestamp >= current_interval_start + interval_seconds:
            folder_counter += 1
            current_interval_start = timestamp
            print(f"Creating new folder for interval starting at {current_interval_start} (Folder {folder_counter})")

        folder_name = str(folder_counter)
        folder_path = os.path.join(os.getcwd(), "results", folder_name)

        if not os.path.exists(folder_path):
            os.makedirs(folder_path)

        target_path = os.path.join(folder_path, os.path.basename(file_path))
        copy(file_path, target_path)
        print(f"(Timestamp: {timestamp_str}) {file_path} copied to {target_path}")


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python extract_res.py <workspace> <interval_seconds>")
        sys.exit(1)

    workspace = sys.argv[1]
    interval_seconds = int(sys.argv[2])
    file_data = []
    for x in os.listdir(workspace):
        cur_dir = os.path.join(workspace, x)
        log_file = os.path.join(cur_dir, "log.txt")
        generated_file = None
        for f in os.listdir(cur_dir):
            if f.endswith(".c"):
                generated_file = os.path.join(cur_dir, f)
        if generated_file is None:
            continue

        generate_time = get_time_from_log(log_file)
        file_data.append({
            "file": generated_file,
            "timestamp": generate_time,
        })

    organize_files_by_interval(file_data, interval_seconds)

    

    

