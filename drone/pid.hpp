class pid {
public:
   std::chrono::high_resolution_clock::time_point _lastTime;
   float _kp, _ki, _kd, _errSum, _lastErr;

   pid(float kp, float ki, float kd) :
   _kp(kp), _ki(ki), _kd(kd), 
   _lastTime(std::chrono::high_resolution_clock::now()),  _lastErr(0), _errSum(0)
   {
   }

   float compute(float input, float setpoint) {
      auto now = std::chrono::high_resolution_clock::now();
      auto dt = std::chrono::duration_cast<std::chrono::microseconds>(now - _lastTime).count()/1000000.0;
      auto error = setpoint - input;
      _errSum += error * dt;
      auto  dErr = (error - _lastErr) / dt;
      _lastErr = error;
      _lastTime = now;

      return _kp*error +  _ki*_errSum + _kd*dErr; 
   }
};
