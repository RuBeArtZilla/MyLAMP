#pragma once

using namespace mylamp;

#define DEFAULT_SETTINGS_SAVE_PATH		L"config\\"
#define DEFAULT_SETTINGS_SAVE_FILENAME	L"main"
#define DEFAULT_SETTINGS_SAVE_FILEEXT	L".cfg"
#define DSS_SEPARATOR					L"="

struct settings_item
{
	tstring name;
	tstring data;
};

typedef std::vector<settings_item> SettingsItemVector;
typedef std::vector<settings_item>::iterator SettingsItemIterator;

bool SaveSetting(tstring tsComponent, tstring tsName, tstring tsData);
bool SaveSetting(tstring tsComponent, SettingsItemVector sivItem);

class stat_ss
{
public:
	static bool saveSetting(tstring tsComponent, tstring tsName, tstring tsData);
};

tstring LoadSetting(tstring tsName, tstring tsComponent = tstring());