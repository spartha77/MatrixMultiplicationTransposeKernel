#pragma once
#include <memory>
#include <stack>
#include <functional>

template<typename NodeType>
struct Node
{
	NodeType _node;
	std::unique_ptr<Node> _left, _right;

	Node(const NodeType& n) :_node(n) {}
	void setLeft(NodeType& ll)
	{
		this->_left = std::make_unique<Node>(ll);
	}
	void setRight(NodeType& rr)
	{
		this->_right = std::make_unique<Node>(rr);
	}
	NodeType getNodeObject() const
	{
		return _node;
	}
};


template<typename NodeType>
struct Tree
{
	using TreeNodeType = Node<NodeType>;
	std::unique_ptr<TreeNodeType> _rootNode;
	
	std::function<void(void)> _visitFn;
	std::function<void(TreeNodeType*)> _BuildFn;

	Tree() {}
	
	void setRootNode(NodeType& nodeData)
	{
		TreeNodeType trNode(nodeData);
		_rootNode = std::make_unique(trNode);
	}
	Tree(const NodeType& obj)
	{	
		_rootNode = std::make_unique<TreeNodeType>(Node<NodeType>(obj));
	}
	
	void buildByPreorderTraversal()
	{
		TreeNodeType* node = _rootNode.get();

		if (node == nullptr)
			return;

		std::stack<TreeNodeType*> stack;
		stack.push(node);

		while (!stack.empty())
		{
			TreeNodeType* poppedNode = stack.top();
			stack.pop();

			_BuildFn(poppedNode);

			if (poppedNode->_right != nullptr)
				stack.push(poppedNode->_right.get());
			if (poppedNode->_left != nullptr)
				stack.push(poppedNode->_left.get());
		}
	}

	std::vector<TreeNodeType*> getPreOrderExpression()
	{
		std::vector<TreeNodeType*> preOrderExpression;
		TreeNodeType* node = _rootNode.get();

		if (node == nullptr)
			return preOrderExpression;

		std::stack<TreeNodeType*> stack;
		stack.push(node);

		while (!stack.empty())
		{
			TreeNodeType* poppedNode = stack.top();
			stack.pop();

			preOrderExpression.push_back(poppedNode);

			if (poppedNode->_right != nullptr)
				stack.push(poppedNode->_right.get());
			if (poppedNode->_left != nullptr)
				stack.push(poppedNode->_left.get());
		}
		return preOrderExpression;
	}
};