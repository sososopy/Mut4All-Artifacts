import os
import logging
import random
import copy
import json

from tqdm import tqdm
import torch
from transformers import AutoModelForCausalLM, AutoTokenizer, get_scheduler
from transformers import TrainingArguments, Trainer
from peft import LoraConfig, get_peft_model, TaskType
from torch.utils.data import DataLoader, Dataset
import deepspeed

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)


class Qwen2_5:
    def __init__(self, model_path):
        model = AutoModelForCausalLM.from_pretrained(
            model_path,
            # torch_dtype="auto",
            device_map="auto",
            trust_remote_code=True
        )
        model = model.bfloat16()
        model = model.eval()
        # model = model.half()
        device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
        tokenizer = AutoTokenizer.from_pretrained(
            model_path,
            pad_token='<|endoftext|>',
            eos_token='<|im_end|>',
            padding_side="right",
            trust_remote_code=True
        )
        self.BOS = '<|im_start|>'
        self.EOM = '<|fim_middle|>'
        self.EOS = '<|im_end|>'
        self.PAD = '<|endoftext|>'
        tokenizer.add_special_tokens(
            {"additional_special_tokens": ["<|im_end|>", "<|im_start|>"]})
        logger.info("model loaded from %s", model_path)
        self.model = model.to(device)
        self.tokenizer = tokenizer

    def __gen_tokens(self, tokenizer, code_prefix="", code_suffix=""):

        # nl_tokens = tokenizer('\n').input_ids
        # fim_prefix = tokenizer('<|fim_prefix|>').input_ids
        # fim_suffix = tokenizer('<|fim_suffix|>').input_ids
        # fim_middle = tokenizer('<|fim_middle|>').input_ids
        # prefix = fim_prefix + tokenizer(code_prefix).input_ids + fim_suffix + tokenizer(
        #     code_suffix).input_ids + fim_middle + nl_tokens
        # return prefix
        nl_tokens = '\n'
        fim_prefix = '<|fim_prefix|>'
        fim_suffix = '<|fim_suffix|>'
        fim_middle = '<|fim_middle|>'
        prefix = fim_prefix + code_prefix + fim_suffix + \
            code_suffix + fim_middle + nl_tokens
        return prefix

    def generate(self,  input, max_to_generate=128, temperature=0.2):
        input_ids = self.tokenizer(input, return_tensors="pt").input_ids
        input_ids = input_ids.to(self.model.device)
        max_length = max_to_generate + input_ids.flatten().size(0)
        if max_length > 2048:
            logging.warning(
                "warning: max_length {} is greater than the context window {}"
                .format(max_length, 2048))
        with torch.no_grad():
            output = self.model.generate(
                input_ids=input_ids,
                do_sample=True,
                top_p=0.95,
                temperature=temperature,
                pad_token_id=self.tokenizer.pad_token_id,
                # max_length=max_length,
                max_new_tokens=max_to_generate,
            )
            torch.cuda.empty_cache()
        detok_hypo_str = self.tokenizer.decode(
            output.flatten(), clean_up_tokenization_spaces=False)
        if detok_hypo_str.startswith(self.BOS):
            detok_hypo_str = detok_hypo_str[len(self.BOS):]
        return detok_hypo_str

    def infill(self, prefix, suffix, max_to_generate=128, temperature=0.2, extra_sentinel=True, max_retries=1):
        retries_attempted = 0
        done = False

        prompt = self.__gen_tokens(self.tokenizer, prefix, suffix)
        r_prefix = prefix

        while (not done) and (retries_attempted < max_retries):
            retries_attempted += 1

            done = True
            completion = self.generate(
                prompt, max_to_generate, temperature)
            completion = completion[len(prompt):]
            if self.EOS not in completion and \
                    self.PAD not in completion:
                completion += self.PAD
                # done = False
            completion = completion[:completion.index(
                self.PAD) + len(self.PAD)]
            infilled = completion[:-len(self.EOM)]
            prompt += completion
            r_prefix += infilled

        return r_prefix + suffix


def infill_one_file(qwen, input_file, output_file):
    with open(input_file, "r", encoding="utf-8") as f:
        code = f.read()

    # The split part is <|[Break Here]|>

    while "<|[Break Here]|>" in code:
        prefix, suffix = code.split("<|[Break Here]|>", 1)
        # print(prefix)
        # print(suffix)
        infilled = qwen.infill(prefix, suffix)
        code = infilled

    with open(output_file, "w", encoding="utf-8") as f:
        f.write(code)

import os
if __name__ == "__main__":
    args = os.sys.argv
    begin = 0
    end = 100000000000
    if len(args) > 1:
        begin = int(args[1])
    if len(args) > 2:
        end = int(args[2])
    model_path = "QwenCoder_Save"
    qwen = Qwen2_5(model_path)
    input_dirs = [
        "llm_input"
    ]
    output_dir = "llm_output"

    os.makedirs(output_dir, exist_ok=True)

    file_count = 0
    for input_dir in input_dirs:
        for root, dirs, files in os.walk(input_dir):
            for file in files:
                if file.endswith(".rs"):
                    file_idx = int(file.split(".")[0].split("_")[-1])
                    if file_idx < begin or file_idx >= end:
                        continue
                    file_count += 1
    print(f"Total files to process: {file_count}")
    print("Processing files...")

    pbar = tqdm(total=file_count, desc="Processing files", unit="file")
    for input_dir in input_dirs:
        for root, dirs, files in os.walk(input_dir):
            for file in files:
                if file.endswith(".rs"):
                    file_idx = int(file.split(".")[0].split("_")[-1])
                    if file_idx < begin or file_idx >= end:
                        continue
                    input_file = os.path.join(root, file)
                    pbar.set_postfix(file=file)
                    output_file = os.path.join(
                        output_dir, file.replace(".rs", "_out.rs"))
                    infill_one_file(qwen, input_file, output_file)
                    pbar.update(1)
