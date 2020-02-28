#ifndef BUSINESSMANAGEMENT
#define BUSINESSMANAGEMENT

#include <stdint.h>
#include <string>
#include <map>
#include <memory>
#include <vector>
#include "taskinterface.h"
#include "common.h"
//#include "MDSParser.h"

using namespace std;


//文件ID 多线程安全
class onlyID
{
	public:
		onlyID():m_id(0x10000000){}
		uint64_t getOnlyNumber()
		{
			uint32_t number;
			m_lock.lock();
			number = ++m_id;
			m_lock.unlock();
			return number;
		}

	private:
		std::mutex  m_lock;
		uint32_t    m_id;
};


//typedef map<uint64_t,MDS_Handle> Map_file;

class Business :public TaskInterface
{
	public:
		Business();
		~Business();


		int32_t  init();
		//接口 from TaskInterface
		int32_t doPlatform(struct evkeyvalq &params,string &reInfo);

	public:
		int32_t cmdOpenMdsFile(struct evkeyvalq &params,string &reInfo);
		int32_t cmdCloseMdsFile(struct evkeyvalq& params, string& reInfo);
		int32_t cmdGetMdsInfo(struct evkeyvalq &params,string &reInfo);
    		int32_t cmdGetLabelJpeg(struct evkeyvalq &params,string &reInfo);
    		int32_t cmdGetTileImage(struct evkeyvalq &params,string &reInfo);
    		int32_t cmdGetPreviewImage(struct evkeyvalq &params,string &reInfo);
		int32_t cmdGetThumbnailImage(struct evkeyvalq& params, string& reInfo);
		int32_t cmdGetLayerSize(struct evkeyvalq& params, string& reInfo);

private:
			int32_t encode(uint8_t* input, int32_t len, string& outstr);

	private:
		onlyID m_file_id;
//		Map_file  m_map_file;
		std::mutex  m_lock;
};

#endif // BUSINESSMANAGEMENT

