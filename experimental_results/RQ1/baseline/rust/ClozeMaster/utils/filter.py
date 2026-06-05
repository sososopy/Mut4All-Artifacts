import os
from tqdm import tqdm
import subprocess

def get_rs_files(directory,suffix=['rs']):
    rs_files = []
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.split('.')[-1] in suffix:
                rs_files.append(os.path.join(root, file))
    return rs_files

def compile_rust(filepath,rsfile,opt):
    cmd="rustc {} -C opt-level={}  --out-dir /temp".format(rsfile,opt) #--out-dir temp
    time_limit=180
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

rs_files=get_rs_files("/clozeMaster/dataset")

for rsfile in tqdm(rs_files):
    filename=rsfile.split('/')[-1]
    filepath=os.path.dirname(rsfile)
    res,err=compile_rust(filepath,filename,"0")
    if "test" in rsfile:
        continue
    if res!="ok":
        if os.path.exists(rsfile):
            os.remove(rsfile)
            print(rsfile)