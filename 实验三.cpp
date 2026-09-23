#include <iostream>
#define upLimit 10000
#define lowLimit 0

using namespace std;

int CHF(int times)
{
	if (times == 0)
		return 1;
	int sum = 1;
	for (int i = 0; i < times; i++)
		sum *= 2;
	return sum;
}



//二叉链表基础节点
struct Node {
	int data;
	Node* left;
	Node* right;
};


//前序遍历输入
void prein(Node* root,int floor,int& count)
{
	if (floor == 1 || count == 0) {  // 到达指定层数或输入完所有节点
		root->left = nullptr;
		root->right = nullptr;
		if (floor == 1 && count > 0) {	// 叶子节点，不创建子节点
			cin >> root->data;
			count--;
		}
		return;
	}
	cin >> root->data;
	count--;
	if (count > 0)
	{
		root->left = new Node;
		prein(root->left, floor - 1, count);
	}else {
	 root->left = nullptr;
	}
	if (count > 0)
	{
		root->right = new Node;
		prein(root->right, floor - 1, count);
	}
	else {
		root->right = nullptr;
	}
}
//输入的包装
void in_pack(Node* root)
{
	int n;
	cout << "请问共输入多少个节点" << endl;
	cin >> n;
	int floor = 0;
	while (CHF(floor) - 1 < n)
		floor++;
	cout << "输入上下限分别为" << upLimit << "和" << lowLimit << endl;
	cout << "请按前序遍历顺序输入" << n << "个节点值:" << endl;
	int count = n;
	prein(root, floor, count);
}


//前序遍历输出
void preorder(Node* root)
{
	if (root == nullptr)  // 统一处理空指针
		return;
	cout << root->data << endl;
	preorder(root->left);
	preorder(root->right);
}
//中序遍历输出
void inorder(Node* root)
{
	if (root == nullptr)  // 统一处理空指针
		return;
	inorder(root->left);
	cout << root->data << endl;
	inorder(root->right);
}
//后序遍历输出
void postorder(Node* root)
{
	if (root == nullptr)  // 统一处理空指针
		return;
	postorder(root->left);
	postorder(root->right);
	cout << root->data << endl;
}
//后序遍历删除
void postorder_delete(Node*& root)
{
	if (root == nullptr)  // 统一处理空指针
		return;
	postorder_delete(root->left);
	postorder_delete(root->right);
	delete root;
	root = nullptr;
}


//单个节点的检查计数
void singleCheck(Node* root,int& sum_1, int& sum_2, int& max, int& min) {
	if (root == nullptr)
		return;
	if (max < root->data)
		max = root->data;
	if (min > root->data)
		min = root->data;
	if (root->left != nullptr && root->right != nullptr)
		sum_2++;
	if ((root->left != nullptr) != (root->right != nullptr))
		sum_1++;
}
//非遍历所需栈的实现
class Stack {
private:
	int Length;
	int capacity;
	Node** arr;
public:
	Stack() {
		Length = 100;	//可根据需求大致调整
		arr = new Node*[Length];
		capacity = 0;
	}
	~Stack() {
		delete[] arr;
	}
	bool isFull()
	{
		return capacity >= 100;
	}
	bool isEmpty()
	{
		return capacity <= 0;
	}
	void push(Node* root) {
		if (isFull())
			return;
		arr[capacity] = root;
		capacity++;
	}
	Node* pop() {
		if (isEmpty())
			return nullptr;
		capacity--;
		return arr[capacity];
	}
	int getcapacity() 
	{
		return capacity;
	}
};
//整体统计的包装
void statistics(Node* root, int& sum_1, int& sum_2, int& max, int& min)
{
	if (root == nullptr)
		return;
	Stack s1;
	s1.push(root);
	Node* p;
	while (!s1.isEmpty())
	{
		p = s1.pop();
		singleCheck(p, sum_1, sum_2, max, min);
		if (p->right != nullptr)
			s1.push(p->right);
		if (p->left != nullptr)
			s1.push(p->left);
	}
}


// 层次遍历所需队列的实现
class Queue {
private:
	int front;
	int rear;
	int capacity;
	Node** arr;

public:
	Queue() {
		capacity = 100;
		arr = new Node * [capacity];
		front = 0;
		rear = 0;
	}

	~Queue() {
		delete[] arr;
	}

	bool isEmpty() {
		return front == rear;
	}

	bool isFull() {
		return rear >= capacity;
	}

	void push(Node* root) {
		if (isFull()) return;
		arr[rear++] = root;
	}

	Node* pop() {
		if (isEmpty()) return nullptr;
		return arr[front++];
	}
};
// 层次遍历输出
void levelOrder(Node* root)
{
	if (root == nullptr) return;

	Queue q;
	q.push(root);

	while (!q.isEmpty()) {
		Node* p = q.pop();
		cout << p->data << " ";

		if (p->left != nullptr)
			q.push(p->left);
		if (p->right != nullptr)
			q.push(p->right);
	}
	cout << endl;
}


int main()
{
	Node* root = new Node;
	root->left = nullptr;
	root->right = nullptr;
	in_pack(root);		//输入
	preorder(root);		//前序遍历输出
	cout << endl;
	inorder(root);		//中序遍历输出
	cout << endl;
	postorder(root);	//后序遍历输出
	cout << endl;

	//统计二叉树的节点个数、度为1、度为2和叶子节点的个数和最大值、最小值
	int sum_degree_1, sum_degree_2,max,min;
	sum_degree_1 = sum_degree_2 = 0;
	max = lowLimit;
	min = upLimit;
	statistics(root, sum_degree_1, sum_degree_2, max, min);
	cout << "总结点数为"<< 2*sum_degree_2+sum_degree_1+1 <<endl;		//利用 n=n0+n1+n2=1+n1+2*n2
	cout << "度为1节点数为"<< sum_degree_1 <<endl;
	cout << "度为2节点数为"<< sum_degree_2 <<endl;
	cout << "叶子节点数为"<< sum_degree_2+1 <<endl;
	cout << "最大值为"<< max <<endl;
	cout << "最小值为"<< min <<endl;

	//层次遍历
	cout << "层次遍历结果: ";
	levelOrder(root);

	//使用后序删除释放内存
	postorder_delete(root);

	return 0;
}