import os
import random
import argparse
import time
import logging

from tqdm import tqdm
import torch

from model.IncoderModel import InCoder
from utils.masking import ClozeMask

os.environ["TOKENIZERS_PARALLELISM"] = "false"

def get_rs_files(directory, suffix=['rs']):
    rs_files = []
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.split('.')[-1] in suffix:
                rs_files.append(os.path.join(root, file))
    return rs_files


def ensure_dir_exists(directory):
    if not os.path.exists(directory):
        os.makedirs(directory)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--model_path', type=str, default="./model/Incoder1b")
    parser.add_argument('--tokenizer_path', type=str, default="./model/Incoder1b")
    parser.add_argument('--rs_files', type=str, default='./dataset/code_set')
    parser.add_argument('--output_dir', type=str, default='./output5')
    parser.add_argument('--hours', type=int, default=24)
    args = parser.parse_args()

    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    print(f"Using device: {device}")

    incoder = InCoder(args.model_path, args.tokenizer_path, device)
    cloze_mask = ClozeMask()

    rs_files_all = get_rs_files(args.rs_files)
    random.shuffle(rs_files_all)

    ensure_dir_exists(args.output_dir)

    logging.basicConfig(
        level=logging.INFO,
        filename=os.path.join(args.output_dir, "cloze_generation.log"),
        filemode="w",
        format="%(asctime)s - %(levelname)-9s - %(message)s",
        datefmt="%Y-%m-%d %H:%M:%S"
    )

    logging.info(f"Total rs files found: {len(rs_files_all)}")

    start_time = time.time()
    hour_seconds = 3600

    for hour in range(1, args.hours + 1):
        hour_dir = os.path.join(args.output_dir, f"clozemaster_hour_{hour}")
        ensure_dir_exists(hour_dir)
        logging.info(f"Starting hour {hour}, saving to {hour_dir}")

        hour_start_time = time.time()

        if not rs_files_all:
            logging.info(f"No more rs files left to process at hour {hour}. Exiting.")
            break

        rs_files_hour = rs_files_all.copy()

        for rs_file in tqdm(rs_files_hour, desc=f"Hour {hour} - remaining seeds: {len(rs_files_all)}"):
            try:
                with open(rs_file, 'r', errors='ignore') as f:
                    code = f.read() 
            except Exception as e:
                logging.warning(f"Failed to read file {rs_file}: {e}")
                rs_files_all.remove(rs_file)
                continue

            if len(code) > 500:
                logging.info(f"Skipping long file: {rs_file} ({len(code)} chars)")
                rs_files_all.remove(rs_file)
                continue

            #mask
            masked_codes = cloze_mask.mask_singel_code(code)

            cnt = 0
            for masked_code in masked_codes:
                cnt += 1

                # infilling
                new_code = incoder.code_infilling(masked_code, temperature=0.2)

                filename = os.path.basename(rs_file)
                base_name, ext = os.path.splitext(filename)
                new_filename = f"{base_name}_{cnt}{ext}"

                new_filepath = os.path.join(hour_dir, new_filename)

                with open(new_filepath, 'w', encoding='utf-8', errors='ignore') as f:
                    f.write(new_code)

                logging.info(f"Generated: {new_filepath}")

            rs_files_all.remove(rs_file)

            if time.time() - hour_start_time > hour_seconds:
                logging.info(f"Hour {hour} ended due to time limit.")
                break

        logging.info(f"Finished hour {hour}.")

    logging.info(f"All done for {args.hours} hours.")

