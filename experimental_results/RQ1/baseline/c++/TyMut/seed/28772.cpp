
//$ cat task.cpp
export module task;

#include "b.h"
#include "d.h"

//$ cat b.h
#pragma once

export struct Base {
  virtual ~Base() = default;

  void DoStuff();
};

//$ cat d.h
#include "b.h"

export struct Derived : Base {
  ~Derived();
};

//$ cat main.cpp
import task;

int main() {
    auto p = new Derived;
    p->DoStuff();
    delete p;
}
