
struct B
{
    int offset;
    bool is_loaded(unsigned char *p1)
    {
        if (offset) {
            *p1 = offset;
            return true;
        }
        return false;
    }
};

unsigned char match(struct B ref)
{
        unsigned char ref_offset;
        if (!ref.is_loaded(&ref_offset) || false)
                return ref_offset;
        else
                return 0;
}

