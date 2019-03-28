#pragma once

namespace pfile {

// 判断word97--2003文档是否加密
extern bool IsEncrypted_doc(PSIHANDLE pFile);

// 判断exl97--2003是否加密
extern bool IsEncrypted_xls(PSIHANDLE pFile);

// 判断ppt97--2003是否加密
extern bool IsEncrypted_ppt(PSIHANDLE pFile);

// 判断ppt2007是否加密
extern bool IsEncrypted_pptx(PSIHANDLE pFile);

// 判断pdf是否加密
extern bool IsEncrypted_pdf(PSIHANDLE pFile);

// 判断rar压缩文件是否加密
extern bool IsEncrypted_rar(PSIHANDLE pFile);

// 判断zip压缩文件是否加密
extern bool IsEncrypted_zip(PSIHANDLE pFile);

// 判断dmg文件是否加密
extern bool IsEncrypted_dmg(PSIHANDLE pFile);

// 判断word2007--2010是否加密
extern bool IsEncrypted_docx(PSIHANDLE pFile);


// 判断exl2007--2010是否加密
extern bool IsEncrypted_xlsx(PSIHANDLE pFile);


// 判断ppt2010--2010是否加密
extern bool IsEncrypted_pptx(PSIHANDLE pFile);

// 判断7z压缩文件是否加密
extern bool IsEncrypted_7z(PSIHANDLE pFile);


//#ifndef min
//#define min(a,b)    a < b //(((a) < (b)) ? (a) : (b))
//#endif

}