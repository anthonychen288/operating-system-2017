# because i can't setup the hiveserver2, so i use mysqldb instead
#from pyhive import presto
import MySQLdb

conn = MySQLdb.connect(host="localhost", user="root", passwd="0556097", db="os_db")
cur = conn.cursor()
# socialAds
print("table - socialAds")
cur.execute("select count(*), AVG(clicks), STDDEV(clicks), AVG(interact), STDDEV(interact) from socialAds")
social = cur.fetchall()
for line in social:
	print("count:\t%d"%line[0])
	print("clicks:\t%.3f / %.3f"%(line[1], line[2]))
	print("interactions:\t%.3f / %.3f\n\n"%(line[3], line[4]))
# videoAds
print("table - videoAds")
cur.execute("select count(*), AVG(views), STDDEV(views) from videoAds")
video = cur.fetchall()
for line in video:
	print("counts:\t%d"%(line[0]))
	print("views:\t%.3f / %.3f\n\n"%(line[1], line[2]))
# searchAds
print("table - searchAds")
cur.execute("select count(*), AVG(clicks), STDDEV(clicks), AVG(reach), STDDEV(reach), AVG(conver), STDDEV(conver) from searchAds")
search = cur.fetchall()
for line in search:
	print("count:\t%d"%line[0])
	print("clicks:\t%.3f / %.3f"%(line[1], line[2]))
	print("reach:\t%.3f / %.3f"%(line[3], line[4]))
	print("conversion:\t%.3f / %.3f\n\n"%(line[5], line[6]))
# displayAds
print("table - displayAds")
cur.execute("select count(*), AVG(impression), STDDEV(impression), AVG(reach), STDDEV(reach), AVG(clicks), STDDEV(clicks) from displayAds")
display = cur.fetchall()
for line in display:
	print("count:\t%d"%line[0])
	print("impression:\t%.3f / %.3f"%(line[1], line[2]))
	print("reach:\t%.3f / %.3f"%(line[3], line[4]))
	print("clicks:\t%.3f / %.3f\n"%(line[5], line[6]))

print("reseting DB")
try:
	cur.execute("truncate videoAds")
	cur.execute("truncate socialAds")
	cur.execute("truncate displayAds")
	cur.execute("truncate searchAds")
finally:
	conn.close()