import os
import logging
import random
import copy
import json

from tqdm import tqdm
import torch
from transformers import AutoModelForCausalLM, AutoTokenizer, get_scheduler
from transformers import TrainingArguments, Trainer, DataCollatorForSeq2Seq
from peft import LoraConfig, get_peft_model, TaskType
from torch.utils.data import DataLoader, Dataset
import deepspeed

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)


def Args():
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('-e', '--epochs', type=int, default=10)
    parser.add_argument('-b', '--batch_size', type=int, default=None)
    parser.add_argument('-lr', '--learning_rate', type=float, default=1e-5)
    parser.add_argument('-i', '--input', type=str, help='input dir', nargs='+')
    parser.add_argument('-m', '--model', type=str,
                        help='model path', default='./model')
    parser.add_argument('-s', '--save', type=str,
                        help='save path', default='./model_out')
    dtype_group = parser.add_mutually_exclusive_group()
    dtype_group.add_argument('--fp16', action='store_true',
                             help='use fp16', default=False)
    dtype_group.add_argument('--bf16', action='store_true',
                             help='use bf16', default=False)
    parser.add_argument('--layers', type=int, default=10)
    parser.add_argument('--trainer', action='store_true',
                        help='use huggingface trainer', default=False)
    parser.add_argument('--lora', action='store_true',
                        help='enable lora trainer', default=False)

    parser.add_argument('--local_rank', type=int, default=-1,
                        help='local rank passed from distributed launcher')
    parser = deepspeed.add_config_arguments(parser)
    args = parser.parse_args()
    return args


