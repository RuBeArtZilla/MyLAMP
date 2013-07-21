#include "stdafx.h"
#include "mylamp_lib.h"

static Components* _pComponents = 0;

Components*		GetComponents(){return _pComponents;};
void			SetComponents(Components* pComponents){_pComponents = pComponents;};
DllDetailVector GetDetailVector(){return *_pComponents->getDetailVector();};

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

bool Components::Load(Component* pParent)
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

tstring GetFileVersionInfo(tstring tsName, tstring tsParamName)
{
	tstring tsResult;

	TCHAR paramNameBuf[256] = { 0 };
	TCHAR *paramValue = 0;
	UINT paramSz = 0;

	DWORD dwSize = GetFileVersionInfoSize(tsName.c_str(), NULL);
	if (dwSize)
	{
		LPVOID pBuf = new BYTE[dwSize];

		VS_FIXEDFILEINFO vsFFI = { 0 };
		
		struct LANGANDCODEPAGE {
			WORD wLanguage;
			WORD wCodePage;
		} *pLangCodePage;

		GetFileVersionInfo(tsName.c_str(), NULL, dwSize, pBuf);

		UINT cpSz = 0;

		if (VerQueryValue(pBuf, TEXT("\\VarFileInfo\\Translation"), (LPVOID*) &pLangCodePage, &cpSz))
		{
			for (int cpIdx = 0; cpIdx < (int)(cpSz / sizeof(struct LANGANDCODEPAGE)); cpIdx++ )
			{
				//TODO: maybe change _stprintf(...);
				_stprintf(paramNameBuf, _T("\\StringFileInfo\\%04x%04x\\%s"), pLangCodePage[cpIdx].wLanguage, pLangCodePage[cpIdx].wCodePage, tsParamName.c_str());

				if ( VerQueryValue(pBuf, paramNameBuf, (LPVOID*)&paramValue, &paramSz))
				{
					//TODO: Return result for all translation, not only first;
					return tstring(paramValue);
				}
			}	
		}

		delete pBuf;
	}
	return tsResult;
}