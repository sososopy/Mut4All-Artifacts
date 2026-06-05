

typedef void (*obj_t)(void);

class parent {
  public:

    struct obj_base {
      virtual ~obj_base() {}
      virtual void operator()(void)=0;
    };
 
    template <typename T> struct obj : public obj_base {
      typedef void (T::*obj_t)(void);
      T* p; obj_t f;
      obj(T* p, obj_t f) : p(p), f(f) {}
      int *operator()(void) {
        return 0;
      }
    };
};

