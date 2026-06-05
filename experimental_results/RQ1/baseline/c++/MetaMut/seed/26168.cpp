
#include <iostream>
#include <string>

using namespace std;

bool ispalindrome(string &s){
  // replace with below line to see the error.
  // for(string::iterator i1=s.begin(),string::reverse_iterator i2=s.rbegin() ; i1!=s.end();  i1++, i2++) {
  string::reverse_iterator i2=s.rbegin();
  for(string::iterator i1=s.begin() ; i1!=s.end();  i1++, i2++) {
    if (*i1!=*i2) return false;
  }
  return true;
}

int main()
{
  string s;
	cin >> s ; 
  if (ispalindrome(s)) {
    cout << "First\n";
  }else{
    cout << "Second\n";
  }
	return 0;
}
