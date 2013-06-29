#pragma once

#ifdef MYLAMP_HEADER_EXPORT
#define MYLAMP_HEADER_API __declspec(dllexport) 
#else
#define MYLAMP_HEADER_API __declspec(dllimport) 
#endif


namespace mylamp
{

#define TEST_FILLER		0

	struct COMPONENT_INFO
	{
		UINT64			id;
		UINT64			version;
	};

	class component
	{
	public:
		virtual ~component(){};
		virtual COMPONENT_INFO GetInfo() = 0;
		virtual UINT64 GetCoreMinVersion() = 0;
	};
	
	MYLAMP_HEADER_API component* RegComponent(); 
}