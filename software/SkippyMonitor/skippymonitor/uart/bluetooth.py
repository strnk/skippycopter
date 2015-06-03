import socket


class BluetoothSerial:
	def __init__(self, mac, channel):
		self.MACAddress = mac
		self.channel = channel

	def open(self):
		self.socket = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM)
		self.socket.setblocking(True)
		self.socket.connect((self.MACAddress, self.channel))
		self.sfile = self.socket.makefile()

	def write(self, data):
		self.sfile.write(data)

	def readline(self):
		return self.sfile.readline()

	def close(self):
		self.socket.close()