
constexpr int test(const int *arr) {
  if(arr == 0){
     //
   }
  return 5;
}

int main(){
  static constexpr int arr[] = {1,2,3};
  static constexpr int b =  test(arr);
  return 0;
}

