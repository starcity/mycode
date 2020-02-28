#include <ctime>
#include <string.h>
#include <cstdarg>
#include <stdio.h>

#include "Log.h"
using std::fstream;
using std::string;
using std::ios;
using std::streampos;

#ifdef		_WINDOWS
#define		LOGFILENAME			"D:\\virtualManage"
#else
#define		LOGFILENAME			"/var/log/httpServer"
#endif

// ���캯��  filepath��ʾ��־�ļ�����·��
CLog::CLog(const char *filePath, LOG_LEVEL level):m_LogLevel(level),
	m_maxLogFileSize(LOG_DEFAULT_FILESIZE)
{
	m_showLogFlag = true;
	if(NULL == filePath){
		time_t nowTime = time(NULL);
		struct tm *pLocalTime = localtime(&nowTime);
		char timeString[100] = {0};
		sprintf(timeString, "%s%04d-%02d-%02d.log",LOGFILENAME, pLocalTime->tm_year + 1900, pLocalTime->tm_mon + 1, 
			pLocalTime->tm_mday);
		filePath = timeString;
	}
	m_fileOut.open(filePath, ios::out | ios::app);
	m_openSuccess = m_fileOut.is_open();
	 m_logFilePath = filePath;
}


void CLog::OpenLogFile(const char *pFilePath, LOG_LEVEL level)
{
	if (pFilePath)
	{
		if (m_openSuccess)  // ����Ѿ����ļ� �ر�ԭ�ļ�
		{
			CloseLogFile();
		}
		m_fileOut.open(pFilePath, ios::out | ios::app);
	}
	m_openSuccess = m_fileOut.is_open();
	m_LogLevel = level; 
	m_logFilePath = pFilePath;
}


// <Summary>�õ���־�ļ���С</Summary>
// <DateTime>2013/9/9</DateTime>
// <Returns>��־�ļ���С</Returns>
size_t CLog::GetLogFileSize()
{
	if (!m_openSuccess)
	{
		return 0;
	}
	streampos curPos = m_fileOut.tellg();  // ��ǰ�ļ�ָ��λ��

	m_fileOut.seekg(0, ios::end);  // ���ļ�ָ��ָ���ļ�ĩβ 
	streampos pos = m_fileOut.tellg(); // ��ȡ�ļ�ָ���λ�� ��ֵ��Ϊ�ļ���С
	m_fileOut.seekg(curPos);   // �ָ��ļ�ָ��λ��
	return static_cast<size_t>(pos);
}

// ��logTextд����־�ļ�
// level: ������־��Ϣ����  logText����־����
void CLog::WriteLog(string logText, LOG_LEVEL level)
{
    if (m_openSuccess && level <= m_LogLevel)
    {
        string flag;
        if (level <= LL_ERROR)
        {
            flag = "[ERROR] ";
        }
        else if (level >= LL_INFORMATION)
        {
            flag = "[INFORMATION] ";
        }
        else 
        {
            flag = "[WARN] ";
        }

        if (m_maxLogFileSize > 0 && GetLogFileSize() > m_maxLogFileSize)  // ��־��Ϣ���������־��С ���������־
		{
			ChangeLogFile();
		}
		// д��־��Ϣ
		if (m_showLogFlag)
		{
			m_fileOut.write(flag.c_str(), flag.size());
		}
		// ����־�����ʱ����Ϣ
		time_t nowTime = time(NULL);
        struct tm *pLocalTime = localtime(&nowTime);
		char timeString[100] = {0};
		sprintf(timeString, " [%04d-%02d-%02d %02d:%02d:%02d]	", pLocalTime->tm_year + 1900, pLocalTime->tm_mon + 1, 
			pLocalTime->tm_mday, pLocalTime->tm_hour, pLocalTime->tm_min, pLocalTime->tm_sec);
		m_fileOut.write(timeString, strnlen(timeString, sizeof(timeString)));
		logText +="\n";
		m_fileOut.write(logText.c_str(), logText.size()); 
        m_fileOut.flush();
    }
    return;
}

void CLog::WriteLog(LOG_LEVEL level, const char *pLogText, ...)
{
    va_list args, args_tmp;

    // ��ʼ��args
    va_start(args, pLogText);

    // args1 �� args ��һ������
    va_copy(args_tmp, args);

    // ʹ��nullptr��0��Ϊǰ������������ȡ��ʽ������䳤�����б�����Ҫ���ַ�������
    // ʹ�� string(size_t n, char c) ���캯��������һ������Ϊn���ַ���������Ϊn��c�Ŀ���
    string res(1 + vsnprintf(nullptr, 0, pLogText, args_tmp), 0);
    // args1 ������ɣ�����رգ�����
    va_end(args_tmp);

    // ʹ��args����ʽ��Ҫ���ص��ַ���res�� ָ������size
    vsnprintf(&res[0], res.size(), pLogText, args);
    m_mutex.lock();
    WriteLog(res, level);
    m_mutex.unlock();
    // args ������ɣ��رգ�����
    va_end(args);
}

// <Summary>��ʽ���ַ���</Summary>
// <DateTime>2013/10/31</DateTime>
// <Parameter name="srcString" type="IN">��Ҫ���и�ʽ�����ַ���</Parameter>
// <Parameter name="pFormatString" type="IN">��ʽ�ַ���</Parameter>
// <Returns>��ʽ������ַ���</Returns>
const string& CLog::StringFormat(std::string &srcString, const char *pFormatString, ...)
{
    va_list args;
    char tempString[2048] = {0};  // ��ʽ���ַ������֧��2048���ֽ�
    va_start(args, pFormatString);
    vsnprintf(tempString, sizeof(tempString), pFormatString, args);
    srcString = tempString;
    return srcString;
}

