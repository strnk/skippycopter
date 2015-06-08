from PyQt5 import QtCore, QtWidgets
from PyQt5.QtWidgets import QProgressDialog, QMessageBox

from enum import Enum

class CalibrationData:
	maximum = 0
	minimum = 0
	mean = 0
	data = [ ]

	def add(self, value):
		self.data.append(value)

	def reset(self):
		self.maximum = 0
		self.minimum = 0
		self.mean = 0
		self.data = [ ]

	def finalize(self):
		self.mean = sum(self.data) / len(self.data)
		self.maximum = max(self.data)
		self.minimum = min(self.data)
		
		ac = [ x - self.mean for x in self.data ]
		self.noise = max(abs(max(ac)), abs(min(ac)))

class CalibrationData3:
	x = CalibrationData()
	y = CalibrationData()
	z = CalibrationData()

	def add(self, x, y, z):
		self.x.add(x)
		self.y.add(y)
		self.z.add(z)

	def reset(self):
		self.x.reset()
		self.y.reset()
		self.z.reset()

	def finalize(self):
		self.x.finalize()
		self.y.finalize()
		self.z.finalize()

class CalibrationState(Enum):
	Init = 0
	GyroInit = 1
	GyroCalibration = 2
	End = 3
	Canceled = 1000


class IMUCalibration(QtCore.QObject):
	GYRO_CALIBRATION_VALUES = 750

	state = CalibrationState.Init
	calibrationEnd = QtCore.pyqtSignal()
	calibrationCanceled = QtCore.pyqtSignal()

	requiredCalibrationValues = 0
	currentCalibrationValues = 0

	gyro = CalibrationData3()

	def __init__(self, interpreterClass, parent):
		super(IMUCalibration, self).__init__(parent)
		self.parent = parent
		self.interpreter = interpreterClass()

	def start(self):
		self.state = CalibrationState.GyroInit

	def stateMachine(self):
		if self.state == CalibrationState.Init:
			pass
		elif self.state == CalibrationState.GyroInit:
			self.requiredCalibrationValues = self.GYRO_CALIBRATION_VALUES
			self.currentCalibrationValues = 0
			self.startGyroCalibration(self.GYRO_CALIBRATION_VALUES)
			self.gyro.reset()

			self.state = CalibrationState.GyroCalibration
		elif self.state == CalibrationState.GyroCalibration:
			self.currentCalibrationValues += 1
			self.gyro.add(self.interpreter.gyro.x, self.interpreter.gyro.y, self.interpreter.gyro.z)
			self.progress.setValue(self.currentCalibrationValues)

			if self.currentCalibrationValues >= self.requiredCalibrationValues:
				self.gyro.finalize()
				self.state = CalibrationState.End
		elif self.state == CalibrationState.End:
			self.calibrationEnd.emit()
			self.state = CalibrationState.Init
		elif self.state == CalibrationState.Canceled:
			self.calibrationCanceled.emit()
			self.state = CalibrationState.Init

	def startGyroCalibration(self, values):
		self.progress = QProgressDialog("Calibrating gyroscope, keep the board still...", "Cancel", 0, values, self.parent)
		self.progress.setWindowModality(QtCore.Qt.WindowModal)

		self.progress.canceled.connect(self.cancel)

	def cancel(self):
		self.state = CalibrationState.Canceled

	def newData(self, data):
		self.interpreter.decode(data)
		if self.interpreter.gyro.x > 10:
			print("GX noise: " + data)
		if self.interpreter.gyro.y > 10:
			print("GY noise: " + data)
		if self.interpreter.gyro.z > 10:
			print("GZ noise: " + data)
		self.stateMachine()