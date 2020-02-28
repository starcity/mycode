#include "CTcxml.h"
#include <stdlib.h>


CTcxml::CTcxml()
{
	m_pDoc = new TiXmlDocument(); 
}

CTcxml::CTcxml(const char *fileName)
{
	m_pDoc = new TiXmlDocument(fileName);
	bool flag = m_pDoc->LoadFile();
	if (flag)
	{
		m_pNode = m_pDoc->FirstChild();
	}else{
		m_pNode = NULL;
	}
}

CTcxml::~CTcxml()
{
	if(m_pDoc)
	{
		delete m_pDoc;
		m_pDoc = NULL;
	}
}

bool CTcxml::pushXmlString(const char *xml)
{
	if(NULL == m_pDoc->Parse(xml))
	{
		m_pNode = NULL;
	}else{
		m_pNode = m_pDoc->FirstChild();
	}
	return m_pNode?true:false;
}
	
void CTcxml::saveFile(const char *filePath)
{
	if(filePath == NULL)
	{
		m_pDoc->SaveFile();
	}
	else {
		m_pDoc->SaveFile(filePath);
	}

}

bool CTcxml::enter(const char *node)
{
	if (m_pNode)
	{
		m_pNode = m_pNode->FirstChild(node);
	}

	return m_pNode?true:false;
}

bool CTcxml::leave()
{
	if (m_pNode)
	{
		m_pNode = m_pNode->Parent();
	}

	return m_pNode?true:false;
}

bool CTcxml::enterNextNode(const char *nextNode)
{
	if (!m_pNode)
	{
		return m_pNode;
	}
	if(NULL == nextNode)
	{
		m_pNode = m_pNode->NextSibling();
	}
	else 
	{
		m_pNode = m_pNode->NextSibling(nextNode);
	}
	
	return m_pNode?true:false;
}

bool CTcxml::getElementTextString(const char *elementName,string &str)
{
	if (!m_pNode)
	{
		return false;
	}

	TiXmlElement *element = m_pNode->FirstChildElement(elementName);
	if (NULL == element)
	{
		str = "";
	}else{
            const char * p = element->GetText();
            if(NULL == p)
                str = "";
            else str = p;
	}

	return true;
}
	
bool CTcxml::getElementTextInt(const char *elementName,int &istr)
{
	if (!m_pNode)
	{
		return false;
	}

	TiXmlElement *element = m_pNode->FirstChildElement(elementName);
	if (NULL == element)
	{
		istr = 0;
	}else{
		istr = atoi(element->GetText());
	}

	return true;
}

bool CTcxml::getElementTextUInt32(const char *elementName,uint32_t &istr)
{
    if (!m_pNode)
    {
            return false;
    }

    TiXmlElement *element = m_pNode->FirstChildElement(elementName);
    if (NULL == element)
    {
            istr = 0;
    }else{
            istr = atoi(element->GetText());
    }

    return true;
}
	
bool CTcxml::getElementTextDouble(const char *elementName,double &dstr)
{
	if (!m_pNode)
	{
		return false;
	}

	TiXmlElement *element = m_pNode->FirstChildElement(elementName);
	if (NULL == element)
	{
		dstr = 0;
	}else{
		dstr = atof(element->GetText());
	}

	return true;
}

bool CTcxml::setElementTextString(const char *elementName,string str)
{
	if (!m_pNode)
	{
		return false;
	}
	TiXmlElement *element = m_pNode->FirstChildElement(elementName);
	if(NULL == element)
	{
		TiXmlElement *pNewElement = new TiXmlElement(elementName);
		if (NULL==pNewElement)
		{
			return false;
		}
		// 设置节点文本，然后插入节点
		TiXmlText *pNewValue = new TiXmlText(str.c_str());
		pNewElement->LinkEndChild(pNewValue);

		m_pNode->InsertEndChild(*pNewElement);
	}
	else
	{
		element->Clear();
		TiXmlText *pValue = new TiXmlText(str.c_str());
		element->LinkEndChild(pValue);
	}
	saveFile(NULL);
	return true;
}

