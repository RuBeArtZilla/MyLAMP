#include "stdafx.h"
#include "ver_info.h"

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