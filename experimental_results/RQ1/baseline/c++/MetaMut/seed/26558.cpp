
#include <coroutine>
#include <iostream>
#include <set>
#include <thread>
#include <immintrin.h>
#include <chrono>
using namespace std;
struct HelloCoroutine {
    struct HelloPromise {
        HelloCoroutine get_return_object() {
            return std::coroutine_handle<HelloPromise>::from_promise(*this);
        }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() {
            std::cout <<" exception!" << std::endl;
        }
    };
    using promise_type = HelloPromise;
    HelloCoroutine(std::coroutine_handle<HelloPromise> h) : handle(h) {}

    std::coroutine_handle<HelloPromise> handle;
};

#define AVX512 0
#define AVX2 1
#define SSE 0

HelloCoroutine hello(int& index, int id, int group_size) {
    unsigned res=0;
#if AVX512
// g++ simd.cpp -std=gnu++20 -fcoroutines -O2 -march=skylake -mavx512f
// segment fault
    for(auto i= index++; i< 20; i=index++)
    {
        std::cout <<"step 1" <<std::endl;
        __m512i v_offset = _mm512_set1_epi64(int64_t (i));
        std::cout <<"step 2" <<std::endl;
        __m512i v_size = _mm512_set1_epi64(int64_t(group_size));
        std::cout <<"step 3" <<std::endl;
        res = _mm512_cmpgt_epi64_mask(v_offset, v_size);
        cout <<i << " > " << group_size <<" ? " << (int)res<<endl;
        co_await std::suspend_always();
    }
#elif AVX2 
// g++ simd.cpp -std=gnu++20 -fcoroutines -O2 -march=skylake
// only specify `-O2 -march=skylake` and runs ok on local machine, otherwise segment fault (also on godbolt)
    for(auto i= index++; i< 20; i=index++)
    {
        std::cout <<"step 1" <<std::endl;
        __m256i v_offset = _mm256_set1_epi32(int32_t (i));
        std::cout <<"step 2" <<std::endl;
        __m256i v_size = _mm256_set1_epi32(int32_t(group_size));
        std::cout <<"step 3" <<std::endl;
        res = _mm256_movemask_epi8(_mm256_cmpgt_epi32(v_offset, v_size));
        cout <<i << " > " << group_size <<" ? " << (int)res<<endl;
        co_await std::suspend_always();
    }
#elif SSE
    for(auto i= index++; i< 20; i=index++)
    {
        __m128i v_offset = _mm_set1_epi32(int32_t (i));
        __m128i v_size = _mm_set1_epi32(int32_t(group_size));
        res = _mm_movemask_epi8(_mm_cmpgt_epi32(v_offset, v_size));
        cout <<i << " > " << group_size <<" ? " << res<<endl;
        co_await std::suspend_always();
    }    
#else
    for(auto i= index++; i< 20; i=index++)
    {
        res = i > group_size;
        cout <<i << " > " << group_size <<" ? " << res<<endl;
        co_await std::suspend_always();
    }
#endif
}


int main() {
    set<std::coroutine_handle<HelloCoroutine::HelloPromise>>handles;
    int group_size = 10;
    int index =0;
    for(auto i=0; i< group_size;++i)
    {
        handles.insert(hello(index, i, group_size).handle);
    }
    while(!handles.empty()) {
        for (auto it = handles.begin(); it != handles.end();) {
            if (it->done()) {
                it->destroy();
                it = handles.erase(it);
            } else {
                it->resume();
                it++;
            }
        }
    }
    return 0;
}
