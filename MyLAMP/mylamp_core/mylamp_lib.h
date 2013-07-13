#pragma once

typedef mylamp::Component* (*_RegComponent)();
typedef void (*_FreeComponent)();

struct dll_detail
{
	tstring svName;
	HMODULE hModule;
	mylamp::Component* pComponent;
};

typedef std::vector<dll_detail> DllDetailVector;
typedef std::vector<dll_detail>::iterator DllDetailIterator;

StringVector GetComponentNames(tstring tsPath);

class Components
{
private:
	DllDetailVector DDV;

public:
	Components();
	~Components();
	bool isLoad();
	bool Load();
	void Unload();
	DllDetailVector* getDetailVector();
};

Components* GetComponents();
void		SetComponents(Components* pComponents);

DllDetailVector GetDetailVector();