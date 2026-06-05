import numpy as np
import random
import os
from tqdm import tqdm
from pathlib import Path

def random_delete_line(code,p=0.2):
    '''
    code:str
    p:float
    '''
    code = code.split('\n')
    code = [i for i in code if random.random() > p]
    code = '\n'.join(code)
    return code

def random_swap_line(code,p=0.4):
    '''
    code:str
    p:float
    '''
    code = code.split('\n')
    for i in range(len(code)):
        if random.random() < p:
            
            j = random.randint(0,len(code)-1)

            code[i],code[j] = code[j],code[i]
    code = '\n'.join(code)
    return code


def random_delete_word(code,p=0.2):
    '''
    code:str
    p:float
    '''
    code = code.split(' ')
    code = [i for i in code if random.random() > p]
    code = ' '.join(code)
    return code

def random_swap_word(code,p=0.4):
    '''
    code:str
    p:float
    '''
    code = code.split(' ')
    for i in range(len(code)):
        if random.random() < p:
            
            j = random.randint(0,len(code)-1)

            code[i],code[j] = code[j],code[i]
    code = ' '.join(code)
    return code