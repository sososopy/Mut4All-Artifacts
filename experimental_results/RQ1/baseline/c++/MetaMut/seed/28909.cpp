
class PCEFast_PSG {
  void UpdateOutput_Noise(int, int *);
};
template <int> void PCEFast_PSGRunChannel() {
  void (PCEFast_PSG::*ch_0)(int, int *);
  &PCEFast_PSG::UpdateOutput_Noise == ch_0;
}
