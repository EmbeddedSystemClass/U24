#include "stdafx.h"
#include "AudioSpectrum.h"
#include<fstream>

#pragma warning(disable: 4996)


#define SAMPLE_COUNT	4410

int AudioSpectrum::Set_Audio_file( std::string file_name )
{
    this->file_name =file_name;
	return 0;
}


int AudioSpectrum::Open_Audio_file()
{
	/* FFmpeg init */
	//av_register_all();
	CFav_register_all();

	//init variable
    unsigned i;
	fmt_ctx = NULL;
	stream_id =-1;
	m_channel = CH_LR;
	memset( (void*)&ctx ,0, sizeof(las) );

	//if (avformat_open_input(&fmt_ctx, file_name.c_str()  , NULL, NULL) ||
	if (CFavformat_open_input(&fmt_ctx, file_name.c_str()  , NULL, NULL) ||
		//av_find_stream_info(fmt_ctx) < 0)
		CFav_find_stream_info(fmt_ctx) < 0)
		printf( "unable to load %s", file_name.c_str() );

	for (i = 0; i < fmt_ctx->nb_streams; i++)
	{
		if (fmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			stream_id = i;
			break;
		}
	}

	if (stream_id == -1)
	{
		printf( "no audio stream found");
	}

	ctx.codec = fmt_ctx->streams[stream_id]->codec;
	//AVCodec *adec = avcodec_find_decoder(ctx.codec->codec_id);
	AVCodec *adec = CFavcodec_find_decoder(ctx.codec->codec_id);

	if (!adec)
	{
		printf( "unsupported codec");
	}
	//avcodec_open(ctx.codec, adec);
	CFavcodec_open(ctx.codec, adec);
	if (ctx.codec->channels != 1 && ctx.codec->channels != 2)
	{
		printf("unsupported number of channels (%d)", ctx.codec->channels);
	}

	return 0;
}


int AudioSpectrum::Set_Channel_to_Analyze(AudioChannel channel)
{
	m_channel = channel;
	return 0;
}


