#include "ServiceConfig.h"

CServiceConfigUI::CServiceConfigUI()
{
	m_pService = static_cast<CEditUI*>(m_pm.FindControl(TEXT("service")));
	m_pPort = static_cast<CEditUI*>(m_pm.FindControl(TEXT("port")));
	m_pSql = static_cast<CEditUI*>(m_pm.FindControl(TEXT("sql")));

	m_pUserName = static_cast<CEditUI*>(m_pm.FindControl(TEXT("username")));
	m_pPassWord = static_cast<CEditUI*>(m_pm.FindControl(TEXT("password")));

	char szPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, szPath, MAX_PATH);
	m_strFilePath = szPath;
	size_t pos = m_strFilePath.rfind('\\');
	m_strFilePath.erase(pos);
	m_strFilePath += FILE_NAME;
	
	LOG("m_strFileName=%s\n", m_strFilePath.c_str());
}

CServiceConfigUI::~CServiceConfigUI()
{

}

void CServiceConfigUI::Notify(TNotifyUI & msg)
{
	if(msg.sType== DUI_MSGTYPE_CLICK)
	{ 
		if (msg.pSender->GetName() == TEXT("save"))
		{
			this->SaveConfig();
			CMsgTip::ShowMsgTip(GetParent(this->GetHWND()),m_pm.GetFont(4),L"±£´æ³É¹¦",2);
		}
	}
}

LRESULT CServiceConfigUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg,wParam,lParam);
}

void CServiceConfigUI::InitUI(int nShowID, LPVOID lpParam)
{
	this->SetEditContent();
}

void CServiceConfigUI::Close(UINT nRet)
{
	__super::Close(nRet);
}

void CServiceConfigUI::SaveConfig()
{
	string strJson=this->GetEditContent();
	fstream file;
	file.open(m_strFilePath.c_str(), ios::out | ios::binary);
	if (file.is_open())
	{
		file.write(&strJson[0],strJson.size());
		file.close();
	}
	else
	{
		LOG("file %s open failed\n", m_strFilePath.c_str());
	}
	return;
}

string CServiceConfigUI::W2A(const wstring &wstrcode)
{
	string str(wstrcode.length() * 3, ' ');

	int len = WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, &str[0], str.length(), NULL, NULL);

	str.resize(len - 1);
	return str;
}

wstring CServiceConfigUI::A2W(const string & AsciiStr)
{
	wstring str(AsciiStr.length() * 3, ' ');

	int len = MultiByteToWideChar(CP_ACP, 0, AsciiStr.c_str(), -1, &str[0], str.length());

	str.resize(len - 1);
	return str;
}

string CServiceConfigUI::GetEditContent()
{
	 m_strService = W2A(m_pService->GetText().GetData());
	 m_strPort =W2A(m_pPort->GetText().GetData());
	 m_strSql = W2A(m_pSql->GetText().GetData());

	m_strUserName = W2A(m_pUserName->GetText().GetData());
	m_strPassWord = W2A(m_pPassWord->GetText().GetData());


	Json::Value root;
	root["host"] = m_strService;
	root["port"] = atoi(m_strPort.c_str());
	root["name"] = m_strSql;
	root["user"] = m_strUserName;
	root["password"] =m_strPassWord;

	ifstream in;
	stringstream stream;
	in.open(m_strFilePath, ifstream::binary | ifstream::in);
	if (in.is_open())
	{
		stream << in.rdbuf();
		in.close();

	}
	Json::Reader reader;
	Json::Value jInfo;
	if (reader.parse(stream.str(), jInfo))
	{
		jInfo["db"] = root;
		Json::StyledWriter sw;
		string strEditContent = sw.write(jInfo);
		return strEditContent;
	}
}

void CServiceConfigUI::SetEditContent()
{
	ifstream in;
	in.open(m_strFilePath, ifstream::binary | ifstream::in);
	if(in.is_open())
	{
		stringstream stream;
		stream << in.rdbuf();
		in.close();
		Json::Reader reader;
		Json::Value jInfo;
		if (!reader.parse(stream.str(), jInfo))
		{
			LOG("parse Json failed\n");
			return;
		}
		else
		{
			Json::Value jDbInfo = jInfo["db"];
			
			if (!jDbInfo.isNull()&&jDbInfo.isObject())
			{
				m_strService = jDbInfo["host"].asString();
				stringstream stream;
				int nPort=0;
				if (jDbInfo["port"].isInt())
				{
					nPort = jDbInfo["port"].asInt();
				}
				else
					LOG("port is error\n");
				stream << nPort;
				m_strPort = stream.str();
				m_strSql = jDbInfo["name"].asString();
				m_strUserName = jDbInfo["user"].asString();
				m_strPassWord = jDbInfo["password"].asString();

				m_pService->SetText(A2W(m_strService).c_str());
				
				m_pPort->SetText(A2W(m_strPort).c_str());
				m_pSql->SetText(A2W(m_strSql).c_str());
				m_pUserName->SetText(A2W(m_strUserName).c_str());
				m_pPassWord->SetText(A2W(m_strPassWord).c_str());
			}

			
		}
	}
	else
	{
		LOG("Open file failed file is not exist Path=%s\n",m_strFilePath.c_str());
		return;
	}
}
