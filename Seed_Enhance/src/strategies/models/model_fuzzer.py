import os
import urllib.parse
import requests
import json
from zhipuai import ZhipuAI
import os
import re
import random
import logging
from tqdm import tqdm
import subprocess
import time

from typing import List

import torch
import tokenizers
from transformers import AutoModelForCausalLM, AutoTokenizer

PROMPT = """You are a rust professor aimed in finding bugs in rust compilers. You need to give rust code which makes rust compiler throw Internal Compiler Error. You can use any nightly feature and items in std crate. All features has been enabled, no need to enable them. Generate codes as strange as possible, and contains various structures and features. Just contains exectuable codes."""


class ModelFuzzerBase:
    def __init__(self, args):
        """
        Initializes the ModelFuzzerBase with the provided arguments.

        Args:
            args: Arguments for the model fuzzer.
        """

    def infill(self, code_prefix: str, code_suffix: str) -> str:
        """
        Infills the code between the prefix and suffix.

        Args:
            code_prefix (str): The prefix of the code.
            code_suffix (str): The suffix of the code.

        Returns:
            str: The infilled code.
        """
        raise NotImplementedError("Subclasses should implement this method.")


def load_model(model_path, tokenizer_path, device):
    kwargs = {}
    logging.info(f"loading model from {model_path} ...")
    model = AutoModelForCausalLM.from_pretrained(model_path, 
        device_map="auto",
        trust_remote_code=True, **kwargs)

    model = model.to(device)
    logging.info(f"loading tokenizer from {tokenizer_path} ...")
    tokenizer = AutoTokenizer.from_pretrained(tokenizer_path)
    return model, tokenizer

# class Qwen2_5:
#     def __init__(self, model_path):
#         model = AutoModelForCausalLM.from_pretrained(
#             model_path,
#             # torch_dtype="auto",
#             device_map="auto",
#             trust_remote_code=True
#         )
#         device = "cuda" if torch.cuda.is_available() else "cpu"
#         self.model = model.to(device)
#         self.tokenizer = AutoTokenizer.from_pretrained(
#             model_path,
#             pad_token='<|endoftext|>',
#             eos_token='<|im_end|>',
#             padding_side="right",
#             trust_remote_code=True
#         )
#         self.tokenizer.add_special_tokens(
#             {"additional_special_tokens": ["<|im_end|>", "<|im_start|>"]})
#         print(f"model loaded from {model_path}")

#     def __gen_tokens(self, tokenizer, code_prefix="", code_middle="", code_suffix=""):
#         nl_tokens = tokenizer('\n').input_ids
#         fim_prefix = tokenizer('<|fim_prefix|>').input_ids
#         fim_suffix = tokenizer('<|fim_suffix|>').input_ids
#         fim_middle = tokenizer('<|fim_middle|>').input_ids
#         prefix = fim_prefix + tokenizer(code_prefix).input_ids + fim_suffix + tokenizer(
#             code_suffix).input_ids + fim_middle + nl_tokens
#         prefix_len = len(prefix)
#         other_tokens = tokenizer.encode(
#             text=code_middle, padding=True, truncation=True, add_special_tokens=True
#         )
#         other_tokens = other_tokens + [tokenizer.eos_token_id]
#         res = prefix + other_tokens
#         return (res, prefix_len)
    
#     def generate(self, code_prefix="", code_suffix="", gen_max = 128):
#         (input_ids, _) = self.__gen_tokens(
#             self.tokenizer, code_prefix=code_prefix, code_suffix=code_suffix
#         )
#         with torch.no_grad():
#             output = self.model.generate(
#                 input_ids=input_ids.to(self.model.device),
#                 do_sample=True,
#                 top_p=0.95,
#                 temperature=0.4,
#                 max_length=len(input_ids) + gen_max,
#             )
#             torch.cuda.empty_cache()
        



