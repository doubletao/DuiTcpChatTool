#include "StdAfx.h"
#include "GlobalFunction.h"
#include "imagehlp.h"
#include <vector>
#include <sstream>
using namespace std;

#pragma comment(lib, "Rpcrt4.lib")
#pragma comment(lib, "imagehlp.lib")

CString CGlobalFunction::GetNewGUID()
{
	GUID guid = GUID_NULL;
	::CoCreateGuid(&guid);
	CString strGUID;
	GuidToString( guid, strGUID );
	strGUID.MakeUpper();
	return strGUID;
}

HRESULT CGlobalFunction::GuidToString( const GUID& guid, CString& str )
{
	GUID guidNo = guid;
#ifdef UNICODE
	RPC_WSTR pUuidString;
#else
	BYTE* pUuidString;
#endif
	UuidToString(&guidNo, &pUuidString);
	str =(LPCTSTR)pUuidString;
	str.MakeUpper();

	if (RPC_S_OK == RpcStringFree(&pUuidString))
		return S_OK;

	return S_FALSE;
}

void CGlobalFunction::GetPathAndName( const CString &strSrcFilePath, CString &strPath, CString &strName )
{
	strPath.Empty();
	strName.Empty();

	int nPos = strSrcFilePath.ReverseFind(_T('\\'));
	if(nPos < 0)
	{
		//δ���ַ�б��ʱ������ֻ���ļ�������·�������Լ�����ȡ�ļ���
		strPath.Empty();
	}
	else
	{
		strPath = strSrcFilePath.Left(nPos);
		strName = strSrcFilePath.Right(strSrcFilePath.GetLength() - nPos - 1);
	}

	nPos = strName.ReverseFind(_T('.'));
	if(nPos < 0)
	{
		//δ���ֵ��(.)����Ϊ�޺�׺���ļ�����������
	}
	else
	{
		strName = strName.Left(nPos);
	}
}

CString CGlobalFunction::GetUnDuplicateFileName( const CString &strSrcFilePath )
{
	CString strName = GetFileNameWithExt(strSrcFilePath);
	CString strPath = GetFilePath(strSrcFilePath);
	CString strPureName = GetFileName(strName);
	int nCount = 1;
	while(IsFileExists(strPath + _T("\\") + strName))
	{
		CString strExt = GetFileNameExt(strName);
		strName.Format(_T("%s(%d)%s"), strPureName, nCount++, strExt);
	}
	return strPath + _T("\\") + strName;
}

BOOL CGlobalFunction::IsFileExists( const CString &strSrcFilePath )
{
	if(strSrcFilePath.IsEmpty())
		return FALSE;

	return  (_taccess(strSrcFilePath, 00) == 0);
}

std::string CGlobalFunction::ConverCStringToStdString( CString & cstrValue )
{
	string strRet;
	strRet.clear();
	int nSize = ::WideCharToMultiByte(CP_UTF8, 0, cstrValue.GetBuffer(), -1, NULL, 0, NULL, NULL);
	char * szcSrc = new char[nSize];
	memset(szcSrc, 0, sizeof(char) * nSize);
	::WideCharToMultiByte(CP_UTF8, 0, cstrValue.GetBuffer(), -1, szcSrc, nSize, NULL, NULL);
	strRet = szcSrc;
	delete[] szcSrc;
	szcSrc = NULL;
	return strRet;
}

std::string CGlobalFunction::ConverCStringToStdString( CString & cstrValue, DWORD dwFormatW)
{
	string strRet;
	strRet.clear();
	int nSize = ::WideCharToMultiByte(dwFormatW, 0, cstrValue.GetBuffer(), -1, NULL, 0, NULL, NULL);
	char * szcSrc = new char[nSize];
	memset(szcSrc, 0, sizeof(char) * nSize);
	::WideCharToMultiByte(dwFormatW, 0, cstrValue.GetBuffer(), -1, szcSrc, nSize, NULL, NULL);
	strRet = szcSrc;
	delete[] szcSrc;
	szcSrc = NULL;
	return strRet;
}

