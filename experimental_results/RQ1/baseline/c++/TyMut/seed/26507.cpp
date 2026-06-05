
class ErrorClass {
    void causeError() {
#pragma omp target map(size, ptr[:size]) nowait
#pragma omp teams
    float error = ptr[0];
}

unsigned size = 5;
float* ptr = new float[size];
};

