#include "stdafx.h"
#include "mylamp_lib.h"

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
	dll_detail ddTemp = { 0 };

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

void Components::UnLoad()
{
	if (isLoad())
	{
		DllDetailIterator ddiIterator = DDV.begin();

		do
		{
			delete ddiIterator->pComponent;
			FreeLibrary(ddiIterator->hModule);

			ddiIterator++;
		}
		while (ddiIterator != DDV.end());
	}
}

bool Components::isLoad()
{
	return !DDV.empty();
}