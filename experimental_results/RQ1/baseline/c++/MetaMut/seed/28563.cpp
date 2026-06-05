
template <typename derived_type>
class alphabet_base
{
    [[deprecated("message")]] unsigned char_to_rank_table(char const chr)
    {
        using index_t = unsigned;
        return derived_type::char_to_rank[static_cast<index_t>(chr)];
    }
};

