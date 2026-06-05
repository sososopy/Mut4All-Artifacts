
#include<bits/stdc++.h>
using namespace std;

int main()
{
vector<int> arr(5);
for(int i=0;i<5;i++)
{
cin>>arr[i];
}
sort(arr.begin(),arr.end());
long int  sum = 0,min_sum=0,max_sum=0;
sum = arr[1]+arr[2]+arr[3]
min_sum = arr[0]+ sum; 
max_sum = arr[4]+ sum;
cout<<min_sum<<" "<<max_sum<<endl;

return 0;
}
      
