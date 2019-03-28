//////////////////////////////////////////////////////////////////////
//
// MIME message encoding/decoding
//
// Jeff Lee
// Dec 11, 2000
//
//////////////////////////////////////////////////////////////////////

#ifndef SI_OFFLINE
#include "stdafx.h"
#endif

#include "PMimeCode.h"
#include "PMimeChar.h"
#include "PMime.h"
#include <stdlib.h>
#include <time.h>

#include <fstream>
#include <memory>
#include <iostream>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//const char* CMimeHeader::m_TypeTable[TABLE_SIZE] = {0};
//const CMimeHeader::MediaTypeCvt CMimeHeader::m_TypeCvtTable[TABLE_SIZE] = {0};



extern int FindSubString(const char * pBuffer,const string & strSubString, int bufSize);

// search for a character in the current line (before CRLF)
static int LineFind(const char* pBuf, int ch, int bufSize=0)
{
    if (pBuf == NULL)
    {


        return 0;


    }

    // pBuf is a string
    if (bufSize == 0)
    {
        bufSize = (int)::strlen(pBuf);
    }

    for (int i=0; i<bufSize; i++)
    {
        if (pBuf[i] == '\r' || pBuf[i] == '\n')
        {
            return -1;
        }
        else if (pBuf[i] == ch)
        {
            return i;
        }
    }

    return -1;
}

/*const char* CMimeHeader::m_TypeTable[] =
{
    "text", "image", "audio", "vedio", "application", "multipart", "message", NULL
};
*/
//////////////////////////////////////////////////////////////////////
// CMimeField class - Represents a field of a MIME body part header
//////////////////////////////////////////////////////////////////////

void CMimeField::GetValue(string& strValue, bool bAll) const

{
    if (bAll)
    {
        strValue = m_strValue;
        return;
    }

    string::size_type nEnd = m_strValue.find(';');
    if (nEnd != string::npos)
    {
        while (nEnd > 0 && CMimeChar::IsSpace((unsigned char)m_strValue[nEnd-1]))
            nEnd--;
        strValue.assign(m_strValue.c_str(), nEnd);
    }
    else
        strValue = m_strValue;
}

// set a parameter (attribute=value) of the field
void CMimeField::SetParameter(const char* pszAttr, const char* pszValue)
{
    int nSize = pszValue ? (int)::strlen(pszValue) : 0;
    string strValue;
    strValue.reserve(nSize+3);
    if (!pszValue || *pszValue != '"')
        strValue = "\"";
    if (pszValue != NULL)
        strValue += pszValue;
    if (nSize < 2 || pszValue[nSize-1] != '"')
        strValue += "\"";

    int nPos;
    bool bSuc = FindParameter(pszAttr, nPos, nSize);

    if (!bSuc || nPos < 0 || nPos >= (int)m_strValue.length() ||
        nSize <= 0 || nPos + nSize > (int)m_strValue.length())
    {
        // add new parameter

        m_strValue.reserve(m_strValue.size() + ::strlen(pszAttr) + strValue.size() + 5);
        //if (CMimeEnvironment::AutoFolding())
        //	m_strValue += ";\r\n\t";
        //else
        //	m_strValue += "; ";
        m_strValue += "; ";
        m_strValue += pszAttr;
        m_strValue += '=';
        m_strValue += strValue;
    }
    else							// update existing parameter
    {
        m_strValue.replace(nPos, nSize, strValue);
    }
}

// get the value of a parameter
bool CMimeField::GetParameter(const char* pszAttr, string& strValue) const
{
    int nPos, nSize;
    if (!FindParameter(pszAttr, nPos, nSize))
    {
        strValue.clear();
        return false;
    }

    if (nPos < 0 || nPos >= (int)m_strValue.length() ||
        nSize <= 0 || nPos + nSize > (int)m_strValue.length())
    {
        return false;
    }

    if (m_strValue[nPos] == '"')
    {
        nPos++;
        nSize--;
        if (nSize > 0 && m_strValue[nPos+nSize-1] == '"')
            nSize--;
    }

    strValue.assign(m_strValue.data()+nPos, nSize);

    return true;
}

int CMimeField::GetLength() const
{
    int nLength = (int) m_strName.size() + 4;
    CFieldCodeBase* pCoder = CMimeEnvironment::CreateFieldCoder(GetName());
    pCoder->SetCharset(m_strCharset.c_str());
    pCoder->SetInput(m_strValue.c_str(), (int)m_strValue.size(), true);
    nLength += pCoder->GetOutputLength();
    delete pCoder;
    return nLength;
}

// store a field to string buffer
int CMimeField::Store(char* pszData, int nMaxSize) const
{
    if (pszData == NULL)
    {
        return 0;
    }

    int nMinSize = (int)m_strName.size() + 4;
    if (nMaxSize < nMinSize)
        return 0;
    ::strcpy(pszData, m_strName.c_str());
    pszData += m_strName.size();
    *pszData++ = ':';
    *pszData++ = ' ';

    CFieldCodeBase* pCoder = CMimeEnvironment::CreateFieldCoder(GetName());
    pCoder->SetCharset(m_strCharset.c_str());
    pCoder->SetInput(m_strValue.c_str(), (int)m_strValue.size(), true);
    int nEncoded = pCoder->GetOutput((unsigned char*) pszData, nMaxSize-nMinSize);
    delete pCoder;
    pszData += nEncoded;

    *pszData++ = '\r';
    *pszData++ = '\n';
    return nMinSize + nEncoded;
}

