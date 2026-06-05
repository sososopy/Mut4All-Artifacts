

class QVariantOutputArchive {
public:
  QVariantOutputArchive(int);
  template <typename...> void operator()() {
    {
      {
        {
          {
            {
              {
                {
                  {
                    {
                      {
                        {
                          {
                            {
                              {
                                [](auto) { operator(); };
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
};
template <typename, int> void qVariantArchiveTestVersioned() {
  int variant;
  QVariantOutputArchive oar(variant);
  oar();
}
void qVariantArchiveTest() { qVariantArchiveTestVersioned<int, true>; }


