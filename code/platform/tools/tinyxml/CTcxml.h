#ifndef    NETBANK_TCXML_H
#define    NETBANK_TCXML_H
#include "tinyxml.h"
#include <iostream>
#include <stdint.h>

using namespace std;


class CTcxml
{
public:
	CTcxml();
	CTcxml(const char *fileName);
	~CTcxml();

public:
	bool      pushXmlString(const char *xml);
	void      saveFile(const char *filePath);
	bool	  enter(const char *node);//����֮��
	bool	  leave();//����֮��
	bool	  enterNextNode(const char *nextNode);//�¸��ڵ㣬�ֵ�֮��


public://<type/>��ȡ ��������޽��
	bool	 getElementTextString(const char *elementName,string &str);
	bool     getElementTextInt(const char *elementName,int &istr);
        bool     getElementTextUInt32(const char *elementName,uint32_t &istr);
	bool     getElementTextDouble(const char *elementName,double &dstr);

	bool	 getElementAttributeString(const char *elementName,const char *attr,string &str);
	bool	 getElementAttributeInt(const char *elementName,const char *attr,int &istr);
	bool	 getElementAttributeDouble(const char *elementName,const char *attr,double &dstr);

	bool	 getNodeAttributeString(const char *attr,string &str);
	bool	 getNodeAttributeInt(const char *attr,int &istr);
	bool	 getNodeAttributeDouble(const char *attr,double &dstr);

	bool     isExistElement(const char *elementName,bool &isExist);

	bool     setElementTextString(const char *elementName,string str);
	bool     setElementTextInt(const char *elementName,int istr);
	bool     setElementTextDouble(const char *elementName,double dstr);

	CTcxml * getNode(const char *nodeName);


private:
	TiXmlDocument *m_pDoc;
	TiXmlNode  *m_pNode;
};



#endif