int AudioSpectrum::Analyze_Audio()
{
	if (LoadAudioDLL() == false) return false;

	/* LAS init */
	//precalc_hann();
	ctx.samples_bsize = FFT_WINSIZE * ctx.codec->channels * sizeof(*ctx.samples);

	//ctx.samples       = (int16_t*)av_malloc(ctx.samples_bsize);
	//ctx.rdft          = av_rdft_init(FFT_NBITS, DFT_R2C);
	//ctx.rdft_data     = (FFTSample *)av_malloc(FFT_WINSIZE * sizeof(*ctx.rdft_data));
	//ctx.audio_buf     = (uint8_t *)av_malloc(AVCODEC_MAX_AUDIO_FRAME_SIZE);

	ctx.samples       = (int16_t*)CFav_malloc(ctx.samples_bsize);
	ctx.rdft          = CFav_rdft_init(FFT_NBITS, DFT_R2C);
	ctx.rdft_data     = (FFTSample *)CFav_malloc(FFT_WINSIZE * sizeof(*ctx.rdft_data));
	ctx.audio_buf     = (uint8_t *)CFav_malloc(AVCODEC_MAX_AUDIO_FRAME_SIZE);

	int counter = 0;
	/* Process packets */
	AVPacket pkt;
	//while (av_read_frame(fmt_ctx, &pkt) >= 0)
	while (CFav_read_frame(fmt_ctx, &pkt) >= 0)
	{
		counter++;
		if (pkt.stream_index == stream_id && process_audio_pkt(&ctx, &pkt) < 0)
		{
			printf("error while processing audio packet");
		}
		if(counter == 4)
			break;
	}

	//if (ctx.filled) // flush buffer
	//{
	//	memset((uint8_t*)ctx.samples + ctx.filled, 0, ctx.samples_bsize - ctx.filled);
	//	process_samples(&ctx);
	//}

	return 0;
}
bool AudioSpectrum::LoadAudioDLL(){
	m_hAvformatModule = NULL;
	m_hAvformatModule = LoadLibrary("avformat-55.dll");
	if(m_hAvformatModule == NULL)
	{
		printf("Load LoadAudioDLL DLL fail");
		return false;
	}

	CFav_find_stream_info = (PFav_find_stream_info)GetProcAddress(m_hAvformatModule, "av_find_stream_info");
	if(CFav_find_stream_info == NULL)
	{
		return false;
	}

	CFavformat_open_input = (PFavformat_open_input)GetProcAddress(m_hAvformatModule, "avformat_open_input");
	if(CFavformat_open_input == NULL)
	{
		return false;
	}


	CFav_read_frame = (PFav_read_frame)GetProcAddress(m_hAvformatModule, "av_read_frame");
	if(CFav_read_frame == NULL)
	{
		return false;
	}

	CFav_register_all = (PFav_register_all)GetProcAddress(m_hAvformatModule, "av_register_all");
	if(CFav_register_all == NULL)
	{
		return false;
	}

	CFav_close_input_file = (PFav_close_input_file)GetProcAddress(m_hAvformatModule, "av_close_input_file");
	if(CFav_close_input_file == NULL)
	{
		return false;
	}


	/******************************************/

	m_hAvcodecModule = LoadLibrary("avcodec-55.dll");
	if(m_hAvcodecModule == NULL)
	{
		printf("Load LoadAudioDLL DLL fail");
		return false;
	}

	CFavcodec_open = (PFavcodec_open)GetProcAddress(m_hAvcodecModule, "avcodec_open");
	if(CFavcodec_open == NULL)
	{
		return false;
	}

	CFavcodec_find_decoder = (PFavcodec_find_decoder)GetProcAddress(m_hAvcodecModule, "avcodec_find_decoder");
	if(CFavcodec_find_decoder == NULL)
	{
		return false;
	}

	CFav_rdft_init = (PFav_rdft_init)GetProcAddress(m_hAvcodecModule, "avcodec_find_decoder");
	if(CFav_rdft_init == NULL)
	{
		return false;
	}

	CFavcodec_decode_audio3 = (PFavcodec_decode_audio3)GetProcAddress(m_hAvcodecModule, "avcodec_decode_audio3");
	if(CFavcodec_decode_audio3 == NULL)
	{
		return false;
	}

	CFav_rdft_end = (PFav_rdft_end)GetProcAddress(m_hAvcodecModule, "av_rdft_end");
	if(CFav_rdft_end == NULL)
	{
		return false;
	}

	CFavcodec_close = (PFavcodec_close)GetProcAddress(m_hAvcodecModule, "avcodec_close");
	if(CFavcodec_close == NULL)
	{
		return false;
	}




	/****************************************/
	m_hAvutilModule = LoadLibrary("avutil-52.dll");
	if(m_hAvutilModule == NULL)
	{
		printf("Load LoadAudioDLL DLL fail");
		return false;
	}

	CFav_malloc = (PFav_malloc)GetProcAddress(m_hAvutilModule, "av_malloc");
	if(CFav_malloc == NULL)
	{
		return false;
	}

	CFav_free = (PFav_free)GetProcAddress(m_hAvutilModule, "av_free");
	if(CFav_free == NULL)
	{
		return false;
	}


	return true;
}

//Anderson
int AudioSpectrum::Save_To_txt(bool bToPNG)
{
    char cTmp[64];
	m_Gunplot_output="";

	std::string m_strSpectrumPath =m_strSpectrum + "AudioSpectrum.txt";
	ofstream outputfile;
	outputfile.open(m_strSpectrumPath.c_str());

	if (bToPNG)
	{
		m_Gunplot_output.append("set terminal png\n") ;
		m_Gunplot_output.append("set output \"output.png\" \n");
	}

	m_Gunplot_output.append("set style data lines\n");
	m_Gunplot_output.append("set xtics 0, 1 \n");
	m_Gunplot_output.append("set grid  \n");
	m_Gunplot_output.append("set yrange [-140:40]\n");
	m_Gunplot_output.append("set xlabel 'Frequency (kHz)'\n");
	m_Gunplot_output.append("set ylabel 'Log magnitude (dB)'\n");

    outputfile<<m_Gunplot_output;

	sprintf_s (cTmp,64,"set xrange [0:%f]\n", 44.1f/4.f);
	outputfile<< cTmp ;

	sprintf_s (cTmp,64,"plot '-' using ($0/%d.*%f):1 notitle\n", FFT_WINSIZE, 44.1f);
	outputfile<< cTmp;


	for (int i = 0; i < sizeof(ctx.fft)/sizeof(*ctx.fft); i++)
	{
		float x = ctx.fft[i] / ctx.win_count / FFT_WINSIZE;
		ctx.fft[i] = static_cast<float>(10 * log10(x ? x : -1e20));

		sprintf_s (cTmp,64,"%f\n", ctx.fft[i]);

		outputfile << ctx.fft[i] << endl;

	}

	outputfile.close();
	return 0;
}

