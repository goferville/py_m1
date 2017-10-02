import pymysql
conn = pymysql.connect(host='localhost', port=3306, user='root', passwd='555555', db='koala')

cur = conn.cursor()
cur.execute("SELECT VERSION()")
data = cur.fetchone()

print ("Database version : %s " % data)
cur.execute("SELECT * FROM koala.table1 ")

print(cur.description)

print()

for row in cur:
    print(row)
cur.execute('SELECT * FROM koala.table1 WHERE id="2"')
for row in cur:
    print(row)

#need backticks `last name` for col name with space inside
cur.execute('SELECT * FROM koala.table1 WHERE `last name`="Li"')
for row in cur:
    print(row)
cur.close()
conn.close()