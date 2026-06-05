
export module mod;

const int default_val { 0 };

export class Example
{
  // This line causes the problem.
  int value { default_val };
};