// load a field from string buffer
int CMimeField::Load(const char* pszData, int nDataSize)
{
    Clear();
    if (pszData == NULL || nDataSize == 0)
    {
        return 0;
    }

    int octet = 0;
    // find the next field (e.g. "\r\nContent...")
    while (octet < nDataSize)
    {
        if (!CMimeChar::IsSpace((unsigned char)pszData[octet]))
        {
            break;
        }

        if (pszData[octet] == '\r' || pszData[octet] == '\n')		// end of header ?
            return 0;

        int pos = 0;
        if ((pos = ::FindSubString(pszData+octet, "\n", nDataSize-octet)) != -1)
        {
            octet += (pos + 1);
        }
        else
        {
            return 0;
        }
    }

    // get the field name
    int nNameSize = ::LineFind(pszData+octet, ':', nDataSize-octet);
    if (nNameSize != -1)				// if colon not found, Name would be empty
    {
        m_strName.assign(pszData+octet, nNameSize);
        octet += (nNameSize + 1);
    }

    // find the end of the field
    while (octet < nDataSize)
    {
        if (pszData[octet] != ' ' && pszData[octet] != '\t')
        {
            break;
        }
        octet++;
    }
    int nEnd = octet;
    int nCRLF = 0;
    do
    {
        int pos = 0;
        if (((pos = ::FindSubString(pszData+nEnd, "\n", nDataSize-nEnd)) != -1) ||
            ((pos = ::FindSubString(pszData+nEnd, "\r", nDataSize-nEnd)) != -1))
        {
            if (pos > 0 && pszData[nEnd+pos-1] == '\r')
            {
                nCRLF = 2;
            }
            else
            {
                nCRLF = 1;
            }
            nEnd += (pos + 1);
        }
        else
        {
            return 0;
        }
    } while (pszData[nEnd] == '\t' || pszData[nEnd] == ' ');	// linear-white-space

    // decode and unfold the field value
    CFieldCodeBase* pCoder = CMimeEnvironment::CreateFieldCoder(GetName());
    pCoder->SetInput(pszData+octet, nEnd-octet-nCRLF, false);
    m_strValue.resize(pCoder->GetOutputLength());
    int nSize = pCoder->GetOutput((unsigned char*) m_strValue.c_str(), (int) m_strValue.capacity());
    m_strValue.resize(nSize);
    m_strCharset = pCoder->GetCharset();
    delete pCoder;
    return nEnd;
}

bool CMimeField::FindParameter(const char* pszAttr, int& nPos, int& nSize) const
{
    if (pszAttr == NULL)
    {
        return false;
    }

    const char* pszParms = ::strchr(m_strValue.data(), ';');
    int nAttrSize = (int)::strlen(pszAttr);
    while (pszParms != NULL)
    {
        while (CMimeChar::IsSpace((unsigned char)*pszParms) || *pszParms == ';')
            pszParms++;

        const char* pszName = pszParms;		// pszName -> attribute
        pszParms = ::strchr(pszParms, '=');
        if (!pszParms)
            break;

        pszParms++;					// pszParams -> parameter value
        //while (*pszParms == ' ' || *pszParms == '\t')
        //	pszParms++;

        const char* pszParmEnd = NULL;
        if (*pszParms == '"')		// quoted string
            pszParmEnd = ::strchr(pszParms+1, '"');
        if (!pszParmEnd)			// non quoted string
        {
            pszParmEnd = pszParms;
            //修改附件名称不带双引号，文件名不完整问题
            while (CMimeChar::IsToken(*pszParmEnd) || (!::stricmp(pszAttr, "name") == 0 && *pszParmEnd == '.'))
            {
                 pszParmEnd++;
            }


        }
        else  pszParmEnd++;			// pszParmEnd -> end of parameter value

        if (0 != ::memicmp(pszAttr, pszName, nAttrSize) &&
            (CMimeChar::IsSpace((unsigned char)pszName[nAttrSize]) || pszName[nAttrSize] == '='))
        {
            nPos = (int)(pszParms - m_strValue.data());
            nSize = (int)(pszParmEnd - pszParms);
            return true;
        }

        pszParms = pszParmEnd;
    }
    return false;
}



int memicmp(const char * pStr1, const char * pStr2, size_t size)
{
  std::string str1(pStr1, size);
  std::string str2(pStr2, size);

  int ret = boost::algorithm::iequals(str1, str2);
  return ret;
}


int stricmp(const char * pStr1, const char * pStr2)
{
  return boost::algorithm::iequals(pStr1, pStr2);

}



//////////////////////////////////////////////////////////////////////
// CMimeHeader class - Represents the header of a MIME body part
//////////////////////////////////////////////////////////////////////

// Return the media type represented by Content-Type field (see RFC 2046)
CMimeHeader::MediaType CMimeHeader::GetMediaType() const
{
    const char* pszType = GetContentType();
    if (!pszType)
        pszType = "text";

    int nIndex = 0;
    while (m_TypeTable[nIndex] != NULL &&
        ::memicmp(pszType, m_TypeTable[nIndex], ::strlen(m_TypeTable[nIndex])) == 0)
        nIndex++;
    return (MediaType) nIndex;
}

// get the top-level media type
string CMimeHeader::GetMainType() const
{
    string strType;
    const char* pszType = GetContentType();
    if (pszType != NULL)
    {
        const char* pszSlash = ::strchr(pszType, '/');
        if (pszSlash != NULL)
            strType.assign(pszType, pszSlash-pszType);
        else
            strType = pszType;
    }
    else
        strType = "text";
    return strType;
}

