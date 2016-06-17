#!/usr/bin/python
import xlrd
import math
import os,re,sys
def notNone(lists):
    v = []
    for i in lists:
        if i == 'N/A':
            i = 0
        if i != '':
            v.append(i)
    return v
def readExcel(filepath,name):
    cols = []
    filepath = filepath
    data = xlrd.open_workbook(filepath)
    sheet = data.sheet_by_name(name)
    ncols = sheet.ncols 
    for col in xrange(ncols):
        cols.append(sheet.col_values(col))
    return cols
def shoplistaExcel(lists):
    value = []
    x_list = []
    x_labels= []
    for i in lists:
        v = notNone(i)
        value.append(v)
    l = len(value)
    for i in range(l):
        x_label = value[i][0]
        x = value[i][1:]
#        print x_label
        x_labels.append(x_label)
        x_list.append(x)   
    return x_list,x_labels
def readlog(files,arg_list):
    filepath = files#get_path(files)
    v = []
    with open(filepath) as f:
        for line in f .readlines():
            if len(arg_list) == 1:
                a = arg_list[0]
                if a in line:
                    v.append(line.split())
            else:
                for i in arg_list :
                    if i in line:
                        v.append(line.split())
#    n = os.path.split(filename)
#    name = n[1]
    return v
def get_path(files):
    pwd = sys.path[0]
    print pwd
    path = os.path.abspath(os.path.join(pwd,os.pardir))
    filepath = os.path.join(pwd,files)
    return filepath
def shoplistlog(lists):
    v = []
    for i in lists:
        value=notNone(i)
        v.append(value)
    x = [' '.join(i[2:]) for i in v]
    a = [i[1] for i in v]
    y = [[eval(i) for i in a]]
    return x,y
#def get_filename(file)
def pivot(X,Y,line_n):
    
    x = X[0][1:]
    y = Y[n-1][1:]
    x_label = [0][0]
    y_label = [n-1][0]
    v = []
    n = len(Y)
    if line_n == 3:
#        n = len(lists)
        for i in range(n/3):
            v.append(y[(i*3):(i+1)*3])
    else:
        x = X[0][1:]
        y = Y[n-1][1:]
        x_label = [0][0]
        Y_label = [n-1][0]
    return v,x,y,x_label,y_label 
        
   

    
        
    





