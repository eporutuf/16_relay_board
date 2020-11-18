import socket
import time

UDP_IP = "192.168.11.2"
UDP_PORT = 12345
MESSAGE = "FFE000"
buf=1024

print("UDP target IP:", UDP_IP)
print("UDP target port:", UDP_PORT)
print("message:", MESSAGE)
 # UDP
for a in range(3,4):
	UDP_IP = "192.168.11.{0}".format(a)
	print("IP target is : ",UDP_IP)
	for i in range(1,17):
		a,b = str(i /10).split(".")
		MESSAGE = "FF{0}{1}01".format(a,b)
		sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		sock.sendto(bytes(MESSAGE, "utf-8"), (UDP_IP, UDP_PORT))
		reponse, adr = sock.recvfrom(buf)
		print("=> {0}".format(reponse))
		#print(MESSAGE)
		time.sleep(0.05)
	MESSAGE = "FFE001"
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	sock.sendto(bytes(MESSAGE, "utf-8"), (UDP_IP, UDP_PORT))
	time.sleep(1)
	MESSAGE = "FFE000"
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	sock.sendto(bytes(MESSAGE, "utf-8"), (UDP_IP, UDP_PORT))