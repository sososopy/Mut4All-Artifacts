namespace std {
    template<class _Tp>
    class __attribute__ ((__visibility__("default"))) allocator;
    template <class _Iter>
    class __wrap_iter {
        template <class _Tp, class _Alloc> friend class vector;
    };
    template <class _Tp, class _Allocator>
    class __vector_base : protected __vector_base_common<true> {
    };
    template <class _Tp, class _Allocator = allocator<_Tp> >
    class __attribute__ ((__visibility__("default"))) vector : private __vector_base<_Tp, _Allocator> {
        typedef typename __base::pointer pointer;
        typedef __wrap_iter<pointer> iterator;
    };
}
namespace Eigen {
    template<class T>
    class aligned_allocator {
    };
    template<typename T>
    class vector<T,Eigen::aligned_allocator<T> > : public vector<T, Eigen::aligned_allocator_indirection<T> > {
        typedef typename vector_base::size_type size_type;
        void resize(size_type new_size) {
        }
    };
}
namespace pcl {
    namespace detail {
        struct FieldMapping {
        };
    }
    template <typename PointT>
    class PointCloud;
    typedef std::vector<detail::FieldMapping> MsgFieldMap;
    template<typename PointT>
    void createMapping (const std::vector<sensor_msgs::PointField>& msg_fields, MsgFieldMap& field_map) {
        std::sort(field_map.begin(), field_map.end(), detail::fieldOrdering);
        MsgFieldMap::iterator i = field_map.begin(), j = i + 1;
    }
    template <typename PointT>
    void pcl::copyPointCloud (const pcl::PointCloud<PointT> &cloud_in, const std::vector<int, Eigen::aligned_allocator<int> > &indices, pcl::PointCloud<PointT> &cloud_out) {
        cloud_out.points.resize (indices.size ());
    }
}
