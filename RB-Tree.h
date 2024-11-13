#ifndef _RB_TREE_H
#define _RB_TREE_H

#include <iostream>
// 节点的颜色常量
enum Color {RED, BLACK};

typedef struct Data{
	int key;
	//..
	
	Data(){}
	Data(int key):key(key) {}
	
	operator < (const Data& d1)const{
		return key < d1.key;
	}
	
	operator == (const Data& d1)const{
		return key == d1.key;
	}
	friend std::ostream& operator<<(std::ostream& os, const Data& dt);
}Data;


std::ostream& operator<<(std::ostream& os, const Data& dt)
{
	os << dt.key;
	return os;
}

// 红黑树节点类定义
struct Node {
	Data data;                    // 节点存储的数据
	Color color;                 // 节点的颜色
	Node *left, *right, *parent; // 左子节点、右子节点和父节点指针
	
	// 构造函数初始化节点数据和颜色为红色
	Node(Data data) : data(data), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

// 红黑树类定义
class RedBlackTree {
private:
	Node *root; // 根节点
	
protected:
	// 左旋转操作
	void leftRotate(Node *&node);
	// 右旋转操作
	void rightRotate(Node *&node);
	// 插入修复函数，保证插入后的红黑树平衡
	void insertFix(Node *&node);
	// 删除修复函数，保证删除后的红黑树平衡
	void deleteFix(Node *&node);
	// 替换两个节点的位置
	void transplant(Node *u, Node *v);
	// 查找树中的最小节点
	Node* minimum(Node *node);
	
public:
	// 构造函数初始化根节点为空
	RedBlackTree() : root(nullptr) {}
	
