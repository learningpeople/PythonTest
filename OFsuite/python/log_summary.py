#!/usr/bin/python
#-*- coding:utf-8 -*-

import xlrd
import xlwt
import re
#path = input('please input the file path and name:')
#name = input('Please input the sheet name:')
path = '/home/ubuntu-user/H3C.xlsx'
name = 'Sheet1'
excel = xlrd.open_workbook(path)
sheet = excel.sheet_by_name(name)
contents = []
for rownum in range(sheet.nrows):
    contents.append(sheet.row_values(rownum))
#print contents
fail_all = []
result = []
dic = {}
zone_num = []
zone = ['10.','40.','50.','60.','80.','90.','100.','130.','140.','150.','180.','200.','210.','230.','250.','260.','300.','310.','320.','330.','340.','380.','390.','410.','420.','430.','440.','450.']
for row in contents:

    if 'F' or 'f' in row:
        fail_all.append(row)
dic['FALL']=len(fail_all)
def fail_find(arg):
    fail_one = []
    for ones in fail_all:
        if arg in str(ones[0]):
            fail_one.append(ones)
    return arg,len(fail_one)
def part_sum(arg):
    one = {}
    for ones in contents:
#        print ones
        if arg in str(ones[0]):
            one[arg] = len(ones)
    print len(ones)
    return len(one)
total = len(zone)
print total
for value in zone:
    res,num = fail_find(str(value))
    part = part_sum(value)
#    percentage_total = float(num\total)
#    percentage_part = float(num\part)
    dic[res] = (num,part)#percentage_total,percentage_part) 

print dic,result#,fail_all

workbook = xlwt.Workbook(encoding='utf-8')
booksheet = workbook.add_sheet('summary',cell_overwrite_ok=True)

DATA = ()

#for row in fail_all:
#    if '10.' in row:
#        print len(row)
#    if '40.' in row:
#        print

#fail_zone = []
#fail_one = []
 
#test_list = range(0,500,10)

#for row in contents:
#    if '10.' in row:
#        if 'FALL' in row :
#            fail_one.append(contents)
#            len(row)
#print fail_one,row
#fail_zone.append(fail_one)
#print fail_zone
#print fail_all,fail_zone


    





