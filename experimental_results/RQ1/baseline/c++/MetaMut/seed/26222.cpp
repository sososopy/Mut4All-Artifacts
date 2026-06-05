typedef enum {
    P_ALL, P_PID, P_PGID }
    idtype_t;
    typedef long unsigned size_t;
    typedef __typeof__(((int*)0)-((int*)0)) ptrdiff_t;
    namespace std {
        inline namespace __1 {
            struct __two {
            };
            template <class _Tp, _Tp __v> struct __attribute__ ((__visibility__("default"))) integral_constant {
                static constexpr const _Tp value = __v;
            };
            template <class _Tp> struct __attribute__ ((__visibility__("default"))) remove_reference {
                typedef _Tp type;
            };
            template <class _Tp> struct __has_iterator_category {
            };
            template <class _Iter, bool> struct __iterator_traits {
            };
            template <class _Iter> struct __attribute__ ((__visibility__("default"))) iterator_traits : __iterator_traits<_Iter, __has_iterator_category<_Iter>::value> {
            };
            template<class _Tp> struct __attribute__ ((__visibility__("default"))) iterator_traits<_Tp*> {
                typedef _Tp& reference;
            };
            template <class _Iter> class __wrap_iter {
                public: typedef _Iter iterator_type;
                        typedef typename iterator_traits<iterator_type>::reference reference;
                private: iterator_type __i;
                public: __attribute__ ((__visibility__("hidden"), __always_inline__)) __wrap_iter() noexcept {
                        }
                         __attribute__ ((__visibility__("hidden"), __always_inline__)) reference operator*() const noexcept {
                             return *__i;
                         }
                         __attribute__ ((__visibility__("hidden"), __always_inline__)) __wrap_iter& operator++() noexcept {
                             ++__i;
                             return *this;
                         }
                         __attribute__ ((__visibility__("hidden"), __always_inline__)) iterator_type base() const noexcept {
                             return __i;
                         }
            };
            template <class _Iter1, class _Iter2> inline __attribute__ ((__visibility__("hidden"), __always_inline__)) bool operator==(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter2>& __y) noexcept {
                return __x.base() == __y.base();
            }
            template <class _Iter1, class _Iter2> inline __attribute__ ((__visibility__("hidden"), __always_inline__)) bool operator!=(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter2>& __y) noexcept {
                return !(__x == __y);
            }
            template <class _Tp> struct __has_difference_type {
                template <class _Up> static char __test(typename _Up::difference_type* = 0);
                public: static const bool value = sizeof(__test<_Tp>(0)) == 1;
            };
            template <class _Ptr> struct __attribute__ ((__visibility__("default"))) pointer_traits {
                typedef ptrdiff_t difference_type;
            };
            namespace __has_pointer_type_imp {
                template <class _Up> static __two test(...);
            }
            template <class _Tp> struct __has_pointer_type : public integral_constant<bool, sizeof(__has_pointer_type_imp::test<_Tp>(0)) == 1> {
            };
            namespace __pointer_type_imp {
                template <class _Tp, class _Dp, bool = __has_pointer_type<_Dp>::value> struct __pointer_type {
                    typedef _Tp* type;
                };
            }
            template <class _Tp, class _Dp> struct __pointer_type {
                typedef typename __pointer_type_imp::__pointer_type<_Tp, typename remove_reference<_Dp>::type>::type type;
            };
            template <class _Tp> struct __has_size_type {
                template <class _Up> static char __test(typename _Up::size_type* = 0);
                public: static const bool value = sizeof(__test<_Tp>(0)) == 1;
            };
            template <class _Alloc, class _DiffType, bool = __has_size_type<_Alloc>::value> struct __size_type {
                typedef typename _Alloc::size_type type;
            };
            template <class _Alloc, class _Ptr, bool = __has_difference_type<_Alloc>::value> struct __alloc_traits_difference_type {
                typedef typename pointer_traits<_Ptr>::difference_type type;
            };
            template <class _Alloc> struct __attribute__ ((__visibility__("default"))) allocator_traits {
                typedef _Alloc allocator_type;
                typedef typename allocator_type::value_type value_type;
                typedef typename __pointer_type<value_type, allocator_type>::type pointer;
                typedef typename __alloc_traits_difference_type<allocator_type, pointer>::type difference_type;
                typedef typename __size_type<allocator_type, difference_type>::type size_type;
            };
            template <class _Tp> class __attribute__ ((__visibility__("default"))) allocator {
                public: typedef size_t size_type;
                        typedef ptrdiff_t difference_type;
                        typedef _Tp value_type;
            };
            template <bool> class __vector_base_common {
            };
            template <class _Tp, class _Allocator> class __vector_base : protected __vector_base_common<true> {
                protected: typedef _Tp value_type;
                           typedef _Allocator allocator_type;
                           typedef allocator_traits<allocator_type> __alloc_traits;
                           typedef typename __alloc_traits::size_type size_type;
                           typedef typename __alloc_traits::pointer pointer;
            };
            template <class _Tp, class _Allocator = allocator<_Tp> > class __attribute__ ((__visibility__("default"))) vector : private __vector_base<_Tp, _Allocator> {
                private: typedef __vector_base<_Tp, _Allocator> __base;
                public: typedef vector __self;
                        typedef typename __base::size_type size_type;
                        typedef typename __base::pointer pointer;
                        typedef __wrap_iter<pointer> iterator;
                        __attribute__ ((__visibility__("hidden"), __always_inline__)) iterator begin() noexcept;
                        __attribute__ ((__visibility__("hidden"), __always_inline__)) iterator end() noexcept;
                        __attribute__ ((__visibility__("hidden"), __always_inline__)) size_type size() const noexcept {
                        }
            };
        }
    }
namespace cv {
    using std::vector;
    template<typename _Tp> class Point3_ {
    };
    typedef Point3_<float> Point3f;
    class Mesh3D {
        public: struct EmptyMeshException {
                };
                vector<Point3f> vtx;
    };
    class SpinImageModel {
        void selectRandomSubset(float ratio);
        vector<int> subset;
        Mesh3D mesh;
    };
    inline namespace __1 {
        template <class _ForwardIterator, class _Tp> inline __attribute__ ((__visibility__("hidden"), __always_inline__)) void iota(_ForwardIterator __first, _ForwardIterator __last, _Tp __value_) {
            for (;
                    __first != __last;
                    ++__first, ++__value_) *__first = __value_;
        }
    }
}
void cv::SpinImageModel::selectRandomSubset(float ratio) {
    size_t vtxSize = mesh.vtx.size();
    size_t setSize = static_cast<size_t>(vtxSize * ratio);
    if (setSize == 0) {
        iota(subset.begin(), subset.end(), 0);
    }
}
