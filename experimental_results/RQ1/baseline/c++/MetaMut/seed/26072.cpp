
class FruitCpp {};

class AppleCpp : public FruitCpp {};

class OrangeCpp : public FruitCpp {};
Sadly, these block assignments don't compile any more:

FruitCpp *(^getFruitCpp)();
AppleCpp *(^getAppleCpp)();
getFruitCpp = getAppleCpp; // error!

