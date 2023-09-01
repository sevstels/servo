//==============================================================================
//File name:    "files.cpp"
//Purpose:      Source File, Memory Buffer class
//Version:      1.00
//Copyright:    (c) 2012, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "files.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-----------------------------------------------------------------------------
//Save binary data to file, функция сохранения данных в файле 
//-----------------------------------------------------------------------------
bool Save_File( CString filename, 
				unsigned char *pBuf,
				unsigned long start_addr, 
				unsigned long data_size)
{

	TRY
		{	
			
			//Open and check status
			//file.Open(filename, CFile::modeRead, )
				
			//открываем файл для записи
			CFile file (  filename, CFile::modeCreate|		//
									CFile::modeWrite|		//Only for record
									CFile::shareDenyNone|	//
									CFile::typeBinary);		//binary mode

			//устанавливаем позицию начала блока данных относительно начала файла
			file.Seek(start_addr, CFile::begin);
						
			//записываем блок данных в файл из буфера 
			file.Write(pBuf, data_size);
			file.Close();
			return true;					
		}

	CATCH( CFileException, pEx)
					
		{
			#ifdef _DEBUG
			afxDump << _T("File could not be opened ") << pEx->m_cause << "\n";
			pEx->Delete();
			#endif
			AfxMessageBox(_T("Can not write file: \n") + filename);
			return false;
		}
				
	END_CATCH	
}

//-----------------------------------------------------------------------------
//Add and Save binary data to file, функция сохранения данных в файле 
//-----------------------------------------------------------------------------
bool Add_to_File(CString filename, unsigned char *pBuf,
			   unsigned long start_addr, unsigned long data_size)
{

	TRY
		{	
			//открываем файл для записи
			CFile file (  filename, CFile::modeNoTruncate|	//Not delite data
									CFile::modeWrite|		//Only for record
									CFile::shareDenyNone|	//share all
									CFile::typeBinary);		//binary mode

			//устанавливаем позицию начала блока данных относительно начала файла
			file.Seek(start_addr,CFile::begin);
						
			//записываем блок данных в файл из буфера 
			file.Write(pBuf, data_size);
			file.Close();
			return true;					
		}

	CATCH( CFileException, pEx)
					
		{
			#ifdef _DEBUG
			afxDump << _T("File could not be opened ") << pEx->m_cause << "\n";
			pEx->Delete();
			#endif
			AfxMessageBox(_T("Can not write file: \n") + filename);
			return false;
		}
				
	END_CATCH	
}
//-----------------------------------------------------------------------------                
//Read file size, функция чтения размера файла 
//-----------------------------------------------------------------------------
unsigned long Get_FileSize(CString filename)
{
  TRY
  {	
	 //Open file for read, открываем файл для чтения
	 CFile fileuser(filename, CFile::modeRead | 
							  CFile::shareDenyNone | 
							  CFile::typeBinary);
	
	 //Get file size 
	 unsigned long size = (unsigned long)fileuser.GetLength();
	 //Close					
	 fileuser.Close();
	 return size;	
  }

 CATCH( CFileException, pEx)
  {
	#ifdef _DEBUG
	afxDump << _T("File could not be opened ") << pEx->m_cause << "\n";
	pEx->Delete();
	#endif
	AfxMessageBox(_T("Can not reade file:\n") + filename);
	return 0;
  }
 END_CATCH	
}

//-----------------------------------------------------------------------------                
//Read binary data from file, функция чтения данных из файла 
//-----------------------------------------------------------------------------
bool Read_File( CString filename, unsigned char *pBuf,
				unsigned long start_addr, unsigned long data_size)
{
	TRY
		{	
			//открываем файл для чтения
			//open file for read
			CFile fileuser (filename, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary);
							
			//устанавливаем позицию начала блока данных относительно начала файла
			fileuser.Seek(start_addr,CFile::begin);
						
			//читаем блок данных из файл в буфер 
			fileuser.Read(pBuf, data_size);
			fileuser.Close();
			return true;	
		}

	CATCH( CFileException, pEx)
					
		{
			#ifdef _DEBUG
			afxDump << _T("File could not be opened ") << pEx->m_cause << "\n";
			pEx->Delete();
			#endif
			AfxMessageBox(_T("Can not reade file:\n") + filename);
			return false;
		}
				
	END_CATCH		
}
//-----------------------------------------------------------------------------                
//Delete file, функция удаления файла 
//-----------------------------------------------------------------------------
bool Delete_File(CString filename)
{

	TRY
		{		
			//Remove file
			CFile::Remove(filename);
			return true;	
		}

	CATCH( CFileException, pEx)
					
		{
			#ifdef _DEBUG
			afxDump << _T("File could not be removed ") << pEx->m_cause << "\n";
			pEx->Delete();
			#endif
			AfxMessageBox(_T("Can not remove file:\n") + filename);
			return false;
		}
				
	END_CATCH	
}
