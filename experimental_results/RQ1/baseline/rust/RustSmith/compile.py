import os
import subprocess
import random
import time
from datetime import datetime

def run_generate(seed, out_dir):
    os.makedirs(out_dir, exist_ok=True)

    try:
        result = subprocess.run(
            ['../rustsmith', 'BASE_SELECTION', '-n', '10',
             '--directory', out_dir,
             '-s', str(seed)],
            check=True,
            capture_output=True,
            text=True
        )
        return True
    except subprocess.CalledProcessError as e:
        print(f"Error running rustsmith with seed {seed} in {out_dir}: {e}")
        print(f"stdout: {e.stdout}")
        print(f"stderr: {e.stderr}")
        return False

def main():
    start_time = datetime.now()
    sequence = 1

    while (datetime.now() - start_time).total_seconds() < 1 * 3600:
        out_dir = f"rustsmith_{sequence}"
        seed = random.randint(1, 10**18)

        if run_generate(seed, out_dir):
            sequence += 1
        else:
            print(f"Failed at sequence {sequence}, continuing...")

if __name__ == "__main__":
    main()
