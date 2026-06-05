import os
import subprocess
import time
from datetime import datetime, timedelta

def run_one_hour(hour_num, compile_script_path):
    folder_name = f"rustsmith_hour_{hour_num}"
    os.makedirs(folder_name, exist_ok=True)

    process = subprocess.Popen(
        ['python3', compile_script_path],
        cwd=folder_name
    )
    return process

def main():
    total_hours = 24
    start_time = datetime.now()

    compile_script_path = "../compile.py"

    for hour in range(1, total_hours + 1):
        print(f"=== Hour {hour} started ===")

        proc = run_one_hour(hour, compile_script_path)

        next_hour_time = start_time + timedelta(hours=hour)
        while True:
            time.sleep(10)
            if datetime.now() >= next_hour_time:
                break

        proc.terminate()
        try:
            proc.wait(timeout=10)
        except subprocess.TimeoutExpired:
            proc.kill()
            print(f"Force killed process for hour {hour}")

        print(f"=== Hour {hour} finished ===")

    print("completed")

if __name__ == "__main__":
    main()
