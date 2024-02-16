#ifndef _H_AGC_
#define _H_AGC_

#include "volumeleveler.h"
#include <stdio.h>


class AGC {
private :
  double strength = 0.9;
  int max_multiplier = 2048;
  
  int n_channels;
  int n_hop;

  float **in=nullptr, **out=nullptr;

  void init_buffer();
  void release_buffer();

  VolumeLeveler proc;

public : 
  AGC(int n_channels, int n_hop, double strength, int max_multiplier);
  ~AGC();
  void control(float** buf_in, float** buf_out);
  void control(short* buf_in, short* buf_out);
  void control(float* buf_in, float* buf_out);
  void control(double** buf_in, double** buf_out);

  void SetStrength(double strength);
  void SetMaxScale(int max_multiplier);
  void SetSize(int n_channels, int n_hop);

};

#endif