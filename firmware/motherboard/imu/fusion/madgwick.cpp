// Implementation of Madgwick's IMU and AHRS algorithms.
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms

#include "madgwick.h"
#include <math.h>
#define M_PI 3.14159265358979323846

float invSqrt(float x);

#define sampleFreq	500.0f		// sample frequency in Hz
#define betaDef		0.1f		// 2 * proportional gain

MadgwickAHRS::MadgwickAHRS(void) 
	: beta(betaDef), q0(1.0f), q1(0.0f), q2(0.0f), q3(0.0f)
{

}


void 
MadgwickAHRS::update(data3f gyro, data3f accelero, data3f magneto) {
	float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float hx, hy;
	float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;

	// Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
	if((magneto.x == 0.0f) && (magneto.y == 0.0f) && (magneto.z == 0.0f)) {
		updateIMU(gyro, accelero);
		return;
	}

	// Rate of change of quaternion from gyro.yroscope
	qDot1 = 0.5f * (-q1 * gyro.x - q2 * gyro.y - q3 * gyro.z);
	qDot2 = 0.5f * (q0 * gyro.x + q2 * gyro.z - q3 * gyro.y);
	qDot3 = 0.5f * (q0 * gyro.y - q1 * gyro.z + q3 * gyro.x);
	qDot4 = 0.5f * (q0 * gyro.z + q1 * gyro.y - q2 * gyro.x);

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((accelero.x == 0.0f) && (accelero.y == 0.0f) && (accelero.z == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = invSqrt(accelero.x * accelero.x + accelero.y * accelero.y + accelero.z * accelero.z);
		accelero.x *= recipNorm;
		accelero.y *= recipNorm;
		accelero.z *= recipNorm;   

		// Normalise magnetometer measurement
		recipNorm = invSqrt(magneto.x * magneto.x + magneto.y * magneto.y + magneto.z * magneto.z);
		magneto.x *= recipNorm;
		magneto.y *= recipNorm;
		magneto.z *= recipNorm;

		// Auxiliary variables to avoid repeated arithmetic
		_2q0mx = 2.0f * q0 * magneto.x;
		_2q0my = 2.0f * q0 * magneto.y;
		_2q0mz = 2.0f * q0 * magneto.z;
		_2q1mx = 2.0f * q1 * magneto.x;
		_2q0 = 2.0f * q0;
		_2q1 = 2.0f * q1;
		_2q2 = 2.0f * q2;
		_2q3 = 2.0f * q3;
		_2q0q2 = 2.0f * q0 * q2;
		_2q2q3 = 2.0f * q2 * q3;
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

		// Reference direction of Earth's magnetic field
		hx = magneto.x * q0q0 - _2q0my * q3 + _2q0mz * q2 + magneto.x * q1q1 + _2q1 * magneto.y * q2 + _2q1 * magneto.z * q3 - magneto.x * q2q2 - magneto.x * q3q3;
		hy = _2q0mx * q3 + magneto.y * q0q0 - _2q0mz * q1 + _2q1mx * q2 - magneto.y * q1q1 + magneto.y * q2q2 + _2q2 * magneto.z * q3 - magneto.y * q3q3;
		_2bx = sqrt(hx * hx + hy * hy);
		_2bz = -_2q0mx * q2 + _2q0my * q1 + magneto.z * q0q0 + _2q1mx * q3 - magneto.z * q1q1 + _2q2 * magneto.y * q3 - magneto.z * q2q2 + magneto.z * q3q3;
		_4bx = 2.0f * _2bx;
		_4bz = 2.0f * _2bz;

		// Gradient decent algorithm corrective step
		s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - accelero.x) + _2q1 * (2.0f * q0q1 + _2q2q3 - accelero.y) - _2bz * q2 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - magneto.x) + (-_2bx * q3 + _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - magneto.y) + _2bx * q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - magneto.z);
		s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - accelero.x) + _2q0 * (2.0f * q0q1 + _2q2q3 - accelero.y) - 4.0f * q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - accelero.z) + _2bz * q3 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - magneto.x) + (_2bx * q2 + _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - magneto.y) + (_2bx * q3 - _4bz * q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - magneto.z);
		s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - accelero.x) + _2q3 * (2.0f * q0q1 + _2q2q3 - accelero.y) - 4.0f * q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - accelero.z) + (-_4bx * q2 - _2bz * q0) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - magneto.x) + (_2bx * q1 + _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - magneto.y) + (_2bx * q0 - _4bz * q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - magneto.z);
		s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - accelero.x) + _2q2 * (2.0f * q0q1 + _2q2q3 - accelero.y) + (-_4bx * q3 + _2bz * q1) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - magneto.x) + (-_2bx * q0 + _2bz * q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - magneto.y) + _2bx * q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - magneto.z);
		recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
		s0 *= recipNorm;
		s1 *= recipNorm;
		s2 *= recipNorm;
		s3 *= recipNorm;

		// Apply feedback step
		qDot1 -= beta * s0;
		qDot2 -= beta * s1;
		qDot3 -= beta * s2;
		qDot4 -= beta * s3;
	}

	// Integrate rate of change of quaternion to yield quaternion
	q0 += qDot1 * (1.0f / sampleFreq);
	q1 += qDot2 * (1.0f / sampleFreq);
	q2 += qDot3 * (1.0f / sampleFreq);
	q3 += qDot4 * (1.0f / sampleFreq);

	// Normalise quaternion
	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;
}

