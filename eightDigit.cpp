#include<queue>
#include<stack>
#include<array>
#include<math.h>
#include<iostream>
using namespace std;

static const int LENGTH = 9;  // 3 × 3

struct Node
{
	std::array<int, LENGTH> state;
	int value;  // H 估计值
	int depth;  // G 代价值
	Node* parent;
	Node();
	Node(const Node& paretn);
	
	friend bool operator<(const Node& A,const Node& B)  // 在结构体里重载运算符，方便在优先队列里比较大小
	{
		return A.value + A.depth > B.value + B.depth;  // F = G + H
	}
};

Node::Node()  // 默认构造
{
	value = 0; depth = 0; parent = nullptr;
}

Node::Node(const Node& parent)  // 复制构造
{
	value = parent.value;
	depth = parent.depth;
	this->parent = parent.parent;
	state = parent.state;
}

bool judge(const Node& A,const Node& B)  // 判断是否有解
{
	// 计算两个结点的逆序数
	int countA = 0, countB = 0;
	for (int i = 0; i < LENGTH - 1; i++)
		for (int j = i + 1; j < LENGTH; j++)
			if (A.state[i] > A.state[j]&&A.state[i]*A.state[j]!=0)
				countA++;

	for (int i = 0; i < LENGTH - 1; i++)
		for (int j = i + 1; j < LENGTH; j++)
			if (B.state[i] > B.state[j]&&B.state[i]*B.state[j]!=0)
				countB++;

	// 如果逆序数一个是奇数另一个是偶数，则无解
	return (countA % 2 == countB % 2) ? true : false;
}
bool equal(const Node& A, const Node& B) {  // 判断两个结点是否相等
	for (int i = 0; i < LENGTH; i++) {
		if (A.state[i] != B.state[i])return false;
	}
	return true;
}
int value(const Node& cur, const Node& Target) {  // 计算估计值 H
	int H = 0;
	for (int i = 0; i < LENGTH; i++) {
		if (cur.state[i] == Target.state[i]||cur.state[i]==0)continue;
		int j = 0;
		while (cur.state[i] != Target.state[j])j++;
		H += (abs(i % 3 - j % 3) + abs(i / 3 - j / 3)); // 哈夫曼距离之和
	}
	return H;
}

int main()
{
	priority_queue<Node> open;  // 存放待检测的结点
	queue<Node> close;          // 存放已经检测过的结点
	stack<array<int,LENGTH>> path;           

	Node cur, target;  // 定义起始结点和目标结点
	
	cout << "请输入起始结点：" << endl;
	for (int i = 0; i < LENGTH; i++)
		cin >> cur.state[i];
	cout << "请输入目标结点：" << endl;
	for (int i = 0; i < LENGTH; i++)
		cin >> target.state[i];

	if (!judge(cur, target)) {  
		cout << "无解!" << endl;
		return 0;  // 无解则终止程序
	}

	open.push(cur);  // 将初始结点放入优先队列
	
    // 扩展结点
	int counter = 0;  // 弄一个计算器，避免while计入死循环
	while (open.size() > 0 && !equal(open.top(), target) && counter++ < 100)
	{
		close.push(open.top());
		open.pop();

		int index = 0;
		for (; index < LENGTH && close.back().state[index] != 0; index++);  // 找到 0 的位置

		if (index > 2) {  // 0 可以向上移动
			Node temp(close.back());
			// 如果Node的parent指向一个变量，且这个变量被释放了，则parent会指向自己
			// 示例：
			// Node parent=close.back();
			// temp.parent=&parent;  // error! 离开if块后父节点parent释放，temp.parent最终会指向自身
			// 为避免上述情况，这里的parent指针直接指向已经存入了close表里的父节点
			temp.parent = &close.back();
			temp.depth = close.back().depth + 1;
			swap(temp.state[index], temp.state[index - 3]);
			temp.value = value(temp, target);
			if(temp.parent->parent==nullptr||!equal(temp,*temp.parent->parent))
				open.push(temp);
		}
		if (index < 5) {  // 0 可以向下移动
			Node temp(close.back());
			temp.parent = &close.back();
			temp.depth = close.back().depth + 1;
			swap(temp.state[index], temp.state[index + 3]);
			temp.value = value(temp, target);
			if (temp.parent->parent == nullptr || !equal(temp, *temp.parent->parent))
				open.push(temp);
		}
		if (index%3 != 2) {  // 0 可以向右移动
			Node temp(close.back());
			temp.parent = &close.back();
			temp.depth = close.back().depth + 1;
			swap(temp.state[index], temp.state[index + 1]);
			temp.value = value(temp, target);
			if (temp.parent->parent == nullptr || !equal(temp, *temp.parent->parent))
				open.push(temp);
		}
		if (index % 3 != 0) {  // 0 可以向左移动
			Node temp(close.back());
			temp.parent = &close.back();
			temp.depth = close.back().depth + 1;
			swap(temp.state[index], temp.state[index - 1]);
			temp.value = value(temp, target);
			if (temp.parent->parent == nullptr || !equal(temp, *temp.parent->parent))
				open.push(temp);
		}
	}

	if (open.size() == 0 || !equal(open.top(), target)) {  // 如果解不出来，返回error
		cout << "error！" << endl; return 0;
	}

	// 将路径上的结点依次存入栈，方便逆序输出
	path.push(open.top().state);
	Node* temp_ptr = open.top().parent;
	while (temp_ptr != nullptr) {
		path.push(temp_ptr->state);
		temp_ptr = temp_ptr->parent;
	}

	cout << "至少 " << path.size() - 1 << " 步" << endl;
	while (path.size() != 0) {
		cout << endl;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				cout << path.top()[3*i + j] << " ";
			}
			cout << endl;
		}
		path.pop();
	}
}