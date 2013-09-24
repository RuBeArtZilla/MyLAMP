#include "stdafx.h"
#include "mylamp_lib.h"

static Components* _pComponents = 0;

Components*		GetComponents(){return _pComponents;};
void			SetComponents(Components* pComponents){_pComponents = pComponents;};
DllDetailVector GetDetailVector(){return *_pComponents->getDetailVector();};

//---------------------------------------------------------------------------
Components::Components()
{
	Load();
}

//---------------------------------------------------------------------------
Components::~Components()
{
	Unload();
}

//---------------------------------------------------------------------------
StringVector GetComponentNames(tstring tsPath)
{
	tstring tsFindPath = COMPONENTS_PATH_FILTER;
	tsFindPath.insert(tsFindPath.begin(), tsPath.begin(), tsPath.end());
	StringVector svResult;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFile = FindFirstFile(tsFindPath.c_str(), &FindFileData);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			svResult.push_back(FindFileData.cFileName);
			(--(svResult.end()))->insert(0, tsPath.c_str());//BAD: rewrite this section
		}
		while ( FindNextFile(hFile, &FindFileData) );
		FindClose(hFile);
	}
	return svResult;
}

//---------------------------------------------------------------------------
bool Components::Load(Component* pParent)
{
	dll_detail ddTemp;

	StringVector svNames = GetComponentNames(COMPONENTS_PATH);

	if (svNames.empty())
		return false;

	StringVectorIterator svIterator = svNames.begin();

	while (svIterator != svNames.end())
	{
		ddTemp.hModule = LoadLibrary(svIterator->c_str());
		if (ddTemp.hModule)
		{
			_RegComponent pRegComponent = (_RegComponent) GetProcAddress(ddTemp.hModule, "RegComponent"); 
			if (pRegComponent)
			{
				ddTemp.pComponent = pRegComponent();
				if (ddTemp.pComponent)
				{
					ddTemp.svName = *svIterator;
					if (ddTemp.pComponent->Load(this))
						DDV.push_back(ddTemp);
					else
						FreeLibrary(ddTemp.hModule);
				}
				else
					FreeLibrary(ddTemp.hModule);
			}
			else 
				FreeLibrary(ddTemp.hModule);
		}
		svIterator++;
	}

	return !DDV.empty();
}

//---------------------------------------------------------------------------
void Components::Unload()
{
	if (isLoad())
	{
		DllDetailIterator ddiIterator = DDV.begin();

		do
		{
			_FreeComponent pFreeComponent = (_FreeComponent) GetProcAddress(ddiIterator->hModule, "FreeComponent");
			
			if (pFreeComponent)
				pFreeComponent();
			else
			{
				delete ddiIterator->pComponent;
				ddiIterator->pComponent = NULL;
			}

			FreeLibrary(ddiIterator->hModule);

			ddiIterator++;
		}
		while (ddiIterator != DDV.end());
	}
}


//---------------------------------------------------------------------------
bool Components::AddWndProc(_WndProc wndproc)
{
	//TODO: check input
	if (wndproc)
		v_WndProc.push_back(wndproc);

	return false;
}

//---------------------------------------------------------------------------
bool Components::DelWndProc(_WndProc wndproc)
{
	//TODO: test this function
	WndProcIterator wpiIterator = v_WndProc.begin();

	if (!v_WndProc.empty())
	{
		do
		{
			if (*wpiIterator == wndproc)
			{
				v_WndProc.erase(wpiIterator);
				return true;
			}

			wpiIterator++;
		}
		while (wpiIterator != v_WndProc.end());
	}
	return false;
}

//---------------------------------------------------------------------------
bool Components::isLoad()
{
	return !DDV.empty();
}