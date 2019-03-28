#pragma once

namespace app
{
	class CPAppAnalyzer
	{
	public:
		CPAppAnalyzer();
		~CPAppAnalyzer();
		void doAppTask(CPTask* task, CPRecord* rootRecord);

	private:
		class Impl;
		Impl*_impl;
	};
}