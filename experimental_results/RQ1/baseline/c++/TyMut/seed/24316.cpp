
template< int xyz >
struct wovo {

   template< int n >
   void us(){}
   
   template< int n >
   struct us< n > {};
};
