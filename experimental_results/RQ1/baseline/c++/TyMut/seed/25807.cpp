
#include <cstddef>
struct vec {
	double x;
	double y;
	double z;

	double& operator[](size_t i) {
		switch (i) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		}
	}
};

