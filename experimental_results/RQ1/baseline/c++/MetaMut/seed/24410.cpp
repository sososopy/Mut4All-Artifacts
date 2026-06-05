
#include <string>
#include <vector>
#include <valarray>
#include <unordered_map>
#include <algorithm>


using std::vector;
using std::valarray;
using std::string;
using std::unordered_map;
using std::sort;

typedef valarray <string> Options;

vector <Options> groups = {
                         { "red", "green", "orange", "purple", "yellow"},
                         { "apple", "banana", "cherry"},
                         { "butterscotch", "chocolate", "vanilla"},
                         { "rain", "sunny", "hot", "pleasant" }
                       };

// enum {color, fruits, flavors, weather} option_choices;

Options categories = { "color", "fruits", "flavors", "weather" };
unordered_map <string, int> catmap;

int main( int argc, char *argv[] ) {

 int c = 0;
 for (auto &cattxt:categories) catmap[cattxt]=c++;

 for (auto &group:groups) sort(group.begin(), group.end());

 for (auto &cat:catmap) {
   const char *ch="";
   printf("Category: %s\n  ", cat.first.c_str());
   for (auto &opt:groups[cat.second]) {
     printf("%s%s ",ch, opt.c_str());
     ch=",";
   }
   printf("\n");
 }
 exit(0);
}
