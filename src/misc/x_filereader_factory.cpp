#include "x_filereader_factory.h"
#include "x_string.h"

int CFileReaderFactory::RegisterFileReader(const char *fileReaderType, J_MakeFileReaderFun pFun)
{
	FileReaderRegistMap::iterator it = m_fileReaderRegistMap.find(fileReaderType);
	if (it == m_fileReaderRegistMap.end())
	{
		m_fileReaderRegistMap[fileReaderType] = pFun;
		J_OS::LOGINFO("CFileReaderFactory::RegisterFilter %s Regist Sucess!!!", fileReaderType);

		return J_OK;
	}

	return J_EXIST;
}

J_FileReader *CFileReaderFactory::GetFileReader(int nSocket, const char *pType, const char *pResid)
{
	TLock(m_lock);
	J_Obj *fileReader = NULL;
	FileReaderMap::iterator it = m_fileReaderMap.find(nSocket);
	if (it == m_fileReaderMap.end())
	{
		FileReaderRegistMap::iterator itFileReader = m_fileReaderRegistMap.find(pType);
		if (itFileReader == m_fileReaderRegistMap.end())
		{
			J_OS::LOGINFO("CFileReaderFactory::GetFileReader FileReader not registed, fileReaderType = %s", pType);
		}
		else
		{
			itFileReader->second(fileReader, pResid);
		}
		
		if (fileReader != NULL)
			m_fileReaderMap[nSocket] = dynamic_cast<J_FileReader *>(fileReader);	}
	else
		fileReader = it->second;

	TUnlock(m_lock);

	return dynamic_cast<J_FileReader *>(fileReader);
}

void CFileReaderFactory::DelFileReader(int nSocket)
{
	TLock(m_lock);
	FileReaderMap::iterator it = m_fileReaderMap.find(nSocket);
	if (it != m_fileReaderMap.end())
	{
		delete it->second;
		m_fileReaderMap.erase(it);
	}
	TUnlock(m_lock);
}
