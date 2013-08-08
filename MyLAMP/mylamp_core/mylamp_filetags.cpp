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
				UINT32 remained_data;
				sFile.read(buf, 4);



				tstring test;
			}
		}

		sFile.close();	
	}
}