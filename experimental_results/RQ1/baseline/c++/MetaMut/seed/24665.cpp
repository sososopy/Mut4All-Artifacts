
int main()
{
 auto x = [y = 5](){}; 
 auto z = x.y;
}