// get the subtype
string CMimeHeader::GetSubType() const
{
    string strSubType;
    const CMimeField *pfd = GetField(CMimeConst::ContentType());
    if (pfd != NULL)
    {
        string strType;
        pfd->GetValue(strType);
        string::size_type nSlash = strType.find('/');
        if (nSlash > 0)
            strSubType = strType.substr(nSlash+1);
    }
    else
        strSubType = "plain";
    return strSubType;
}

// set the 'charset' parameter (for text) of Content-Type field
void CMimeHeader::SetCharset(const char* pszCharset)
{
    CMimeField *pfd = GetField(CMimeConst::ContentType());
    if (!pfd)
    {
        CMimeField fd;
        fd.SetName(CMimeConst::ContentType());
        fd.SetValue("text/plain");
        fd.SetParameter(CMimeConst::Charset(), pszCharset);
        m_listFields.push_back(fd);
    }
    else
        pfd->SetParameter(CMimeConst::Charset(), pszCharset);
}

// set the 'name' parameter (for attachment) of Content-Type field
void CMimeHeader::SetName(const char* pszName)
{
    if (pszName == NULL || *pszName == 0)
    {
        return;
    }

    CMimeField *pfd = GetField(CMimeConst::ContentType());
    if (!pfd)
    {
        // get the appropriate media-type/subtype according to file extension
        string strType;
        const char* pszType = "application/octet-stream";
        const char* pszFileExt = ::strrchr(pszName, '.');
        if (pszFileExt != NULL)
        {
            pszFileExt++;
            int nIndex = 0;
            while (m_TypeCvtTable[nIndex].nMediaType != MEDIA_UNKNOWN)
            {
                if (!::stricmp(pszFileExt, m_TypeCvtTable[nIndex].pszFileExt))
                {
                    strType = m_TypeTable[m_TypeCvtTable[nIndex].nMediaType];
                    strType += '/';
                    strType += m_TypeCvtTable[nIndex].pszSubType;
                    pszType = strType.c_str();
                    break;
                }
                nIndex++;
            }
        }

        CMimeField fd;
        fd.SetName(CMimeConst::ContentType());
//		fd.SetValue(pszType);
        fd.SetParameter(CMimeConst::Name(), pszName);
        m_listFields.push_back(fd);
    }
    else
        pfd->SetParameter(CMimeConst::Name(), pszName);
}

// set 'boundary' parameter (for multipart) of Content-Type field
void CMimeHeader::SetBoundary(const char* pszBoundary/*=NULL*/)
{
    static int s_nPartNumber = 0;
    char buf[80];
    if (!pszBoundary)				// generate a new boundary delimeter
    {
      typedef unsigned long DWORD_PTR;

        ::srand((uint)(((DWORD_PTR)::time(NULL)) ^ (DWORD_PTR)this));
        ::sprintf(buf, "__=_Part_Boundary_%03d_%06d.%06d", ++s_nPartNumber, rand(), rand());
        if (s_nPartNumber >= 9)
            s_nPartNumber = 0;
        pszBoundary = buf;
    }

    CMimeField *pfd = GetField(CMimeConst::ContentType());
    if (!pfd)
    {
        CMimeField fd;
        fd.SetName(CMimeConst::ContentType());
        fd.SetValue("multipart/mixed");
        fd.SetParameter(CMimeConst::Boundary(), pszBoundary);
        m_listFields.push_back(fd);
    }
    else
    {
        if (::memicmp(pfd->GetValue(), "multipart", 9) != 0)
            pfd->SetValue("multipart/mixed");
        pfd->SetParameter(CMimeConst::Boundary(), pszBoundary);
    }
}

void CMimeHeader::Clear()
{
    m_listFields.clear();
}

// return the length needed to store this header to string buffer
int CMimeHeader::GetLength() const
{
    int nLength = 0;
    list<CMimeField>::const_iterator it;
    for (it = m_listFields.begin(); it != m_listFields.end(); it++)
        nLength += (*it).GetLength();
    return nLength + 2;				// a pair of CRLF indicate the end of header
}

// store the header to string buffer
int CMimeHeader::Store(char* pszData, int nMaxSize) const
{
    if (pszData == NULL)
    {
        return 0;
    }

    int nOutput = 0;
    list<CMimeField>::const_iterator it;
    for (it = m_listFields.begin(); it != m_listFields.end(); it++)
    {
        const CMimeField& fd = *it;
        int nSize = fd.Store(pszData+nOutput, nMaxSize-nOutput);
        if (nSize <= 0)
            return nSize;
        nOutput += nSize;
    }

    pszData[nOutput++] = '\r';		// add CRLF indicating the end of header
    pszData[nOutput++] = '\n';
    return nOutput;
}

// load a header from string buffer
int CMimeHeader::Load(const char* pszData, int nDataSize)
{
    if (pszData == NULL || nDataSize == 0)
    {
        return 0;
    }

    int nInput = 0;
    while (nInput < nDataSize)
    {
        if (pszData[nInput] == 0 || pszData[nInput] == '\r' ||
            pszData[nInput] == '\n')
        {
            break;
        }

        CMimeField fd;
        int nSize = fd.Load(pszData+nInput, nDataSize-nInput);
        if (nSize <= 0)
            return nSize;

        nInput += nSize;

        std::string strValue;
        std::string strName;
        fd.GetValue(strValue,true);
        strName = fd.GetName();
        //std::cout << strName << ": " << strValue << std::endl;


        m_listFields.push_back(fd);	// don't use SetField in case of same name fields
    }

    if (nInput >= nDataSize)
    {
        return nInput;
    }
    else if (pszData[nInput] == 0)
    {
        return 0;
    }
    else if (nInput+1 < nDataSize && pszData[nInput] == '\r' &&
            pszData[nInput+1] == '\n')
    {
        return nInput + 2;			// skip the ending CRLF
    }
    else
    {
        return nInput + 1;			// skip the ending CR or LF
    }
}

