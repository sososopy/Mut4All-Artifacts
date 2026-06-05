import os
import re
import random
import logging
from tqdm import tqdm
import subprocess
import time
import logging


class ClozeMask():
    def __init__(self, left_token_list=['(','[','{','<'], right_token_list=[')',']','}','>'],mask_num=1,mask_token='<insert>'):
        self.left_token_list = left_token_list
        self.right_token_list = right_token_list
        self.mask_num = mask_num
        self.mask_token = mask_token

    def find_parentheses_indices_list(self,string):
        indices = []

        def helper(string, start_index,left,right):
            if not string:
                return

            stack = []
            for i in range(len(string)):
                if string[i] == left:
                    stack.append(i)
                elif string[i] == right:
                    if stack:
                        indices.append((start_index + stack.pop(), start_index + i))

            if stack:
                
                offset = len(string) - start_index
                new_start_index = start_index + offset
                helper(string[offset:], new_start_index,left,right)

        for(left,right) in zip(self.left_token_list,self.right_token_list):
            helper(string, 0, left, right)
        return indices
    
    def select_mask_indices(self,indices):
        mask_indices = []
        if len(indices) <= self.mask_num:
            mask_indices = indices
        else:
            mask_indices = random.sample(indices,self.mask_num)
        return mask_indices

    def mask_select_code(self,code):
        indices = self.find_parentheses_indices_list(code)
        mask_indices = self.select_mask_indices(indices)
        for (start,end) in mask_indices:
            code = code[:start] + self.mask_token + code[end:]
        return code
    

    def mask_singel_code(self,code):
        indices = self.find_parentheses_indices_list(code)
        codes=[]
        for (start,end) in indices:
            new_code = code[:start+1] + self.mask_token + code[end:]
            codes.append(new_code)
        return codes
    
    

        

    

if __name__=="__main__":
    clozemask = ClozeMask()
    code='''
 typedef double v4d __attribute__ ((vector_size (32), aligned (32)));
 v4d f (v4d x)
 {
   return x;
 }
 int main ()
 {
   v4d x = { 1.0, 2.0, 3.0, 4.0, };
   v4d r = f (x);
 }
'''
    codes=clozemask.mask_singel_code(code)
    for c in codes:
        print(c)
        print("===================================")

    