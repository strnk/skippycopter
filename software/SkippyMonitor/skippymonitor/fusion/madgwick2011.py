# Madgwick, S.O.H., Harrison, A.J.L., Vaidyanathan, R.
# "Estimation of IMU and MARG orientation using a gradient descent algorithm"
# Published in IEEE Conference on Rehabilitation Robotics (ICORR), 2011
#
# DOI: 10.1109/ICORR.2011.5975346 

from math import pi, asin, atan2, sqrt

class Madgwick2011:
	beta = 0.1
	sampleFreq = 150.0

	# Sensor frame quaternion (relative to auxiliary frame)
	q0 = 1.0
	q1 = 0.0
	q2 = 0.0
	q3 = 0.0

	def __init__(self, beta = 0.4, freq = 150.0):
		self.beta = beta
		self.freq = freq

	def update(self, gx, gy, gz, ax, ay, az, mx, my, mz):
		# Use the IMU-only update if the magnetometer readings are invalid
		if mx == 0 and my == 0 and mz == 0:
			self.updateIMUonly(gx, gy, gz, ax, ay, az)
			return

		q0 = self.q0
		q1 = self.q1
		q2 = self.q2
		q3 = self.q3

		# Rate of change of quaternion from gyroscope
		qDot1 = 0.5 * (-q1 * gx - q2 * gy - q3 * gz);
		qDot2 = 0.5 * (q0 * gx + q2 * gz - q3 * gy);
		qDot3 = 0.5 * (q0 * gy - q1 * gz + q3 * gx);
		qDot4 = 0.5 * (q0 * gz + q1 * gy - q2 * gx);

		if not (ax == 0 and ay == 0 and az == 0):
			recipNorm = (ax * ax + ay * ay + az * az)**-.5;
			ax *= recipNorm;
			ay *= recipNorm;
			az *= recipNorm;   

			# Normalise magnetometer measurement
			recipNorm = (mx * mx + my * my + mz * mz)**-.5;
			mx *= recipNorm;
			my *= recipNorm;
			mz *= recipNorm;

			# Auxiliary variables to avoid repeated arithmetic
			_2q0mx = 2.0 * q0 * mx;
			_2q0my = 2.0 * q0 * my;
			_2q0mz = 2.0 * q0 * mz;
			_2q1mx = 2.0 * q1 * mx;
			_2q0 = 2.0 * q0;
			_2q1 = 2.0 * q1;
			_2q2 = 2.0 * q2;
			_2q3 = 2.0 * q3;
			_2q0q2 = 2.0 * q0 * q2;
			_2q2q3 = 2.0 * q2 * q3;
			q0q0 = q0 * q0;
			q0q1 = q0 * q1;
			q0q2 = q0 * q2;
			q0q3 = q0 * q3;
			q1q1 = q1 * q1;
			q1q2 = q1 * q2;
			q1q3 = q1 * q3;
			q2q2 = q2 * q2;
			q2q3 = q2 * q3;
			q3q3 = q3 * q3;

			# Reference direction of Earth's magnetic field
			hx = mx * q0q0 - _2q0my * q3 + _2q0mz * q2 + mx * q1q1 + _2q1 * my * q2 + _2q1 * mz * q3 - mx * q2q2 - mx * q3q3;
			hy = _2q0mx * q3 + my * q0q0 - _2q0mz * q1 + _2q1mx * q2 - my * q1q1 + my * q2q2 + _2q2 * mz * q3 - my * q3q3;
			_2bx = (hx * hx + hy * hy)**.5;
			_2bz = -_2q0mx * q2 + _2q0my * q1 + mz * q0q0 + _2q1mx * q3 - mz * q1q1 + _2q2 * my * q3 - mz * q2q2 + mz * q3q3;
			_4bx = 2.0 * _2bx;
			_4bz = 2.0 * _2bz;

			# Gradient decent algorithm corrective step
			s0 = -_2q2 * (2.0 * q1q3 - _2q0q2 - ax) + _2q1 * (2.0 * q0q1 + _2q2q3 - ay) - _2bz * q2 * (_2bx * (0.5 - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q3 + _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5 - q1q1 - q2q2) - mz);
			s1 = _2q3 * (2.0 * q1q3 - _2q0q2 - ax) + _2q0 * (2.0 * q0q1 + _2q2q3 - ay) - 4.0 * q1 * (1 - 2.0 * q1q1 - 2.0 * q2q2 - az) + _2bz * q3 * (_2bx * (0.5 - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q2 + _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q3 - _4bz * q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5 - q1q1 - q2q2) - mz);
			s2 = -_2q0 * (2.0 * q1q3 - _2q0q2 - ax) + _2q3 * (2.0 * q0q1 + _2q2q3 - ay) - 4.0 * q2 * (1 - 2.0 * q1q1 - 2.0 * q2q2 - az) + (-_4bx * q2 - _2bz * q0) * (_2bx * (0.5 - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q1 + _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q0 - _4bz * q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5 - q1q1 - q2q2) - mz);
			s3 = _2q1 * (2.0 * q1q3 - _2q0q2 - ax) + _2q2 * (2.0 * q0q1 + _2q2q3 - ay) + (-_4bx * q3 + _2bz * q1) * (_2bx * (0.5 - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q0 + _2bz * q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5 - q1q1 - q2q2) - mz);
			recipNorm = (s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3)**-.5; # normalise step magnitude
			s0 *= recipNorm;
			s1 *= recipNorm;
			s2 *= recipNorm;
			s3 *= recipNorm;

			# Apply feedback step
			qDot1 -= self.beta * s0;
			qDot2 -= self.beta * s1;
			qDot3 -= self.beta * s2;
			qDot4 -= self.beta * s3;

		# Integrate rate of change of quaternion to yield quaternion
		q0 += qDot1 * (1.0 / self.sampleFreq);
		q1 += qDot2 * (1.0 / self.sampleFreq);
		q2 += qDot3 * (1.0 / self.sampleFreq);
		q3 += qDot4 * (1.0 / self.sampleFreq);

		# Normalise quaternion
		recipNorm = (q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3)**-.5;
		self.q0 = q0*recipNorm;
		self.q1 = q1*recipNorm;
		self.q2 = q2*recipNorm;
		self.q3 = q3*recipNorm;

	def updateIMUonly(self, gx, gy, gz, ax, ay, az):
		q0 = self.q0
		q1 = self.q1
		q2 = self.q2
		q3 = self.q3

		# Rate of change of quaternion from gyroscope
		qDot1 = 0.5 * (-q1 * gx - q2 * gy - q3 * gz);
		qDot2 = 0.5 * (q0 * gx + q2 * gz - q3 * gy);
		qDot3 = 0.5 * (q0 * gy - q1 * gz + q3 * gx);
		qDot4 = 0.5 * (q0 * gz + q1 * gy - q2 * gx);

		if not (ax == 0 and ay == 0 and az == 0):
			recipNorm = (ax * ax + ay * ay + az * az)**-.5;
			ax *= recipNorm;
			ay *= recipNorm;
			az *= recipNorm;

			# Auxiliary variables to avoid repeated arithmetic
			_2q0 = 2.0 * q0;
			_2q1 = 2.0 * q1;
			_2q2 = 2.0 * q2;
			_2q3 = 2.0 * q3;
			_4q0 = 4.0 * q0;
			_4q1 = 4.0 * q1;
			_4q2 = 4.0 * q2;
			_8q1 = 8.0 * q1;
			_8q2 = 8.0 * q2;
			q0q0 = q0 * q0;
			q1q1 = q1 * q1;
			q2q2 = q2 * q2;
			q3q3 = q3 * q3;

			# Gradient decent algorithm corrective step
			s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
			s1 = _4q1 * q3q3 - _2q3 * ax + 4.0 * q0q0 * q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
			s2 = 4.0 * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
			s3 = 4.0 * q1q1 * q3 - _2q1 * ax + 4.0 * q2q2 * q3 - _2q2 * ay;
			recipNorm = (s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3)**-.5; # normalise step magnitude
			s0 *= recipNorm;
			s1 *= recipNorm;
			s2 *= recipNorm;
			s3 *= recipNorm;

			# Apply feedback step
			qDot1 -= self.beta * s0;
			qDot2 -= self.beta * s1;
			qDot3 -= self.beta * s2;
			qDot4 -= self.beta * s3;

		# Integrate rate of change of quaternion to yield quaternion
		q0 += qDot1 * (1.0 / self.sampleFreq);
		q1 += qDot2 * (1.0 / self.sampleFreq);
		q2 += qDot3 * (1.0 / self.sampleFreq);
		q3 += qDot4 * (1.0 / self.sampleFreq);

		# Normalise quaternion
		recipNorm = (q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3)**-.5;
		self.q0 = q0*recipNorm;
		self.q1 = q1*recipNorm;
		self.q2 = q2*recipNorm;
		self.q3 = q3*recipNorm;


	def to_euler2(self):
		# Gravity vector
		gx = 2 * (self.q1 * self.q3 - self.q0 * self.q2)
		gy = 2 * (self.q0 * self.q1 + self.q2 * self.q3)
		gz = self.q0**2 - self.q1**2  - self.q2**2 + self.q3**2

		yaw = atan2(2 * self.q1 * self.q2 - 2 * self.q0 * self.q3,
		    2 * self.q0**2 + 2 * self.q1**2 - 1)
		pitch = atan(gx / sqrt(gy**2 + gz**2))
		roll = atan(gy / sqrt(gx**2 + gz**2))
		return yaw, pitch, roll

	def to_euler(self):
		qw, qx, qy, qz = self.q0, self.q1, self.q2, self.q3
		test = qx*qy + qz*qw

		if test > 0.499:
			yaw = 2 * atan2(qx,qw)
			pitch = pi/2
			roll = 0
		elif test < -0.499:
			yaw = -2 * atan2(qw,qw)
			pitch = -pi/2
			roll = 0
		else:
			sqx = qx*qx
			sqy = qy*qy
			sqz = qz*qz
			yaw = atan2(2*qy*qw - 2*qx*qz, 1 - 2*sqy - 2*sqz)
			pitch = asin(2*test)
			roll = atan2(2*qx*qw - 2*qy*qz, 1 - 2*sqx - 2*sqz)

		return yaw, pitch, roll
