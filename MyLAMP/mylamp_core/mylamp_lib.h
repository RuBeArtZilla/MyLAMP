#pragma once

using namespace mylamp;

typedef mylamp::Component* (*_RegComponent)();
typedef void (*_FreeComponent)();

#define COMPONENTS_PATH			L"components\\"
#define COMPONENTS_PATH_FILTER	L"*.dll"

StringVector GetComponentNames(tstring tsPath);

class Components: public Component
{
private:
	DllDetailVector DDV;
	WndProcVector v_WndProc;
	bool b_init;
	bool b_load;

public:
	Components();
	virtual bool IsInit(){return isLoad();};
	virtual bool IsLoad(){return isLoad();};

	virtual	bool Load(Component* pParent = 0);
	virtual ~Components();
	virtual COMPONENT_INFO GetInfo(){COMPONENT_INFO ciResult = {1, 0}; return ciResult;};
	virtual UINT64 GetCoreMinVersion(){return 1;};	
	
	virtual settings_items GetSettingsItems(){settings_items siResult; return siResult;};
	virtual bool CheckSelectedItem(StringVector svReversedItem){return false;};
	
	virtual INT_PTR CALLBACK SettingsWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){return (INT_PTR)FALSE;};

	virtual bool AddWndProc(_WndProc wndproc);
	virtual bool DelWndProc(_WndProc wndproc);
	
	bool isLoad();
	void Unload();
	DllDetailVector* getDetailVector(){return &DDV;};
	WndProcVector*	 getWndProcVector(){return &v_WndProc;};
};

Components* GetComponents();
void		SetComponents(Components* pComponents);

DllDetailVector GetDetailVector();
