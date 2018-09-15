#include "MsgTip.h"
#define SHOWTIMER 10080
CMsgTip::~CMsgTip()
{
}

void CMsgTip::MsgLoop()
{
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CMsgTip::MessageHandle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CMsgTip *pThis = NULL;
	if (uMsg != WM_CREATE)
	{
		pThis = (CMsgTip*)GetWindowLong(hWnd, GWLP_USERDATA);
	}

	switch (uMsg)
	{
	case WM_CREATE:
	{
		if (!pThis)
			break;

		SendMessage(hWnd, WM_SETFONT, (WPARAM)pThis->m_hFont, 0);
		break;
	}
	case WM_TIMER:
	{
		if (wParam == SHOWTIMER)
		{
			if (!pThis)
				break;

			pThis->m_nShowTime -= 100;
			if (pThis->m_nShowTime <= 500)
			{
				int newAlpha = pThis->m_nAlpha - 0xFF / 5;
				pThis->m_nAlpha = newAlpha > 0 ? newAlpha : 0;
				SendMessage(hWnd, WM_PAINT, 0, 0);
			}
			if (pThis->m_nShowTime <= 0)
			{
				KillTimer(hWnd, SHOWTIMER);
				DestroyWindow(hWnd);
			}
		}
		break;
	}
	case WM_PAINT:
	{
		if (!pThis)
			break;

		pThis->Paint(hWnd, pThis->m_strMsg.c_str());
		break;
	}
	case WM_DESTROY:
	{
		if (!pThis)
			break;

		//DeleteObject(pThis->m_hFont);
		PostQuitMessage(0);
		break;
	}
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

SIZE CMsgTip::GetTextSize(wstring szText)
{
	HDC hdc = GetDC(NULL);
	HDC hDc = CreateCompatibleDC(hdc);
	HFONT hOldFont = (HFONT)::SelectObject(hDc, m_hFont);
	//::GetTextExtentPoint32(hDc, szText.c_str(), szText.length(), &m_cSize);

	RECT rc = { 0,0,240,0 };
	DrawText(hDc, szText.c_str(), szText.length(), &rc, DT_CALCRECT | DT_LEFT | DT_WORDBREAK | DT_NOPREFIX | DT_EDITCONTROL);
	::SelectObject(hDc, hOldFont);
	//::DeleteObject(m_hFont);
	DeleteDC(hDc);
	ReleaseDC(NULL, hdc);


	m_cSize.cx = rc.right;
	m_cSize.cy = rc.bottom;
	return m_cSize;
}

void CMsgTip::Paint(HWND hWnd, wstring strMsg)
{
	RECT rc;
	GetClientRect(hWnd, &rc);

	HDC hdcScreen = GetDC(hWnd);
	HDC hdcMem = CreateCompatibleDC(hdcScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, rc.right, rc.bottom);
	HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMem, hBitmap);

	HBRUSH hBrush = CreateSolidBrush(RGB(88, 88, 88));
	FillRect(hdcMem, &rc, hBrush);
	SetTextColor(hdcMem, RGB(255, 255, 255));
	SetBkMode(hdcMem, TRANSPARENT);
	RECT src = rc;
	InflateRect(&src, -5, -5);
	HFONT hOldFont = (HFONT)::SelectObject(hdcMem, m_hFont);
	DrawText(hdcMem, strMsg.c_str(), strMsg.length(), &src, DT_NOPREFIX | DT_EDITCONTROL | DT_CENTER | DT_WORDBREAK);

	BLENDFUNCTION blend = { 0 };
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = m_nAlpha;
	blend.AlphaFormat = AC_SRC_OVER;	//按通道混合会导致黑色被当透明处理
	POINT	pSrc = { 0, 0 };
	SIZE	sizeWnd = { rc.right, rc.bottom };
	UpdateLayeredWindow(hWnd, hdcScreen, NULL, &sizeWnd, hdcMem, &pSrc, RGB(255, 255, 255), &blend, ULW_ALPHA);		//更新分层窗口


																													//资源释放
	SelectObject(hdcMem, hOldFont);
	SelectObject(hdcMem, hBitmapOld);
	DeleteObject(hBitmap);
	DeleteDC(hdcMem);
	ReleaseDC(hWnd, hdcScreen);
}

void CMsgTip::ShowMsgTip(HWND hParentWnd, HFONT hFont, wstring lpszMsg, UINT nShowTime)
{
	CMsgTip *pWnd = new CMsgTip(hFont);
	pWnd->m_strMsg = lpszMsg;
	pWnd->m_nShowTime = nShowTime * 1000;
	thread th([=] {
		WNDCLASSEX wcex = { 0 };
		wcex.cbSize = sizeof(wcex);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.hInstance = GetModuleHandle(NULL);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.lpfnWndProc = CMsgTip::MessageHandle;
		wcex.lpszClassName = TEXT("MsgTip");
		wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
		RegisterClassEx(&wcex);

		SIZE cSize = pWnd->GetTextSize(pWnd->m_strMsg);
		cSize.cx += 10;
		cSize.cy += 10;

		RECT rc;
		GetClientRect(hParentWnd, &rc);
		POINT pos = { rc.right / 2,  rc.bottom / 2 };
		ClientToScreen(hParentWnd, &pos);
		pos.x = pos.x - cSize.cx / 2;
		pos.y = pos.y - cSize.cy / 2 + 10;
		HWND hWnd = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_LAYERED,
			TEXT("MsgTip"), NULL, WS_POPUP, pos.x, pos.y, cSize.cx, cSize.cy, NULL, NULL, GetModuleHandle(NULL), NULL);
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)pWnd);

		HRGN hRgn = CreateRoundRectRgn(0, 0, cSize.cx, cSize.cy, 5, 5);
		SetWindowRgn(hWnd, hRgn, TRUE);
		ShowWindow(hWnd, SW_SHOW);
		SetTimer(hWnd, SHOWTIMER, 100, NULL);
		DeleteObject(hRgn);
		pWnd->Paint(hWnd, pWnd->m_strMsg);
		pWnd->MsgLoop();
		delete pWnd;
	});
	th.detach();
}

CMsgTip::CMsgTip(HFONT hFont)
{
	m_nAlpha = 0xFF;
	m_hFont = hFont;
}
