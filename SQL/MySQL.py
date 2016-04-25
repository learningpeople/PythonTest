#!/usr/bin/pyton
#-*- coding:utf8 -*-
class MySQL_TEST():
    import MySQLdb
    import sys
    con = None
    try:
        con = MySQLdb.connect('localhost','testuser1','test1','testdb');
        print con
        cur = con.cursor()
        cur.execute('SELECT VERSION()')
        data =cur.fetchone()
        print 'Database version:%s'%data
    except MySQLdb.Error,e:
        print 'Error %d:%s'%(e.args[0],e.args[1])
        sys.exit(1)
    finally:
        if con:
            con.close()
#把con初始化为None，在无法创建连接的时候（例如硬盘空间已满），我们不能定义该数据库连接变量。这将会导致finally（）语句执行出错。
#connect(<数据库的主机地址>,<数据库的用户名>,<用户的密码>,<数据库的名称>)连接到数据库。
#除了默认的数据库之外要创建心的数据库需要先在数据库下创建，>CREATE DATABASE <database-name>
#默认数据库只有一个root数据库用户，如果要创建新的用户需：
# mysql -u root -p
#CREATE DATABASE <database-name>
#CREATE USER '<username>'@'%%'IDENTIFIED BY '<password>';
#USE testdb;
#GRANT ALL PRIVILEGES ON *.* TO '<username>'@'%%';
#FLUSH PRIVILEGES;
#cursor(游标)，用来遍历结果集中的记录。我们通过调用cursor对象的execute()方法来执行SQL语句。用fetchone（）方法获取一个记录，并将取回的数据打印到终端
#except 。。。 检查错误。
#finally 。。。 释放连接资源
class MySQL_TEST1():
    import MySQLdb
    import sys
    con = MySQLdb.connect('localhost','testuser1','test1','testdb')
    with con:
        cur = con.cursor()
        cur.execute('CREATE TABLE IF NOT EXISTS\
        Writers1(Id INT PRIMARY KEY AUTO_INCREMENT,Name VARCHAR(25))')
        cur.execute("INSERT INTO Writers1(Name) VALUES('Jack London')")
        cur.execute("INSERT INTO Writers1(Name) VALUES('HONORE de Balzac')")
        cur.execute("INSERT INTO Writers1(Name) VALUES('Lion Feuchtwanger')")
        cur.execute("INSERT INTO Writers1(Name) VALUES('Emile Xola')")
        cur.execute("INSERT INTO Writers1(Name) VALUES('Truman Capote')")
#创建表，填充数据。
#使用with。。的语法，在退出with语句之前会执行con对象的__exit__方法

class MySQL_TEST2_fetchall():
    import MySQLdb
    import sys
    con = MySQLdb.connect('localhost','testuser1','test1','testdb')
#    print con
    with con:
        cur = con.cursor()
        cur.execute("SELECT * FROM Writers")
        rows = cur.fetchall()
        for row in rows:
            print row
        cur.execute("SELECT * FROM Writers")
        numrows = int(cur.rowcount)
        for i in range(numrows):
            row = cur.fetchone()#逐行获取记录
            print row[0],row[1]
            print row
 #从writers表中都去所有数据。
 #用fetcall（）查询说有
class MySQL_aTEST3_desctiption():
    import MySQLdb
    import sys
    con = MySQLdb.connect('localhost','testuser1','test1','testdb')
    with con:
        cur = con.cursor(MySQLdb.cursors.DictCursor)
        cur.execute("SELECT * FROM Writers")
        rows = cur.fetchall()
        for row in rows:
            print "%s %s"%(row['Id'],row['Name'])
        cur.execute("SELECT * FROM Writers")
        desc = cur.description
        print "%s %3s"%(desc[0][0],desc[1][0])
        print desc
#使用字典游标来打印Writers表的内容。
class MySQL_TEST4_UPDATE():
    import MySQLdb
    import sys
    con = MySQLdb.connect('localhost','testuser1','test1','testdb')
    with con:
        cur = con.cursor()
        #修改id为4的Name值，
        #cur.rowcount当前操作的行数
        cur.execute("UPDATE Writers SET Name = %s WHERE Id = %s",("Guy de Maupasant","4"))
        print "Number of row updated: %d"%cur.rowcount
        #可在数据库中chakan
        #USE testdb；
        #SELECT Name FROM Writers WHERE Id=4;
class MySQL_picture_writetosql():
    import MySQLdb
    import sys
    try:
        with open('/home/ubuntu-user/图片/Chrysanthemum.jpg') as f:
            img = f.read()
    #        print img
    except IOError,e:
        print 'Error %d:%s'%(e.args[0],e.args[1])
        sys.exit(1)

    try :
        conn = MySQLdb.connect(host='localhost',user='testuser1',passwd='test1',db='testdb')
        cur = conn.cursor()
        cur.execute("INSERT INTO Images SET Data='%s'"%\
        MySQLdb.escape_string(img))
     #   print MySQLdb.excape_string(img)

        conn.commit()
        cur.close()
        conn.close()

    except MySQLdb.Error,e:
        print "Error %d:%s"%(e.args[0],e.args[1])
        sys.exit(1)
    #将图片写入数据库之前要先创建一个库，MySQL中有一种特殊的数据类型用来存储二进制数据，BLOB（Binary Large Object，二进制大对象）。
    #>CREATE TABLE Images(Id INT PRIMARY KEY AUTO_INCREMENT,Data MEDIUMBLOB);
    #读取一张图片并蒋其插入到Images表中。
    #excape_string()数据库自带的转义字符串方法
class MySQL_picture_write():
    import MySQLdb
    import sys
    try:
        conn = MySQLdb.connect('localhost','testuser1','test1','testdb')
        cursor = conn.cursor()
        cursor.execute("SELECT Data FROM Images LIMIT 1")
        with open('/home/ubuntu-user/图片/Chrysanthemum1.jpg','wb') as f:
            f.write(cursor.fetchone()[0])
        cursor.close()
        conn.close()
    except IOError,e:
        print "Error %d:%s"%(e.args[0],e.args[1])
        sys.exit(1)
        
#我们从Images表中读取一张图片,通过select语句从表中查询一条记录：
#cursor.execute('SELECT Data FROM Images LIMIT 1')
#打开一个可写入二进制的文件。并将从数据库抽取的数据写入到文件中。
#open('...','wb')
# .write(cursor.fetchone()[0])

class MySQL_UPDATE2():
    import MySQLdb
    import sys
    try:
        conn = MySQLdb.connect('localhost','testuser1','test1','testdb')
        cursor = conn.cursor()
        cursor.execute("UPDATE Writers SET Name = %s WHERE Id = %s",("Leo Tolstoy","5"))
        cursor.execute("UPDATE Writers SET Name = %s WHERE Id = %s",("Boris Pasternak","6"))
        cursor.execute("UPDATE Writer SET Name = %s WHERE Id = %s",("Leonid Leonov","7"))
        conn.commit()
        cursor.close()
        conn.close()
    except MySQLdb.Error,e:
        conn.rollback()
        print 'Error %d:%s'%(e.args[0],e.args[1])
        sys.exit(1)


