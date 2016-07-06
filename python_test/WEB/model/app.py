#!/usr/bin/python
#-*- coding:utf-8 -*-
from flask import Flask,request,render_template
import jinja2

app = Flask(__name__)

@app.route('/',methods=['GET','POST'])
def home():
    return render_template('home.html')
#    return '<h>Home</h>'

@app.route('/signin',methods=['GET'])
def signin_form():
    return render_template('form.html')
#    return '''<form action='/signin' method="post">
#              <p><input name="username"></p>
#              <p><input name="password" type="password"></p>
#              <p><button type="submit">Sign In</button></p>
#              </form>'''

@app.route('/signin',methods=['POST'])
def signin():
    username = request.form['username']
    password = request.form['password']
    if username == 'admin' and password == 'password':
        return render_template ('signin_form.html',username=username)
    return render_template('form.html',massage='Bad username or password',username=username)
#    if request.form['username']=='admin' and request.form['password']=='password':
#        return '<h3>Hello,admin!</h3>'
#    return '<h3>Bad username or password .</h3>'

if __name__ == '__main__':
    app.run(debug=True)

