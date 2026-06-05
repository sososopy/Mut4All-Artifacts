

export module A;

class A; // forward declaration

export class A {}; // actual declaration

import A;

int main()
{
  A a;

  return 0;
}

