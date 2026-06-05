
class ResourceResponseBase {
  enum class Tainting { Opaque, Opaqueredirect };
  void sanitizeHTTPHeaderFieldsAccordingToTainting();
  Tainting m_tainting : 2;
};
void ResourceResponseBase::sanitizeHTTPHeaderFieldsAccordingToTainting() {
  switch (m_tainting)
  case Tainting::Opaque:
  case Tainting::Opaqueredirect:;
}

