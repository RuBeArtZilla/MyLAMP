#pragma once

#ifdef  UNICODE 
typedef std::wstring tstring, TSTRING;
#else /* UNICODE */
typedef std::string tstring, TSTRING;
#endif

typedef std::vector<tstring> StringVector;
typedef std::vector<tstring>::iterator StringVectorIterator;
typedef std::vector<mylamp::Component*> ComponentVector;
typedef std::vector<mylamp::Component*>::iterator ComponentVectorIterator;

typedef mylamp::Component* (*_RegComponent)();

struct dll_detail
{
	tstring svName;
	HMODULE hModule;
	mylamp::Component* pComponent;
};

typedef std::vector<dll_detail> DllDetailVector;
typedef std::vector<dll_detail>::iterator DllDetailIterator;

StringVector GetComponentNames(tstring tsPath);

void FreeComponents(StringVector);

class Components
{
private:
	DllDetailVector DDV;

public:
	bool isLoad();
	bool Load();
	void UnLoad();
};