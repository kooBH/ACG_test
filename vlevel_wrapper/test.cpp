#include "agc.h"
#include "WAV.h"

int main() {
  WAV wav;

	int n_channel = 1;
	int sr = 16000;
	int n_fft = 1024;
	int n_hop = 256;


	int length;
	WAV input(n_channel, sr);
	WAV output(1, sr);

	AGC agc(n_channel, n_hop, 0.9, 2048);

	short* buf_in = new short[n_channel * n_hop];
	short* buf_out = new short[n_channel * n_hop];

	//std::string InputFileName = "speech";
	std::string InputFileName = "../../mpB_noisy";
	input.OpenFile(InputFileName + ".wav");
	output.NewFile(InputFileName + "_vlevel.wav");

	while (!input.IsEOF()) {
		length = input.ReadUnit(buf_in, n_hop * n_channel);
		agc.control(buf_in, buf_out);
		output.Append(buf_out, n_hop * 1);
	}

	output.Finish();
	input.Finish();

	delete[] buf_in;
	delete[] buf_out;

  return 0;
}