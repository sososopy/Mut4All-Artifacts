
template <class T> class nsRefPtr
{
  T *mRawPtr;

public:
  typedef T element_type;
  ~nsRefPtr () { mRawPtr->Release (); }
  void get ();
};

namespace CSF
{
class CC_Call
{
public:
  void Release () { delete this; }
  virtual ~CC_Call ();
};
class CC_SIPCCCallMediaData
{
public:
  void Release ();
};
class A : public CC_Call
{
public:
  static nsRefPtr<CSF::A> wrap (int);
  nsRefPtr<CSF::CC_SIPCCCallMediaData> pMediaData;
};
}

using namespace CSF;
void fn1 () { A::wrap (0).get (); }

