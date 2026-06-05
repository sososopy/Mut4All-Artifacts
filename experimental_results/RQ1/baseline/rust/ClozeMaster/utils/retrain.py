import torch
import tokenizers
from transformers import AutoModelForCausalLM, AutoTokenizer
from torch.utils.data import DataLoader, TensorDataset
from torch.optim import AdamW
import torch.nn as nn 
import pandas as pd
import os
import random
from tqdm import tqdm

# You can modify the training parameters below based on your computer's configuration.
epochs = 100
learning_rate = 1e-5
batch_size = 16

# other configurations
train_data_path='./dataset'
finetune_saved_path="./weight/finetuned"
source_model_path="./weight/source" # You need to fill in the path where the pre-trained model weights are stored, for example "/home/facebook/incoder-1B"



torch.backends.cuda.deterministic = True
os.environ['TORCH_USE_CUDA_DSA'] = '1'

model_name = source_model_path 
tokenizers_version = tuple(int(n) for n in tokenizers.__version__.split('.'))
if tokenizers_version < (0, 12, 1):
    print("warning: Your tokenizers version looks old and you will likely have formatting issues. We recommend installing tokenizers >= 0.12.1")
kwargs = {}
print("loading model")
model = AutoModelForCausalLM.from_pretrained(model_name, **kwargs)

tokenizer = AutoTokenizer.from_pretrained(model_name)
tokenizer.pad_token = "<pad>"
tokenizer.padding_side = "left"
print("loading complete")

def get_rs_files(directory,suffix=['rs']):
    rs_files = []
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.split('.')[-1] in suffix:
                rs_files.append(os.path.join(root, file))
    return rs_files



rs_files=get_rs_files(train_data_path)
input_sentences=[]
for rsfile in rs_files:
    with open(rsfile, "r") as file:
        try:
            code=file.read()
        except:
            continue
    input_sentences.append(code)
print("train dataset contains:",len(input_sentences))
input_ids = tokenizer(input_sentences, padding=True, truncation=True, return_tensors="pt", add_special_tokens=True)



for name, param in model.named_parameters():
    
    try:
        layer_num = int(name.split(".")[2])
    except:
        continue
    
    
    if layer_num <= 20:
        param.requires_grad = False
    else:
        continue


criterion = nn.CrossEntropyLoss()
optimizer = AdamW(filter(lambda p: p.requires_grad, model.parameters()), lr=learning_rate)


train_dataset = TensorDataset(input_ids["input_ids"])
train_loader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True)


cnt=0
for epoch in tqdm(range(epochs)):
    model.train()
    total_loss = 0
    for batch in tqdm(train_loader):
        inputs = batch[0]
        cnt+=1
        
        
        optimizer.zero_grad()
        
        outputs = model(inputs, labels=inputs) 
        loss = outputs.loss
        
        loss.backward()
        optimizer.step()
        total_loss += loss.item()
        torch.cuda.empty_cache()
    
    avg_loss = total_loss / len(train_loader)
    print(f"Epoch {epoch+1}, Average Loss: {avg_loss}")



model.save_pretrained(finetune_saved_path)

