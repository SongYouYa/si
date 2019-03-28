#pragma once
#include <string>
#include <vector>
#include <iostream>

namespace expr
{
	class Node
	{
	public:
		enum NodeType
		{
			TYPE_NONE,
			TYPE_COND,
			TYPE_GROUP
		};
		virtual ~Node() {};

		NodeType nodeType() { return this->_node_type; }

	protected:
		Node(NodeType node_type)
			: _node_type(node_type)
		{}

	private:
		NodeType _node_type;
	};
	typedef std::vector<Node*> Nodes;//AND条件
	typedef std::vector<Nodes> Nodess; //OR条件

	//标准条件类
	class Condition : public Node
	{
	public:
		std::wstring attr;
		std::wstring relop;
		std::wstring value;

		Condition(const std::wstring& at, const std::wstring& re,
			const std::wstring& va)
		: Node(TYPE_COND), attr(at), relop(re), value(va) {
		}
		~Condition() {
		}
	private:

	};
	typedef std::vector<Condition*> Conditions; //AND
	typedef std::vector<Conditions> Conditionss; //OR

	//多个条件组
	class Group : public Node
	{
	public:
		Group() :Node(TYPE_GROUP) {
		}
		~Group();
		void addNode(Node* node);
		void newPart(); //结束当前AND组
		//把节点递归展开合并，成为一个一个平行数组
		void getConditionss(Conditionss& condss);
	private:
		//合并另一个Group或Cond对象
		void mergeNode(Node* node);
		//把一组条件都合并到其中一个group对象中，若没有group对象，则不变
		Group* merge2Group(Nodes& nodes);
		//合并后的条件组
		void getMergedNodess(Nodess& mergedNodess);

		Nodess _nodess; //OR条件
		Nodess _mergingNodess; //合并节点时，临时存放
	};
}