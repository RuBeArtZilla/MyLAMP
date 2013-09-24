#include "stdafx.h"
#include "mlp_settings_connector.h"

bool stat_ss::saveSetting(tstring tsComponent, tstring tsName, tstring tsData)
{
	return SaveSetting(tsComponent, tsName, tsData);
}

bool RCreateDirectory(tstring tsPathTo)//WARNING: Not tested.
{
	tstring		tsTemp(tsPathTo);
	
	//Code below don't help to resolve a problem with
	// wrong directory for config files.
	
	/*DWORD		nBufferLength	= 0;
	LPTSTR		lpBuffer		= NULL;

	nBufferLength = GetCurrentDirectory(0, NULL);
	
	lpBuffer = new TCHAR[nBufferLength + 1];
	int nNewBufferLength = GetCurrentDirectory(nBufferLength, lpBuffer);
	lpBuffer[nNewBufferLength] = L'\\';
	lpBuffer[nNewBufferLength + 1] = 0;

	tsTemp.insert(0, lpBuffer, nNewBufferLength+1);//BAD: rewrite

	delete [] lpBuffer;
	lpBuffer = NULL;*/

	int iPos = 0;
	while(!CreateDirectory(tsTemp.c_str(), NULL))
	{
		iPos = tsTemp.find_last_of(TEXT("\\"));
		
		if (iPos <= 1)
			return false;
		
		tsTemp.erase(iPos, tsTemp.length() - iPos);
	}
	return true;
}

bool SaveSetting(tstring tsComponent, tstring tsName, tstring tsData)
{
	tstring tsFile;
	RCreateDirectory(DEFAULT_SETTINGS_SAVE_PATH);
	tsFile += DEFAULT_SETTINGS_SAVE_PATH;
	tsFile += (tsComponent.empty()) ? DEFAULT_SETTINGS_SAVE_FILENAME : tsComponent;
	tsFile += DEFAULT_SETTINGS_SAVE_FILEEXT;
	std::wfstream file(tsFile,  std::ios::out | std::ios::in | std::ios::binary | std::ios::trunc);
	
	if (file.is_open())
	{
		//TODO: add code here...
		
		file << tsName.c_str() << DSS_SEPARATOR << tsData.c_str() << L"\n";
		file.close();
		
		return true;
	}
	else 
		return false;
}

bool SaveSetting(tstring tsComponent, SettingsItemVector sivItem)
{
	//TODO: add code here...

	return false;
}

tstring LoadSetting(tstring tsName, tstring tsComponent)
{
	//TODO: add code here...

	return tstring();
}
