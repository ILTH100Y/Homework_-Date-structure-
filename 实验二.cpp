#include <iostream>
#include <string>

using namespace std;

class numStack {
private:
	long long* data;
	int top;
	int capacity;
public:
	numStack(int size = 100)
	{
		data = new long long[100];
		capacity = size;
		top = -1;
	}
	~numStack()
	{
		delete[] data;
	}
	bool isFull() {
		return top == capacity - 1;
	}
	bool isEmpty() {
		return top == -1;
	}
	bool push(long long val) {
		if (isFull()) {
			cout << "栈已满，无法入栈" << endl;
			return false;
		}
		data[++top] = val;
		return true;
	}
	bool pop() {
		if (isEmpty()) {
			cout << "栈为空，无法出栈" << endl;
			return false;
		}
		top--;
		return true;
	}
	long long peek() {
		if (isEmpty()) {
			cout << "栈为空" << endl;
			return -1;
		}
		return data[top];
	}
	int size() {
		return top + 1;
	}
};

class opStack {
private:
	char* data;
	int top;
	int capacity;
public:
	opStack(int size = 100)
	{
		data = new char[100];
		capacity = size;
		top = -1;
	}
	~opStack()
	{
		delete[] data;
	}
	bool isFull() {
		return top == capacity - 1;
	}
	bool isEmpty() {
		return top == -1;
	}
	bool push(int val) {
		if (isFull()) {
			cout << "栈已满，无法入栈" << endl;
			return false;
		}
		data[++top] = val;
		return true;
	}
	bool pop() {
		if (isEmpty()) {
			cout << "栈为空，无法出栈" << endl;
			return false;
		}
		top--;
		return true;
	}
	char peek() {
		if (isEmpty()) {
			cout << "栈为空" << endl;
			return -1;
		}
		return data[top];
	}
	int size() {
		return top + 1;
	}
};

//乘方运算
long long pow10(int times)
{
	long long num = 1;
	for (int i = 0; i < times; i++)
		num *= 10;
	return num;
}
long long intPow(int base, int exp, int n) {
	long long result = 1;
	int intExp = exp / pow10(n);  // 还原实际指数
	if (intExp == 0) {
		return pow10(n);
	}
	for (int i = 0; i < intExp; i++) 
		result = result * base;
	result /= pow10(n * (intExp - 1));
	return result;
}

//多位数、小数转换
long long transform(string numStr,int n)
{
	bool negative = (numStr[0] == '-');
	int start = negative ? 1 : 0;

	int size = numStr.size();
	int m=0;
	long long num = 0;
	if (numStr.find('.') != string::npos)
		m = numStr.size() - numStr.find('.') - 1;
	string numStr1;
	for (int i = start; i < size; i++)
	{
		if (numStr[i] == '.')
			continue;
		numStr1 += numStr[i];
	}
	for (int i = 0; i < numStr1.size(); i++)
		num += (numStr1[i]-'0') * pow10(numStr1.size() - 1 - i);
	num *= pow10(n - m);
	return (1-2* negative)*num;
}

//局部计算
void calcOnce(numStack& a, opStack& b,int n) {
	long long right = a.peek(); a.pop();
	long long left = a.peek(); a.pop();
	char op = b.peek(); b.pop();

	long long result;

	switch (op) {
	case '+':
		result = left + right;
		break;
	case '-':
		result = left - right;
		break;
	case '*':
		result = left * right/pow10(n);
		break;
	case '/':
		if (right == 0) {
			result = 0;
		}
		else {
			result = left*pow10(n) / right;
		}
		break;
	case '%':
		if (right == 0) {
			result = 0;
		}
		else {
			result = left % right;
		}
		break;
	case '^':
		result = intPow(left, right, n);
		break;
	default:
		result = 0;
	}

	a.push(result);
}

//运算符优先级
int priority(char op) {
	switch (op) {
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
	case '%':
		return 2;
	case '^':
		return 3;
	default:
		return 0;
	}
}

//扫描字符串并运算
double calculate(string line,int n,numStack& a,opStack& b)
{
	for (int i = 0; i < line.length(); i++)
	{
		char c = line[i];
		if (isdigit(c) || c == '.')		// 收集数字字符串
		{
			string numStr;
			while (i < line.length() && (isdigit(line[i]) || line[i] == '.'))
			{
				numStr += line[i];
				i++;
			}
			i--; // 回退一位，因为外层循环会 ++
			a.push(transform(numStr, n)); //转换为整数结构并入栈
		}
		else if (c == '(') {
			b.push('(');
		}
		else if (c == ')') {
			while (b.peek() != '(') {
				calcOnce(a, b,n);
			}
			b.pop();
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^') {
			if (c == '-' && (i == 0 || (!isdigit(line[i - 1]) && line[i - 1] != ')')))
			{
				string numStr = "-";
				i++;
				while (i < line.length() && (isdigit(line[i]) || line[i] == '.'))
				{
					numStr += line[i];
					i++;
				}
				i--; // 回退一位，因为外层循环会 ++
				a.push(transform(numStr, n)); //转换为整数结构并入栈
				continue;
			}
			if (c == '^') {
				// 右结合：不弹出同优先级的 '^'，只弹出更高优先级的
				while (!b.isEmpty() && priority(b.peek()) > priority(c)) {
					calcOnce(a, b, n);
				}
			}
			else {
				// 左结合：弹出优先级 >= 当前的所有运算符
				while (!b.isEmpty() && priority(b.peek()) >= priority(c)) {
					calcOnce(a, b, n);
				}
			}
			b.push(c);
		}
	}
	while (!b.isEmpty()) {
		calcOnce(a, b, n);
	}
	double result = (double)a.peek()/pow10(n); a.pop();
	return result;
}

int main()
{
	numStack a;
	opStack b;
	string line;
	int n;
	cout << "简单计算器展示" << endl;
	cout << "请在一行内输入计算式(请勿留空格,且暂时只支持整数指数、底数)" << endl;
	getline(cin, line);
	cout << "请清点最高几位小数" << endl;
	cin >> n;
	cout << "结果为" << calculate(line,n,a,b) <<endl;
	return 0;
}