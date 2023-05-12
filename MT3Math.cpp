#include "MT3Math.h"

float Clamp(float num, float max, float min) {

	if (num > max) {
		return max;
	}
	else if (num < min) {
		return min;
	}
	else {
		return num;
	}

}