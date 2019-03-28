#pragma once

namespace devtask
{
	typedef std::map<PSIHANDLE, std::vector<PSIHANDLE>> ParentSubHandles;
	//生成父节点和子节点的map
	class CPFileHandleHelper
	{
	public:
		CPFileHandleHelper(devtask::ParentSubHandles& handles);
		void update(const std::vector<PSIHANDLE>& rootHandles);

		uint64 getTotalSize();

	private:
		void insert(PSIHANDLE parent);
		devtask::ParentSubHandles& _handles;
		uint64 _totalSize;
	};
}
