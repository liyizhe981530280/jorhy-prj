#pragma once

#include "pl_singleton.h"
#include "pl_core.h"
#include "pl_lock.h"
#include "pl_player.h"
#include <map>
#include <string>

typedef int (*J_MakePlayerFun)(PlPlayer *&, int nWorkMode, void *pUser);
typedef int (*J_MakeWindowFun)(CWnd *&, HWND hParent,UINT nID);

class PL_API CPlFactoryWnd : public SingletonTmpl<CPlFactoryWnd>
{
	typedef std::map<std::string, J_MakeWindowFun> WindowRegistMap;
	typedef std::map<int, CWnd *> WindowMap;
public:
	CPlFactoryWnd() {}
	~CPlFactoryWnd() {}

public:
	int RegisterWindow(const char *wndType, J_MakeWindowFun pFun);
	CWnd *GetWindow(const char *pType, HWND hParent, UINT nId);
	void DelWindow(int nId);

private:
	WindowRegistMap m_wndRegistMap;
	WindowMap m_wndMap;
	PlLock m_lock;
};

///Window
#define WND_BEGIN_MAKER(type)\
	static int pl_##type =

#define WND_ENTER_MAKER(plType, maker)\
	CPlFactoryWnd::Instance()->RegisterWindow((plType), (maker))

#define WND_END_MAKER()\
	;