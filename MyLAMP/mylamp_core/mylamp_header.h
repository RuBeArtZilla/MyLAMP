#pragma once

#ifdef MYLAMP_HEADER_EXPORT
#define MYLAMP_HEADER_API extern "C" __declspec(dllexport) 
#else
#define MYLAMP_HEADER_API extern "C" __declspec(dllimport) 
#endif


namespace mylamp
{
	struct COMPONENT_INFO
	{
		UINT64			id;
		UINT64			version;
	};

	class Component
	{
	public:
		virtual bool IsInit() = 0;
		virtual bool IsLoad() = 0;

		virtual ~Component(){};
		virtual bool Load() = 0;
		virtual COMPONENT_INFO GetInfo() = 0;
		virtual UINT64 GetCoreMinVersion() = 0;
		virtual INT_PTR CALLBACK SettingsWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
	};
	
	MYLAMP_HEADER_API Component* RegComponent(); 
	MYLAMP_HEADER_API void FreeComponent(); 

}