void 
MadgwickAHRS::updateIMU(data3f gyro, data3f accelero) {
	float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2 ,_8q1, _8q2, q0q0, q1q1, q2q2, q3q3;

	// Rate of change of quaternion from gyro.yroscope
	qDot1 = 0.5f * (-q1 * gyro.x - q2 * gyro.y - q3 * gyro.z);
	qDot2 = 0.5f * (q0 * gyro.x + q2 * gyro.z - q3 * gyro.y);
	qDot3 = 0.5f * (q0 * gyro.y - q1 * gyro.z + q3 * gyro.x);
	qDot4 = 0.5f * (q0 * gyro.z + q1 * gyro.y - q2 * gyro.x);

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((accelero.x == 0.0f) && (accelero.y == 0.0f) && (accelero.z == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = invSqrt(accelero.x * accelero.x + accelero.y * accelero.y + accelero.z * accelero.z);
		accelero.x *= recipNorm;
		accelero.y *= recipNorm;
		accelero.z *= recipNorm;   

		// Auxiliary variables to avoid repeated arithmetic
		_2q0 = 2.0f * q0;
		_2q1 = 2.0f * q1;
		_2q2 = 2.0f * q2;
		_2q3 = 2.0f * q3;
		_4q0 = 4.0f * q0;
		_4q1 = 4.0f * q1;
		_4q2 = 4.0f * q2;
		_8q1 = 8.0f * q1;
		_8q2 = 8.0f * q2;
		q0q0 = q0 * q0;
		q1q1 = q1 * q1;
		q2q2 = q2 * q2;
		q3q3 = q3 * q3;

		// Gradient decent algorithm corrective step
		s0 = _4q0 * q2q2 + _2q2 * accelero.x + _4q0 * q1q1 - _2q1 * accelero.y;
		s1 = _4q1 * q3q3 - _2q3 * accelero.x + 4.0f * q0q0 * q1 - _2q0 * accelero.y - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * accelero.z;
		s2 = 4.0f * q0q0 * q2 + _2q0 * accelero.x + _4q2 * q3q3 - _2q3 * accelero.y - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * accelero.z;
		s3 = 4.0f * q1q1 * q3 - _2q1 * accelero.x + 4.0f * q2q2 * q3 - _2q2 * accelero.y;
		recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
		s0 *= recipNorm;
		s1 *= recipNorm;
		s2 *= recipNorm;
		s3 *= recipNorm;

		// Apply feedback step
		qDot1 -= beta * s0;
		qDot2 -= beta * s1;
		qDot3 -= beta * s2;
		qDot4 -= beta * s3;
	}

	// Integrate rate of change of quaternion to yield quaternion
	q0 += qDot1 * (1.0f / sampleFreq);
	q1 += qDot2 * (1.0f / sampleFreq);
	q2 += qDot3 * (1.0f / sampleFreq);
	q3 += qDot4 * (1.0f / sampleFreq);

	// Normalise quaternion
	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;
}

orientation_t
MadgwickAHRS::getOrientation(void)
{
	orientation_t o;
	float test = q1*q2 + q3*q0, sqx, sqy, sqz;

	if (test > 0.499) {
		o.yaw = 2 * atan2(q1, q0);
		o.pitch = M_PI/2;
		o.roll = 0;
	}
	else if (test < -0.499) {
		o.yaw = -2 * atan2(q0, q0);
		o.pitch = -M_PI/2;
		o.roll = 0;
	}
	else {
		sqx = q1*q1;
		sqy = q2*q2;
		sqz = q3*q3;
		o.yaw = atan2(2*q2*q0 - 2*q1*q3, 1 - 2*sqy - 2*sqz);
		o.pitch = asin(2*test);
		o.roll = atan2(2*q1*q0 - 2*q2*q3, 1 - 2*sqx - 2*sqz);
	}

	o.yaw *= 180.0/M_PI;
	o.pitch *= 180.0/M_PI;
	o.roll *= 180.0/M_PI;

	return o;
}

//---------------------------------------------------------------------------------------------------
// Fast inverse square-root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root

float invSqrt(float x) {
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}