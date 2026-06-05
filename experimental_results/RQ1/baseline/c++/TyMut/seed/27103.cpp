
#include <utility>

template <typename component_types>
struct alphabet_tuple_base
{
    template <typename component_type>
        requires std::is_same_v<component_type, component_types>
    constexpr alphabet_tuple_base(component_type) {} // commenting out constexpr works?!

    template <typename indirect_component_type>
        requires (!std::is_same_v<indirect_component_type, component_types>)
    alphabet_tuple_base(indirect_component_type) {};
};

template <typename sequence_alphabet_t>
struct structured_rna : alphabet_tuple_base<sequence_alphabet_t> {
    using base_type = alphabet_tuple_base<sequence_alphabet_t>;
    using base_type::base_type;
};

struct dna4 {};
struct rna4 {};

structured_rna<rna4> t1{rna4{}}; // commenting out any of these works?!
structured_rna<rna4> t2{dna4{}}; // commenting out any of these works?!
structured_rna<rna4> t3{rna4{}}; // commenting out any of these works?!
