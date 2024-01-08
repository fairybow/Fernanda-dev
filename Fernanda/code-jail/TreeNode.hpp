#pragma once

#include <QDebug>
#include <QString>
#include <QList>
#include <QVariant>

#include <ostream>
#include <string>

class TreeNode
{
public:
	TreeNode(const QString& name, TreeNode* parent = nullptr)
		: m_name(name), m_parent(parent)
	{}

	~TreeNode()
	{
		for (auto& child : m_children)
			delete child;
	}

	friend std::ostream& operator<<(std::ostream& os, const TreeNode& treeNode)
	{
		os << "("
			<< "TreeNode(" << &treeNode << "), "
			<< treeNode.name().toStdString() << ", "
			<< (treeNode.hasParent() ? "child" : "orphan") << ", "
			<< treeNode.m_children.count() << " children"
			<< ")";

		return os;
	}

	friend QDebug operator<<(QDebug debug, const TreeNode& treeNode)
	{
		debug.nospace() << "("
			<< "TreeNode(" << &treeNode << "), "
			<< treeNode.name() << ", "
			<< (treeNode.hasParent() ? "child" : "orphan") << ", "
			<< treeNode.m_children.count() << " children"
			<< ")";

		return debug.maybeSpace();
	}

	friend QDebug operator<<(QDebug debug, QList<TreeNode*> treeNodes)
	{
		debug.nospace() << "[";

		for (auto& tree_node : treeNodes) {
			debug << *tree_node;

			if (tree_node != treeNodes.last())
				debug << ",\n";
		}

		debug << "]";

		return debug.maybeSpace();
	}

	bool hasParent() const
	{
		return m_parent != nullptr;
	}

	bool hasChildren() const
	{
		return !m_children.isEmpty();
	}

	TreeNode* parent() const
	{
		return m_parent;
	}

	QList<TreeNode*> children() const
	{
		return m_children;
	}

	QString name() const
	{
		return m_name;
	}

	void rename(const QString& newName)
	{
		m_name = newName;
	}

	QVariant data() const
	{
		return m_data;
	}

	void setData(const QVariant& data)
	{
		m_data = data;
	}

	QList<TreeNode*> siblings() const
	{
		if (!m_parent) return QList<TreeNode*>();

		auto siblings_list = m_parent->children();
		siblings_list.removeAll(this);

		return siblings_list;
	}

	QList<TreeNode*> leafNodes()
	{
		QList<TreeNode*> leaves;

		if (!m_children.isEmpty())
			for (auto& child : m_children)
				leaves << child->leafNodes();
		else
			leaves << this;

		return leaves;
	}

	QList<TreeNode*> lineage() const
	{
		QList<TreeNode*> ancestors;

		auto ancestor = m_parent;
		while (ancestor) {
			ancestors.prepend(ancestor);
			ancestor = ancestor->parent();
		}

		return ancestors;
	}

	void addChild(TreeNode* child)
	{
		if (child == nullptr) return;

		if (child->hasParent())
			child->parent()->m_children.removeAll(child);

		child->m_parent = this;
		m_children << child;
	}

	TreeNode* addChild(const QString& name)
	{
		auto child = new TreeNode(name, this);
		m_children << child;

		return child;
	}

	void reparent(TreeNode* newParent)
	{
		if (m_parent == newParent) return;

		if (m_parent)
			m_parent->m_children.removeAll(this);

		newParent->m_children << this;
		m_parent = newParent;
	}

private:
	QString m_name;
	QVariant m_data;
	TreeNode* m_parent;
	QList<TreeNode*> m_children;
};
