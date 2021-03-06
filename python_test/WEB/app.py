#!/usr/bin/python
#-*- coding:utf-8 -*-
from flask import Flask
from flask import request

app = Flask(__name__)

@app.route('/',methods=['GET','POST'])
def home():
    return '<h1>Home</h1>'

@app.route('/signin',methods=['GET'])
def signin_form():
    return '''<form action="/signin" method="post">
           <p><input name="username"></p>
           <p><input name="password" type="password"></p>
           <p><button type="submit">Sign In</button></p>
           </form>'''

@app.route('/signin',methods=['POST'])
def signin():
    #需要从request对象读取表单内容：
    if request.form['username']=='admin' and request.form['password']=='password':
        return '<h3>Hello, admin!</h3>'
    return '<h3>Bad username or password.</h3>'

if __name__ == '__main__':
    app.run(host='0.0.0.0',port=8000)
#    app.run()


#一个函数处理一个URL：
#* GET / ：首页，返回Home；
#* GET /signin ：登陆页，显示登陆表单；
#* POST /signin ：处理登陆表单，显示登陆结果。
#注：同一个URL /signin，分别有GET和POST两种请求，分别映射到两个处理函数中。
