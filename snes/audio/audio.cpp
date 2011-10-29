#ifdef SYSTEM_CPP

Audio audio;

void Audio::coprocessor_enable(bool state) {
  coprocessor = state;

  dsp_rdoffset = cop_rdoffset = 0;
  dsp_wroffset = cop_wroffset = 0;
  dsp_length = cop_length = 0;

  r_sum_l = r_sum_r = 0;
}

void Audio::coprocessor_frequency(double input_frequency) {
  double output_frequency;
  output_frequency = system.apu_frequency / 768.0;
  r_step = input_frequency / output_frequency;
  r_frac = 0;
}

void Audio::sample(int16 lsample, int16 rsample) {
  if(coprocessor == false) return snes_interface->audioSample(lsample, rsample);

  dsp_buffer[dsp_wroffset] = ((uint16)lsample << 0) + ((uint16)rsample << 16);
  dsp_wroffset = (dsp_wroffset + 1) & buffer_mask;
  dsp_length = (dsp_length + 1) & buffer_mask;
  flush();
}

void Audio::coprocessor_sample(int16 left, int16 right) {
  if(r_frac >= 1.0) {
    r_frac -= 1.0;
    r_sum_l += left;
    r_sum_r += right;
    return;
  }

  r_sum_l += static_cast<unsigned int>(left  * r_frac);
  r_sum_r += static_cast<unsigned int>(right * r_frac);

  uint16 output_left  = sclamp<16>(int(r_sum_l / r_step));
  uint16 output_right = sclamp<16>(int(r_sum_r / r_step));

  double first = 1.0 - r_frac;
  r_sum_l = static_cast<unsigned int>(left  * first);
  r_sum_r = static_cast<unsigned int>(right * first);
  r_frac = r_step - first;

  cop_buffer[cop_wroffset] = (output_left << 0) + (output_right << 16);
  cop_wroffset = (cop_wroffset + 1) & buffer_mask;
  cop_length = (cop_length + 1) & buffer_mask;
  flush();
}

void Audio::init() {
}

void Audio::flush() {
  while(dsp_length > 0 && cop_length > 0) {
    uint32 dsp_sample = dsp_buffer[dsp_rdoffset];
    uint32 cop_sample = cop_buffer[cop_rdoffset];

    dsp_rdoffset = (dsp_rdoffset + 1) & buffer_mask;
    cop_rdoffset = (cop_rdoffset + 1) & buffer_mask;

    dsp_length--;
    cop_length--;

    signed dsp_left  = (int16)(dsp_sample >>  0);
    signed dsp_right = (int16)(dsp_sample >> 16);

    signed cop_left  = (int16)(cop_sample >>  0);
    signed cop_right = (int16)(cop_sample >> 16);

    snes_interface->audioSample(
      sclamp<16>((dsp_left  + cop_left ) / 2),
      sclamp<16>((dsp_right + cop_right) / 2)
    );
  }
}

#endif
