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
				
/*
<16>	 The minimum block size (in samples) used in the stream.
<16>	 The maximum block size (in samples) used in the stream. (Minimum blocksize == maximum blocksize) implies a fixed-blocksize stream.
<24>	 The minimum frame size (in bytes) used in the stream. May be 0 to imply the value is not known.
<24>	 The maximum frame size (in bytes) used in the stream. May be 0 to imply the value is not known.
<20>	 Sample rate in Hz. Though 20 bits are available, the maximum sample rate is limited by the structure of frame headers to 655350Hz. Also, a value of 0 is invalid.
<3>	 (number of channels)-1. FLAC supports from 1 to 8 channels
<5>	 (bits per sample)-1. FLAC supports from 4 to 32 bits per sample. Currently the reference encoder and decoders only support up to 24 bits per sample.
<36>	 Total samples in stream. 'Samples' means inter-channel sample, i.e. one second of 44.1Khz audio will have 44100 samples regardless of the number of channels. A value of zero here means the number of total samples is unknown.
*/
				sFile.read(buf, remained_data);

				UINT16 min_block_size = (buf[0] << 8) | (buf[1]);
				UINT16 max_block_size = (buf[2] << 8) | (buf[3]);



				tstring test;
			}
		}

		sFile.close();	
	}
}