CString CGlobalFunction::ConvertStdStringToCString( const string & strValue )
{
	CString cstrRet;
	cstrRet.Empty();
	int nLength = ::MultiByteToWideChar(CP_UTF8, 0, strValue.c_str(), -1, NULL,0);
	TCHAR * szText1 = new TCHAR[nLength+1];
	::MultiByteToWideChar(CP_UTF8, 0, strValue.c_str(), -1, szText1, nLength+1);
	cstrRet = szText1;
	delete[] szText1;
	szText1 = NULL;
	return cstrRet;
}

CString CGlobalFunction::GetFileName( CString strFullFileName )
{
	CString strFileName,strFileTemp;
	int nLength = strFullFileName.GetLength();
	int nIndex = strFullFileName.ReverseFind('\\');
	strFileTemp = strFullFileName.Mid(nIndex + 1);
	strFileName = strFileTemp;
	int nPos = strFileTemp.ReverseFind(_T('.'));
	if (nPos >= 0 && nPos < strFileTemp.GetLength())
	{
		strFileName = strFileTemp.Mid(0, nPos);
	}
	return strFileName;
}

CString CGlobalFunction::GetFileNameExt( const CString strSrcFileName )
{
	int nPos = strSrcFileName.ReverseFind(_T('.'));
	if(nPos < 0)
		return _T("");

	CString strExt = strSrcFileName.Right(strSrcFileName.GetLength() - nPos);
	return strExt;
}

CString CGlobalFunction::GetFileNameWithExt( CString strFullFileName )
{
	CString strFileName;
	int nLength = strFullFileName.GetLength();
	int nIndex = strFullFileName.ReverseFind('\\');
	strFileName = strFullFileName.Mid(nIndex + 1);

	return strFileName;
}

CString CGlobalFunction::FMT( LPCTSTR lpszFormat, ... )
{
	CString str;

	va_list argList;
	va_start(argList, lpszFormat);
	str.FormatV(lpszFormat, argList);
	va_end(argList);

	return str;
}

BOOL CGlobalFunction::MakeSureDirectoryExists( CString strFilePath, CString & strErrInfo )
{
	BOOL bRet = FALSE;
	CString strPath;
	CString strName;
	GetPathAndName(strFilePath, strPath, strName);

	if (strPath.IsEmpty())
	{
		//�����������·�����������ǰĿ¼Ҳ��Ҫ��./
		return FALSE;
	}

	if (strPath == _T("."))
	{
		//���·����.�ţ��϶��ǵ�ǰ·��������ֱ�ӷ��سɹ�
		return TRUE;
	}

	//����ļ��в����ڣ��򴴽�
	//�����".\"��ͷ�����ȡ��".\"�滻Ϊ��ǰ·��
	if (strPath.Left(2) == _T(".\\"))
	{
		strPath = strPath.Mid(2);
		CString strCurName;
		GetModuleFileName(NULL, strCurName.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
		strCurName.ReleaseBuffer();
		CString strCurPath;
		CString strCurPurName;
		GetPathAndName(strCurName, strCurPath, strCurPurName);
		strCurPurName = strPath;
		strPath.Format(_T("%s\\%s"), strCurPath, strCurPurName);
	}
	//ȷ����'\'��β�Դ������һ��Ŀ¼
	if (strPath.GetAt(strPath.GetLength() - 1) != _T('\\'))
	{
		strPath.AppendChar(_T('\\'));
	}
	std::vector<CString> vPath;//���ÿһ��Ŀ¼�ַ���
	CString strTemp;//һ����ʱ����,���Ŀ¼�ַ���
	//����Ҫ�������ַ���
	for (int i = 0;i < strPath.GetLength(); ++i)
	{
		if (strPath.GetAt(i) != _T('\\'))
		{
			//�����ǰ�ַ�����'\\'
			strTemp.AppendChar(strPath.GetAt(i));
		}
		else
		{
			//�����ǰ�ַ���'\\'
			vPath.push_back(strTemp);//����ǰ����ַ�����ӵ�������
			strTemp.AppendChar(_T('\\'));
		}
	}

	//�������Ŀ¼������,����ÿ��Ŀ¼
	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++)
	{
		//���CreateDirectoryִ�гɹ�,����true,���򷵻�false
		bRet = CreateDirectory(*vIter, NULL) ? true : false;
		if (!bRet)
		{
			int nErr = GetLastError();
			if (nErr == 183)
			{
				//�����183˵��·���Ѿ��������贴���������������Ϊ����
				bRet = TRUE;
				continue;
			}
			else if (nErr == 5)
			{
				//�����5˵���ܾ����ʣ��п�������ͼ������Ŀ¼���µģ�����Ϊ����
				if (vIter == vPath.begin())
				{
					bRet = TRUE;
					continue;
				}
			}
			else
			{
				LPVOID lpMsgBuf;
				FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					GetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf,
					0,
					NULL
					);

				// Display the string.
				strErrInfo = (LPTSTR)lpMsgBuf;

				// Free the buffer.
				LocalFree( lpMsgBuf );
				break;
			}
		}
	}

	return bRet;
}

