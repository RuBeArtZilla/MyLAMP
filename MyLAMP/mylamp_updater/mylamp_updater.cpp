// mylamp_updater.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

class Updater: public mylamp::component
{
public:
	~Updater(){};
	mylamp::COMPONENT_INFO GetInfo()
	{
		mylamp::COMPONENT_INFO ciResult = {1, 0};
		return ciResult;
	};
	UINT64 GetCoreMinVersion(){return 1;};
};

MYLAMP_HEADER_API mylamp::component* RegComponent()
{
	Updater U;
	return &U;
}; 