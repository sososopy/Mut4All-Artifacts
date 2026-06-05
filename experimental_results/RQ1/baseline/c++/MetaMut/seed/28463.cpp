
struct R3 {
  double z;
  R3(R3 A, R3 B) : z(B.z - A.z) {}
  double norme() { return z; }
};
struct TBoundaryEdge {
  int *vertices[2];
  int &operator[](int i) { return *vertices[i]; }
};
struct Mesh {
  int vertices;
  TBoundaryEdge *bedges;
  int operator()(int &vv) { return &vv - &vertices; }
  TBoundaryEdge be(int i) { return bedges[i]; }
};
template <typename Data> struct GenericElement {
  typedef typename Data::V Vertex;
  static const int nv = Data::NbOfVertices;
  Vertex *vertices[nv];
  double mes;
  void set(int *iv) {
    Vertex *v0;
    for (int i = 0; i < nv; ++i)
      vertices[i] = v0 + iv[i];
    mes = Data::mesure(vertices);
  }
};
struct DataSeg3 {
  static const int NbOfVertices = 2;
  typedef R3 V;
  static double mesure(V *pv[]) { return R3(*pv[0], *pv[1]).norme(); }
};
struct MeshS {
  MeshS();
};
template <class> struct Movemesh_Op { int operator()() const; };
template <> int Movemesh_Op<int>::operator()() const {
  Mesh pTh;
  GenericElement<DataSeg3> *bS = new GenericElement<DataSeg3>;
  for (int ibe; ibe; ibe++) {
    TBoundaryEdge K(pTh.be(ibe));
    int iv[2];
    for (int i = 0; i < 2; i++) {
      int &__trans_tmp_2 = K[i];
      iv[i] = pTh(__trans_tmp_2);
    }
    bS[ibe].set(iv);
  }
  MeshS T_Th;
}
