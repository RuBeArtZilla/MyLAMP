#include "stdafx.h"
#include "mylamp_lib.h"

static Components* _pComponents = 0;


Components* GetComponents(){return _pComponents;};
void		SetComponents(Components* pComponents){_pComponents = pComponents;};

Components::Components()
{
	Load();
}

Components::~Components()
{
	Unload();
}

StringVector GetComponentNames(tstring tsPath)
{
	StringVector svResult;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFile = FindFirstFile(tsPath.c_str(), &FindFileData);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			svResult.push_back(FindFileData.cFileName);
		}
		while ( FindNextFile(hFile, &FindFileData) );
		FindClose(hFile);
	}
	return svResult;
}

bool Components::Load()
{
	dll_detail ddTemp;

	StringVector svNames = GetComponentNames(TEXT("..\\debug\\*.dll"));
	
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
					if (ddTemp.pComponent->Load())
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
				delete ddiIterator->pComponent;

			FreeLibrary(ddiIterator->hModule);

			ddiIterator++;
		}
		while (ddiIterator != DDV.end());
	}
}

DllDetailVector* Components::getDetailVector()
{
	return &DDV;
}

bool Components::isLoad()
{
	return !DDV.empty();
}