list<CMimeField>::const_iterator CMimeHeader::FindField(const char* pszFieldName) const
{
    list<CMimeField>::const_iterator it;
    for (it = m_listFields.begin(); it != m_listFields.end(); it++)
    {
        const CMimeField& fd = *it;

        if (::stricmp(fd.GetName(), pszFieldName))

          break;
    }
    return it;
}

list<CMimeField>::iterator CMimeHeader::FindField(const char* pszFieldName)
{
    list<CMimeField>::iterator it;
    for (it = m_listFields.begin(); it != m_listFields.end(); it++)
    {
        CMimeField& fd = *it;

        if (!::stricmp(fd.GetName(), pszFieldName))
            break;
    }
    return it;
}

//////////////////////////////////////////////////////////////////////
// CMimeBody class - Represents a body part in a MIME message
//////////////////////////////////////////////////////////////////////
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef WIN32
#include <io.h>
#endif

// initialize the content with text
int CMimeBody::SetText(const char* pbText, int nLength/*=0*/)
{
    if (pbText == NULL)
    {
        return -1;
    }

    if (!nLength)
        nLength = (int)::strlen((char*)pbText);

    if (!AllocateBuffer(nLength+4))
        return -1;

    ::memcpy(m_pbText, pbText, nLength);
    m_pbText[nLength] = 0;
    m_nTextSize = nLength;
    return nLength;
}

int CMimeBody::GetText(char* pbText, int nMaxSize)
{
    int nSize = min(nMaxSize, m_nTextSize);
    if (m_pbText != NULL)
        ::memcpy(pbText, m_pbText, nSize);
    return nSize;
}

int CMimeBody::GetText(string& strText)
{
    if (m_pbText != NULL)
        strText.assign((const char*) m_pbText, m_nTextSize);
    return m_nTextSize;
}

// initialize the content of this body part with a mail message
bool CMimeBody::SetMessage(const CMimeMessage* pMM)
{
    if (pMM == NULL)
    {
        return false;
    }

    int nSize = pMM->GetLength();
    if (!AllocateBuffer(nSize+4))
        return false;

    nSize = pMM->Store((char*)m_pbText, nSize);
    m_pbText[nSize] = 0;
    m_nTextSize = nSize;

    const char* pszType = m_mimeHeader.GetContentType();
    if (!pszType || ::memicmp(pszType, "message", 7) != 0)
        m_mimeHeader.SetContentType("message/rfc822");
    //SetTransferEncoding(CMimeConst::EncodingBinary());	// in case the default 7bit cause folding
    return true;
}

void CMimeBody::GetMessage(CMimeMessage* pMM) const
{
    if (pMM != NULL && m_pbText != NULL)
    {
        pMM->Load((const char*)m_pbText, m_nTextSize);
    }
}

// initialize the content (attachment) by reading from a file
bool CMimeBody::ReadFromFile(const char* pszFilename)
{
    int hFile = ::open(pszFilename, O_RDONLY);

    if (hFile < 0)
        return false;

    try
    {
        int nFileSize = (int)::lseek(hFile, 0L, SEEK_END);	// get file length
        ::lseek(hFile, 0L, SEEK_SET);

        FreeBuffer();
        if (nFileSize > 0)
        {
            AllocateBuffer(nFileSize+4);
            unsigned char* pszData = m_pbText;

            for (;;)
            {
                int nRead = ::read(hFile, pszData, 512);
                if (nRead < 0)
                {
                    ::close(hFile);
                    return false;
                }
                pszData += nRead;
                if (nRead < 512)
                    break;
            }
            *pszData = 0;
            m_nTextSize = nFileSize;
        }
    }
    catch (...)
    {
        ::close(hFile);
        throw;
    }

    ::close(hFile);
    const char* pszName = ::strrchr(pszFilename, '\\');
    if (!pszName)
        pszName = pszFilename;
    else
        pszName++;
    m_mimeHeader.SetName(pszName);				// set 'name' parameter:
    return true;
}

// write the content (attachment) to a file
bool CMimeBody::WriteToFile(const char* pszFilename)
{
    if (!m_nTextSize)
        return true;


    int hFile = ::open(pszFilename, O_CREAT | O_TRUNC | O_RDWR , S_IREAD | S_IWRITE);

    if (hFile < 0)
        return false;

    const unsigned char* pszData = m_pbText;
    int nLeft = m_nTextSize;

    try
    {
        for (;;)
        {
            int nWritten = ::write(hFile, pszData, min(512, nLeft));
            if (nWritten <= 0)
            {
                ::close(hFile);
                return false;
            }
            pszData += nWritten;
            nLeft -= nWritten;
            if (nLeft <= 0)
                break;
        }
    }
    catch (...)
    {
        ::close(hFile);
        throw;
    }

    ::close(hFile);
    return true;
}

// delete all child body parts
void CMimeBody::DeleteAll()
{
    while (!m_listBodies.empty())
    {
        CMimeBody* pBP = m_listBodies.back();
        m_listBodies.pop_back();
        if (pBP != NULL)
        {
            delete pBP;					// surely delete because it was allocated by CreatePart()
        }
    }
}

// create a new child body part, and add it to body part list
CMimeBody* CMimeBody::CreatePart(CMimeBody* pWhere/*=NULL*/)
{
    CMimeBody* pBP = new CMimeBody;
    if (pBP == NULL)
    {
        return NULL;
    }

    if (pWhere != NULL)
    {
         for (CBodyList::iterator it = m_listBodies.begin(); it != m_listBodies.end(); it++)
            if (*it == pWhere)
            {
                m_listBodies.insert(it, pBP);
                return pBP;
            }
    }
    m_listBodies.push_back(pBP);
    return pBP;
}