	// 插入节点
	void insert(Data data);
	// 删除节点
	void deleteNode(Data data);
	// 中序遍历并打印树的节点
	void inorder();
	// 递归打印树的节点
	void inorderHelper(Node *node, int depth = 0);
};

// 左旋操作：将当前节点node及其右子节点进行左旋转
// 也就是处理该结点的右子结点 和 右子节点的左子节点 和 该节点的父节点 的指针关系
void RedBlackTree::leftRotate(Node *&node) {
	Node *rightChild = node->right;     // 右子节点成为新的父节点
	node->right = rightChild->left;     // 将右子节点的左子树赋值给当前节点的右子树
	
	if (rightChild->left != nullptr)    // 更新右子节点左孩子的父节点为当前节点
		rightChild->left->parent = node;
	
	rightChild->parent = node->parent;  // 更新右子节点的父节点为当前节点的父节点
	
	if (node->parent == nullptr)        // 如果当前节点是根节点，则更新根节点为右子节点
		root = rightChild;
	else if (node == node->parent->left) // 判断当前节点是父节点的左孩子还是右孩子
		node->parent->left = rightChild;
	else
		node->parent->right = rightChild;
	
	rightChild->left = node;            // 当前节点变成右子节点的左孩子
	node->parent = rightChild;          // 更新当前节点的父节点为右子节点
}

// 右旋操作：将当前节点node及其左子节点进行右旋转
void RedBlackTree::rightRotate(Node *&node) {
	Node *leftChild = node->left;       // 左子节点成为新的父节点
	node->left = leftChild->right;      // 将左子节点的右子树赋值给当前节点的左子树
	
	if (leftChild->right != nullptr)    // 更新左子节点右孩子的父节点为当前节点
		leftChild->right->parent = node;
	
	leftChild->parent = node->parent;   // 更新左子节点的父节点为当前节点的父节点
	
	if (node->parent == nullptr)        // 如果当前节点是根节点，则更新根节点为左子节点
		root = leftChild;
	else if (node == node->parent->left) // 判断当前节点是父节点的左孩子还是右孩子
		node->parent->left = leftChild;
	else
		node->parent->right = leftChild;
	
	leftChild->right = node;            // 当前节点变成左子节点的右孩子
	node->parent = leftChild;           // 更新当前节点的父节点为左子节点
}

// 插入修复：维持红黑树的平衡性
void RedBlackTree::insertFix(Node *&node) {
	Node *parent = nullptr;
	Node *grandparent = nullptr;
	
	// 非根节点且红红
	while (node != root && node->color == RED && node->parent->color == RED) {
		parent = node->parent;
		grandparent = parent->parent;
		
		if (parent == grandparent->left) { // 父节点是祖父节点的左孩子
			Node *uncle = grandparent->right;
			
			if (uncle != nullptr && uncle->color == RED) { // 情况1：叔叔节点是红色
				grandparent->color = RED;
				parent->color = BLACK;
				uncle->color = BLACK;
				node = grandparent;
			} else {
				if (node == parent->right) { // 情况2：当前节点是父节点的右孩子 
					leftRotate(parent);
					node = parent;
					parent = node->parent;
				}
				// 情况3：当前节点是父节点的左孩子
				rightRotate(grandparent);
				std::swap(parent->color, grandparent->color);
				node = parent;
			}
		} else { // 父节点是祖父节点的右孩子
			Node *uncle = grandparent->left;
			
			if (uncle != nullptr && uncle->color == RED) { // 情况1：叔叔节点是红色
				grandparent->color = RED;
				parent->color = BLACK;
				uncle->color = BLACK;
				node = grandparent;
			} else {
				if (node == parent->left) { // 情况2：当前节点是父节点的左孩子
					rightRotate(parent);
					node = parent;
					parent = node->parent;
				}
				// 情况3：当前节点是父节点的右孩子
				leftRotate(grandparent);
				std::swap(parent->color, grandparent->color);
				node = parent;
			}
		}
	}
	root->color = BLACK; // 确保根节点为黑色
}

// 插入新节点
void RedBlackTree::insert(Data data) {
	Node *node = new Node(data);
	Node *parent = nullptr;
	Node *current = root;
	
	while (current != nullptr) { // 找到新节点的插入位置
		parent = current;
		if (node->data < current->data)
			current = current->left;
		else
			current = current->right;
	}
	
	node->parent = parent; // 设置新节点的父节点
	if (parent == nullptr)
		root = node;
	else if (node->data < parent->data)
		parent->left = node;
	else
		parent->right = node;
	
	insertFix(node); // 修复红黑树的平衡性
}

// 查找最小值节点
Node* RedBlackTree::minimum(Node *node) {
	while (node->left != nullptr)
		node = node->left;
	return node;
}

// 替换两个节点
void RedBlackTree::transplant(Node *u, Node *v) {
	if (u->parent == nullptr)
		root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;
	
	if (v != nullptr)
		v->parent = u->parent;
}

// 删除修复：保证红黑树的平衡性
void RedBlackTree::deleteFix(Node *&node) {
	// 如果当前节点不是根节点且颜色为黑色，则循环进行平衡调整
	while (node != root && node->color == BLACK) {
		// 如果当前节点是其父节点的左子节点
		if (node == node->parent->left) {
			Node *sibling = node->parent->right; // 获取兄弟节点
			
			// 如果兄弟节点是红色
			if (sibling->color == RED) {
				sibling->color = BLACK;            // 将兄弟节点染黑
				node->parent->color = RED;         // 将父节点染红
				leftRotate(node->parent);          // 对父节点左旋
				sibling = node->parent->right;     // 更新兄弟节点
			}
			
			// 如果兄弟节点的两个子节点均为黑色
			if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
				sibling->color = RED;             // 将兄弟节点染红
				node = node->parent;              // 向上递归
			} else {
				// 如果兄弟节点的右子节点是黑色
				if (sibling->right->color == BLACK) {
					sibling->left->color = BLACK;  // 将兄弟节点的左子节点染黑
					sibling->color = RED;          // 将兄弟节点染红
					rightRotate(sibling);          // 对兄弟节点右旋
					sibling = node->parent->right; // 更新兄弟节点
				}
				
				sibling->color = node->parent->color; // 兄弟节点颜色变为父节点的颜色
				node->parent->color = BLACK;          // 父节点染黑
				sibling->right->color = BLACK;        // 兄弟节点的右子节点染黑
				leftRotate(node->parent);             // 对父节点左旋
				node = root;                          // 将当前节点设为根节点，退出循环
			}
		} else { // 当前节点是其父节点的右子节点（与上面相似的逻辑）
			Node *sibling = node->parent->left;
			
			if (sibling->color == RED) {
				sibling->color = BLACK;
				node->parent->color = RED;
				rightRotate(node->parent);
				sibling = node->parent->left;
			}
			
			if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
				sibling->color = RED;
				node = node->parent;
			} else {
				if (sibling->left->color == BLACK) {
					sibling->right->color = BLACK;
					sibling->color = RED;
					leftRotate(sibling);
					sibling = node->parent->left;
				}
				
				sibling->color = node->parent->color;
				node->parent->color = BLACK;
				sibling->left->color = BLACK;
				rightRotate(node->parent);
				node = root;
			}
		}
	}
	node->color = BLACK; // 将当前节点染黑
}

