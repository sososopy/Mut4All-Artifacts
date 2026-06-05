
struct GP {
  double mx, my, mz;
  GP (double x, double y, double z) : mx(x), my(y), mz(z) { };
};
typedef struct GP GP;

void do_something(const GP& n) { }

struct VV {
  double mx;
  VV (double x) : mx(x) { };
};
typedef struct VV VV;

struct PP {
  double mx;
  PP (double x) : mx(x) { };
  double x() { return mx; };
  PP& operator+=(const VV& rhs) { mx += rhs.mx; return *this; };
};
typedef struct PP PP;

int get_size() { return 0; };

void abc() {
  double tanDistToFace[6] = {0,0,0,0,0,0};
  unsigned int iFDestSorted[6] = {0,0,0,0,0,0};
  unsigned int nDestSorted = 0;

  unsigned int nFaces = get_size();
  for (unsigned int iFace = 0; iFace < nFaces; ++iFace) {
    nDestSorted++;
  }

  for (unsigned int i = 0; i < nDestSorted; ++i) {
    unsigned int iMax = nDestSorted - i - 1;
    for (unsigned int j=0; j < nDestSorted-i; ++j) {
      if (tanDistToFace[iFDestSorted[j]] > tanDistToFace[iFDestSorted[iMax]]) {
        iMax = j;
      }
    }
    unsigned int iTmp = iFDestSorted[nDestSorted - i - 1];
    iFDestSorted[nDestSorted - i - 1] = iFDestSorted[iMax];
    iFDestSorted[iMax] = iTmp;
  }

    int iFDest = iFDestSorted[0];
    PP lp(0);
    lp += VV(tanDistToFace[iFDest]);
    GP gp(lp.x(), lp.x(), lp.x());
    do_something(gp);
}
