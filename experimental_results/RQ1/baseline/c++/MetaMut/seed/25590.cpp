
@class NSString;
extern "C" NSString *NSUserName(void);
void foo() {
  [NSUserName() UTF8String];
}

