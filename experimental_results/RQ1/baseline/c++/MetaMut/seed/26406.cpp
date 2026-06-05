

struct basic_string {
  basic_string(const char *);
  ~basic_string();
};
char InformationRequest, AddressMaskRequest;
struct ICMP {
  basic_string MatchFilter() const;
};
basic_string ICMP::MatchFilter() const {
  if (InformationRequest || AddressMaskRequest) {
    basic_string ret_string = "";
    return ret_string;
  }
  return "";
}
