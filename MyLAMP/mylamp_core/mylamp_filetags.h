#pragma once

using namespace mylamp;

class FileTags: public Component
{
private:
	bool b_init;
	bool b_load;
	mylamp::Component* p_parent;

public:
	FileTags(){b_init = false; b_load = false;};
	virtual bool IsInit(){return b_init;};
	virtual bool IsLoad(){return b_load;};

	virtual	bool Load(Component* pParent = 0){p_parent = pParent; return (b_load = true);};
	virtual ~FileTags(){};
	virtual COMPONENT_INFO GetInfo(){COMPONENT_INFO ciResult = {1, 0}; return ciResult;};
	virtual UINT64 GetCoreMinVersion(){return 1;};	
	
	virtual settings_items GetSettingsItems(){settings_items siResult; return siResult;};
	virtual bool CheckSelectedItem(StringVector svReversedItem){return false;};
	
	virtual INT_PTR CALLBACK SettingsWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){return (INT_PTR)FALSE;};

	virtual bool AddWndProc(_WndProc wndproc){return false;};
	virtual bool DelWndProc(_WndProc wndproc){return false;};

	void GetFileTags(tstring file);
};