// <Summary>����־�ַ���ת��Ϊ��������־��Ϣ</Summary>
// <DateTime>2013/10/31</DateTime>
// <Parameter name="pLogText" type="IN">��־�ַ���</Parameter>
// <Parameter name="level" type="IN">��־����</Parameter>
// <Returns>��������־��Ϣ</Returns>
string CLog::ConvertToRealLogText(const char *pLogText, LOG_LEVEL level)
{
    if (NULL == pLogText)
    {
        return string("");
    }

    // �õ���ǰϵͳʱ��
    time_t nowTime = time(NULL);
    struct tm *pLocalTime = localtime(&nowTime);

    string strLogText(pLogText);
    string::size_type pos = strLogText.find("$(");
    while (pos != string::npos)
    {
        string::size_type tempPos = strLogText.find(")", pos);
        if (tempPos != string::npos)
        {
            string strSubString = strLogText.substr(pos, tempPos - pos + 1);
            if (strSubString == string("$(DATE)"))
            {
                StringFormat(strSubString, "%04d-%02d-%02d", pLocalTime->tm_year + 1900, pLocalTime->tm_mon + 1, pLocalTime->tm_mday);
            }
            else if (strSubString == string("$(TIME)"))
            {
                StringFormat(strSubString, "%02d:%02d:%02d", pLocalTime->tm_hour, pLocalTime->tm_min, pLocalTime->tm_sec);
            }
            else if (strSubString == string("$(DATETIME)"))
            {
                StringFormat(strSubString, "%04d-%02d-%02d %02d:%02d:%02d", pLocalTime->tm_year + 1900, 
                    pLocalTime->tm_mon + 1, pLocalTime->tm_mday, pLocalTime->tm_hour, pLocalTime->tm_min, 
                    pLocalTime->tm_sec);
            }
            else if (strSubString == string("$(LEVELFLAG)"))
            {
                if (LL_ERROR == level)
                {
                    strSubString = "ERROR";
                }
                else if (LL_WARN == level)
                {
                    strSubString = "WARN";
                }
                else if (LL_INFORMATION == level)
                {
                    strSubString = "INFORMATION";
                }
            }
            strLogText.replace(pos, tempPos - pos + 1, strSubString);
            pos = strLogText.find("$(", tempPos + 1);
        }
        else 
        {
            break;
        }
    }

    return strLogText;
}

// <Summary>
// �������� ���ڽ���־��Ҫ��ĸ�ʽ���
// ��logText��ʵ�����ݺ͸�ʽ��¼��־��Ϊ�˱�����־��¼������һЩԤ���峣�������ڴ���ĳЩ�������Ϣ
// ���������ø�ʽ������makefile�ļ��еı�������$(��������),��������Ϊ��д��Ŀ���Ѿ����õĳ����У�
//   1.$(DATE):��ʾ��ǰ���� ��ʽΪYYYY-MM-DD
//   2.$(TIME):��ʾ��ǰʱ�� ��ʽΪHH:MM:SS
//   3.$(LEVELFLAG):��ʾ����־�ļ����־
//   4.$(DATETIME):��ʾ��ǰ���ں�ʱ�� ��ʽΪYYYY-MM-DD HH:mm:SS
// ���ӣ�WriteLogEx(LL_ERROR, "[$(DATE) $(TIME)] �߳��˳� �߳�id=%d [$(LEVELFLAG)]", 10);
// Ч����[2013-10-31 09:55:30] �߳��˳� �߳�id=10 [ERROR]
// </Summary>
// <DateTime>2013/10/31</DateTime>
// <Parameter name="logText" type="IN">��־����</Parameter>
// <Parameter name="level" type="IN">��־����</Parameter>
void CLog::WriteLogEx(LOG_LEVEL level, const char *pLogText, ...)
{
    if (m_openSuccess && level <= m_LogLevel)
    {
        va_list args;
        char szLogText[LOG_LINE_MAX] = {0};
        va_start(args, pLogText);
        vsnprintf(szLogText, LOG_LINE_MAX - 1, pLogText, args);
        string strRealLogText = ConvertToRealLogText(szLogText, level);
        if (m_maxLogFileSize > 0 && GetLogFileSize() > m_maxLogFileSize)  // ��־��Ϣ���������־��С ���������־
        {
            ChangeLogFile();
        }
        m_fileOut.write(strRealLogText.c_str(), strRealLogText.length());
    }
}

void CLog::CloseLogFile()
{
    m_fileOut.clear();  // ������״̬
    m_fileOut.close();  // �ر���
    m_openSuccess = false;
}

// ɾ����־�ļ�����
void CLog::ChangeLogFile()
{
	time_t nowTime = time(NULL);
	struct tm *pLocalTime = localtime(&nowTime);
	char timeString[100] = {0};
        sprintf(timeString, "%s%04d-%02d-%02d.log",m_logFilePath.c_str(), pLocalTime->tm_year + 1900, pLocalTime->tm_mon + 1,
		pLocalTime->tm_mday);
	string filePath = timeString;

	OpenLogFile(filePath.c_str());
}
