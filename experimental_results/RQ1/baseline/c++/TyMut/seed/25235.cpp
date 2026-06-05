
enum SdrHitKind {};
class FuSelection {
  void m_fn1(void *);
};
void FuSelection::m_fn1(void *) {
  SdrHitKind eHit;
  if (eHit && dynamic_cast<const FuSelection *>(this) != nullptr)
    ;
}
