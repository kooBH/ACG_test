#include "AGC.h"

AGC::AGC(int n_channels_, int n_hop_, double strength_, int max_multiplier_)
  :proc(n_hop_,n_channels_,strength_,max_multiplier_) {
  this->n_channels = n_channels_;
  this->n_hop = n_hop_;
  this->strength = strength_;
  this->max_multiplier = max_multiplier_;
  printf("AGC(%d,%d,%lf,%d)\n",n_channels,n_hop,strength,max_multiplier);

  init_buffer();
}

AGC::~AGC() {
  release_buffer();
}

void AGC::init_buffer() {
  if (in)release_buffer();
  in = new float*[n_channels];
  out = new float*[n_channels];
  for (int i = 0; i < n_channels; i++) {
    in[i] = new float[n_hop];
    out[i] = new float[n_hop];
  }
}

void AGC::release_buffer() {
  if(!in)return;
  for (int i = 0; i < n_channels; i++) {
    delete[] in[i];
    delete[] out[i];
  }
  delete[] in;
  delete[] out;

  in = nullptr;
  out = nullptr;
}



void AGC::control(float** buf_in, float** buf_out) {
  proc.Exchange(in, out, n_hop);
}

// Assuming buf_in and buf_out are interleaved
// buf_in: [ch1, ch2, ch1, ch2, ...]
void AGC::control(short* buf_in, short* buf_out) {
  for (int i = 0; i < n_hop; i++) {
    for (int j = 0; j < n_channels; j++) {
      in[j][i] = static_cast<float>(buf_in[i*n_channels+j])/32768;
    }
  }
  proc.Exchange(in, out ,n_hop);
  for (int i = 0; i < n_hop; i++) {
    for (int j = 0; j < n_channels; j++) {
      buf_out[i*n_channels+j] = static_cast<short>(out[j][i]*32768);
    }
  }
}

// Assuming buf_in and buf_out are interleaved
// buf_in: [ch1, ch2, ch1, ch2, ...]
void AGC::control(float* buf_in, float* buf_out) {
  for (int i = 0; i < n_hop; i++) {
    for (int j = 0; j < n_channels; j++) {
      in[j][i] = buf_in[i*n_channels+j];
    }
  }
  proc.Exchange(in, out ,n_hop);
  for (int i = 0; i < n_hop; i++) {
    for (int j = 0; j < n_channels; j++) {
      buf_out[i*n_channels+j] = out[j][i];
    }
  }
}

void AGC::control(double** buf_in, double** buf_out) {
  for (int i = 0; i < n_hop; i++) {
    for (int j = 0; j < n_channels; j++) {
      in[j][i] = static_cast<float>(buf_in[j][i]);
    }
  }
  proc.Exchange(in, out ,n_hop);
  printf("%lf %lf\n",in[0][0], out[0][0]);
  for (int i = 0; i < n_hop; i++) {
    for (int j = 0; j < n_channels; j++) {
      buf_out[j][i] = out[j][i];
    }
  }
}


void AGC::SetStrength(double strength) {
  this->strength = strength;
  proc.SetStrength(strength);
}

void AGC::SetMaxScale(int max_multiplier) {
  this->max_multiplier = max_multiplier;
  proc.SetMaxMultiplier(max_multiplier);
}

void AGC::SetSize(int n_channels, int n_hop) {
  this->n_channels = n_channels;
  this->n_hop = n_hop;
  init_buffer();
  proc.SetSamplesAndChannels(n_hop, n_channels);
}