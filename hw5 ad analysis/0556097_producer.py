import csv
import sys
import stomp

HOST = 'master'
PORT = 61613
# create connection to activemq
if len(sys.argv) < 2:
	print("no input file.")
	sys.exit()

c = stomp.Connection([(HOST, PORT)])
# set listener
c.set_listener('print', stomp.PrintingListener())
c.start()
print("Connect to activemq broker.")
c.connect('admin', 'admin', wait=True)

MYQUEUE = '/queue/test_queue'
line_count = 0
with open(sys.argv[1]) as file:
	print("starting transaction.")
	# trid = c.begin()
	for line in file:
		line_count += 1
		try:
			# Send to broker
			c.send(MYQUEUE, line) # , transaction=trid)
		except:
			print(sys.stderr)
			print('disconnectin\' activemq broker.')
			c.disconnect()
	# c.commit(trid)

c.send(MYQUEUE, "done")
print("Producer finished.")
print("total %d line sent" %line_count)
c.disconnect()
