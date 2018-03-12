import stomp
import sys
import time
import threading
# because i can't setup hiveserver2 successfully, so i use mysqldb instead
import MySQLdb
#from pyhive import presto
''' 
received msg with thread handling
https://stackoverflow.com/questions/9328863/stomp-py-return-message-from-listener/10102673#10102673
'''
class myListener(object):
	def on_message(self, headers, msg):
		# msg == receive message
		# print(msg)
		if msg == 'done':
			msg_len = len(msg_list)
			print("receive %d lines" %msg_len)
			threads = []
			# create 4 threads
			thread0 = myThread(0, "thread0", msg_len)
			thread1 = myThread(1, "thread1", msg_len)
			thread2 = myThread(2, "thread2", msg_len)
			thread3 = myThread(3, "thread3", msg_len)
			# start 4 threads
			thread0.start()
			thread1.start()
			thread2.start()
			thread3.start()
			# append to thread list
			threads.append(thread0)
			threads.append(thread1)
			threads.append(thread2)
			threads.append(thread3)
			# wait for all threads to complete
			for t in threads:
				t.join()
			print("sending to warehouse finished.")
			# reset all object
			threads[:] = []
			msg_list[:] = []
			PAUSE = True
		else:
			msg_list.append(msg)

	def on_error(self, headers, msg):
		print("[ERROR]"+msg)

class myThread(threading.Thread):
	def __init__(self, threadID, name, length):
		threading.Thread.__init__(self)
		self.threadID = threadID
		self.name = name
		self.length = length
	def run(self):
		interval = self.length / 4
		start = interval * self.threadID
		end = interval * (self.threadID + 1)
		if self.threadID == 3:
			end = self.length
		to_warehouse(self.name, start, end, msg_list)

def to_warehouse(threadname, start, end, msg_list):
	print("%s: %d to %d" %(threadname, start, end-1))
	# connect to warehouse
	conn = MySQLdb.connect(host="localhost", user="root", passwd="0556097", db="os_db")
	cur = conn.cursor()
	# store ad
	for i in range(start, end, 1):
		line = msg_list[i].split(",")
		if line[0] == 'social':
			# create table socialAds (ad_id varchar(100), clicks int, interact int, onDate varchar(100))
			# line[1] = ad_id. line[2] = clicks, line[3] = interact, line[4] = onDate
			query = "insert into socialAds (ad_id, clicks, interact, onDate) values (\'%s\', %d, %d, \'%s\')" \
					%((line[1], int(line[2]), int(line[3]), line[4]))
			social_data = (line[1], int(line[2]), int(line[3]), line[4])
			cur.execute(query)
		elif line[0] == 'video':
			# create table videoAds (ad_id varchar(100), views int, onDate varchar(100))
			# line[1] = ad_id, line[2] = views, line[3] = onDate
			query = "insert into videoAds (ad_id, views, onDate) values (\'%s\', %d, \'%s\')"\
					%(line[1], int(line[2]), line[3])
			#print(data)
			video_data = []
			cur.execute(query)
		elif line[0] == 'search':
			# create table searchAds (ad_id varchar(100), clicks int, reach int, conver int, onDate varchar(100))
			# line[1] = ad_id, line[2] = clicks, line[3] = reach, line[4] = conver, line[5] = onDate
			query = "insert into searchAds (ad_id, clicks, reach, conver, onDate) values (\'%s\', %d, %d, %d, \'%s\')"\
					%(line[1], int(line[2]), int(line[3]), int(line[4]), line[5])
			search_data = []
			#print(data)
			cur.execute(query)
		elif line[0] == 'display':
			# create table displayAds (ad_id varchar(100), clicks int, reach int, impression int, onDate varchar(100))
			# line[1] = ad_id, line[2] = impression. line[3] = clicks, line[4] = reach, line[5] = onDate
			query = "insert into displayAds (ad_id, clicks, reach, impression, onDate) values (\'%s\', %d, %d, %d, \'%s\')"\
					%(line[1], int(line[3]), int(line[4]), int(line[2]), line[5])
			#print(data)
			display_data = []
			cur.execute(query)
		else:
			print("not a correct line.")
		conn.commit()
		
	conn.close()
	print("%s finished." %threadname)

msg_list = []
MYQUEUE = '/queue/test_queue'
QHOST = 'master'
QPORT = 61613
PAUSE = True
# create connection
c = stomp.Connection([(QHOST, QPORT)])
# set listener
c.set_listener('my listener', myListener())
# c.set_listener('print', stomp.PrintingListener())
c.start()
c.connect('admin', 'admin', wait=True)
c.subscribe(MYQUEUE, id=123123)

while True:
	if PAUSE == False:
		time.sleep(1)
# never get here
c.unsubscribe(123123)
c.disconnect()