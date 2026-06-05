
#include <vector>

int attributesAsString(std::vector<int> & v)
{
  int attributes;

  std::vector<int>::const_iterator itAttribute = v.begin();
  while ( itAttribute != v.end() )
  {
    const int &attribute = *itAttribute++;
    attributes += attribute;
  }

  return attributes;
}