int AudioSpectrum::renametxt()
{
	char szOldname[MAX_PATH] = {0};
	char szNewName[MAX_PATH] = {0};

	m_strSpectrum = m_strSpectrum + "www.txt";
	strcpy(szOldname, m_strSpectrum.c_str());

	SYSTEMTIME now;
	GetLocalTime (&now);
	::sprintf_s(szNewName, MAX_PATH, "%d%d%d_%02d%02d%02d.txt",
		/*m_strPicasso.c_str(),*/
		now.wYear,
		now.wMonth,
		now.wDay,
		now.wHour,
		now.wMinute,
		now.wSecond);

	int int_ret = ::rename(szOldname, szNewName);

	return 0;
}

int AudioSpectrum::Generate_Gnuplot( bool bToPNG )
{
	m_Gunplot_output="";
	//m_Gunplot_output.resize(2048);

	char cTmp[64];

	if (bToPNG)
	{
		m_Gunplot_output.append("set terminal png\n");
		m_Gunplot_output.append("set output \"output.png\" \n");
	}

	m_Gunplot_output.append("set style data lines\n");
	m_Gunplot_output.append("set xtics 0, 1 \n");
	m_Gunplot_output.append("set grid  \n");
	m_Gunplot_output.append("set yrange [-140:40]\n");
	m_Gunplot_output.append("set xlabel 'Frequency (kHz)'\n");
	m_Gunplot_output.append("set ylabel 'Log magnitude (dB)'\n");

	sprintf_s (cTmp,64,"set xrange [0:%f]\n", 44.1f/4.f);
	m_Gunplot_output.append(cTmp);

	sprintf_s (cTmp,64,"plot '-' using ($0/%d.*%f):1 notitle\n", FFT_WINSIZE, 44.1f);
	m_Gunplot_output.append(cTmp);



	for (int i = 0; i < sizeof(ctx.fft)/sizeof(*ctx.fft); i++)
	{
		float x = ctx.fft[i] / ctx.win_count / FFT_WINSIZE;
		ctx.fft[i] = static_cast<float>(10 * log10(x ? x : -1e20));

		sprintf_s (cTmp,64,"%f\n", ctx.fft[i]);
		m_Gunplot_output.append(cTmp);
	}

	return 0;
}

void AudioSpectrum::precalc_hann(void)
{
    for (int i = 0; i < FFT_WINSIZE; i++)
        hann[i] = static_cast<float>(.5f * (1 - cos(2*M_PI*i / (FFT_WINSIZE-1))));
}

