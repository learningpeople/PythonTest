#eaort/bin/python
#-*- coding:utf-8 -*-

__author__ = 'xu'

import urllib
import configparser
#import urllib.parse
import json
import unittest
#from htmlreport import *
import htmlreport

html_report = htmlreport.HtmlReport()
class ConfigHttp:
    """
    用于封装http请求方法，http头设置
    """
    def __init__(self,host,port):
        self.host = host
        self.port = port
        self.headers = {}#http 头
    "HTTP有GET和POST两种请求方式，GET：请求指定的页面信息，并返回实体主体；POST：请求服务器接受所指定的的文档作为对所标识的URI的新的从属实体。"
    #设置http头
    def set_header(self,headers):
        self.headers = headers
    
    #封装HTTP GET请求方法
    def get(self,url,params):
        params = urllib.parse.urlencode(params) 
        #将参数转为url编码字符串
        url = 'http://'+self.host+':'+str(self.port)+url+params
        request = urllib.request.Request(url,headers=self.headers)
        try:
            response = urllib.request.urlopen(request)
            response = response.read().decode('utf-8')
            #decode函数对获取的字节数据进行解码
            json_request = json.loads(responsea)
            #将返回数据转为json格式的数据
            return json_response
        except Exception:
            print('no json data returned')
            return{}

    #封装HTTP POST请求方法
    def post(self,url,data):
        data = json.dump(data)
        data = data.encode('utf-8')
        url = 'http://'+self.host+':'+str(self.port)+url
        try:
            request = urllib.request.Request(url,headers=self.headers)
            response = urllib.request.urlopen(request,data)
            response = response.read().decode('utf-8')
            json_response = json.loads(response)
            print(json_response)
            return json_response
        except Exception:
            print ('no json data returned')
            return {}
        #封装HTTP xxx 请求方法
        #自由扩展


#定义结构体
class DataStruct:
    """
    用于接收excel读取的测试数据，记录要写入测试报告的数据
    """
    pass

test_data = DataStruct()
test_data.url = ''   #接收借口url
test_data.params = {}  #接收借口参数
test_data.expected_result = {}  #接收预期结果
test_data.result = 'Fail'   #接收测试结果

#测试用例组（类）
class TestInterfaceCase(unittest.TestCase):
    def setUp(self):
        self.config_http = ConfigHttp(base_http.get_host(),base_http.get_port())

    #测试接口1
    def test_get_diffcheckcode(self):
        #根据被测接口的实际情况，合理的添加HTTP头
        header = {
            'Accept':'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
            'User-Agent':'Mozilla/5.0(Windows NT 6.1;RV:29.0) Gecko/20100101 Firefox/29.0'
        }

        self.config_http.set_header(header)

        response = self.config_http.get(test_data.url,test_data.params)

        if {} == response:
            test_data.result = 'Error'
            html_report.error_num = html_report.reeor_num + 1
            return
        try:
            self.assertEqual(response['msg'],test_data.expected_result,msg='exception')
            test_data.result = 'Pass'
            html_report.success_num = html_report.success_num + 1
        except AssertionError:
            test_data.result = 'Fail'
            html_report.fail_num = html_report.fail_num + 1

    #测试接口2
    def test_get_checkcode(self):
        header = {
            'Content-Type':'application/json',
            'charset':'utf-8'
            }
        self.config_http.set_header(header)
        response = self.config_http.post(test_data.url,tast_data.params)
        if {} == response:
            test_data.result = 'Error'
            html_report.error_num = html_report.error_num + 1
            return
        try:
            self.assertEqual(response['msg'],test_data.expected_result,msg='exception')
            print(response['msg'])
            test_data.result = 'Pass'
            html_report.success_num = html_report.success_num + 1
        except AssertionError:
            test_data.result = 'Fail'
            html_report.fail_num = html_report.fail_num + 1

    def tearDown(self):
        pass

#获取测试套件
def get_test_suite(index):
    test_suite = unittest.TestSuite()
    function = sheet1.row_values(index)[8]
    #根据选择的用例，获取对应的测试用例方法。
    test_suite.addTest(TestInterfaceCase(function))
    return test_suite

#运行测试用例函数
def run_case(sheet,runner,config_file=''):
    html_report.case_total = 0
    config = configparser.ConfigParser()
    #从配置文件中读取要运行测试的测试用例所在行索引
    config.read(config_file)
    try:
        run_mode = config['DEFAULT']['runmode']
        run_mode = int(run_mode) #把字符串类型的转换为list
        html_report.run_mode = run_mode
    except Exception:
        print('error happend in case config_file')

    if True == run_mode:  #运行全部用例
        #获取用例个数
        test_case_num = sheet.nrows

        #循环执行测试用例
        for index in range(1,test_case_num):
            test_data.url = sheet.row_values(index)[4]
            test_data.params = json.loads(sheet.row_values(index)[5])
            test_data.expected_result = sheet.row_values(index)[6]
            test_suite = get_test_suite(index)
            runner.run(test_suite)

            #记录运行结果
            sheet1.put_cell(index,7,1,test_data_result,0)
            #测试用例数加1
            html_report.case_total = html_report.case_total + 1
    else:
        try:
            case_list = config['DEFAULT']['index']
            case_list = eval(case_list)
            #把字符串类型的list转换为list
            html_report.case_list = case_list
        except Exception:
            print('error happend in case config_file')

        for index in case_list:
            test_data.url = sheet.row_values(index)[4]
            test_data.params = json.loads(sheet.row_values(index)[5])
            test_data.expected_result = sheet.row_values(index)[6]
            test_suite = get_test_suite(index)
            runner.run(test_suite)

            #记录运行结果
            sheet1.put_cell(index,7,1,test_data.result,0)

            #测试用例数加1
            html_report.case_total = html_report.case_total +1


