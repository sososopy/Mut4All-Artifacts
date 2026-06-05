
struct allocator { };

struct string {
  string(const string&);
  string(const allocator&);
};

struct NotString : allocator { };

struct String {
  operator string() const;
  operator NotString() const;
};

struct Message {
  template<typename T>
    Message(T&& t) : s{t} { }
  string s;
};

int main()
{
  String s;
  Message m(s);
}