bool CTcxml::setElementTextInt(const char *elementName,int istr)
{
	if (!m_pNode)
	{
		return false;
	}
	char tmp[20] = {0};
	sprintf(tmp,"%d",istr);
	TiXmlElement *element = m_pNode->FirstChildElement(elementName);
	if(NULL == element)
	{
		TiXmlElement *pNewElement = new TiXmlElement(elementName);
		if (NULL==pNewElement)
		{
			return false;
		}
		// 设置节点文本，然后插入节点
		TiXmlText *pNewValue = new TiXmlText(tmp);
		pNewElement->LinkEndChild(pNewValue);

		m_pNode->InsertEndChild(*pNewElement);
	}
	else
	{
		element->Clear();
		TiXmlText *pValue = new TiXmlText(tmp);
		element->LinkEndChild(pValue);
	}
	saveFile(NULL);
	return true;
}

bool CTcxml::setElementTextDouble(const char *elementName,double dstr)
{
	if (!m_pNode)
	{
		return false;
	}
	char tmp[20] = {0};
	sprintf(tmp,"%f",dstr);
	TiXmlElement *element = m_pNode->FirstChildElement(elementName);
	if(NULL == element)
	{
		TiXmlElement *pNewElement = new TiXmlElement(elementName);
		if (NULL==pNewElement)
		{
			return false;
		}
		// 设置节点文本，然后插入节点
		TiXmlText *pNewValue = new TiXmlText(tmp);
		pNewElement->LinkEndChild(pNewValue);

		m_pNode->InsertEndChild(*pNewElement);
	}
	else
	{
		element->Clear();
		TiXmlText *pValue = new TiXmlText(tmp);
		element->LinkEndChild(pValue);
	}
	saveFile(NULL);
	return true;
}

bool CTcxml::getElementAttributeString(const char *elementName,const char *attr,string &str)
{
	if (!m_pNode)
	{
		return false;
	}

	TiXmlElement *element = m_pNode->FirstChildElement(elementName);
	if (NULL == element)
	{
		str = "";
	}else{
		str = element->Attribute(attr);
	}

	return true;
}

bool CTcxml::getElementAttributeInt(const char *elementName,const char *attr,int &istr)
{
	if (!m_pNode)
	{
		return false;
	}

	TiXmlElement *element = m_pNode->FirstChildElement(elementName);
	if (NULL == element)
	{
		istr = 0;
	}else{
		element->Attribute(attr,&istr);
	}

	return true;
}

bool CTcxml::getElementAttributeDouble(const char *elementName,const char *attr,double &dstr)
{
	if (!m_pNode)
	{
		return false;
	}

	TiXmlElement *element = m_pNode->FirstChildElement(elementName);
	if (NULL == element)
	{
		dstr = 0;
	}else{
		element->Attribute(attr,&dstr);
	}

	return true;
}

bool CTcxml::getNodeAttributeString(const char *attr,string &str)
{
	if (!m_pNode)
	{
		return false;
	}

	str = m_pNode->Parent()->FirstChildElement(m_pNode->Value())->Attribute(attr);

	return true;
}

bool CTcxml::getNodeAttributeInt(const char *attr,int &istr)
{
	if (!m_pNode)
	{
		return false;
	}

    m_pNode->Parent()->FirstChildElement(m_pNode->Value())->Attribute(attr,&istr);

	return true;
}

bool CTcxml::getNodeAttributeDouble(const char *attr,double &dstr)
{
	if (!m_pNode)
	{
		return false;
	}

	m_pNode->Parent()->FirstChildElement(m_pNode->Value())->Attribute(attr,&dstr);

	return true;
}

bool CTcxml::isExistElement(const char *elementName,bool &isExist)
{
	if (!m_pNode)
	{
		return false;
	}

	TiXmlElement *tmp = m_pNode->FirstChildElement(elementName);

	isExist =  tmp?true:false;
	return isExist;
}
	
CTcxml * CTcxml::getNode(const char *nodeName)
{
	if (!m_pNode)
	{
		return NULL;
	}
	m_pNode = m_pNode->FirstChild(nodeName);
	return this;
}

