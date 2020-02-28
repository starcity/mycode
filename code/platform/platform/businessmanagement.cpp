#include "businessmanagement.h"
#include <algorithm>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "cmd.h"
#include "json/json.h"
#include "tinyxml/CTcxml.h"
#include "cencode.h"





extern CLog    gClog;



typedef int32_t (Business::*downLinkFunction)(struct evkeyvalq &params,string &reInfo);
typedef std::map<string,downLinkFunction> downMapFunction;




//下行指令映射消息列表
static downMapFunction gdownMapFunction = {
	{ HTTP_CMD_OPEN_MDS_FILE		,&Business::cmdOpenMdsFile},
};


Business::Business()
{
}


Business::~Business()
{
}

int32_t Business::init()
{
	WRITELOGINFO("init start");

	return CODE_RETURN_SUCESSED;
}

//接口   指令分发
int Business::doPlatform(evkeyvalq &params, string &reInfo)
{
	Json::Value		root;
	const char *tmp;
	string cmd;
	tmp = evhttp_find_header(&params,HTTP_CMD_COMMAND);
	if (tmp == NULL)
	{
		WRITELOGERROR("%s is null",HTTP_CMD_COMMAND);
		root[HTTP_JSON_URL_CODE] = CODE_HTTP_FAILED;
		root[HTTP_JSON_URL_MSG] = HTTP_URL_CODE_FIELD_NULL;
		reInfo = root.toStyledString();
		return CODE_RETURN_ERROR_NO_COMMAND;
	}
	cmd = tmp;
	//命令分发
	downMapFunction::iterator iter = gdownMapFunction.find(cmd);
	if (iter == gdownMapFunction.end())
	{
		WRITELOGERROR("cmd:%s",cmd.c_str());
		root[HTTP_JSON_URL_CODE] = CODE_HTTP_FAILED;
		root[HTTP_JSON_URL_MSG] = HTTP_URL_CODE_CMD_NOT_FOUND;
		reInfo = root.toStyledString();
		return CODE_RETURN_ERROR_NO_COMMAND;
	}

	(this->*gdownMapFunction[cmd])(params,reInfo);

	return CODE_RETURN_SUCESSED;
}


int32_t Business::cmdOpenMdsFile(evkeyvalq &params, string &reInfo)
{
	const char *path= NULL;

	Json::Value  root;

	path = evhttp_find_header(&params, HTTP_URL_PATH);
	if(NULL == path)
	{
		root[HTTP_JSON_URL_CODE] = CODE_HTTP_FAILED;
		root[HTTP_JSON_URL_MSG] = HTTP_URL_CODE_FIELD_NULL;
		reInfo = root.toStyledString();
		WRITELOGERROR("path is NULL");
		return CODE_RETURN_ERROR_HTTP_FIELD;
	}

	root[HTTP_JSON_URL_CODE] = CODE_HTTP_SUCESSED;
	root[HTTP_JSON_URL_MSG]  = HTTP_URL_CODE_SUCESSED;

	reInfo = root.toStyledString();

	return CODE_RETURN_SUCESSED;
}


int32_t Business::encode(uint8_t * input, int32_t len,string& outstr)
{
	/* set up a destination buffer large enough to hold the encoded data */
	int size = 2 *len;
	char* output = (char*)malloc(size);
	/* keep track of our encoded position */
	char* c = output;
	/* store the number of bytes encoded by a single call */
	int cnt = 0;
	/* we need an encoder state */
	base64_encodestate s;

	/*---------- START ENCODING ----------*/
	/* initialise the encoder state */
	base64_init_encodestate(&s);
	/* gather data from the input and send it to the output */
	cnt = base64_encode_block((const char *)input, len, c, &s);
	c += cnt;
	/* since we have encoded the entire input string, we know that
	   there is no more input data; finalise the encoding */
	cnt = base64_encode_blockend(c, &s);
	c += cnt;
	/*---------- STOP ENCODING  ----------*/

	/* we want to print the encoded data, so null-terminate it: */
	*c = 0;

	outstr = output;

	free(output);

	return CODE_RETURN_SUCESSED;
}