CString CGlobalFunction::GetSmartFileSize( unsigned long long llFileSize )
{
	CString strValue = _T("");
	double lfFileSize = (double)llFileSize;
	lfFileSize /= 1024;
	if (lfFileSize < 1024)
		strValue.Format(_T("%0.2f KB"), lfFileSize);
	else {
		lfFileSize /= 1024;
		if (lfFileSize < 1024)
			strValue.Format(_T("%0.2f MB"), lfFileSize);
		else {
			lfFileSize /= 1024;
			strValue.Format(_T("%0.2f GB"), lfFileSize);
		}
	}
	return strValue;
}

CString CGlobalFunction::GetFilePath( CString strFullFileName )
{
	CString strPath;
	strPath.Empty();

	int nPos = strFullFileName.ReverseFind(_T('\\'));
	if(nPos < 0)
		return strPath;

	strPath = strFullFileName.Left(nPos);

	return strPath;
}

CString CGlobalFunction::GetCurPath()
{
	CString strCurName;
	GetModuleFileName(NULL, strCurName.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	strCurName.ReleaseBuffer();
	CString strCurPath;
	CString strCurPurName;
	GetPathAndName(strCurName, strCurPath, strCurPurName);
	return strCurPath;
}

CString CGlobalFunction::GetNameOfComputer()
{
	const int BUFFERSIZE = 80;
	DWORD  bufCharCount = BUFFERSIZE;
	TCHAR buffer[BUFFERSIZE];
	if(!::GetComputerName(buffer, &bufCharCount))
		return _T("Unknown");
	else
		return buffer;
}

CString CGlobalFunction::ConvertNumToCString(int nValue)
{
	CString strRet;
	strRet.Format(_T("%d"), nValue);
	return strRet;
}

CString CGlobalFunction::ConvertNumToCString(float fValue)
{
	CString strRet;
	strRet.Format(_T("%.2f"), fValue);
	return strRet;
}

CString CGlobalFunction::ConvertNumToCString(double dValue)
{
	CString strRet;
	strRet.Format(_T("%.2lf"), dValue);
	return strRet;
}

CString CGlobalFunction::ConvertNumToCString(DWORD dwValue)
{
	CString strRet;
	strRet.Format(_T("%d"), dwValue);
	return strRet;
}

int CGlobalFunction::ConvertCStringToInt(CString strValue)
{
	int nRet = _ttoi(strValue);
	return nRet;
}

float CGlobalFunction::ConvertCStringToFloat(CString strValue)
{
	float fRet = (float)_ttof(strValue);
	return fRet;
}

double CGlobalFunction::ConvertCStringToDouble(CString strValue)
{
	double dRet = _ttof(strValue);
	return dRet;
}

int CGlobalFunction::ConvertStdSringToInt(string & strValue)
{
	int nRet = 0;
	std::stringstream streamTmp;
	streamTmp<<strValue;
	streamTmp>>nRet;
	return nRet;
}

DWORD CGlobalFunction::ConvertStdSringToDWORD(string & strValue)
{
	DWORD dwRet = 0;
	if (strValue.substr(0, 2) == "0x")
	{
		std::stringstream streamTmp;
		streamTmp<<std::hex<<strValue;
		streamTmp>>dwRet;
	}
	else
	{
		std::stringstream streamTmp;
		streamTmp<<strValue;
		streamTmp>>dwRet;
	}
	return dwRet;
}

float CGlobalFunction::ConvertStdStringToFloat(string & strValue)
{
	float fRet = 0.0f;
	std::stringstream streamTmp;
	streamTmp<<strValue;
	streamTmp>>fRet;
	return fRet;
}

double CGlobalFunction::ConvertStdStringToDouble(string & strValue)
{
	double dRet = 0.0f;
	std::stringstream streamTmp;
	streamTmp<<strValue;
	streamTmp>>dRet;
	return dRet;
}

std::vector<string> CGlobalFunction::SplitStdString(string & strValue, std::set<char> & setMark, BOOL bIncludeMark)
{
	int nCutter = 0;
	if (!bIncludeMark)
	{
		nCutter = 1;
	}
	std::vector<string> vecStrRet;
	UINT nLastPos = 0;
	for (UINT i = 0; i < strValue.size(); i++)
	{
		//ͨ��mark�����Ͼ䲢���뵽vec��
		if (setMark.count(strValue[i]) > 0)
		{
			if (i > nLastPos)
			{
				//�ҵ�һ������vec
				std::string strPos = strValue.substr(nLastPos, i - nLastPos);
				if (!strPos.empty())
				{
					vecStrRet.push_back(strPos);
				}
			}
			nLastPos = i + nCutter;
		}
	}
	//���һ���ָ�������β������ҲӦ�÷���
	if (nLastPos < strValue.size())
	{
		std::string strPos = strValue.substr(nLastPos, strValue.size() - nLastPos);
		if (!strPos.empty())
		{
			vecStrRet.push_back(strPos);
		}
	}
	return vecStrRet;
}

std::vector<CString> CGlobalFunction::SplitCString(CString & strValue, std::set<TCHAR> & setMark, BOOL bIncludeMark)
{
	int nCutter = 0;
	if (!bIncludeMark)
	{
		nCutter = 1;
	}
	std::vector<CString> vecStrRet;
	int nLastPos = 0;
	for (int i = 0; i < strValue.GetLength(); i++)
	{
		//ͨ��mark�����Ͼ䲢���뵽vec��
		if (setMark.count(strValue[i]) > 0)
		{
			if (i > nLastPos)
			{
				//�ҵ�һ������vec
				CString strPos = strValue.Mid(nLastPos, i - nLastPos);
				if (!strPos.IsEmpty())
				{
					vecStrRet.push_back(strPos);
				}
			}
			nLastPos = i + nCutter;
		}
	}
	//���һ���ָ�������β������ҲӦ�÷���
	if (nLastPos < strValue.GetLength())
	{
		CString strPos = strValue.Mid(nLastPos, strValue.GetLength() - nLastPos);
		if (!strPos.IsEmpty())
		{
			vecStrRet.push_back(strPos);
		}
	}
	return vecStrRet;
}

CString CGlobalFunction::ConvertNumToChinese(DWORD nNum)
{
	//Ŀǰ��֧�������ҽ�֧�ֵ�ǧ����
	ASSERT(nNum >= 0 && nNum < 1000);
	CString strRet;
	strRet.Format(_T("%d"), nNum);
	if (nNum >= 0 && nNum < 1000)
	{
		CString standard = _T("0123456789");
		TCHAR zh[] = {_T('��'), _T('һ'), _T('��'), _T('��'), _T('��'), _T('��'), _T('��'), _T('��'), _T('��'), _T('��')};
		TCHAR unit[] = {_T(' '), _T('ʮ'), _T('��'), _T('ǧ')};

		CString strNum = strRet;
		strRet.Empty();
		for (int i = 0; i < strNum.GetLength(); i++)
		{
			//��ת������
			if (standard.Find(strNum[i]) >= 0)
			{
				TCHAR ch = zh[standard.Find(strNum[i])];
				//Ȼ�����������
				if (strRet.GetLength() > 0 && strRet[strRet.GetLength() - 1] == _T('��') && ch == _T('��'))
				{
					//��Ҫ��������������0
				}
				else
				{
					strRet.AppendChar(ch);
				}
				//ÿ��λ�ķ���ֵ��һ����λ
				if (ch != _T('��'))
				{
					strRet.AppendChar(unit[(strNum.GetLength() - i - 1) % 4]);
				}
			}
		}
		//�ɵ�ȫ���Ŀո�
		strRet.Trim();
		//������ַ�0��ĩβΪ0����ɵ�ĩβ��0
		if (strRet.GetLength() > 0 && strRet != _T("��") && strRet[strRet.GetLength() - 1] == _T('��'))
		{
			strRet = strRet.Left(strRet.GetLength() - 1);
		}
		//��ԡ�һʮ�ˡ�Ӧ�ô���Ϊ��ʮ�ˡ�
		if (strRet.GetLength() == 3 && strRet.Left(2) == _T("һʮ"))
		{
			strRet = strRet.Mid(1);
		}
		//��ԡ�һʮ��Ӧ�ô���Ϊ��ʮ��
		if (strRet == _T("һʮ"))
		{
			strRet = _T("ʮ");
		}
	}

	return strRet;
}

BOOL CGlobalFunction::CanConvertToNum(CString strNum)
{
	BOOL bRet = FALSE;
	if (strNum.GetLength() > 0 
		&& strNum[0] >= _T('0')
		&& strNum[0] <= _T('9')
		)
	{
		bRet = TRUE;
	}
	return bRet;
}

std::vector<CString> CGlobalFunction::ScanFilePath(CString strFilePath)
{
	std::vector<CString> vecRet;
	//��������·��
	WIN32_FIND_DATA wfd32;
	HANDLE hFind = NULL;
	BOOL bPanduan=FALSE;
	CString strScanPath = strFilePath;
	strScanPath += _T("\\*.*");
	hFind = ::FindFirstFile(strScanPath, &wfd32);

	if(INVALID_HANDLE_VALUE != hFind)
	{
		do
		{
			if((wfd32.cFileName[0] == '.')/* || ((wfd32.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) */)
				continue;

			//����һ���ļ�
			CString strFindName = wfd32.cFileName;
			strFindName = strFilePath + _T("\\") + strFindName;
			vecRet.push_back(strFindName);
		}while(::FindNextFile(hFind, &wfd32) != 0);

		FindClose(hFind);
	}
	return vecRet;
}

BOOL CGlobalFunction::IsDirectory(CString strPath)
{
	BOOL bDirectory = FALSE;
	WIN32_FIND_DATA findData;
	HANDLE hFind = FindFirstFile(strPath, &findData);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//Ŀ¼
// 			if (_tcscmp(findData.cFileName, _T(".")) != 0
// 				&& _tcscmp(findData.cFileName, _T("..")) != 0
// 				&& !(findData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) //ϵͳĿ¼
// 				&& !(findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)) //����Ŀ¼
			{
				bDirectory = TRUE;
			}
		}
	}
	FindClose(hFind);
	return bDirectory;
}