// remove and delete a child body part
void CMimeBody::ErasePart(CMimeBody* pBP)
{
    if (pBP != NULL)
    {
        m_listBodies.remove(pBP);
        delete pBP;
    }
}

// return a list of all child body parts belong to this body part
int CMimeBody::GetBodyPartList(CBodyList& rList) const
{
    int nCount = 0;
    int nMediaType = m_mimeHeader.GetMediaType();

    if (CMimeHeader::MEDIA_MULTIPART != nMediaType)
    {
        rList.push_back((CMimeBody*)this);
        nCount++;
    }
    else
    {
        list<CMimeBody*>::const_iterator it;
        for (it=m_listBodies.begin(); it!=m_listBodies.end(); it++)
        {
            CMimeBody* pBP = *it;
            if (pBP == NULL)
            {
                continue;
            }
            nCount += pBP->GetBodyPartList(rList);
        }
    }
    return nCount;
}

// return a list of all attachment body parts belong to this body part
int CMimeBody::GetAttachmentList(CBodyList& rList) const
{
    int nCount = 0;
    int nMediaType = m_mimeHeader.GetMediaType();

    if (CMimeHeader::MEDIA_MULTIPART != nMediaType)
    {
        string strName = m_mimeHeader.GetName();
        if (strName.size() > 0)
        {
            rList.push_back((CMimeBody*)this);
            nCount++;
        }
    }
    else
    {
        list<CMimeBody*>::const_iterator it;
        for (it=m_listBodies.begin(); it!=m_listBodies.end(); it++)
        {
            CMimeBody* pBP = *it;
            if (pBP == NULL)
            {
                continue;
            }
            nCount += pBP->GetAttachmentList(rList);
        }
    }
    return nCount;
}

void CMimeBody::Clear()
{
    DeleteAll();
    m_itFind = m_listBodies.end();
    FreeBuffer();
    m_mimeHeader.Clear();
}

// return the length needed to store this body part to string buffer
int CMimeBody::GetLength() const
{
    int nLength = m_mimeHeader.GetLength();
    CMimeCodeBase* pCoder = CMimeEnvironment::CreateCoder(m_mimeHeader.GetTransferEncoding());
    if (pCoder == NULL)
    {
        return -1;
    }

    pCoder->SetInput((const char*)m_pbText, m_nTextSize, true);
    nLength += pCoder->GetOutputLength();
    delete pCoder;

    if (m_listBodies.empty())
        return nLength;

    string strBoundary = m_mimeHeader.GetBoundary();
    int nBoundSize = (int) strBoundary.size();
    list<CMimeBody*>::const_iterator it;
    for (it=m_listBodies.begin(); it!=m_listBodies.end(); it++)
    {
        nLength += nBoundSize + 6;	// include 2 leading hyphens and 2 pair of CRLFs
        CMimeBody* pBP = *it;
        if (pBP == NULL)
        {
            continue;
        }
        nLength += pBP->GetLength();
    }
    nLength += nBoundSize + 8;		// include 2 leading hyphens, 2 trailng hyphens and 2 pair of CRLFs
    return nLength;
}

// store the body part to string buffer
int CMimeBody::Store(char* pszData, int nMaxSize) const
{
    // store header fields
    int nSize = m_mimeHeader.Store(pszData, nMaxSize);
    if (nSize <= 0)
        return nSize;

    // store content
    char* pszDataBegin = pszData;	// preserve start position
    pszData += nSize;
    nMaxSize -= nSize;

    CMimeCodeBase* pCoder = CMimeEnvironment::CreateCoder(m_mimeHeader.GetTransferEncoding());
    if (pCoder == NULL)
    {
        return nSize;
    }

    pCoder->SetInput((const char*)m_pbText, m_nTextSize, true);
    int nOutput = pCoder->GetOutput((unsigned char*)pszData, nMaxSize);
    delete pCoder;
    if (nOutput < 0)
        return nOutput;

    pszData += nOutput;
    nMaxSize -= nOutput;
    if (m_listBodies.empty())
        return (int)(pszData - pszDataBegin);

    // store child body parts
    string strBoundary = m_mimeHeader.GetBoundary();
    if (strBoundary.empty())
        return -1;					// boundary not be set

    int nBoundSize = (int)strBoundary.size() + 6;
    for (CBodyList::const_iterator it=m_listBodies.begin(); it!=m_listBodies.end(); it++)
    {
        if (nMaxSize < nBoundSize)
            break;
        CMimeBody* pBP = *it;
        if (pBP == NULL)
        {
            continue;
        }

        if (m_listBodies.begin() == it && *(pszData-2) == '\r' && *(pszData-1) == '\n')
        {
            pszData -= 2;
            nMaxSize += 2;
        }
        ::sprintf(pszData, "\r\n--%s\r\n", strBoundary.c_str());
        pszData += nBoundSize;
        nMaxSize -= nBoundSize;

        nOutput = pBP->Store(pszData, nMaxSize);
        if (nOutput < 0)
            return nOutput;
        pszData += nOutput;
        nMaxSize -= nOutput;
    }

    if (nMaxSize >= nBoundSize+2)	// add closing boundary delimiter
    {
        ::sprintf(pszData, "\r\n--%s--\r\n", strBoundary.c_str());
        pszData += nBoundSize + 2;
    }
    return (int)(pszData - pszDataBegin);
}

