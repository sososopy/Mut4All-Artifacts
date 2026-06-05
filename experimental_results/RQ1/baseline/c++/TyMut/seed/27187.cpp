
struct pq {
  pq (const pq &);
};

struct a9 {
  operator pq () const;
};

struct zp : pq {
  zp (const a9 &k3) : pq (k3) { }
};

