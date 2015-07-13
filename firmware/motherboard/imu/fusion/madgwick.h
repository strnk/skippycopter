#ifndef IMU_FUSION_MAGDWICK_H
#define IMU_FUSION_MAGDWICK_H

#include <common.h>

class MadgwickAHRS
{
private:
	float beta;
	float q0, q1, q2, q3;

	void updateIMU(data3f gyro, data3f accelero);

public:
	MadgwickAHRS();

	void update(data3f gyro, data3f accelero, data3f magneto);
	orientation_t getOrientation();
	quaternion_t getQuaternion();
};

#endif // IMU_FUSION_MAGDWICK_H