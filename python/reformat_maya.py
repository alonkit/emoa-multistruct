"""
About: python helper APIs
Author: Zhongqiang (Richard) Ren
"""

import numpy as np
import sys
import subprocess



def reformat(src_fname, dst_fname, headers):
  with open(src_fname) as f:

    num_nodes, num_edges= f.readline().split()
    edges={}
    line = f.readline().strip()
    while line:
      parts = line.split()
      edges[f'{parts[0]} {parts[1]}'] = parts[2:]
      line = f.readline().strip()
    
    for i,h in enumerate(headers):
      with open(dst_fname+'-'+h+'.gr','x') as f:
        f.write(f'p sp {num_nodes} {num_edges}\n')
        for edge, values in edges.items():
          f.write(f'a {edge} {values[i]}\n')

  
    
if __name__ == "__main__":
    reformat(r'C:\Users\alonk\technion\a-project\emoa-multistruct\data\ToMaya\ToMaya\NY-road-l-d-t-m-r.txt',r'C:\Users\alonk\technion\a-project\emoa-multistruct\data\NY-road','l-d-t-m-r'.split('-'))


