import serial


class DirectSerial:
	def __init__(self, port, speed):
		self.port = port
		self.speed = speed

	def open(self):
		self.serial = serial.Serial(self.port, self.speed, timeout=0)

	def write(self, data):
		self.serial.write(bytes(data, 'UTF-8'))

	def readline(self):
		return self.serial.readline().decode('UTF-8')

	def close(self):
		self.serial.close()