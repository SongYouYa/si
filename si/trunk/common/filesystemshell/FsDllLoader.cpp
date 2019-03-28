#include "stdafx.h"
#include "FsDllLoader.h"

CFsDllLoader::CFsDllLoader()
    :impl_(NULL)
{
    impl_ = GetImpl();
}
CFsDllLoader::~CFsDllLoader()
{
    impl_->Release();
}
CFsDllLoader::CImpl* CFsDllLoader::GetImpl()
{
    return CImpl::Create();
}
void CFsDllLoader::setDllDir(const std::wstring& dllDir)
{
    impl_->setDllDir(dllDir);
}
bool CFsDllLoader::Load()
{
    return impl_->Load();
}
void CFsDllLoader::Release()
{
    return impl_->Release();
}
void* CFsDllLoader::GetFunction( const char* strFuncName )
{
    return impl_->GetFunction(strFuncName);
}
