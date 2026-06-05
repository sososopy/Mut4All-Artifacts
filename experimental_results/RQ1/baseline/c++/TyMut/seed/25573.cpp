
typedef void (*ArrayFunPtr)(int d[1]);
ArrayFunPtr getArrayFunPtr() { return 0; }
// CHECK: "\01?getArrayFunPtr@@YAP6AXQAH@ZXZ"
// FOUND: "\01?getArrayFunPtr@@YAP6AXPAH@ZXZ"

