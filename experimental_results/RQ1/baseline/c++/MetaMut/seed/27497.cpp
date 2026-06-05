
#include <tuple>
namespace seqan3 {
template <typename, typename = void> struct pipeable_config_element;
template <typename derived_t>
struct pipeable_config_element<derived_t, void> {};
template <typename, typename value_t> struct pipeable_config_element {
  value_t value;
};
struct gap_scheme {
  int gap = 1;
};
template <typename... configs_t>
struct configuration : std::tuple<configs_t...> {
  template <typename derived_t, typename value_t>
  constexpr configuration(pipeable_config_element<derived_t, value_t> const) {}
  template <typename...> constexpr configuration(std::tuple<> const) {}
  template <typename config_element_t>
  constexpr auto push_back(config_element_t) const {
    return configuration<configs_t..., config_element_t>{std::tuple{}};
  }
};
template <typename derived_t, typename value_t>
configuration(pipeable_config_element<derived_t, value_t> const)
    -> configuration<derived_t>;
} // namespace seqan3
namespace seqan3::align_cfg {
template <typename gap_scheme_t>
struct gap : pipeable_config_element<gap<gap_scheme_t>, gap_scheme_t> {
  gap() = default;
  constexpr gap(gap_scheme_t const)
      : pipeable_config_element<gap, gap_scheme_t>() {}
};
} // namespace seqan3::align_cfg
namespace seqan3::detail {
struct method_local_tag : pipeable_config_element<method_local_tag> {};
} // namespace seqan3::detail
namespace seqan3::align_cfg {
seqan3::detail::method_local_tag method_local;
template <typename scoring_scheme_t>
struct scoring_scheme : pipeable_config_element<int, void> {
  scoring_scheme() = default;
  constexpr scoring_scheme(scoring_scheme_t) {};
};
struct aminoacid_scoring_scheme {};
template <typename config_t> struct alignment_fixture { config_t config; };
template <typename config_t>
alignment_fixture(config_t) -> alignment_fixture<config_t>;
inline constexpr auto align_config =
    seqan3::configuration{seqan3::align_cfg::method_local}.push_back(
        seqan3::align_cfg::gap{seqan3::gap_scheme{}});
static auto aa27_02 = [] {
  return alignment_fixture{align_config.push_back(
      seqan3::align_cfg::scoring_scheme{aminoacid_scoring_scheme{}})};
}();
} // namespace seqan3::align_cfg

