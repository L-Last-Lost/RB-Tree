#include "RB-Tree.h"

using namespace std;

int main() { 
	RedBlackTree tree; 
	for(int i = 1; i <= 20; i++){
		tree.insert(i);
	}

	cout << "红黑树的中序遍历结果为: \n";
	tree.inorder();
	
	tree.deleteNode(20);
	tree.deleteNode(8);
	cout << "删除20后的中序遍历结果为: \n";
	tree.inorder();
	
	return 0;
}
