
export module group:tres;

int mul()
{
    return 0;
}


export module group;
export import :tres;

import group:tres;

int main() 
{
	return mul();
}
