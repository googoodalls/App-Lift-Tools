// Win32App.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Win32App.h"
#include "../../src/pugixml.hpp"
#include <iostream>
#include "../../parse_data/parse_data.hpp"
#include "afxinet.h"
#include <fstream>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

bool ReadIni();
void Start();

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			Start();
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}

#define TEXT_INPUT

void Start()
{
	long lBufLen = 0;
	void * pBuf=NULL;
	CString strSource;
    pugi::xml_document doc;

	bool bInternetFile = ReadIni();
    if(bInternetFile) {
		CString strAddress("http://partner.applift.com/offers/offers.xml?api_key=AFFX1FHk7x39ewkWdJGcaAP3PxwROI");
		CInternetSession mSession(NULL,0);
		CHttpFile* mHttpFile=NULL;
		CString strLine;

		mHttpFile=(CHttpFile*)mSession.OpenURL(strAddress.GetBuffer(),1UL,INTERNET_FLAG_TRANSFER_BINARY);
		if( !mHttpFile )
			return;

		while(mHttpFile->ReadString(strLine)) {
			strSource+=strLine;
		}

		mHttpFile->Close();
		mSession.Close();

		pBuf=(void *)(strSource.GetBuffer());
		lBufLen = strSource.GetAllocLength();

		CStdioFile mStdioFile;
		if (!mStdioFile.Open(L"Saved_Local_File.txt", CFile::modeReadWrite|CFile::modeCreate|CFile::typeBinary))
			return;
		mStdioFile.WriteString(strSource);
		mStdioFile.Close();
	} else {
		FILE * fp = NULL;

		fp = fopen("Saved_Local_File.txt","r");
		if(!fp)  return;
		fseek(fp,0,SEEK_END); 
		lBufLen=ftell(fp);
		pBuf = (char*)malloc(lBufLen * sizeof(char));
		fseek(fp,0,SEEK_SET); 
		fread((void*)pBuf,lBufLen, 1, fp);
		fclose(fp);
	}

	pugi::xml_parse_result result = doc.load_buffer((void *)pBuf, lBufLen);
    std::cout << "Load result: " << result.description() << std::endl;
    std::cout << "First node value: [" << doc.first_child().value() << "], node child value: [" << doc.child_value("node") << "]\n";
    std::cout << "First node value: [" << doc.first_child().name() << "], node child value: [" << doc.child_value("node") << "]\n";

    pugi::xml_node tools = doc.first_child();
    load_data2(tools);

	if(bInternetFile)
		strSource.ReleaseBuffer();
	else
		free(pBuf);

    std::cout << "\n\n......finished....";
	getchar();
	return;
}

bool ReadIni()
{
	long lBufLen = 0;
	char * pBuf=NULL;
    FILE * fp = NULL;

	fp = fopen("Win32App.ini","r");
	if(!fp)  return false;
    fseek(fp,0,SEEK_END); 
	lBufLen=ftell(fp);
	pBuf = (char*)malloc(lBufLen * sizeof(char));
    fseek(fp,0,SEEK_SET); 
	fread((void*)pBuf,lBufLen, 1, fp);
	fclose(fp);
	char p = pBuf[0];
	int result = atoi( (const char *)(&p) );
    free(pBuf);

	if( result == 1 )
		return true;
	else
		return false;
}