class MyDataset(Dataset):
    def __collect_file_one_dir(self, input_dir):
        codes = []
        for root, _, files in os.walk(input_dir):
            for file in files:
                if file.endswith('.rs'):
                    with open(os.path.join(root, file), 'r', encoding="utf-8") as f:
                        code = f.read()
                        # Remove comments, empty lines, and leading/trailing whitespace
                        # lines = code.splitlines()
                        # lines = [line.strip()
                        #          for line in lines if line.strip()]
                        # in_comment = False
                        # new_lines = []
                        # for line in lines:
                        #     if line.startswith('//'):
                        #         continue
                        #     if line.startswith('/*'):
                        #         in_comment = True
                        #     if line.endswith('*/'):
                        #         in_comment = False
                        #         continue
                        #     if in_comment:
                        #         continue
                        #     new_lines.append(line)
                        # code = '\n'.join(new_lines)
                        codes.append(code)

        return codes

    def __generate_train_data(self, input_dirs):
        codes = []
        for input_dir in input_dirs:
            codes.extend(self.__collect_file_one_dir(input_dir))
        return codes

    def __gen_tokens(self, tokenizer, code_prefix="", code_middle="", code_suffix=""):

        # im_start = tokenizer.im_start_id
        # im_end = tokenizer.im_end_id
        # nl_tokens = tokenizer('\n').input_ids
        # _system = tokenizer('system').input_ids
        # _user = tokenizer('user').input_ids
        # _assistant = tokenizer('assistant').input_ids
        # fim_prefix = tokenizer('<|fim_prefix|>').input_ids
        # fim_suffix = tokenizer('<|fim_suffix|>').input_ids
        # fim_middle = tokenizer('<|fim_middle|>').input_ids
        # prefix = fim_prefix + tokenizer(code_prefix).input_ids + fim_suffix + tokenizer(
        #     code_suffix).input_ids + fim_middle + nl_tokens
        # prefix_len = len(prefix)
        # other_tokens = tokenizer.encode(
        #     text=code_middle, padding=True, truncation=True, add_special_tokens=True
        # )
        # other_tokens = other_tokens + [tokenizer.eos_token_id]
        # res = prefix + other_tokens
        # return (res, prefix_len)
        nl_tokens  = '\n'
        fim_prefix = '<|fim_prefix|>'
        fim_suffix = '<|fim_suffix|>'
        fim_middle = '<|fim_middle|>'
        prefix = fim_prefix + code_prefix + fim_suffix + code_suffix + fim_middle + nl_tokens
        prefix_tokens = tokenizer(prefix, add_special_tokens=False)
        prefix_len = len(prefix_tokens.input_ids)
        resp_tokens = tokenizer(code_middle, add_special_tokens=False)
        input_ids = prefix_tokens.input_ids + resp_tokens.input_ids + [tokenizer.pad_token_id]
        attention_mask = (
            prefix_tokens.attention_mask + resp_tokens.attention_mask + [1]
        )
        labels = [-100] * prefix_len + resp_tokens.input_ids + [tokenizer.pad_token_id]
        return (input_ids, prefix_len, attention_mask, labels)

    def __init__(self, input_dirs, tokenizer, args):
        TOKEN_MAX_LEN = 1024
        codes = self.__generate_train_data(input_dirs)

        datas = []
        for code in codes:
            if len(code) <= 5:
                continue
            # if len(code) > 1024:
            #     continue  # OOM...

            # (tokens, prefix_len) = self.__gen_tokens(
            #     tokenizer,
            #     code_prefix="",
            #     code_middle=code,
            #     code_suffix=""
            # )
            # if len(tokens) > TOKEN_MAX_LEN:
            #     continue
            # labels = [-100] * prefix_len + tokens[prefix_len:]
            # datas.append((tokens, labels))

            code_split_point = random.randrange(1, len(code))

            (tokens, _, attention_mask, labels) = self.__gen_tokens(
                tokenizer,
                code_prefix=code[:code_split_point],
                code_middle=code[code_split_point:],
                code_suffix=""
            )
            if len(tokens) > TOKEN_MAX_LEN:
                tokens = tokens[:TOKEN_MAX_LEN]
                labels = labels[:TOKEN_MAX_LEN]
                attention_mask = attention_mask[:TOKEN_MAX_LEN]
            datas.append((tokens, attention_mask, labels))

            code_split_front = random.randrange(1, len(code) // 2)
            code_split_back = random.randrange(len(code) // 2, len(code))

            (tokens, _, attention_mask, labels) = self.__gen_tokens(
                tokenizer,
                code_prefix=code[:code_split_front],
                code_middle=code[code_split_front:code_split_back],
                code_suffix=code[:code_split_back]
            )
            if len(tokens) > TOKEN_MAX_LEN:
                tokens = tokens[:TOKEN_MAX_LEN]
                labels = labels[:TOKEN_MAX_LEN]
                attention_mask = attention_mask[:TOKEN_MAX_LEN]
            datas.append((tokens, attention_mask, labels))

        self.datas = datas

    def __len__(self):
        return len(self.datas)

    def __getitem__(self, idx):
        data = self.datas[idx]
        return {
            "input_ids": data[0],
            "attention_mask": data[1],
            "labels": data[2]
        }


class DataCollator(object):
    def __init__(self, tokenizer):
        self.tokenizer = tokenizer
        self.pad_token_id = tokenizer.pad_token_id

    def __call__(self, batch):
        lengths = [
            len(i[0]) for i in batch
        ]
        max_len = max(lengths)

        tokens = []
        labels = []
        for i in batch:
            token = i[0]
            label = i[1]

            padding_len = max_len - len(token)
            token = token + [self.pad_token_id] * padding_len
            label = label + [-100] * padding_len

            tokens.append(token)
            labels.append(label)

        return {
            "input_ids": torch.tensor(tokens, dtype=torch.long),
            "labels": torch.tensor(labels, dtype=torch.long)
        }


def save_model(model, args):
    os.makedirs(args.save, exist_ok=True)
    model = copy.deepcopy(model)
    if args.lora:
        smodel = model.merge_and_unload()
    else:
        smodel = model
    smodel.save_pretrained(args.save)
    logger.info("model saved to %s", args.save)


def save_model_epoch(model, args, epoch):
    model_p = os.path.join(args.save, f"{epoch}")
    os.makedirs(model_p, exist_ok=True)
    model = copy.deepcopy(model)
    if args.lora:
        smodel = model.merge_and_unload()
    else:
        smodel = model
    smodel.save_pretrained(model_p)
    logger.info("model saved to %s", model_p)


def print_trainable_parameters(model):
    trainable_params = 0
    all_params = 0
    for _, param in model.named_parameters():
        num_params = param.numel()
        all_params += num_params
        if param.requires_grad:
            trainable_params += num_params
    print(f"Total param: {all_params}")
    print(f"Trainable param: {trainable_params}")
    print(f"Percentage: {100 * trainable_params / all_params:.2f}%")


def self_train(model, tokenizer, dataset, device, args):
    train_size = int(len(dataset) * 0.8)
    eval_size = int(len(dataset) * 0.15)
    rem_size = len(dataset) - train_size - eval_size
    train_dataset, eval_dataset, rem_dataset = torch.utils.data.random_split(
        dataset, [train_size, eval_size, rem_size]
    )
    model = model.to(device)
    model.train()

    # collator = DataCollator(tokenizer)
    collator = DataCollatorForSeq2Seq(tokenizer = tokenizer, model=model, padding=True)
    train_loader = DataLoader(train_dataset, collate_fn=collator,
                              batch_size=args.batch_size, shuffle=True, num_workers=0)

    eval_loader = DataLoader(eval_dataset, collate_fn=collator,
                             batch_size=args.batch_size, shuffle=True, num_workers=0)

    optimizer = torch.optim.AdamW(model.parameters(), lr=args.learning_rate)
    lr_scheduler = get_scheduler(
        name='linear', optimizer=optimizer, num_warmup_steps=0,
        num_training_steps=len(train_loader) * args.epochs
    )
    if args.fp16:
        scaler = torch.amp.GradScaler(device=device)
    else:
        scaler = None

    loss_history = []
    eval_loss_history = []
    for epoch in tqdm(range(args.epochs)):
        model.train()
        losses = 0.
        optimizer.zero_grad()
        cnt = 0
        for batch in (pbar := tqdm(train_loader)):
            optimizer.zero_grad()
            inputs = batch
            inputs = {k: v.to(device) for k, v in inputs.items()}

            if args.fp16:
                with torch.cuda.amp.autocast(device_type=device):
                    outputs = model(**inputs)
                    loss = outputs.loss
            elif args.bf16:
                with torch.amp.autocast(device_type=device, dtype=torch.bfloat16):
                    outputs = model(**inputs)
                    loss = outputs.loss
            else:
                outputs = model(**inputs)
                loss = outputs.loss
            losses += float(loss.item())

            if args.fp16:
                scaler.scale(loss).backward()
                scaler.step(optimizer)
                scaler.update()
            else:
                loss.backward()
                optimizer.step()
            lr_scheduler.step()

            cnt += 1
            pbar.set_description(f"epoch {epoch}, loss: {losses / cnt:.2f}")

            del inputs
            del outputs
            del loss
            torch.cuda.empty_cache()

        logger.info("epoch %d, loss: %f", epoch, losses / len(train_loader))
        loss_history.append(losses / len(train_loader))

        #  Evaluate
        model.eval()
        eval_losses = 0.
        for batch in tqdm(eval_loader):
            inputs = batch
            inputs = {k: v.to(device) for k, v in inputs.items()}

            with torch.no_grad():
                outputs = model(**inputs)
                loss = outputs.loss
            eval_losses += float(loss.item())

            del inputs
            del outputs
            del loss
            torch.cuda.empty_cache()

        logger.info("eval epoch %d, loss: %f",
                    epoch, eval_losses / len(eval_loader))
        eval_loss_history.append(eval_losses / len(eval_loader))

        save_model_epoch(model, args, epoch)

    with open('train_log.json', 'w', encoding='utf-8') as f:
        json.dump(loss_history, f)
    with open('eval_log.json', 'w', encoding='utf-8') as f:
        json.dump(eval_loss_history, f)
    save_model(model, args)


def trainer_train(model, tokenizer, dataset, device, args):
    train_size = int(len(dataset) * 0.8)
    eval_size = int(len(dataset) * 0.15)
    rem_size = len(dataset) - train_size - eval_size
    train_dataset, eval_dataset, rem_dataset = torch.utils.data.random_split(
        dataset, [train_size, eval_size, rem_size]
    )
    model = model.to(device)
    training_args = TrainingArguments(
        output_dir=args.save,
        per_device_train_batch_size=args.batch_size,
        per_device_eval_batch_size=args.batch_size,
        num_train_epochs=args.epochs,
        learning_rate=args.learning_rate,
        logging_dir='./logs',
        logging_strategy="epoch",
        save_strategy="epoch",
        save_total_limit=5,
        eval_strategy="epoch",
        bf16=args.bf16,
        fp16=args.fp16,
        gradient_accumulation_steps=2,
        load_best_model_at_end=True,
        deepspeed=None if not args.deepspeed else args.deepspeed_config,
        push_to_hub=False
    )
    trainer = Trainer(
        model=model,
        args=training_args,
        data_collator=DataCollatorForSeq2Seq(tokenizer = tokenizer, model=model, padding=True),
        train_dataset=train_dataset,
        eval_dataset=eval_dataset,
        tokenizer=tokenizer,
    )
    trainer.train()

    with open('train_log.json', 'w', encoding='utf-8') as f:
        json.dump(trainer.state.log_history, f)
    save_model(model, args)


def train(model, tokenizer, dataset, device, args):
    if args.trainer:
        trainer_train(model, tokenizer, dataset, device, args)
    else:
        self_train(model, tokenizer, dataset, device, args)


def main(args):
    if torch.cuda.is_available():
        device = 'cuda'
    # elif torch.npu.is_available():
        # device = 'npu'
    else:
        device = 'cpu'
    logger.info("device: %s", device)
    model_path = args.model

    model = AutoModelForCausalLM.from_pretrained(
        model_path,
        # torch_dtype="auto",
        device_map="auto",
        trust_remote_code=True
    )
    tokenizer = AutoTokenizer.from_pretrained(
        model_path,
        pad_token='<|endoftext|>',
        eos_token='<|im_end|>',
        padding_side="right",
        trust_remote_code=True
    )
    tokenizer.add_special_tokens(
        {"additional_special_tokens": ["<|im_end|>", "<|im_start|>"]})
    logger.info("model loaded from %s", model_path)

    if args.lora:
        lora_config = LoraConfig(
            task_type=TaskType.CAUSAL_LM,
            r=8,
            lora_alpha=32,
            lora_dropout=0.1
        )
        model = get_peft_model(model, lora_config)
    else:
        for name, param in model.named_parameters():
            try:
                idx = int(name.split('.')[2])
            except:
                continue
            if idx < args.layers:
                param.requires_grad = False
            else:
                param.requires_grad = True

    logger.info("model: \n%s", repr(model))

    print_trainable_parameters(model)

    logger.info("loading codes from %s", args.input)
    dataset = MyDataset(args.input, tokenizer, args)
    logger.info("train data loaded")
    logger.info("train data size: %d", len(dataset))

    train(model, tokenizer, dataset, device, args)


if __name__ == '__main__':
    a = Args()
    main(a)
