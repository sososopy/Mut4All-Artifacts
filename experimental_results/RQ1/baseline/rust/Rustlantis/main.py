import os
import subprocess
import random
import time
from datetime import datetime, timedelta

def ensure_directory(directory):
    if not os.path.exists(directory):
        os.makedirs(directory)

def run_generate(seed, output_file, hour_dir):
    ensure_directory(hour_dir)

    try:
        result = subprocess.run(['./generate', str(seed)],
                                capture_output=True, text=True)
        output_path = os.path.join(hour_dir, output_file)
        with open(output_path, 'w') as f:
            f.write(result.stdout)
    except subprocess.CalledProcessError as e:
        print(f"Error running generate with seed {seed}: {e}")

def main():
    start_time = datetime.now()
    total_hours = 24
    sequence = 1

    for hour in range(1, total_hours + 1):
        print(f"=== Hour {hour} started ===")

        hour_start_time = datetime.now()
        while (datetime.now() - hour_start_time).total_seconds() < 3600:
            hour_dir = f"rustlantis_hour_{hour}"

            seed = random.randint(1, 10**18)
            output_file = f"rustlantis_{sequence}.rs"

            run_generate(seed, output_file, hour_dir)

            sequence += 1

        print(f"=== Hour {hour} finished ===")

    print("completed")

if __name__ == "__main__":
    main()
