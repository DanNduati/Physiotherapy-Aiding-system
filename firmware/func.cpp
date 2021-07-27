void Calculate_FSR()
{
  const unsigned short other_resistor = 10000; // 10[K Ohm] resistor

  // force calculation parameters
  float fsr_tot_force_now = 0, resistor_volt [NUM_OF_FSR], fsr_force[NUM_OF_FSR], force[NUM_OF_FSR];
  const unsigned short  a = 35423 ;
  const float  b = -0.735, c = -0.0496 ;
  int A = 9.81; // הנחה שזו התאוצה של כדור הארץ
  float mass[NUM_OF_FSR]={0};
 
  // Take the measure the force applied to the FSRs and resistance
  for (short i = 0; i < NUM_OF_FSR; i++) // The number of pressure sensors is 6
  {
    resistor_volt[i] = (analogRead(fsrPins[i]) * 3300 / 4095.0); //Converting to milivolts

    if (resistor_volt[i] != 0) // If there is force
    {
      fsr_resistance[i] = (3300 * other_resistor) / resistor_volt[i] - other_resistor;
      mass[i] = (a * pow(fsr_resistance[i], b) + c);
      force[i]= A * mass[i];
      fsr_tot_force_now = fsr_tot_force_now + force[i];
    }
    else
    {
      fsr_resistance[i] = 10000000; // 10^7 [Ohm]
      force[i] = 0;
      fsr_tot_force_now = fsr_tot_force_now + force[i];
    }
  }
}
