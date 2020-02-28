#ifndef		_CODE_DEFINE_H_
#define     _CODE_DEFINE_H_


#define		NETPROBEXML				"httpServer.xml"
#define		MYHTTPD_VERSION				"http v 0.0.1"



//配置文件信息
#define     SERVER					"SERVER"
#define     HTTPLISTENIP				"HTTPLISTENIP"
#define     HTTPLISTENPORT				"HTTPLISTENPORT"
#define     HTTPDTIMEOUT              		        "HTTPDTIMEOUT"
#define     THREADNUMBER                                "THREADNUMBER"


#define     CODE_RETURN_SUCESSED			0x00
#define     CODE_RETURN_ERROR_INITIALIZE		-0x01
#define     CODE_RETURN_ERROR_CREATEPTHREAD		-0x02
#define     CODE_RETURN_ERROR_OPERATOR_NULL		-0x03
#define     CODE_RETURN_ERROR_NO_OPERATOR		-0x04
#define     CODE_RETURN_ERROR_NO_COMMAND                -0X05
#define     CODE_RETURN_ERROR_MYSQL_INIT		-0x10
#define     CODE_RETURN_ERROR_MYSQL_QUERY		-0x11
#define     CODE_RETURN_ERROR_MYSQL_GETEXEC		-0x12
#define     CODE_RETURN_ERROR_MYSQL_ROLLBACK		-0x13
#define     CODE_RETURN_ERROR_MYSQL_COMMIT		-0x14
#define     CODE_RETURN_ERROR_GROUPEXIST		-0x50
#define     CODE_RETURN_ERROR_USERNOEXIST		-0x51
#define     CODE_RETURN_ERROR_MYSQL_DATAWRONG		-0xA0
#define     CODE_RETURN_ERROR_HTTP_FIELD		-0xA1
#define     CODE_RETURN_ERROR_QUERY_DOMAIN_TIME_OUT     -0xA2
#define     CODE_RETURN_ERROR_REDIS_FALED               -0xA3
#define     CODE_RETURN_ERROR_NODE_OFF_LINE             -0xA4
#define     CODE_RETURN_ERROR_DOWN_DOMAIN_FAILED        -0xA5
#define     CODE_RETURN_ERROR_GENERAL                   -0xA6
#define     CODE_RETURN_ERROR_DOWN_DOMAIN_CONFLICT      -0xA7


#define    CODE_HTTP_SUCESSED				0x00
#define    CODE_HTTP_BUSY				0x01
#define    CODE_HTTP_FAILED				0x02


//页面指令操作码
#define   HTTP_CMD_COMMAND                     "cmd"

#define   HTTP_CMD_BUSINESS_MANAGEMENT          "businessManagement"
//cmd 指令
#define   HTTP_CMD_OPEN_MDS_FILE                "openMdsFile"
#define   HTTP_CMD_CLOSE_MDS_FILE		"closeMdsFile"
#define   HTTP_CMD_GET_MDS_INFO			"getMdsInfo"
#define   HTTP_CMD_GET_LABEL_JPEG		"getLableJpeg"
#define   HTTP_CMD_GET_TILE_IMAGE	        "getTileImage"
#define   HTTP_CMD_GET_PREVIEW_IMAGE		"getPreviewImage"
#define   HTTP_CMD_GET_THUMBNAIL_IMAGE		"getThumbnailImage"
#define   HTTP_CMD_GET_LAYER_SIZE		"getLayerSize"


#define   HTTP_URL_PATH				"path" 
#define   HTTP_URL_FILE_ID			"fileID" 
#define   HTTP_URL_LAYER			"layer"
#define   HTTP_URL_WIDTH			"width"
#define   HTTP_URL_HEIGHT			"height"



//json响应字段
#define   HTTP_JSON_URL_CODE			"code"
#define   HTTP_JSON_URL_MSG			"msg"
#define   HTTP_JSON_URL_FILE_ID			"fileID"
#define   HTTP_JSON_URL_SCALE                   "scale"
#define   HTTP_JSON_URL_LAYER_COUNT             "layerCount"
#define   HTTP_JSON_URL_WIDTH		        "width"
#define   HTTP_JSON_URL_HEIGHT	   	        "height"
#define   HTTP_JSON_URL_IMAGE	   	        "image"
#define   HTTP_JSON_URL_LENGTH	   	        "length"
#define   HTTP_JSON_URL_OBJ	   	        "obj"
#define   HTTP_JSON_URL_TILE_SIZE	   	"tileSize"
#define   HTTP_JSON_URL_LAYER	   		"layer"
#define   HTTP_JSON_URL_LAYER_ID	   	"layerID"
#define   HTTP_JSON_URL_LAYER_ROW	   	"row"
#define   HTTP_JSON_URL_LAYER_COL	   	"col"


#define  HTTP_URL_CODE_SUCESSED					"操作成功"
#define  HTTP_URL_CODE_FIELD_NULL				"指定的字段名为空" 
#define  HTTP_URL_CODE_FIELD_NO_EXIST  			        "指定的字段名不存在"
#define  HTTP_URL_CODE_OPEN_FILE_FAILED  			"打开文件失败"
#define  HTTP_URL_CODE_FILE_NOT_FOUND  			        "文件不存在"
#define  HTTP_URL_CODE_CMD_NOT_FOUND 			        "指令不存在"
#define  HTTP_URL_CODE_CMD_LABEL_JPEG 			        "获取图片失败"


#endif

