#!/usr/bin/puthon
#-*- coding:utf-8 -*-

#__author__ = 'xu'

from pyh import *
import unittest,xlrd,xlwt
import time,os 
from confighttp import *
#import confighttp.run_case as run_case

class HtmlReport():
    def __init__(self):
        self.title = 'test_report_page'  #网页标签名称
        self.filename = ''   #结果文件名
        self.time_took = '00:00:00'   #测试耗时
        self.success_num = 0   #测试通过的用例数
        self.fail_num = 0   #测试失败的用例数
        self.error_num = 0   #运行出错的用例
        self.case_total = 0    #运行测试用例总数
        self.run_mode = 1    #运行模式
        self.case_list = []    #只运行部分用例模式下，存放运行过的用例所在行索引
        
        #生成HTML报告
    def gennerate_html(self,head,sheet,file):
        page = PyH(self.title)
        page << h1(head,align='center') #标题居中
        page << p('测试总耗时：'+self.time_took)
        page << p('测试用例数：'+str(self.case_total)+'&nbsp'*10 
                 +'成功用例数：'+str(self.success_num)+'&nbsp'*10
                 +'失败用例数：'+str(self.fail_num))
        #表格标题caption 表格边框border 单元边沿与其内容之间的空白cellpadding  单元个之间间隔为cellspacing
        tab = table(border='1',cellpadding= '1',cellspacing='0',cl='table')
        tab1 = page << tab
        tab1 << tr(td("用例ID",bgcolor='#ABABAB',align='center')
                  +td('用例名称',bgcolor='#ABABAB',align='center')
                  +td('接口名称',bgcolor='#ABABAB',align='center')
                  +td('接口协议',bgcolor='#ABABAB',align='center')
                  +td('URL',bgcolor='#ABABAB',align='center')
                  +td('参数/数据',bgcolor='#ABABAB',align='center')
                  +td('测试结果',bgcolor='#ABABAB',align='center'))
        if True == self.run_mode:
            test_case_num = sheet.nrows
            for index in range (1,test_case_num):
                tab1 << tr(td(int(sheet.row_values(index)[0]),align='center')+td(sheet.row_values(index)[1])+
                           td(sheet.row_values(index)[2])+td(sheet.row_values(index)[3],align='center')+
                           td(sheet.row_values(index)[4])+td(sheet.values[5])+
                           td(sheet.row_values(index)[7],align='center'))
        else:
            for index in range (1,test_case_num):
                tab1 << tr(td(int(sheet.row_values(index)[0]),align='center')+td(sheet.row_values(index)[1])+
                           td(sheet.row_values(index)[2])+td(sheet.row_values(index)[3],align='center')+
                           td(sheet.row_values(index)[4])+td(sheet.values[5])+
                           td(sheet.row_values(index)[7],align='center'))
        self._set_result_filename(file)
        page.printOout(self.filename)
    #设置文件名
    def _set_result_filename(self,filename):
        self.filename = filename
        #判断是否为目录
        if os.path.isdir(self.filename):
            raise IOError("%s must point to a file"%path)
        elif '' == self.filename:
            raise IOError('filename can not be empty')
        else:
            parent_path,ext = os.path.splitext(filename)
            #os.path.splitext() 分离文件名与扩展名
            tm = time.strftime('%Y%m%d%H%M%S',time.localtime())
            self.filename = parent_path + tm + ext
    #统计运行耗时
    def time_caculate(self,seconds):
        self.time_took = time.strftime('%H:%M:%S',time.gmtime(seconds))
        return self.time_took

    #运行测试套件
if __name__ == '__main__':
    start_time = time.time()
    runner = unittest.TextTestRunner()
    html_report = HtmlReport()

    #读取用例数据
    excel = xlrd.open_workbook('/home/ubuntu-user/project/sometests/HTTP/TextCase.xlsx')
    sheet1 = excel.sheet_by_index(0)

   # run_case = TestInterfaceCase.run_case()
    run_case(sheet1,runner,'/home/ubuntu-user/project/sometests/HTTP/case_config.ini')

    #测试结束时间
    end_time = time.time()
    html_report.time_caculate(end_time-start_time)  #计算测试消耗时间
    #生成测试报告
    html_report.generate_html('test report ',sheet1,'/home/ubuntu-user/report.html')
    