BOOL CGlobalFunction::DeleteFileOrPath(CString strFilePath)
{
	if (IsDirectory(strFilePath))
	{
		/*ȷ��Ŀ¼��·����2��\0��β*/
		TCHAR czFolder[MAX_PATH+1];
		ZeroMemory(czFolder, (MAX_PATH+1)*sizeof(TCHAR));
		_tcscpy_s(czFolder, strFilePath.GetBuffer());
		unsigned int iPathLen = strFilePath.GetLength();
		czFolder[iPathLen] = _T('\0');
		czFolder[iPathLen+1] = _T('\0');

		SHFILEOPSTRUCT FileOp;
		ZeroMemory(&FileOp, sizeof(SHFILEOPSTRUCT));
		FileOp.fFlags |= FOF_SILENT;			/*����ʾ����*/
		FileOp.fFlags |= FOF_NOERRORUI;			/*�����������Ϣ*/
		FileOp.fFlags |= FOF_NOCONFIRMATION;	/*ֱ��ɾ����������ȷ��*/
		FileOp.hNameMappings = NULL;
		FileOp.hwnd = NULL;
		FileOp.lpszProgressTitle = NULL;
		FileOp.wFunc = FO_DELETE;
		FileOp.pFrom = czFolder;				/*Ҫɾ����Ŀ¼��������2��\0��β*/
		FileOp.pTo = NULL;
		FileOp.fFlags &= ~FOF_ALLOWUNDO;

		return SHFileOperation(&FileOp) == 0;
	}
	else
	{
		return ::DeleteFile(strFilePath);
	}
}

