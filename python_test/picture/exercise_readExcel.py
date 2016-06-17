#!/usr/bin/python
#-*- coding: utf-8 -*-
import xlrd
import xlwt

#文件路径
filename = '/home/ubuntu-user/test_data.xlsx'
#打开文件
excel = xlrd.open_workbook(filename)
#两种方式获取表单：索引和名字
#索引：excel文件第二个表单
#excel.sheet_by_index(1）
#名字：excel表单的名称
sheet = excel.sheet_by_name(u'Topology Discovery Time')
#获取每一行的信息
#for rownum in range(sheet.nrows):
#    print sheet.row_values(rownum)#打印（列表）
for colnum in range(sheet.ncols):
    print sheet.col_values(colnum)
#获取第一列的信息
first_columu = sheet.col_values(0)#默认从0开始读
#第一行一般是标题，如果想从第二行开始读，应改为：
for num in range (1,len(first_columu)):
#     print first_columu[num].encode('utf-8')
     print first_columu[num]
#获取第三行第二列的内容
#cell(row,col)
cell_B1 = sheet.cell(2,1).value
print cell_B1


#创建一个文件
workbook = xlwt.Workbook(encoding = 'utf-8')
#增加一个表单
booksheet = workbook.add_sheet('S1')
#增加一行
#row = booksheet.row(i)
#增加一列
#col = booksheet.col(j)
#为某个单元格赋值
#booksheet.write(i,j,'Test')
#row.write(j,'Test')
#1col.write(i,'Test')
#注：在add_sheet时，置参数cell_overwrite_ok=True，可以覆盖原单元格中数据cell_overwrite_ok默认为False,覆盖的话会抛出异常。

booksheet1 = workbook.add_sheet('S2',cell_overwrite_ok = True)
#workbook.add_sheet('S2')
DATA = (('学号','姓名','年龄','性别','成绩'),
        (1001,'AAAA',23,'男',98),
        (1002,'BBBB',21,'女',90),
        (1003,'CCCC',22,'女',100),
        (1004,'DDDD',21,'男',86))
#枚举，
for i, row in enumerate(DATA):
    for j, col in enumerate(row):
        booksheet1.write(i,j,col)
booksheet1.col(0).width=10
workbook.save('成绩单.xlsx')
#读取一个文件
#workbook = xlrd.open_workbook('成绩单.xlsx')
#读取文件中表单的数量
#workbook.nsheets
#获取一个表单
#workbook.sheets()[i]
#workbook.sheet_by_index(i)
#workbook.sheet_by_name(u'S2')
#获取行，列数
#sheet.nrows
#sheet.ncols
#获取整行整列数
#sheet.row(i)
#sheet.cok(j)
#获取某个单元格数据
#sheet.cell(i,j).value
#sheet.row(i)[j].value
#sheet.col(j)[i],value
#workbookk = xlrd.open_workbook('/home/ubuntu-user/test_data.xlsx')
workbookk = xlrd.open_workbook('成绩单.xlsx')
print 'There are sheets in the workbook'.format(workbookk.nsheets)
for booksheett in workbookk.sheets():
    print booksheett.name
    for row in xrange(booksheett.nrows):
        for col in xrange (booksheett.ncols):
           print xlrd.cellname(row,col)
#            print booksheett.cell(row,col).value


