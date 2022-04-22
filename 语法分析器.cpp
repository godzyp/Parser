#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <stack>
using namespace std;
#define VN_number 5
#define VT_number 5
#define Grammar_number 8

static string Grammar[Grammar_number] = { 
	"A->CB",
	"B->+CB",
	"B->@",
	"C->ED",
	"D->*ED",
	"D->@",
	"E->(A)",
	"E->i"
};
static string SELECT[Grammar_number][2];							
static string FIRST[VN_number] = { "(i","+@","(i","*@","(i" };		
static string FOLLOW[VN_number] = {")#",")#","+)#","+)#","*+)#"};	

static char Nonterminal[VN_number] = {'A','B','C','D','E'};			//非终结符
static char Terminator[VT_number] = { 'i','+','*','(',')'};			//终结符
static char Analysis_Terminator[VT_number + 1] = { 'i','+','*','(',')','#'};
static string LL1_Analysis[VN_number][VT_number + 1];				//预测分析表
/*
static string LL1_Analysis[5][6] = {
	//'i'		'+'			'*'			'('			')'			'#'
	"A->CB",	"ERR",		"ERR",		"A->CB",	"ERR",		"ERR",		//A
	"ERR",		"B->+CB",	"ERR",		"ERR",		"B->@",		"B->@",		//B
	"C->ED",	"ERR",		"ERR",		"C->ED",	"ERR",		"ERR",		//C
	"ERR",		"D->@",		"D->*ED",	"ERR",		"D->@",		"D->@",		//D
	"E->i",		"ERR",		"ERR",		"E->(A)",	"ERR",		"ERR"		//E
};	//预测分析表
*/
void read_file(string &text)
{	ifstream file("text.txt");
	string temp;
	if (file.is_open())
		while (getline(file, temp))
			text += temp;
	else cout << "Read_Error" << endl;
}
bool is_Terminator(char temp)
{	for (int i = 0; i < VT_number; i++)
		if (Terminator[i] == temp)
			return true;
	return false;
}
string Analysis(char A,char a)//返回[A,a]
{	for (int i = 0; i < VN_number; i++)
	{	if (Nonterminal[i] == A)
			for (int j = 0; j < VT_number + 1; j++)
				if (Analysis_Terminator[j] == a)
					return LL1_Analysis[i][j];
	}
	return "ERROR";
}
void Build_Table(char A, char a, string temp)//A->a==temp
{	for (int i = 0; i < VN_number; i++)
	{	if (Nonterminal[i] == A)
			for (int j = 0; j < VT_number + 1; j++)
				if (Analysis_Terminator[j] == a)
					LL1_Analysis[i][j] = temp;
	}
	return;
}


int main()
{	string text;
	read_file(text);
	string temp;
	int n = 0;
	char S = 'A';	//开始符S
	char X, a;		//栈顶元素X 输入符号a
	stack<char> STACK;
	bool FLAG = true;
	cout << "有如下文法: " << endl;
	for (int i = 0; i < Grammar_number; i++)
		cout << Grammar[i] << "\t" << endl;

	//构造SELECT集
	for (int i = 0; i < Grammar_number; i++)
	{	SELECT[i][0] = Grammar[i];
		for (int j = 0; j < VN_number; j++)
			if (Nonterminal[j] == Grammar[i][0])
			{	if (Grammar[i][3] == '@')
					SELECT[i][1] += FOLLOW[j];
				else if (is_Terminator(Grammar[i][3]))
					SELECT[i][1] += Grammar[i][3];
				else
					SELECT[i][1] += FIRST[j];
			}
	}
	cout << endl << "SELECT集：" << endl;
	for (int i = 0; i < Grammar_number; i++)
	{	for (int j = 0; j < 2; j++)
			cout << SELECT[i][j] << "\t";
		cout << endl;
	}

	//构造预测分析表
	for (int i = 0; i < VN_number; i++)
		for (int j = 0; j < VT_number + 1; j++)
			LL1_Analysis[i][j] = "ERR";
	for (int i = 0; i < Grammar_number; i++)
		for (int j = 0; j < SELECT[i][1].length(); j++)
			Build_Table(SELECT[i][0][0], SELECT[i][1][j], SELECT[i][0]);

	cout << endl << "预测分析表: " << endl;
	for (int i = 0; i < VT_number + 1; i++)
		cout << "\t" << Analysis_Terminator[i];
	for (int i = 0; i < VN_number; i++)
	{	cout << endl << Nonterminal[i] << "\t";
		for (int j = 0; j < VT_number + 1; j++)
			cout << LL1_Analysis[i][j] << "\t";
	}cout << endl;

	//分析
	cout << endl << "输入串: " <<  text << endl;
	cout << endl << "分析过程: " << endl;
	a = text[n++];
	STACK.push('#');
	STACK.push(S);
	while (FLAG)
	{	X = STACK.top();
		STACK.pop();
		if (is_Terminator(X))
			if (X == a)
				a = text[n++];	
			else {
				cout << "ERROR" << endl;
				return  0;
			}
		else if (X == '#')	
			if (X == a)
			{
				FLAG = false;
				cout << "单词串为该文法的句子" << endl;
			}
			else {
				cout << "ERROR" << endl;
				return  0;
			}
		else if (Analysis(X, a).compare(" ") != 0)
		{	temp = Analysis(X, a);
			if (temp.compare("ERR") == 0)
			{	cout << "ERROR" << endl;
				return  0;
			}
			cout << temp << endl;	//输出过程
			for (int k = temp.length() - 1; k > 0; k--)
				if (temp[k] == '>'|| temp[k] == '@')
					break;
				else STACK.push(temp[k]);
		}
		else {
			cout << "ERROR" << endl;
			return  0;
		}
	}
    cout << endl; 
	return 0;
}

