//==============================================================================
//File name:   "files.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2012, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================

#ifndef _FILES_H_
#define _FILES_H_

//Save binary data to file, функция сохранения данных в файле  
bool Save_File(CString filename, unsigned char *pBuf,
			   unsigned long start_addr, unsigned long data_size);

//Add and Save binary data to file, функция сохранения данных в файле  
bool Add_to_File(CString filename, unsigned char *pBuf,
			   unsigned long start_addr, unsigned long data_size);

//Read binary data from file, функция чтения данных из файла 
bool Read_File(CString filename, unsigned char *pBuf, 
			   unsigned long start_addr, unsigned long data_size);
//Get File Size
unsigned long Get_FileSize(CString filename);

//Remove(delete) file, функция стирания файла
bool Delete_File(CString filename);
#endif

