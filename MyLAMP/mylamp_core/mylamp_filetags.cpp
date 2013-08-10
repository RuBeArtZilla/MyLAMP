#include "stdafx.h"
#include "mylamp_filetags.h"
#include <iostream>
#include <fstream>

void test_code(tstring file);

#define FLAC_BEGIN_HEX		0x664c6143
#define FLAC_BEGIN_TEXT		"fLaC"
#define BUFFER_SIZE			256

struct METADATA_BLOCK_HEADER
{
	char block_type;
	char lenght[3];
};

#define LAST_METADATA_BLOCK_FLAG	0x80

#define MBH_STREAMINFO				0x00
#define MBH_PADDING					0x01
#define MBH_APPLICATION				0x02
#define MBH_SEEKTABLE				0x03
#define MBH_VORBIS_COMMENT			0x04
#define MBH_CUESHEET				0x05
#define MBH_PICTURE					0x06

struct METADATA_BLOCK_STREAMINFO
{

};

//---------------------------------------------------------------------------
void FileTags::GetFileTags(tstring file)
{
	test_code(file);
}

//---------------------------------------------------------------------------
void test_code(tstring file)
{
	if (!file.empty())	
	{
		std::fstream sFile;
		sFile.open (file, std::ios::in | std::ios::binary);

		char buf[BUFFER_SIZE] = { 0 };
		sFile.read(buf, 4);

		if (!strcmp(buf, FLAC_BEGIN_TEXT))
		{
			METADATA_BLOCK_HEADER mbhHead;
			sFile.read(buf, 4);
			
			if (buf[0] == MBH_STREAMINFO)
			{
				UINT32 header_lenght = 0x00ffffff & (buf[1] << 16) | (buf[2] << 8) | (buf[3]);
				UINT32 remained_data = header_lenght;

				sFile.read(buf, remained_data);

				UINT16 min_block_size = (buf[0] << 8) | (buf[1]);//<16> The minimum block size (in samples) used in the stream. 0-15 invalid.
				UINT16 max_block_size = (buf[2] << 8) | (buf[3]);//<16> The maximum block size (in samples) used in the stream. 0-15 invalid.

				bool fixed_block_size = (min_block_size == max_block_size);//Implies a fixed-blocksize stream.

				UINT32 min_frame_size = 0x00ffffff & (buf[4] << 16) | (buf[5] << 8) | (buf[6]);//<24> The minimum frame size (in bytes) used in the stream. May be 0 to imply the value is not known.
				UINT32 max_frame_size = 0x00ffffff & (buf[7] << 16) | (buf[8] << 8) | (buf[9]);//<24> The maximum frame size (in bytes) used in the stream. May be 0 to imply the value is not known.

				UINT32 sample_rate = (buf[10] << 12) | (0x00000fff & ((buf[11] << 4) | (0x0f & (buf[12] >> 4))));//<20> Sample rate in Hz. Though 20 bits are available, the maximum sample rate is limited by the structure of frame headers to 655350Hz. Also, a value of 0 is invalid.

				UINT16 channels_number = (0x07 & (buf[12] >> 1)) + 1;//<3> FLAC supports from 1 to 8 channels

				UINT16 bps = (0x1f & (0x10 & (buf[12] << 4)) | (0x0f & (buf[13] >> 4))) + 1;//<5> FLAC supports from 4 to 32 bits per sample. Currently the reference encoder and decoders only support up to 24 bits per sample.
				
				UINT64 samples = buf[17] | 0xff00&(buf[16]<<8) | 0xff0000&(buf[15]<<16) | 0xff000000&(buf[14]<<24) | 0x0f00000000&((0x0f&buf[13])<<32);//<36> Total samples in stream. 'Samples' means inter-channel sample, i.e. one second of 44.1Khz audio will have 44100 samples regardless of the number of channels. A value of zero here means the number of total samples is unknown.

				char md5[16] = { 0 };
				memcpy(md5, &buf[18], ( (sizeof (char)) * 16));

				tstring test;
			}
		}

		sFile.close();	
	}
}