// load a body part from string buffer
int CMimeBody::Load(const char* pszData, int nDataSize)
{
    if (pszData == NULL || nDataSize == 0)
    {
        return -1;
    }

    // load header fields
    int octet = 0;
    int nSize = m_mimeHeader.Load(pszData, nDataSize);


    if (nSize <= 0 || nSize >= nDataSize)
        return nSize;

    // determine the length of the content
    FreeBuffer();
    octet += nSize;
    nSize = nDataSize - octet;
    string strBoundary;
    int nCRLF = 0;
    int nMediaType = m_mimeHeader.GetMediaType();
    if (CMimeHeader::MEDIA_MULTIPART == nMediaType)
    {
        // find the begin boundary
        strBoundary = m_mimeHeader.GetBoundary();
        if (!strBoundary.empty())
        {
            if (octet >= 2 && pszData[octet-2] == '\r' && pszData[octet-1] == '\n')
            {
                strBoundary = "\r\n--" + strBoundary;
                int pos = ::FindSubString(pszData+octet-2, strBoundary, nDataSize-octet+2);
                if (pos != -1)
                {
                    nCRLF = 2;
                    nSize = pos;
                }
            }
            else if (pszData[octet-1] == '\n')
            {
                strBoundary = "\n--" + strBoundary;
                int pos = ::FindSubString(pszData+octet-1, strBoundary, nDataSize-octet+1);
                if (pos != -1)
                {
                    nCRLF = 1;
                    nSize = pos;
                }
            }
            else if (octet >= 2 && pszData[octet-2] == '\r' && pszData[octet-1] == '\r')
            {//  entourage database format
                strBoundary = "\r\r--" + strBoundary;
                int pos = ::FindSubString(pszData+octet-2, strBoundary, nDataSize-octet+2);
                if (pos != -1)
                {
                    nCRLF = 2;
                    nSize = pos;
                }
            }
            else
            {
                return 0;
            }
        }
    }

    // load content
    if (nSize > 0)
    {
      const char * pzCodeing = m_mimeHeader.GetTransferEncoding();

        CMimeCodeBase* pCoder = CMimeEnvironment::CreateCoder(pzCodeing);
        if (pCoder == NULL)
        {
            return octet;
        }

        pCoder->SetInput(pszData+octet, nSize, false);
        int nOutput = pCoder->GetOutputLength();
        if (AllocateBuffer(nOutput+4))
            nOutput = pCoder->GetOutput(m_pbText, nOutput);
        else
            nOutput = -1;
        delete pCoder;
        if (nOutput < 0)
            return nOutput;

        if (nOutput >= m_nTextSize)
        {
            return octet;
        }
        m_pbText[nOutput] = 0;
        m_nTextSize = nOutput;

        //std::cout << "pbText: " << m_pbText << std::endl;


        CMimeHeader & mimeHeader = GetMimeHeader();
        string strCharset = mimeHeader.GetCharset();

        if (stricmp(strCharset.c_str(), "gb2312") == 0)
        {

        }

        octet += nSize;
    }



    if (strBoundary.empty() || octet >= nDataSize)
        return octet;

    // load child body parts
    // look for the first boundary (case sensitive)
    octet -= nCRLF;					// go back to CRLF
    int nBound1 = ::FindSubString(pszData+octet, strBoundary, nDataSize-octet);
    while (nBound1 != -1 && octet + nBound1 < nDataSize)
    {
        octet += nBound1;
        int nStart = ::FindSubString(pszData+octet+nCRLF, strBoundary.substr(0, nCRLF), nDataSize-octet-nCRLF);
        if (nStart == -1)
            break;
        nStart += nCRLF;
        if (pszData[octet+strBoundary.size()] == '-' && pszData[octet+strBoundary.size()+1] == '-')
            return octet+nCRLF+nStart;	// reach the closing boundary

        // look for the next boundary
        octet += (nCRLF + nStart);
        int nEntitySize = ::FindSubString(pszData+octet, strBoundary, nDataSize-octet);
        if (nEntitySize == -1)				// overflow, boundary may be truncated
            nEntitySize = nDataSize - octet;

        // create child body part:
        CMimeBody* pBP = CreatePart();

        int nInputSize = pBP->Load(pszData+octet, nEntitySize);
        if (nInputSize < 0)
        {
            ErasePart(pBP);
            return nInputSize;
        }
        nBound1 = nEntitySize;
    }

    return nDataSize;
}

//////////////////////////////////////////////////////////////////////
// CMimeMessage - Represents a MIME message
//////////////////////////////////////////////////////////////////////

CMimeMessage::CMimeMessage()
{
    m_pRootBody = new CMimeBody;
}

CMimeMessage:: ~CMimeMessage()
{
    Clear();
    if (m_pRootBody != NULL)
    {
        delete m_pRootBody;
    }
}

// clear the message , reset it
void CMimeMessage::Clear()
{
    if (m_pRootBody != NULL)
    {
        m_pRootBody->Clear();
    }
}

// get the length of message
int CMimeMessage::GetLength() const
{
    if (m_pRootBody == NULL)
    {
        return -1;
    }

    return m_pRootBody->GetLength();
}

// set the value and charset of field
void CMimeMessage::SetFieldValue(const string & strFieldName, const string & strFieldValue, const string & strCharset)
{
    if (m_pRootBody != NULL)
    {
        CMimeHeader & mimeHeader = m_pRootBody->GetMimeHeader();
        mimeHeader.SetFieldValue(strFieldName.c_str(), strFieldValue.c_str(), strCharset.c_str());
    }
}