class InCoder:
    def __init__(self, model_path, tokenizer_path, device):
        self.model, self.tokenizer = load_model(
            model_path, tokenizer_path, device)
        self.device = device
        self.BOS = "<|endoftext|>"
        self.EOM = "<|endofmask|>"

    def make_sentinel(self, i):
        return f"<|mask:{i}|>"

    def generate(self,  input, max_to_generate=128, temperature=0.2):
        input_ids = self.tokenizer(input, return_tensors="pt").input_ids
        input_ids = input_ids.to(self.device)
        max_length = max_to_generate + input_ids.flatten().size(0)
        if max_length > 2048:
            logging.warning(
                "warning: max_length {} is greater than the context window {}".format(max_length, 2048))
        with torch.no_grad():
            output = self.model.generate(
                input_ids=input_ids, do_sample=True, top_p=0.95, temperature=temperature, max_length=max_length)
            torch.cuda.empty_cache()
        # pass clean_up_tokenization_spaces=False to avoid removing spaces before punctuation, e.g. "from ." -> "from."
        detok_hypo_str = self.tokenizer.decode(
            output.flatten(), clean_up_tokenization_spaces=False)
        if detok_hypo_str.startswith(self.BOS):
            detok_hypo_str = detok_hypo_str[len(self.BOS):]
        return detok_hypo_str

    def infill(self, parts, max_to_generate=128, temperature=0.2, extra_sentinel=True, max_retries=1):
        assert isinstance(parts, list)
        retries_attempted = 0
        done = False

        while (not done) and (retries_attempted < max_retries):
            retries_attempted += 1

            # (1) build the prompt
            if len(parts) == 1:
                prompt = parts[0]
            else:
                prompt = ""
                # encode parts separated by sentinel
                for sentinel_ix, part in enumerate(parts):
                    prompt += part
                    if extra_sentinel or (sentinel_ix < len(parts) - 1):
                        # print("sentinel_ix:",sentinel_ix)
                        # prompt += self.make_sentinel(sentinel_ix)
                        prompt += f"<|mask:{sentinel_ix}|>"

            infills = []
            complete = []

            done = True

            # (2) generate infills
            for sentinel_ix, part in enumerate(parts[:-1]):
                complete.append(part)
                # prompt += self.make_sentinel(sentinel_ix)
                prompt += f"<|mask:{sentinel_ix}|>"
                # TODO: this is inefficient as it requires re-encoding prefixes repeatedly
                completion = self.generate(
                    prompt, max_to_generate, temperature)
                completion = completion[len(prompt):]
                if self.EOM not in completion:

                    completion += self.EOM
                    done = False
                completion = completion[:completion.index(
                    self.EOM) + len(self.EOM)]
                infilled = completion[:-len(self.EOM)]
                infills.append(infilled)
                complete.append(infilled)
                prompt += completion
            complete.append(parts[-1])
            text = ''.join(complete)

        return {
            # str, the completed document (with infills inserted)
            'text': text,
            # List[str], length N. Same as passed to the method
            'parts': parts,
            # List[str], length N-1. The list of infills generated
            'infills': infills,
            # number of retries used (if max_retries > 1)
            'retries_attempted': retries_attempted,
        }

    def code_infilling(self, maskedCode, max_to_generate=128, temperature=0.2):
        parts = maskedCode.split("<insert>")
        result = self.infill(
            parts, max_to_generate=max_to_generate, temperature=temperature)
        # print("completed code:")
        # print(result["text"])
        return result["text"]


class ModelFuzzerIncoder(ModelFuzzerBase):
    def __init__(self, args):
        self.model = InCoder(
            args.model,
            args.model,
            device="cuda" if torch.cuda.is_available() else "cpu"
        )

    def infill(self, code_prefix: str, code_suffix: str) -> str:
        return self.model.code_infilling(code_prefix + "<insert>" + code_suffix, max_to_generate=128, temperature=0.4)


class ModelFuzzerZhipuAI(ModelFuzzerBase):
    def __init__(self, args):
        API_KEY = os.getenv("API_KEY")
        if API_KEY is None:
            raise ValueError("API_KEY environment variable is not set.")
        self.client = ZhipuAI(api_key=API_KEY)

    def infill(self, code_prefix: str, code_suffix: str) -> str:
        r_code_prefix = "// " + PROMPT + "\n" + code_prefix
        resp = self.client.chat.completions.create(
            model="codegeex-4",
            messages=[
                # {
                #     "role": "system",
                #     "content": "You are a rust professor aimed in finding bugs in rust compilers. You need to give rust code which makes rust compiler throw Internal Compiler Error. You can use any nightly feature and items in std crate. All features has been enabled. Generate codes as strange as possible, and contains various structures and features."
                # },
            ],
            extra={
                "target": {
                    "path": "fuzzer_code.rs",
                    "language": "rust",
                    "code_prefix": r_code_prefix,
                    "code_suffix": code_suffix,
                },
                "contexts": []
            },
            top_p=0.8,
            temperature=0.95,
            max_tokens=1024,
            stop=["<|endoftext|>", "<|user|>",
                  "<|assistant|>", "<|observation|>"]
        )
        code_inner = resp.choices[0].message.content
        code = code_prefix + code_inner + code_suffix
        return code


class ModelFuzzerLlama(ModelFuzzerBase):
    def __init__(self, args):
        url = os.getenv("LLAMA_URL", "http://localhost:8080")
        self.url = urllib.parse.urljoin(url, "completion")

    def infill(self, code_prefix: str, code_suffix: str) -> str:
        fim_prefix = '<|fim_prefix|>'
        fim_suffix = '<|fim_suffix|>'
        fim_middle = '<|fim_middle|>'

        msg = [
            fim_prefix,
            code_prefix,
            fim_suffix,
            code_suffix,
            fim_middle,
        ]
        msg = "".join(msg)
        res = requests.post(self.url, json={"prompt": msg}, timeout=10)
        msg = res.json()
        code_middle = msg["content"]
        code = code_prefix + code_middle + code_suffix
        return code


class ModelFuzzer:
    def __init__(self, args):
        """
        Magic class init different model based on args.model
        """
        if args.model.lower() == "zhipuai":
            self.fuzzer = ModelFuzzerZhipuAI(args)
        elif args.model.lower() == "llama":
            self.fuzzer = ModelFuzzerLlama(args)
        else:
            raise ValueError(f"Unknown model: {args.model}")

    def infill(self, code_prefix: str, code_suffix: str) -> str:
        """
        Infills the code between the prefix and suffix.

        Args:
            code_prefix (str): The prefix of the code.
            code_suffix (str): The suffix of the code.

        Returns:
            str: The infilled code.
        """
        return self.fuzzer.infill(code_prefix, code_suffix)


if __name__ == "__main__":
    fuzzer = ModelFuzzer({
        "model": "llama"
    })
    infilled_code = fuzzer.infill("", "")
    print(infilled_code)
