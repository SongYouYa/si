#include "stdafx.h"
#include "PTree2.h"
#include "json/json.h"

#include "PReportPath.h"
#include "handler/PReportHandler.h"

#include <boost/xpressive/xpressive_dynamic.hpp>

namespace report
{
	extern std::string IdPre;
	extern const std::wstring ContentHtml2;

	class CPTree2::Impl
	{
	public:
		Impl() : _id(0){}
		Json::Value getNodeJson(CPRecord* record);
	private:
		int _id;
	};
	
	Json::Value CPTree2::Impl::getNodeJson(CPRecord* record)
	{
		Json::Value nodeObj;
		if (record->flags() & RFlagNoReport) {
			return nodeObj;
		}
		nodeObj["id"] = ++_id;
		size_t childrenCount = record->childrenCount();
		Json::Value dataObj;
		dataObj["target"] = W2U8(ContentHtml2) + "#" + IdPre + I2S(record->id());
		
		nodeObj["data"] = dataObj;

		
		size_t parsedChildrenCount = 0;
		if (childrenCount > 0) {
			Json::Value childrenObj;
			for (size_t i = 0; i < childrenCount; ++i) {
				CPRecord* child = record->child(i);
				if (child->flags() & RFlagNoReport) {
					continue;
				}
				if (child->isFolder()) {
					Json::Value childObj = getNodeJson(child);
					if (!childObj.empty()) {
						childrenObj.append(childObj);
					}
				}
				parsedChildrenCount++;
			}
			nodeObj["children"] = childrenObj;
		}

		std::string nodeName = W2U8(record->getNodeName());
		//先判断节点名没有添加子节点数量的，再添加
		boost::xpressive::sregex nameReg = boost::xpressive::sregex::compile(
			"\\([0-9]+\\)$", boost::xpressive::icase);
		if (!boost::xpressive::regex_search(nodeName, nameReg)) {
			nodeName += " (" + boost::lexical_cast<std::string>(parsedChildrenCount)+")";
		}
		
		nodeObj["text"] = nodeName;

		return nodeObj;
	}


	CPTree2::CPTree2(CPRecord* rootRecord)
		: _treeDict(new TemplateDictionary("")),
		_rootRecord(rootRecord),
		_impl(new Impl)
	{

	}

	CPTree2::~CPTree2()
	{

	}

	bool CPTree2::output(const std::wstring& treePath)
	{
		std::string treeData;
		getDataString(treeData);
		_treeDict->SetValue("TREE_DATA", treeData);
		_treeDict->SetValue("THEMES_DIR", ".");
		bool ret = CPReportHandler::outputTemplate(_treeDict, L"tree.tpl", treePath);
		return ret;
	}

	void CPTree2::getDataString(std::string& data)
	{
		Json::Value nodeObj = _impl->getNodeJson(_rootRecord);

		Json::StyledWriter styleWriter;
		data = styleWriter.write(nodeObj);
	}
}