// get the value of field
void CMimeMessage::GetFieldValue(const string & strFieldName, string & strFieldValue) const
{
    if (m_pRootBody != NULL)
    {
        CMimeHeader & mimeHeader = m_pRootBody->GetMimeHeader();
        mimeHeader.GetFieldValue(strFieldName.c_str(), strFieldValue);
    }
    else
    {
        strFieldValue = "";
    }
}

void CMimeMessage::GetReceived(string & strReceived) const
{
    if (m_pRootBody != NULL)
    {
        CMimeHeader & mimeHeader = m_pRootBody->GetMimeHeader();
        mimeHeader.GetFieldValue("Received", strReceived, true);
    }
    else
    {
        strReceived = "";
    }
}

// get the plain text of message
void CMimeMessage::GetPlainText(string & strPlainText) const
{
    if (m_pRootBody != NULL)
    {
        CMimeBody::CBodyList blist;
        m_pRootBody->GetBodyPartList(blist);
        CMimeBody::CBodyList::iterator it;
        for (it=blist.begin(); it!=blist.end(); it++)
        {
            CMimeBody * pmb = *it;
            if (pmb == NULL)
            {
                continue;
            }

            if (pmb->IsText())
            {
                CMimeHeader & mimeHeader = pmb->GetMimeHeader();
                if (stricmp(mimeHeader.GetSubType().c_str(), "plain") != 0)
                {
                    pmb->GetText(strPlainText);
                    break;
                }
            }
        }
    }
}

// get the html text of message
void CMimeMessage::GetHtmlText(string & strHtmlText) const
{
    if (m_pRootBody != NULL)
    {
        CMimeBody::CBodyList blist;
        m_pRootBody->GetBodyPartList(blist);
        CMimeBody::CBodyList::iterator it;
        for (it=blist.begin(); it!=blist.end(); it++)
        {
            CMimeBody * pmb = *it;
            if (pmb == NULL)
            {
                continue;
            }

            if (pmb->IsText())
            {
                CMimeHeader & mimeHeader = pmb->GetMimeHeader();
                if (stricmp(mimeHeader.GetSubType().c_str(), "html") == 0)
                {
                    pmb->GetText(strHtmlText);
                    break;
                }
            }
        }
    }
}

// get the list of attachment, save the name and size (pair<string, int>) of attachment
void CMimeMessage::GetAttachmentList(vector< pair<string, int> > & arrAttList) const
{
    if (m_pRootBody != NULL)
    {
        CMimeBody::CBodyList blist;
        m_pRootBody->GetBodyPartList(blist);
        CMimeBody::CBodyList::iterator it;
        for (it=blist.begin(); it!=blist.end(); it++)
        {
            CMimeBody * pmb = *it;
            if (pmb == NULL)
            {
                continue;
            }

            // if the body is message, we consider it as attachment
            if (pmb->IsMessage())
            {
                // we set the name of message as "message"
                CMimeHeader & mimeHeader = pmb->GetMimeHeader();
                std::string strMsgName = mimeHeader.GetName();
                if (strMsgName.empty())
                {
                    CMimeMessage msg;
                    pmb->GetMessage(&msg);
                    string strSubject;
                    msg.GetSubject(strSubject);
                    if (!strSubject.empty())
                    {
                        string strName = strSubject + ".eml";
                        arrAttList.push_back(make_pair(strName, pmb->GetContentLength()));
                    }
                    else
                    {
                        arrAttList.push_back(make_pair("message.eml", pmb->GetContentLength()));
                    }
                }
                else
                {
                    arrAttList.push_back(make_pair(strMsgName, pmb->GetContentLength()));
                }
            }
            else if (pmb->IsAttachment())
            {
                CMimeHeader & mimeHeader = pmb->GetMimeHeader();
                arrAttList.push_back(make_pair(mimeHeader.GetName(), pmb->GetContentLength()));
            }
        }
    }
}

// export the attachment to full path
bool CMimeMessage::ExportAttachment(const string & strFullPath, unsigned int index)
{
    if (m_pRootBody == NULL)
    {
        return false;
    }

    uint attIndex = 0;
    CMimeBody::CBodyList blist;
    m_pRootBody->GetBodyPartList(blist);
    CMimeBody::CBodyList::iterator it;
    for (it=blist.begin(); it!=blist.end(); it++)
    {
        CMimeBody * pmb = *it;
        if (pmb == NULL)
        {
            continue;
        }

        if (pmb->IsAttachment() || pmb->IsMessage())
        {
            if (index == attIndex)
            {
                return pmb->WriteToFile(strFullPath.c_str());
            }
            attIndex++;
        }
    }

    return false;
}

