
    template <typename alloc_unit, int width, int offset>
    struct offset_bitfield { alloc_unit : offset, field : width; };

