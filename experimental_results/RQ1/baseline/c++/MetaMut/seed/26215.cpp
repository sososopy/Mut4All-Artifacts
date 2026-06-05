
 int CProfileGenerator::CalcAccProfile(
 pos_type p0,
 vel_type v0,
 acc_type a0,
 jerk_type j,
 vel_type vel,
 acc_type acc,
 CProfile& ret_profile
 ) const {
 time_type t0 = 0.0;
 ret_profile.clear(); ret_profile.reserve(10);

 vel_type dv = vel-v0;

 if (ABS(dv) < m_epsilon_v && ABS(a0) < m_epsilon_a) {
 if (dv == 0.0) {
 ret_profile.push_back(CProfileStep(0.0, p0, v0, a0, 0.0)); VALIDATEPROFILE(ret_profile);
 } else {
 acc_type a0 = (dv<0.0)?-1.0:1.0;
 time_type dt = ABS(dv);
 pos_type p1 = p0 + v0*dt + 0.5*dv*dv;
 ret_profile.push_back(CProfileStep(0.0, p0, v0, a0, 0.0)); VALIDATEPROFILE(ret_profile);
 ret_profile.push_back(CProfileStep(dt, p1, vel, 0.0, 0.0)); VALIDATEPROFILE(ret_profile);
 }
 G_BLOG_4("acc profile calc skiped ");
 return ret_profile.size();
 }
 if (v0 < vel) {
 acc = ABS(acc);
 } else if (v0 == vel) {
 acc = -SIGN(a0)*ABS(acc);
 } else {
 acc = -ABS(acc);
 }

 if (j == 0.0) {
 return InternalAccCalc(t0, p0, v0, 0.0, 0.0, true, true, vel, acc, true, ret_profile);
 // <------------------------------ FAULTS DUE TO RETURNING TO ADDRESS 0x00000000 HERE
 }
