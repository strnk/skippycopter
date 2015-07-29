#ifndef COMMON_H
#define COMMON_H

#include <inttypes.h>

#define floatAsU32(x)   (*((uint32_t*)(&(x))))

typedef struct
{
	int16_t x;
	int16_t y;
	int16_t z;
} data3;

typedef struct
{
	uint16_t x;
	uint16_t y;
	uint16_t z;
} data3u;


typedef struct
{
	int32_t x;
	int32_t y;
	int32_t z;
} data3l;

typedef struct
{
	uint32_t x;
	uint32_t y;
	uint32_t z;
} data3ul;

typedef struct
{
	float x;
	float y;
	float z;
} data3f;

typedef struct {
	float yaw;
	float pitch;
	float roll;
} orientation_t;

typedef struct {
	float q0;
	float q1;
	float q2;
	float q3;
} quaternion_t;


#endif // COMMON_H