// 删除节点
void RedBlackTree::deleteNode(Data data) {
	Node *node = root;
	Node *z = nullptr; // 要删除的节点
	Node *x, *y;
	
	// 查找要删除的节点
	while (node != nullptr) {
		if (node->data == data) {
			z = node;
			break;
		}
		if (data < node->data)
			node = node->left;
		else
			node = node->right;
	}
	
	if (z == nullptr) // 如果未找到节点，直接返回
		return;
	
	y = z;                       // 初始化y为z
	Color yOriginalColor = y->color; // 记录y的初始颜色
	
	if (z->left == nullptr) { // 如果z的左子节点为空
		x = z->right;         // 将x设置为z的右子节点
		transplant(z, z->right); // 用z的右子节点替代z
	} else if (z->right == nullptr) { // 如果z的右子节点为空
		x = z->left;              // 将x设置为z的左子节点
		transplant(z, z->left);   // 用z的左子节点替代z
	} else {
		y = minimum(z->right);    // 找到z右子树的最小节点作为替代节点
		yOriginalColor = y->color; // 记录y的颜色
		x = y->right;
		
		if (y->parent == z)
			x->parent = y;
		else {
			transplant(y, y->right); // 用y的右子节点替换y
			y->right = z->right;     // 将y的右子节点设为z的右子节点
			y->right->parent = y;    // 更新y右子节点的父节点
		}
		
		transplant(z, y);           // 用y替换z
		y->left = z->left;          // 将y的左子节点设为z的左子节点
		y->left->parent = y;        // 更新y左子节点的父节点
		y->color = z->color;        // 将y的颜色设为z的颜色
	}
	
	delete z; // 删除z节点
	
	// 如果删除节点的原始颜色是黑色，则需要进行修复
	if (yOriginalColor == BLACK && x != nullptr)
		deleteFix(x);
}


// 中序遍历，帮助函数，带缩进和颜色信息显示
void RedBlackTree::inorderHelper(Node *node, int depth) {
	if (node == nullptr)
		return;
	
	// 先递归遍历左子树
	inorderHelper(node->left, depth + 1);
	
	// 打印当前节点的值和颜色，并用缩进显示深度
	for (int i = 0; i < depth; ++i)
		std::cout << "   "; // 根据深度打印缩进
	std::cout << node->data << (node->color == RED ? " (R)" : " (B)") << std::endl;
	
	// 递归遍历右子树
	inorderHelper(node->right, depth + 1);
}

// 修改中序遍历接口以适配新的inorderHelper
void RedBlackTree::inorder() {
	inorderHelper(root);
	std::cout << std::endl;
}

#endif
