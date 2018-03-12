import socket
import sys

class displayAds():
	impressions = 0
	count = 0
	clicks = 0
	reach = 0
class videoAds():
	count = 0
	views = 0
class socialAds():
	count = 0
	clicks = 0
	interact = 0
class searchAds():
	count = 0
	clicks = 0
	reach = 0
	conver = 0

display = displayAds()
video = videoAds()
social = socialAds()
search = searchAds()
# create a tcp/ip socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# bind the server to the port
serv_addr = ('localhost', 51717)
print("starting up on %s port %s" %(serv_addr))
sock.bind(serv_addr)
# listen for incomming connections
sock.listen(1)
while True:
	# wait for a connection
	print("Waiting for a connection")
	connection, cli_addr = sock.accept()
	try:
		print("connection from %s %s" % cli_addr)
		while True:
			line = connection.recv(100)
			print("received data: %s" %(line))
			if line == 'done':
				connection.sendall("done back")
				break
			# reponse to client
			print("send back to client")
			connection.sendall("got it")
			var = line.split(",")
			if var[0] == 'display':
				display.count = display.count + 1
				display.impressions += int(var[2])
				display.clicks += int(var[3])
				display.reach += int(var[4])
			elif var[0] == 'search':
				search.count = search.count + 1
				search.clicks += int(var[2])
				search.reach += int(var[3])
				search.conver += int(var[4]) 
			elif var[0] == 'video':
				video.count = video.count + 1
				video.views += int(var[2])
			else:
				social.count = social.count + 1
				social.clicks += int(var[2])
				social.interact += int(var[3])
	finally:
		print("\n==========================================\n")
		print("displayAds:")
		print("count: %d\navg. impressions: %.3f\navg. clicks: %.3f\navg. reach: %.3f\n"\
				%(display.count, display.impressions/float(display.count), \
					display.clicks/float(display.count), display.reach/float(display.count)))

		print("searchAds:")
		print("count: %d\navg. reach: %.3f\navg. clicks: %.3f\navg. conversions: %.3f\n"\
				%(search.count, search.reach/float(search0.count), \
					search.clicks/float(search.count), search.conver/float(search.count)))
		print("videoAds:")
		print("count: %d\navg. views: %.3f\n" %(video.count, video.views/float(video.count) ))
		
		print("socialAds:")
		print("count: %d\navg. clicks: %.3f\navg. interactions: %.3f\n"\
				%(social.count, social.clicks/float(social.count), social.interact/float(social.count)))
		print("end of summary")
		# clean up the connection
		print("end of connection: %s %s" %(cli_addr))
		connection.close()