void AudioSpectrum::process_samples(struct las *c)
{
	fft_audacity_warp *pFFT = new fft_audacity_warp(SAMPLE_COUNT , FFT_NBITS);
	string output = m_strSpectrum +"AudioSpectrum.txt";

	c->win_count++;
    c->filled = 0;

    // Resample (int16_t to float), downmix if necessary and apply Hann window
    int16_t *s16 = c->samples;
    switch (c->codec->channels)
	{
    case 2:

		if ( CH_LR == m_channel)
		{
			for (int i = 0; i < SAMPLE_COUNT; i++)
				//c->rdft_data[i] = (s16[i*2] + s16[i*2+1]) / (2*32768.f) * hann[i];
			//{
			//	INT16 a= s16[i*2];
			//	INT16 b=  s16[i*2+1];
			pFFT->m_data[i]= (s16[i*2] + s16[i*2+1]) / (2*32768.f);
			//}
		}

		if( CH_L == m_channel)
		{
			for (int i = 0; i < SAMPLE_COUNT; i++)
				//c->rdft_data[i] = (s16[i*2] ) / (32768.f) * hann[i];
				pFFT->m_data[i] = (s16[i*2] ) / (32768.f);
		}

		if( CH_R == m_channel)
		{
			for (int i = 0; i < SAMPLE_COUNT; i++)
				//c->rdft_data[i] = (s16[i*2+1] ) / (32768.f) * hann[i];
                pFFT->m_data[i]= (s16[i*2+1] ) / (32768.f);
		}
        break;

    case 1:
        for (int i = 0; i < SAMPLE_COUNT; i++)
            pFFT->m_data[i] = s16[i] / 32768.f;
        break;
    }

	pFFT->Calc();
	pFFT->DoRMS();
	pFFT->dump_done_buff_for_draw(output.c_str());

	//pFFT->dump_done_buff("tt.txt");

    // FFT
//#define FFT_ASSIGN_VALUES(i, re, im) c->fft[i] += re*re + im*im
//    float *bin = c->rdft_data;
//    av_rdft_calc(c->rdft, bin);
//    for (int i = 1; i < FFT_WINSIZE/2; i++)
//        FFT_ASSIGN_VALUES(i, bin[i*2], bin[i*2+1]);
//    FFT_ASSIGN_VALUES(0,             bin[0], 0);
//    FFT_ASSIGN_VALUES(FFT_WINSIZE/2, bin[1], 0);
}

int AudioSpectrum::process_audio_pkt(struct las *c, AVPacket *pkt)
{
	while (pkt->size > 0)
	{
        uint8_t *data = c->audio_buf;
        int data_size = AVCODEC_MAX_AUDIO_FRAME_SIZE;
       // int len       = avcodec_decode_audio3(c->codec, (int16_t*)data, &data_size, pkt);
		int len       = CFavcodec_decode_audio3(c->codec, (int16_t*)data, &data_size, pkt);
        if (len < 0) {
            pkt->size = 0;
            return -1;
        }
        pkt->data += len;
        pkt->size -= len;
        //if ( data_size > 0)  //Anderson ADD

        while (data_size > 0)
		{
            int needed = c->samples_bsize - c->filled; // in bytes
            int ncpy   = data_size >= needed ? needed : data_size;
            memcpy((uint8_t*)c->samples + c->filled, data, ncpy);
            c->filled += ncpy;
            if (c->filled != c->samples_bsize)
                break;
			process_samples(c);
			data      += ncpy;
            data_size -= ncpy;
        }
	}

    return 0;
}

//Anderson
bool AudioSpectrum::setFilepath(std::string filepath)
{
    this->filepath_new = filepath;
	return true;
}

AudioSpectrum::~AudioSpectrum()
{
	if (m_hAvformatModule != NULL)
	{
		FreeLibrary (m_hAvformatModule);
		m_hAvformatModule = NULL;
	}
	if (m_hAvcodecModule != NULL)
	{
		FreeLibrary (m_hAvcodecModule);
		m_hAvcodecModule = NULL;
	}
	if (m_hAvutilModule != NULL)
	{
		FreeLibrary (m_hAvutilModule);
		m_hAvutilModule = NULL;
	}

	/* Cleanup */
	//av_free(ctx.audio_buf);
	//av_free(ctx.rdft_data);
	//av_free(ctx.samples);

	//av_rdft_end(ctx.rdft);
	//avcodec_close(ctx.codec);
	//av_close_input_file(fmt_ctx);

	//CFav_free(ctx.audio_buf);
	//CFav_free(ctx.rdft_data);
	//CFav_free(ctx.samples);

	//CFav_rdft_end(ctx.rdft);
	//CFavcodec_close(ctx.codec);
	//CFav_close_input_file(fmt_ctx);
}
