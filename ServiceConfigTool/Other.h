#pragma once
#include "PageBaseUI.h"



class COtherUI :public CPageBaseUI
{
public:
	COtherUI();
	~COtherUI();
	void Notify(TNotifyUI &msg);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Close(UINT nRet = 1);


private:
};