
export module argparse;

export {

  struct Argument {
    int (*i)(int) = 
      [](int value) { return value; };
  };

}


