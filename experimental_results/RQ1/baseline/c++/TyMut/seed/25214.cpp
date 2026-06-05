
struct Stream {
   Stream & operator<<(const char *);
   virtual void ensure_vtable(); // Important!
};
struct Worker {
   ~Worker();
};
Worker::~Worker() {
   Stream()
      << ""
      << ""
      << ""
      << ""
      << ""
      << ""
      << ""
      << ""
      << ""
      << ""
      << "";
}


