#include "Other.h"

COtherUI::COtherUI()
{
}

COtherUI::~COtherUI()
{

}

void COtherUI::Notify(TNotifyUI & msg)
{
	return __super::Notify(msg);
}

LRESULT COtherUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg,wParam,lParam);
}

void COtherUI::Close(UINT nRet)
{
	return __super::Close(nRet);
}
