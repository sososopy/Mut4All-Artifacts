
#pragma once

struct S
{
	const static int value;
};



constexpr int S::value = 0;



int main()
{
	return S::value;
}