BOOL CGlobalFunction::RenameFileOrPath(CString strDesPath, CString strSrcPath)//�����ļ����У�
{
	// ���·������\\�������ļ���ʧ��
	strDesPath.Replace(_T("\\\\"), _T("\\"));
	strSrcPath.Replace(_T("\\\\"), _T("\\"));

	if (IsDirectory(strSrcPath))
	{
		/*ȷ��Ŀ¼��·����2��\0��β*/
		TCHAR czSrcFolder[MAX_PATH+1];
		ZeroMemory(czSrcFolder, (MAX_PATH+1)*sizeof(TCHAR));
		_tcscpy_s(czSrcFolder, strSrcPath.GetBuffer());
		unsigned int iPathLen = strSrcPath.GetLength();
		czSrcFolder[iPathLen] = _T('\0');
		czSrcFolder[iPathLen+1] = _T('\0');

		TCHAR czDesFolder[MAX_PATH+1];
		ZeroMemory(czDesFolder, (MAX_PATH+1)*sizeof(TCHAR));
		_tcscpy_s(czDesFolder, strDesPath.GetBuffer());
		iPathLen = strDesPath.GetLength();
		czDesFolder[iPathLen] = _T('\0');
		czDesFolder[iPathLen+1] = _T('\0');

		SHFILEOPSTRUCT FileOp;
		ZeroMemory(&FileOp, sizeof(SHFILEOPSTRUCT));
		FileOp.fFlags |= FOF_SILENT;			/*����ʾ����*/
		FileOp.fFlags |= FOF_NOERRORUI;			/*�����������Ϣ*/
		FileOp.fFlags |= FOF_NOCONFIRMATION;	/*ֱ��ɾ����������ȷ��*/
		FileOp.hNameMappings = NULL;
		FileOp.hwnd = NULL;
		FileOp.lpszProgressTitle = NULL;
		FileOp.wFunc = FO_RENAME;
		FileOp.pFrom = czSrcFolder;				/*Ҫ���ĵ�Ŀ¼��������2��\0��β*/
		FileOp.pTo = czDesFolder;
		FileOp.fFlags &= ~FOF_ALLOWUNDO;

		return SHFileOperation(&FileOp) == 0;
	}
	else
	{
		::CopyFile(strSrcPath, strDesPath, FALSE);
		return ::DeleteFile(strSrcPath);
	}
}

