#pragma once

namespace pfile {

// �ж�word97--2003�ĵ��Ƿ����
extern bool IsEncrypted_doc(PSIHANDLE pFile);

// �ж�exl97--2003�Ƿ����
extern bool IsEncrypted_xls(PSIHANDLE pFile);

// �ж�ppt97--2003�Ƿ����
extern bool IsEncrypted_ppt(PSIHANDLE pFile);

// �ж�ppt2007�Ƿ����
extern bool IsEncrypted_pptx(PSIHANDLE pFile);

// �ж�pdf�Ƿ����
extern bool IsEncrypted_pdf(PSIHANDLE pFile);

// �ж�rarѹ���ļ��Ƿ����
extern bool IsEncrypted_rar(PSIHANDLE pFile);

// �ж�zipѹ���ļ��Ƿ����
extern bool IsEncrypted_zip(PSIHANDLE pFile);

// �ж�dmg�ļ��Ƿ����
extern bool IsEncrypted_dmg(PSIHANDLE pFile);

// �ж�word2007--2010�Ƿ����
extern bool IsEncrypted_docx(PSIHANDLE pFile);


// �ж�exl2007--2010�Ƿ����
extern bool IsEncrypted_xlsx(PSIHANDLE pFile);


// �ж�ppt2010--2010�Ƿ����
extern bool IsEncrypted_pptx(PSIHANDLE pFile);

// �ж�7zѹ���ļ��Ƿ����
extern bool IsEncrypted_7z(PSIHANDLE pFile);


//#ifndef min
//#define min(a,b)    a < b //(((a) < (b)) ? (a) : (b))
//#endif

}