/************************************************************************/
// �����ƣ�  CLog
// �����ռ䣺��
// �ļ�����  Log.h  Log.cpp
// ����ʱ��: 2013-9-9
// ���ߣ�    guiyou
// ��˵����  ��־��¼��  ����û��ʹ��winapiʵ�� ������linux�н���ʹ��       
// �������⣺1.���fstream��ͬһ���ļ���д����ʱ���������
//           2.���̶߳�ͬһ��fstreamд���ݣ�ͬ������
// ���������1.����һ����־�����ࣨ������ͨ�������־���������õ���־���󣬶����Ѿ��򿪵��ļ���ֱ�ӷ��ظ���־����
//              ����ʵ����һ���µ���־����
               
//            2.����һ������д�ļ��ķ��������ô˷���ʱ�����ļ�д���ݽ���ͬ��
/************************************************************************/

#ifndef _LOG_INCLUDE_H_
#define _LOG_INCLUDE_H_

#define LOG_LINE_MAX 2048   //��־��������ַ�����
#define LOG_DEFAULT_FILESIZE	(10 * 1024 * 1024)
#include <fstream>
#include <mutex>


using std::string;
using std::fstream;
using std::ios;




class CLog
{
public:
	enum LOG_LEVEL     // ��־����
	{
		LL_ERROR = 1,              
		LL_WARN = 2,               
		LL_INFORMATION = 3         
	};

public:
    CLog(const char *filePath = NULL, LOG_LEVEL level = LL_INFORMATION);
    virtual ~CLog(void)
    {
        if (m_openSuccess)
        {
			CloseLogFile();
        }
    }

    // ��ô��ļ��Ƿ�ɹ��ı�ʶ
    bool GetOpenStatus() const
    {
        return m_openSuccess;
    }

	// ����־�ļ�
    void OpenLogFile(const char *pFilePath, LOG_LEVEL level = LL_INFORMATION);
	// д��־����
    void WriteLog(LOG_LEVEL level, const char *pLogText, ...);
    void WriteLog(string logText, LOG_LEVEL level = LL_INFORMATION);
    void WriteLogEx(LOG_LEVEL level, const char *pLogText, ...);
	// �õ���־�ļ���С
	size_t GetLogFileSize();
	// �����־�ļ�����
	void ChangeLogFile();
	void CloseLogFile();

    void SetLogLevel(LOG_LEVEL level = LL_INFORMATION)
    {
        m_LogLevel = level;
    }
    LOG_LEVEL GetLogLevel() const
    {
        return m_LogLevel;
    }

    void SetShowFlag(bool flag = true)
    {
        m_showLogFlag = flag;
    }
    bool GetShowFlag() const
    {
        return m_showLogFlag;
    }

    void SetMaxLogFileSize(size_t size)
    {
        m_maxLogFileSize = size;
    }
    size_t GetMaxLogFileSize() const
    {
        return m_maxLogFileSize;
    }

private:
    CLog(const CLog &clog){};

protected:
    string ConvertToRealLogText(const char *pLogText, LOG_LEVEL level = LL_INFORMATION);
    const std::string &StringFormat(std::string &srcString, const char *pFormatString, ...);
protected:
    fstream m_fileOut;
    bool m_openSuccess;  // ��־�ļ���״̬ true��ʾ�Ѿ��ɹ��� ����û�д�
	string m_logFilePath; // ��־�ļ�����·�� string����

protected:
    LOG_LEVEL m_LogLevel;  // ��־��¼���� Ĭ��ΪLL_ERROR
	bool m_showLogFlag;    // ��ʾ��־��־  Ĭ��Ϊtrue
	size_t m_maxLogFileSize; // ��־�ļ�����С �����ô�С����ɾ�� Ĭ��Ϊ10MB
        std::mutex  m_mutex;//lock
};



#endif

