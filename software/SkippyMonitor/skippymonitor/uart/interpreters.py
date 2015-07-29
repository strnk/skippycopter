from struct import pack, unpack
from math import pi

class Data3:
	x = 0
	y = 0
	z = 0

class M3A3G3PTL:
	"""	3 magnetometer axes,
		3 accelerometer axes,
		3 gyroscope axes,
		1 pressure data,
		1 temperature data,
		1 loop performance data
	"""

	mag = Data3()
	accel = Data3()
	gyro = Data3()
	pressure = 0
	temperature = 0
	perf = 0

	def __init__(self):
		pass

	def decode(self, string):
		data = [ int(x, 16) for x in string.rstrip('\r\n').split() ]

		if len(data) != 12:
			raise Exception('Unable to decode data string: {datastr}'.format(datastr=string))


		[ self.mag.x, self.mag.y, self.mag.z, self.accel.x, self.accel.y, self.accel.z, \
			self.gyro.x, self.gyro.y, self.gyro.z, self.pressure, self.temperature, self.perf] \
				= unpack('3h3h3hlhH', pack('3H3H3HLHH', *data))

		self.normalize()

	def normalize(self):
		# Magnetometer in milli-gauss (1090LSB/gauss => 0.92mG/LSB)
		self.mag.x = self.mag.x * 0.92
		self.mag.y = self.mag.y * 0.92
		self.mag.z = self.mag.z * 0.92

		# Accelerometer in G (0.04mG/LSB)
		self.accel.x = self.accel.x * 0.008
		self.accel.y = self.accel.y * 0.008
		self.accel.z = self.accel.z * 0.008

		# Gyroscope in deg/sec (70mdps/LSB => 0.07 deg/sec)
		self.gyro.x = self.gyro.x * 0.07
		self.gyro.y = self.gyro.y * 0.07
		self.gyro.z = self.gyro.z * 0.07

		# Temperature in degrees Celsius
		self.temperature = self.temperature / 10

		# Pressure in hPa
		self.pressure = self.pressure / 100

class AT_YPRf:
	"""	Yaw, pitch, roll as floats
	"""

	yaw = 0
	pitch = 0
	roll = 0

	def __init__(self):
		pass

	def decode(self, string):
		data = string.rstrip('\r\n').split()

		if len(data) != 3:
			raise Exception('Unable to decode data string: {datastr}'.format(datastr=string))

		self.pitch = unpack('!f', bytes.fromhex(data[0]))[0]
		self.yaw = unpack('!f', bytes.fromhex(data[1]))[0]
		self.roll = unpack('!f', bytes.fromhex(data[2]))[0]

		self.normalize()

	def normalize(self):
		pass