BOOL CGlobalFunction::CopyFileOrPath(CString strDesPath, CString strSrcPath)//�����ļ����У�
{
	if (IsDirectory(strSrcPath))
	{
		/*ȷ��Ŀ¼��·����2��\0��β*/
		TCHAR czSrcFolder[MAX_PATH+1];
		ZeroMemory(czSrcFolder, (MAX_PATH+1)*sizeof(TCHAR));
		_tcscpy_s(czSrcFolder, strSrcPath.GetBuffer());
		unsigned int iPathLen = strSrcPath.GetLength();
		czSrcFolder[iPathLen] = _T('\0');
		czSrcFolder[iPathLen+1] = _T('\0');

		TCHAR czDesFolder[MAX_PATH+1];
		ZeroMemory(czDesFolder, (MAX_PATH+1)*sizeof(TCHAR));
		_tcscpy_s(czDesFolder, strDesPath.GetBuffer());
		iPathLen = strDesPath.GetLength();
		czDesFolder[iPathLen] = _T('\0');
		czDesFolder[iPathLen+1] = _T('\0');

		SHFILEOPSTRUCT FileOp;
		ZeroMemory(&FileOp, sizeof(SHFILEOPSTRUCT));
		FileOp.fFlags |= FOF_SILENT;			/*����ʾ����*/
		FileOp.fFlags |= FOF_NOERRORUI;			/*�����������Ϣ*/
		FileOp.fFlags |= FOF_NOCONFIRMATION;	/*ֱ��ɾ����������ȷ��*/
		FileOp.hNameMappings = NULL;
		FileOp.hwnd = NULL;
		FileOp.lpszProgressTitle = NULL;
		FileOp.wFunc = FO_COPY;
		FileOp.pFrom = czSrcFolder;				/*Ҫ���ĵ�Ŀ¼��������2��\0��β*/
		FileOp.pTo = czDesFolder;
		FileOp.fFlags &= ~FOF_ALLOWUNDO;

		return SHFileOperation(&FileOp) == 0;
	}
	else
	{
		return ::CopyFile(strSrcPath, strDesPath, FALSE);
	}
}
