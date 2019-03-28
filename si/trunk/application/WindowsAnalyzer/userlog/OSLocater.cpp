#include "stdafx.h"
#include "OSLocater.h"

#include "sidevice/PDeviceManager.h"

namespace app
{
	using namespace boost::filesystem;
	typedef boost::shared_ptr<WindowsOS> WindowsOSPtr;
	class OSLocater::OSLocateImp
	{
	public:
		OSLocateImp()
		{
		}
		~OSLocateImp()
		{
		}
	public:
		bool GetPartitionOSInformation(const wstring path, vector<boost::shared_ptr<WindowsOS> >& oses);
		bool GetNoSystemPartitions(const std::vector<std::wstring>& all_partition, std::vector<std::wstring>& no_system_partition);
		bool GetUserDirectoryPath(const std::vector<std::wstring>& all_partition, std::vector<std::wstring>& user_directory_path);
		// 获取子文件夹(张文攀，20150127)
		bool GetSubDirectory(const std::wstring& rootdir, std::vector<std::wstring>& user_directory_path);
	};
	OSLocater::OSLocater() : os_locater_impl_(new OSLocateImp)
	{
	}
	OSLocater::~OSLocater()
	{
	}
	void OSLocater::GetAllOperationSystemInfo(std::vector<boost::shared_ptr<WindowsOS> >& all_windows_os)
	{
		pdevice::DeviceDisks disks;
		pdevice::CPDeviceManager::get()->getAllDisks(disks);
		
		for (auto &disk : disks)
		{
			for (auto &partition : disk.partitions)
			{
				if (!partition.mountPoints.empty()) {
					std::wstring partition_name = partition.mountPoints.at(0);
					GetPartitionOSInformation(partition_name, all_windows_os);
				}
			}
			
		}
	}
	bool  OSLocater::GetPartitionOSInformation(const wstring path, vector<boost::shared_ptr<WindowsOS> >& oses)
	{
		return os_locater_impl_->GetPartitionOSInformation(path, oses);
	}
	bool OSLocater::GetNoSystemPartitions(const std::vector<std::wstring>& all_partition, std::vector<std::wstring>& no_system_partition)
	{
		return os_locater_impl_->GetNoSystemPartitions(all_partition, no_system_partition);
	}
	bool OSLocater::GetUserDirectoryPath(const std::vector<std::wstring>& all_partition, std::vector<std::wstring>& user_directory_path)
	{
		return os_locater_impl_->GetUserDirectoryPath(all_partition, user_directory_path);
	}
	bool OSLocater::GetSubDirectory(const std::wstring& rootdir, std::vector<std::wstring>& user_directory_path)
	{
		return os_locater_impl_->GetSubDirectory(rootdir, user_directory_path);
	}
	bool OSLocater::OSLocateImp::GetPartitionOSInformation(const wstring partition_path, vector<boost::shared_ptr<WindowsOS> >& oses)
	{
		// if there are more operation system exists in the same   partition, how to distinct their directories???
		//partition_path.append(L"\\");
		path rootpath(partition_path);

		try
		{
			if (!boost::filesystem::exists(rootpath))
			{
				return false;
			}

			directory_iterator start_iterator(rootpath);
			directory_iterator end_iterator;
			boost::shared_ptr<WindowsOS> win_lin_os(new WindowsOS());
			win_lin_os->os_type_ = OS_UNKNOWN;
			bool is_find = false;

			for (; start_iterator != end_iterator; ++start_iterator)
			{
				try
				{
					if (is_directory(start_iterator->path()) && (basename(start_iterator->path()) == "Windows" || basename(start_iterator->path()) == "WINDOWS"))// Windows
					{
						path windowspath(start_iterator->path().string());
						directory_iterator temp_iterator(windowspath);
						directory_iterator end_temp_iterator;

						for (; temp_iterator != end_temp_iterator; ++temp_iterator)
						{
							try
							{
								if (is_directory(temp_iterator->path()) && (basename(temp_iterator->path()) == "System32" || basename(temp_iterator->path()) == "system32"))
								{
									path System32path(temp_iterator->path().string());
									directory_iterator temp2_iterator(System32path);
									directory_iterator end_temp2_iterator;

									for (; temp2_iterator != end_temp2_iterator; ++temp2_iterator)
									{
										try
										{
											if (!is_directory(temp2_iterator->path()) && basename(temp2_iterator->path()) == "ntoskrnl" || //ntoskrnl.exe
												!is_directory(temp2_iterator->path()) && temp2_iterator->path().filename().string() == "ntdll.dll") //win8系统下，boost库判断ntoskrl.exe不存在，所以
											{																										//增加判断ntdll.dll文件
												win_lin_os->os_type_ = OS_WINDOWS;
												break;
											}
										}
										catch (...)
										{
											continue;
										}
									}
								}
							}
							catch (...)
							{
								continue;
							}
						}

						std::wstring user_path = partition_path;

						if (basename(start_iterator->path()) == "Windows")
						{	
							win_lin_os->windows_directory_path_ = partition_path + L"\\" + L"Windows";
							user_path = user_path + L"\\" + L"Users";
							if (boost::filesystem::exists(user_path))
							{
								win_lin_os->user_directory_path_ = user_path;
								win_lin_os->name_ = L"WindowsXP later";
							}
							else
							{
								win_lin_os->os_type_ = OS_UNKNOWN;
							}
						}
						else
						{						
							win_lin_os->windows_directory_path_ = partition_path + L"\\" + L"WINDOWS";				
							user_path = user_path + L"\\" + L"Documents and Settings";
							if (boost::filesystem::exists(user_path))
							{
								win_lin_os->user_directory_path_ = user_path;
								win_lin_os->name_ = L"WindowsXP";
							}
							else
							{
								win_lin_os->os_type_ = OS_UNKNOWN;
							}
						}

						break;
					}
				}
				catch (...)
				{
					continue;
				}

				/*   if (!is_find &&is_directory(start_iterator->path()) && (basename(start_iterator->path()) == "Users" || basename(start_iterator->path()) == "用户"))//Windows Vista  Windows 7
				{
				win_lin_os->user_directory_path_ = start_iterator->path().wstring();
				win_lin_os->name_ = L"WindowsXP later";
				is_find = true;
				}
				if (!is_find && is_directory(start_iterator->path()) && basename(start_iterator->path()) == "Documents and Settings")//windows XP
				{
				win_lin_os->user_directory_path_ = start_iterator->path().wstring();
				win_lin_os->name_ = L"WindowsXP";
				is_find = true;
				}        */
			}

			if (win_lin_os->os_type_ == OS_WINDOWS)
			{
				oses.push_back(win_lin_os);
			}
			else
			{
				return false;
			}
		}
		catch (...)
		{
			return false;
		}

		return true;
	}
	bool OSLocater::OSLocateImp::GetNoSystemPartitions(const std::vector<std::wstring>& all_partition, std::vector<std::wstring>& no_system_partition)
	{
		for (size_t i = 0; i < all_partition.size(); ++i)
		{
			try
			{
				bool is_system_partition = false;
				path rootpath(all_partition.at(i));
				directory_iterator start_iterator(rootpath);
				directory_iterator end_iterator;

				for (; start_iterator != end_iterator; ++start_iterator)
				{
					try
					{
						if (is_directory(start_iterator->path()) && (basename(start_iterator->path()) == "Windows" || basename(start_iterator->path()) == "WINDOWS"))// Windows
						{
							path windowspath(start_iterator->path().string());
							directory_iterator temp_iterator(windowspath);
							directory_iterator end_temp_iterator;

							for (; temp_iterator != end_temp_iterator; ++temp_iterator)
							{
								try
								{
									if (is_directory(temp_iterator->path()) && (basename(temp_iterator->path()) == "System32" || basename(temp_iterator->path()) == "system32"))
									{
										path System32path(temp_iterator->path().string());
										directory_iterator temp2_iterator(System32path);
										directory_iterator end_temp2_iterator;

										for (; temp2_iterator != end_temp2_iterator; ++temp2_iterator)
										{
											try
											{
												if (!is_directory(temp2_iterator->path()) && basename(temp2_iterator->path()) == "ntoskrnl" || //ntoskrnl.exe
													!is_directory(temp2_iterator->path()) && basename(temp2_iterator->path()) == "ntdll") //ntdll.dll
												{
													is_system_partition = true;
													break;
												}
											}
											catch (...)
											{
												continue;
											}
										}
									}
								}
								catch (...)
								{
									continue;
								}
							}
						}
					}
					catch (...)
					{
						continue;
					}
				}

				if (is_system_partition)
				{
					is_system_partition = false;
					continue;
				}
				else
				{
					no_system_partition.push_back(all_partition.at(i));
				}
			}
			catch (...)
			{
				continue;
			}
		}

		return true;
	}
	bool OSLocater::OSLocateImp::GetUserDirectoryPath(const std::vector<std::wstring>& all_partition, std::vector<std::wstring>& user_directory_path)
	{
		std::vector<boost::shared_ptr<WindowsOS> > oses;

		for (size_t i = 0; i < all_partition.size(); ++i)
		{
			GetPartitionOSInformation(all_partition.at(i), oses);
		}

		for (size_t k = 0; k < oses.size(); ++k)
		{
			try
			{
				boost::filesystem::path rootpath(oses.at(k)->GetUserRegisterPath());
				boost::filesystem::directory_iterator start_iterator(rootpath);
				boost::filesystem::directory_iterator end_iterator;

				for (; start_iterator != end_iterator; ++start_iterator)
				{
					if (is_directory(start_iterator->path()))
					{
						user_directory_path.push_back(start_iterator->path().wstring());
					}
				}
			}
			catch (...)
			{
				continue;
			}
		}

		return true;
	}
	//获得子文件夹（张文攀，20150127）
	bool OSLocater::OSLocateImp::GetSubDirectory(const std::wstring& rootdir, std::vector<std::wstring>& user_directory_path)
	{
		try
		{
			boost::filesystem::path rootpath(rootdir);

			if (!boost::filesystem::exists(rootpath))
			{
				return false;
			}

			boost::filesystem::directory_iterator end_iter;

			for (boost::filesystem::directory_iterator iter(rootpath); iter != end_iter; ++iter)
			{
				if (boost::filesystem::is_directory(iter->path()))
				{
					user_directory_path.push_back(iter->path().wstring());
				}
			}
		}
		catch (...)
		{
			return false;
		}

		return true;
	}
}
