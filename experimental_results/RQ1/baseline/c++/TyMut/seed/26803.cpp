
#include <memory>
typedef int32_t cl_int __attribute__((aligned(4)));
struct buffer_unmapper {
    void operator()(cl_int* ptr) const {
    }
};
typedef std::unique_ptr<cl_int[], buffer_unmapper> mapped_array;

