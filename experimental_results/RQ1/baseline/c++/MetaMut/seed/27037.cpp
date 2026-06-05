
bool
base_query::get_number_param(literal_map_t const & params,
			     interned_string k, long & v)
{
  int64_t value;
  bool present = derived_probe_builder::get_param (params, k, value);
  v = (long) value;
  return present;
}

