// mylamp_updater.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

mylamp::Component * pComponent;

class Updater: public mylamp::Component
{
public:
	virtual ~Updater(){};
	virtual mylamp::COMPONENT_INFO GetInfo(){mylamp::COMPONENT_INFO ciResult = {1, 0};return ciResult;};
	virtual UINT64 GetCoreMinVersion(){return 1;};
};

MYLAMP_HEADER_API mylamp::Component* RegComponent()
{
	pComponent = new Updater();
	return pComponent;
}; 

MYLAMP_HEADER_API void FreeComponent()
{
	delete pComponent;
}; 