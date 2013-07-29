#pragma once

#include "stdafx.h"
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifdef  UNICODE 
typedef std::wstring tstring;
#else /* UNICODE */
typedef std::string tstring;
#endif

#ifdef MYLAMP_HEADER_EXPORT
#define MYLAMP_HEADER_API extern "C" __declspec(dllexport) 
#else
#define MYLAMP_HEADER_API extern "C" __declspec(dllimport) 
#endif

#define WM_COMPONENT_SELECTED (WM_APP + 0x0001)
#define WM_COMPONENT_CHANGED (WM_APP + 0x0002)
#define WM_COMPONENT_UNSELECTED (WM_APP + 0x0003) // WPARAM - ptr to Old Component, LPARAM - ptr to new Component

#define MAX_LOADSTRING					100
#define SETTINGS_WINDOW_HEIGHT			450
#define SETTINGS_WINDOW_WIDTH			600
#define SETTINGS_WINDOW_TV_WIDTH		200
#define SETTINGS_WINDOW_OFFSET			12
#define SETTINGS_WINDOW_BUTTON_HEIGHT	32

typedef std::vector<tstring> StringVector;
typedef std::vector<tstring>::iterator StringVectorIterator;

typedef INT_PTR (*_WndProc)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
typedef std::vector<_WndProc> WndProcVector;
typedef std::vector<_WndProc>::iterator WndProcIterator;

struct settings_items
{
	StringVector path;
	StringVector items;
};

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
		virtual bool Load(Component* pParent = 0) = 0;
		virtual COMPONENT_INFO GetInfo() = 0;
		virtual UINT64 GetCoreMinVersion() = 0;

		virtual settings_items GetSettingsItems() = 0; 
		virtual bool CheckSelectedItem(StringVector svReversedItem) = 0;
		virtual INT_PTR CALLBACK SettingsWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
		virtual bool AddWndProc(_WndProc wndproc) = 0;
		virtual bool DelWndProc(_WndProc wndproc) = 0;
	};

	MYLAMP_HEADER_API Component* RegComponent(); 
	MYLAMP_HEADER_API void FreeComponent(); 

	typedef std::vector<Component*> ComponentVector;
	typedef std::vector<Component*>::iterator ComponentVectorIterator;

	struct dll_detail
	{
		tstring svName;
		HMODULE hModule;
		mylamp::Component* pComponent;
	};

	typedef std::vector<dll_detail> DllDetailVector;
	typedef std::vector<dll_detail>::iterator DllDetailIterator;
}