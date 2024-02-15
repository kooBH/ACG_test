#include "volumeleveler.h"
#include "WAV.h"

VolumeLeveler* vl_new_volume_leveler(size_t l, size_t c, value_t s, value_t m)
{
  return new VolumeLeveler(l, c, s, m);
}

void vl_delete_volume_leveler(VolumeLeveler* vl)
{
  delete vl;
  return;
}

void vl_flush(VolumeLeveler* vl)
{
  vl->Flush();
  return;
}
size_t vl_exchange(VolumeLeveler* vl, value_t** in_buf, value_t** out_buf, size_t in_samples)
{
  return vl->Exchange(in_buf, out_buf, in_samples);
}
value_t vl_get_multiplier(VolumeLeveler* vl)
{
  return vl->GetMultiplier();
}

int main() {
  WAV wav;

	int n_channel = 1;
	int sr = 16000;
	int n_fft = 1024;
	int n_hop = 256;


	int length;
	WAV input(n_channel, sr);
	WAV output(1, sr);

	short** buf_in = new short*[1];
	buf_in[0] = new short[n_channel * n_hop];
	short** buf_out = new short*[1];
	buf_out[0] = new short[1 * n_hop];

	float** val_in = new float*[1];
	val_in[0] = new float[n_channel * n_hop];
	float** val_out = new float*[1];
	val_out[0] = new float[1 * n_hop];


	VolumeLeveler* vl =  new VolumeLeveler(n_hop, 1, 0.9, 2048);


	std::string InputFileName = "speech";
	input.OpenFile(InputFileName + ".wav");
	output.NewFile(InputFileName + "_output.wav");

	while (!input.IsEOF()) {
		length = input.ReadUnit(buf_in[0], n_hop * n_channel);
		for (int i = 0; i < n_hop; i++) {
			val_in[0][i] = static_cast<float>(buf_in[0][i]) / 32768.0;
		}
	  vl->Exchange(val_in,val_out, n_hop);
		for (int i = 0; i < n_hop; i++) {
			buf_out[0][i] = static_cast<short>(val_out[0][i] * 32768.0);
		}
		output.Append(buf_out[0], n_hop * 1);
	}

	output.Finish();
	input.Finish();

	delete[] buf_in;
	delete[] buf_out;

  return 0;
}