// set the child body of message (eg. plain text. html text. attachment), note: attachment must be a file
bool CMimeMessage::SetChildBody(const string & strPlainText, const string & strHtmlText, const vector<string> & arrAttPath)
{
    if (m_pRootBody == NULL)
    {
        return false;
    }

    bool bHasAtt = arrAttPath.size() > 0;
    // Initialize root body part header
    CMimeHeader & mimeHeader = m_pRootBody->GetMimeHeader();
    if (bHasAtt)
    {
        mimeHeader.SetContentType("multipart/mixed");
    }
    else
    {
        mimeHeader.SetContentType("multipart/alternative");
    }
    mimeHeader.SetBoundary(NULL);

    // Add an embeded multipart
    if (!strHtmlText.empty())
    {
        CMimeBody * pmb = NULL;
        if (bHasAtt)
        {
            pmb = m_pRootBody->CreatePart();
            if (pmb == NULL)
            {
                return false;
            }

            // Initialize embeded body part header
            CMimeHeader & header = pmb->GetMimeHeader();
            header.SetContentType("multipart/alternative");
            header.SetBoundary(NULL);
        }
        else
        {
            pmb = m_pRootBody;
        }

        if (!strPlainText.empty())
        {
            CMimeBody * pPlainCild = pmb->CreatePart();
            if (pPlainCild == NULL)
            {
                return false;
            }

            // Initialize plain text body part header
            CMimeHeader & plainHeader = pPlainCild->GetMimeHeader();
            plainHeader.SetContentType("text/plain");
            plainHeader.SetCharset("gb2312");
            plainHeader.SetTransferEncoding("base64");
            pPlainCild->SetText(strPlainText.c_str());
        }

        CMimeBody * pHtmlCild = pmb->CreatePart();
        if (pHtmlCild == NULL)
        {
            return false;
        }

        // Initialize html text body part header
        CMimeHeader & htmlHeader = pHtmlCild->GetMimeHeader();
        htmlHeader.SetContentType("text/html");
        htmlHeader.SetCharset("gb2312");
        htmlHeader.SetTransferEncoding("base64");
        pHtmlCild->SetText(strHtmlText.c_str());
    }
    else
    {
        if (!strPlainText.empty())
        {
            CMimeBody * pmb = m_pRootBody->CreatePart();
            if (pmb == NULL)
            {
                return false;
            }

            CMimeHeader & header = pmb->GetMimeHeader();
            header.SetContentType("text/plain");
            header.SetCharset("gb2312");
            header.SetTransferEncoding("base64");
            pmb->SetText(strPlainText.c_str());
        }
    }

    // Add the file attachment body part
    if (bHasAtt)
    {
        for (size_t i=0; i<arrAttPath.size(); i++)
        {
            CMimeBody * pmb = m_pRootBody->CreatePart();
            if (pmb == NULL)
            {
                return false;
            }

            CMimeHeader & header = pmb->GetMimeHeader();
            header.SetTransferEncoding("base64");
            pmb->ReadFromFile(arrAttPath[i].c_str());
        }
    }

    return true;
}

// export the message to the full path, before call it,
// you must call setfield and setchildbody, not call CMimeMessage::Load() function
bool CMimeMessage::ExportMessage(const string & strFullPath)
{
    if (m_pRootBody == NULL)
    {
        return false;
    }

    // fold the long lines in the headers and set global charset as "gb2321"
    CMimeEnvironment::SetAutoFolding(true);
    CMimeEnvironment::SetGlobalCharset("gb2312");
    int bufSize = GetLength();
    if (bufSize > 0 && bufSize <= MESSAGE_MAX_SIZE)
    {
        bufSize += 4;
        char * msgBuff = new char[bufSize];
        int msgLen = Store(msgBuff, bufSize);
        if (msgLen > 0 && msgLen <= bufSize)
        {
            ofstream msgFile;
            msgFile.open(strFullPath.c_str(), ios_base::out | ios_base::binary);
            if (msgFile.is_open())
            {
                msgFile.write(msgBuff, msgLen);
                msgFile.close();
                delete [] msgBuff;
                return true;
            }
        }
        delete [] msgBuff;
    }

    return false;
}

void CMimeMessage::SetDate()
{
    time_t timeNow = ::time(NULL);
    struct tm *ptm = ::localtime(&timeNow);
    SetDate(ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
}

void CMimeMessage::SetDate(int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond)
{
    static const char* s_MonthNames[] =
        { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
    static const char* s_DayNames[] =
        { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

    struct tm tmDate;
    ::memset(&tmDate, 0, sizeof(tmDate));
    tmDate.tm_year = nYear - 1900;
    tmDate.tm_mon = nMonth - 1;
    tmDate.tm_mday = nDay;
    tmDate.tm_hour = nHour;
    tmDate.tm_min = nMinute;
    tmDate.tm_sec = nSecond;
    tmDate.tm_isdst = -1;

    time_t timeDate = ::mktime(&tmDate);
    if (timeDate < 0)
    {
        return;
    }

    tmDate = *::localtime(&timeDate);			// adjusted local time
    struct tm *ptmGmt = ::gmtime(&timeDate);	// Greenwich Mean Time
    long nTimeDiff = tmDate.tm_mday - ptmGmt->tm_mday;
    if (nTimeDiff > 1)
        nTimeDiff = -1;
    else if (nTimeDiff < -1)
        nTimeDiff = 1;
    nTimeDiff *= 60 * 24;
    nTimeDiff +=
        (tmDate.tm_hour - ptmGmt->tm_hour) * 60 +
        tmDate.tm_min - ptmGmt->tm_min;
    if (tmDate.tm_isdst > 0)
        nTimeDiff -= 60;

    char szDate[40];
    if (tmDate.tm_wday >= 7 || tmDate.tm_mon >= 12)
    {
        return;
    }
    ::sprintf(szDate, "%s, %d %s %d %02d:%02d:%02d %c%02d%02d",
        s_DayNames[tmDate.tm_wday],
        tmDate.tm_mday, s_MonthNames[tmDate.tm_mon], tmDate.tm_year+1900,
        tmDate.tm_hour, tmDate.tm_min, tmDate.tm_sec,
        (nTimeDiff >= 0 ? '+' : '-'), abs(nTimeDiff / 60), abs(nTimeDiff % 60));

    SetFieldValue("Date", szDate);
}

int CMimeMessage::Store(char* pszData, int nMaxSize) const
{
    if (m_pRootBody == NULL)
    {
        return -1;
    }

    return m_pRootBody->Store(pszData, nMaxSize);
}

int CMimeMessage::Load(const char* pszData, int nDataSize)
{
    if (m_pRootBody == NULL)
    {
        return -1;
    }

    return m_pRootBody->Load(pszData, nDataSize);
}
