
template<typename> class Obj; 
template<> class Obj<void> { 
  struct secret{}; 
}; 
template<typename T> class Obj { 
  Obj<void>::secret m; 
};

int main()
{ 
  Obj<int> obj; 
}

