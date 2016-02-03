
#include "stdafx.h"

#include "fft_audacity_warp.h"



extern "C" {
#include <../AudioSpectrum/include/libavcodec/avcodec.h> 
#include <../AudioSpectrum/include/libavformat/avformat.h>
#include <../AudioSpectrum/include/libavutil/mathematics.h>
#include <../AudioSpectrum/include/libavutil/mem.h>
#include <../AudioSpectrum/include/libavcodec/avfft.h>


}

#include <stdio.h>
#include <../AudioSpectrum/include/stdint.h>
#include <stdlib.h>
#include <string>



using namespace std;
//#pragma comment(lib,"avutil.lib")
//#pragma comment(lib,"avcodec.lib")
//#pragma comment(lib,"avformat.lib")



#define FFT_NBITS   12
#define FFT_WINSIZE (1<<FFT_NBITS)



struct las {
	AVCodecContext *codec;
	uint8_t *audio_buf;
	int16_t *samples;
	int samples_bsize;
	int filled;
	RDFTContext *rdft;
	FFTSample *rdft_data;
	int win_count;
	float fft[FFT_WINSIZE/2 + 1];
};




class AudioSpectrum
{
public:
	enum AudioChannel
	{
		CH_L  =0,
		CH_R  =1,
		CH_LR =2,
	};

public:
	~AudioSpectrum();

public:
	int Set_Audio_file(std::string file_name);
	int Open_Audio_file();

	int Set_Channel_to_Analyze(AudioChannel channel );
	int Analyze_Audio();

	int Generate_Gnuplot(bool bToPNG );
	std::string m_Gunplot_output;
	//Anderson
	bool setFilepath(std::string filepath);
	std::string filepath_new;
	int Save_To_txt(bool bToPNG);
	std::string m_strSpectrum;
	
	int renametxt();
	std::string m_strSpectrumtxt;
	std::string m_testtxt;

	bool LoadAudioDLL();
private:
	struct las ctx;
	float hann[FFT_WINSIZE];
	std::string file_name;
	AVFormatContext *fmt_ctx ;
	int stream_id ;
	AudioChannel m_channel ;

	void precalc_hann(void);
	void process_samples(struct las *c);
	int process_audio_pkt(struct las *c, AVPacket *pkt);

	HINSTANCE m_hHighgui;
	HINSTANCE		m_hAvformatModule;
	HINSTANCE		m_hAvcodecModule;
	HINSTANCE		m_hAvutilModule;

	typedef int (__cdecl * PFav_find_stream_info)(AVFormatContext *ic); 
	PFav_find_stream_info CFav_find_stream_info;

	typedef int (__cdecl * PFavformat_open_input)(AVFormatContext **ps, const char *filename, AVInputFormat *fmt, AVDictionary **options); 
	PFavformat_open_input CFavformat_open_input;

	typedef int (__cdecl * PFav_read_frame)(AVFormatContext *s, AVPacket *pkt); 
	PFav_read_frame CFav_read_frame;

	typedef int (__cdecl * PFav_register_all)(void);
	PFav_register_all CFav_register_all;

	typedef int (__cdecl * PFav_close_input_file)(AVFormatContext *s);
	PFav_close_input_file CFav_close_input_file;
 
	/***********************/

	typedef int (__cdecl * PFavcodec_open)(AVCodecContext *avctx, AVCodec *codec);
	PFavcodec_open CFavcodec_open;

	typedef AVCodec* (__cdecl * PFavcodec_find_decoder)(enum AVCodecID id);
	PFavcodec_find_decoder CFavcodec_find_decoder;

	typedef RDFTContext* (__cdecl * PFav_rdft_init)(int nbits, enum RDFTransformType trans);
	PFav_rdft_init CFav_rdft_init;

	typedef attribute_deprecated int (__cdecl * PFavcodec_decode_audio3)(AVCodecContext *avctx, int16_t *samples,
                         int *frame_size_ptr,
                         AVPacket *avpkt);
	PFavcodec_decode_audio3 CFavcodec_decode_audio3;

	typedef void (__cdecl * PFav_rdft_end)(RDFTContext *s);
	PFav_rdft_end CFav_rdft_end;

	typedef int (__cdecl * PFavcodec_close)(AVCodecContext *avctx);
	PFavcodec_close CFavcodec_close;

	//int avcodec_close(AVCodecContext *avctx);

	//void av_rdft_end(RDFTContext *s);
//attribute_deprecated int avcodec_decode_audio3(AVCodecContext *avctx, int16_t *samples,
           //              int *frame_size_ptr,
                //         AVPacket *avpkt);
	//RDFTContext *av_rdft_init(int nbits, enum RDFTransformType trans);

	/************************/
	//typedef void (__cdecl * PFav_malloc)(size_t size) av_malloc_attrib av_alloc_size(1);
	typedef void* (__cdecl * PFav_malloc)(size_t size);
	PFav_malloc CFav_malloc  ;

	typedef void* (__cdecl * PFav_free)(void *ptr);;
	PFav_free CFav_free;
	//void *av_malloc(size_t size) av_malloc_attrib av_alloc_size(1);
	
	/************************/
	//typedef void* (__cdecl * PFavcodec_find_decoder)(enum AVCodecID id);
	//PFavcodec_find_decoder CFavcodec_find_decoder;
	//AVCodec *avcodec_find_decoder(enum AVCodecID id);

	//fft_audacity_warp *pFFT;
	//int m_FFT_result_size;

};
