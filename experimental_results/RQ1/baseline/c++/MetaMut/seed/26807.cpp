

typedef struct old_visible_stuff *opaquePointer;

struct old_visible_stuff {
  int things_we_no_longer;
  int wish_to_expose;
} __attribute__((__deprecated__("do not refer to this, the layout might change")));

//A 
opaquePointer runtime_function (opaquePointer someObject);

//B
opaquePointer bad_runtime_call (struct old_visible_stuff *otherObject);

