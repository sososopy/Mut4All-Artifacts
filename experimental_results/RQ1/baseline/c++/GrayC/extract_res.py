import sys
import os
from datetime import datetime, timedelta
from shutil import copy

def get_time_from_file(file_name):
    file_time = "-".join(file_name.split("-")[3:]).split(".")[0]
    return file_time

def get_timestamp_seconds(timestamp_str):
    timestamp = datetime.strptime(timestamp_str, "%Y-%m-%d-%H:%M:%S")
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
        file_name = x
        file_time = get_time_from_file(file_name)
        file_path = os.path.join(workspace, file_name)

        file_data.append({
            "file": file_path,
            "timestamp": file_time,
        })

    organize_files_by_interval(file_data, interval_seconds)

    

    

