

 typedef __builtin_va_list __gnuc_va_list;
 typedef __gnuc_va_list va_list;
 struct ContinuousUDParser 
 {
 va_list err_args;
 };
 void* prepare(){
 return (new ContinuousUDParser());
 }
