struct x {
  int n;
  void bye();

  template<typename T>
  operator T() const
    {
      if (n == 0)
	bye();
      return n;
    }
};
