#include<queue>
#include<stack>
#include<array>
#include<math.h>
#include<iostream>
using namespace std;

static const int LENGTH = 9;  // 3 �� 3

struct Node
{
	std::array<int, LENGTH> state;
	int value;  // H ����ֵ
	int depth;  // G ����ֵ
	Node* parent;
	Node();
	Node(const Node& paretn);
	
	friend bool operator<(const Node& A,const Node& B)  // �ڽṹ������������������������ȶ�����Ƚϴ�С
	{
		return A.value + A.depth > B.value + B.depth;  // F = G + H
	}
};

Node::Node()  // Ĭ�Ϲ���
{
	value = 0; depth = 0; parent = nullptr;
}

Node::Node(const Node& parent)  // ���ƹ���
{
	value = parent.value;
	depth = parent.depth;
	this->parent = parent.parent;
	state = parent.state;
}

bool judge(const Node& A,const Node& B)  // �ж��Ƿ��н�
{
	// ������������������
	int countA = 0, countB = 0;
	for (int i = 0; i < LENGTH - 1; i++)
		for (int j = i + 1; j < LENGTH; j++)
			if (A.state[i] > A.state[j]&&A.state[i]*A.state[j]!=0)
				countA++;

	for (int i = 0; i < LENGTH - 1; i++)
		for (int j = i + 1; j < LENGTH; j++)
			if (B.state[i] > B.state[j]&&B.state[i]*B.state[j]!=0)
				countB++;

	// ���������һ����������һ����ż�������޽�
	return (countA % 2 == countB % 2) ? true : false;
}
bool equal(const Node& A, const Node& B) {  // �ж���������Ƿ����
	for (int i = 0; i < LENGTH; i++) {
		if (A.state[i] != B.state[i])return false;
	}
	return true;
}
int value(const Node& cur, const Node& Target) {  // �������ֵ H
	int H = 0;
	for (int i = 0; i < LENGTH; i++) {
		if (cur.state[i] == Target.state[i]||cur.state[i]==0)continue;
		int j = 0;
		while (cur.state[i] != Target.state[j])j++;
		H += (abs(i % 3 - j % 3) + abs(i / 3 - j / 3)); // ����������֮��
	}
	return H;
}

int main()
{
	priority_queue<Node> open;  // ��Ŵ����Ľ��
	queue<Node> close;          // ����Ѿ������Ľ��
	stack<array<int,LENGTH>> path;           

	Node cur, target;  // ������ʼ����Ŀ����
	
	cout << "��������ʼ��㣺" << endl;
	for (int i = 0; i < LENGTH; i++)
		cin >> cur.state[i];
	cout << "������Ŀ���㣺" << endl;
	for (int i = 0; i < LENGTH; i++)
		cin >> target.state[i];

	if (!judge(cur, target)) {  
		cout << "�޽�!" << endl;
		return 0;  // �޽�����ֹ����
	}

	open.push(cur);  // ����ʼ���������ȶ���
	
    // ��չ���
	int counter = 0;  // Ūһ��������������while������ѭ��
	while (open.size() > 0 && !equal(open.top(), target) && counter++ < 100)
	{
		close.push(open.top());
		open.pop();

		int index = 0;
		for (; index < LENGTH && close.back().state[index] != 0; index++);  // �ҵ� 0 ��λ��

		if (index > 2) {  // 0 ���������ƶ�
			Node temp(close.back());
			// ���Node��parentָ��һ��������������������ͷ��ˣ���parent��ָ���Լ�
			// ʾ����
			// Node parent=close.back();
			// temp.parent=&parent;  // error! �뿪if��󸸽ڵ�parent�ͷţ�temp.parent���ջ�ָ������
			// Ϊ������������������parentָ��ֱ��ָ���Ѿ�������close����ĸ��ڵ�
			temp.parent = &close.back();
			temp.depth = close.back().depth + 1;
			swap(temp.state[index], temp.state[index - 3]);
			temp.value = value(temp, target);
			if(temp.parent->parent==nullptr||!equal(temp,*temp.parent->parent))
				open.push(temp);
		}
		if (index < 5) {  // 0 ���������ƶ�
			Node temp(close.back());
			temp.parent = &close.back();
			temp.depth = close.back().depth + 1;
			swap(temp.state[index], temp.state[index + 3]);
			temp.value = value(temp, target);
			if (temp.parent->parent == nullptr || !equal(temp, *temp.parent->parent))
				open.push(temp);
		}
		if (index%3 != 2) {  // 0 ���������ƶ�
			Node temp(close.back());
			temp.parent = &close.back();
			temp.depth = close.back().depth + 1;
			swap(temp.state[index], temp.state[index + 1]);
			temp.value = value(temp, target);
			if (temp.parent->parent == nullptr || !equal(temp, *temp.parent->parent))
				open.push(temp);
		}
		if (index % 3 != 0) {  // 0 ���������ƶ�
			Node temp(close.back());
			temp.parent = &close.back();
			temp.depth = close.back().depth + 1;
			swap(temp.state[index], temp.state[index - 1]);
			temp.value = value(temp, target);
			if (temp.parent->parent == nullptr || !equal(temp, *temp.parent->parent))
				open.push(temp);
		}
	}

	if (open.size() == 0 || !equal(open.top(), target)) {  // ����ⲻ����������error
		cout << "error��" << endl; return 0;
	}

	// ��·���ϵĽ�����δ���ջ�������������
	path.push(open.top().state);
	Node* temp_ptr = open.top().parent;
	while (temp_ptr != nullptr) {
		path.push(temp_ptr->state);
		temp_ptr = temp_ptr->parent;
	}

	cout << "���� " << path.size() - 1 << " ��" << endl;
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