#pragma once

using namespace mylamp;

typedef mylamp::Component* (*_RegComponent)();
typedef void (*_FreeComponent)();

StringVector GetComponentNames(tstring tsPath);

class Components: public Component
{
private:
	DllDetailVector DDV;
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

	bool isLoad();
	void Unload();
	DllDetailVector* getDetailVector();
};

Components* GetComponents();
void		SetComponents(Components* pComponents);

DllDetailVector GetDetailVector();


#define FI_FILE_DESCRIPTION		_T("FileDescription")
#define FI_COMPANY_NAME			_T("CompanyName")
#define FI_FILE_VERSION			_T("FileVersion")
#define FI_INTERNAL_NAME		_T("InternalName")
#define FI_LEGAL_COPYRIGHT		_T("LegalCopyright")
#define FI_LEGAL_TRADEMARKS		_T("LegalTradeMarks")
#define FI_ORIGINAL_FILENAME	_T("OriginalFilename")
#define FI_PRODUCT_NAME			_T("ProductName")
#define FI_PRODUCT_VERSION		_T("ProductVersion")
#define FI_COMMENTS				_T("Comments")
#define FI_AUTHOR				_T("Author")

tstring GetFileVersionInfo(tstring tsName, tstring tsParamName);