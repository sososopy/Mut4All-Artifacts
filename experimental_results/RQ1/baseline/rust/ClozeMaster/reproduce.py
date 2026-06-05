import os
import re
import random
import logging

import argparse
from tqdm import tqdm
import subprocess
import time
import csv
import pandas as pd

import torch
import tokenizers
from transformers import AutoModelForCausalLM, AutoTokenizer
import pandas as pd

from model.IncoderModel import InCoder
from utils.masking import ClozeMask


os.environ["TOKENIZERS_PARALLELISM"] = "false"
def compile_rust(filepath,rsfile,opt):
    cmd="rustc {} -C opt-level={}  --out-dir /temp".format(rsfile,opt) #--out-dir temp
    time_limit=60 # stable is 180; while 60 make the reproduction faster
    p=subprocess.Popen(cmd,cwd=filepath, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True,text=True)
    try:
        out, err = p.communicate(timeout=time_limit)
        returncode = p.returncode
        if "internal compiler error" in err.lower() or "compiler unexpectedly panicked" in err.lower():
            return "ice",err
        elif returncode==137:
            return "mem err",err
        elif "process didn't exit successfully" in err.lower():
            return "crash",err
        else:
            return "ok",err
    except subprocess.TimeoutExpired:
        p.terminate()
        return "timeout","" 
    
def getTimeOutInfo(rsfile):
    cmd="rustup default nightly"
    p=subprocess.Popen(cmd,shell=True)
    p.communicate()

    cmd="rustc -Z time-passes {}".format(rsfile)
    time_limit=60 # stable is 180; while 60 make the reproduction faster
    p=subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True,text=True)
    out=""
    err=""
    try:
        out, err = p.communicate(timeout=time_limit)
        
    except subprocess.TimeoutExpired:
        p.terminate()
        try:
                if out =="" and err=="":
                    out, err = p.communicate(timeout=5) 
        except subprocess.TimeoutExpired:
                out, err = '', '' 
    cmd="rustup default 1.73"
    p=subprocess.Popen(cmd,shell=True)
    p.communicate()
    return out+err

if __name__=="__main__":
    
    parser = argparse.ArgumentParser()
    parser.add_argument('--model_path', type=str, default = "/clozeMaster/model/Incoder1b")#your model path
    parser.add_argument('--tokenizer_path', type=str, default="/clozeMaster/model/Incoder1b")# #your tokenizer path
    
    parser.add_argument('--seedfile', type=str, default = './reproduce/116681')#your rust files path,default is the rust files path

    parser.add_argument('--time',type=int,default=100)# whether test with different opts

    args = parser.parse_args()


    logging.basicConfig(level=logging.INFO 
                    ,filename="reproduce.log"
                    ,filemode="w" 
                    ,format="%(asctime)s - %(name)s - %(levelname)-9s - %(filename)-8s : %(lineno)s line - %(message)s" 
                    
                    ,datefmt="%Y-%m-%d %H:%M:%S" 
                    )
    
    cnt=args.time


    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model_path = args.model_path
    tokenizer_path = args.tokenizer_path
    incoder = InCoder(model_path, tokenizer_path, device)
    temp_rs_file="./temp/temp.rs"
    if os.path.exists(temp_rs_file):
        os.remove(temp_rs_file)

    for i in tqdm(range(cnt)):
        with open(args.seedfile+"/seed.rs",'r',errors='ignore') as f:
            code = f.read()
        new_code=incoder.code_infilling(code,temperature=0.2)
        with open(temp_rs_file,'w') as f:
            f.write(new_code)
        status,err=compile_rust("./temp","temp.rs","0")
        if status!="ok":
            print("Reproduce the bug of ["+status+"] of "+args.seedfile+";with turns of "+str(i+1))
            logging.info("bug type is:"+status)
            if status=="timeout":
                err=getTimeOutInfo(args.seedfile+"/reproduce_bug.rs")
            logging.info("err:"+err)
            with open(args.seedfile+"/reproduce_bug.rs","w") as f:
                f.write(new_code)
            break
