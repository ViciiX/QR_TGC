#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <ctime>
#include <vector>
#include <algorithm>
#include <sys/stat.h>

using namespace std;

//核心
vector< vector<string> > eventName,eventOption,eventReturn,eventAction,eventCommand,eventQuestion;

//关于设置
string defaultPath = "/storage/emuated/0/";
string defaultWorkPath = "/storage/emulated/0/QR_TGC/";
string extraCommand = "";
string beginCommand = "&begin";
string continueCommand = "&continue";
string mainCommand = "_begin";
string dataFolder = "进程/%群号%/%QQ%/";

//变量&数组等声明区
string path;
string answer, raw_file, Gstr;

vector<string> lineFiles;
vector< vector<string> >files;

//函数原型定义区
void cinClear();
void wait(int s);
void printColorString(string str,string color);
void clear();
void process(vector<string> code);
void generate();
string replace(string str1, string str2, string str3);
void load();
void getLoad(string &var, string Path);
void save(string value, string Path);
void mkdir(string p);
void patch();
string zip(vector<string> v,string sp);

void GUI_Main();
string GUI_startChoose();
string GUI_startSetting();
void GUI_startConvert();

//常量区
const string C_GRAY = "\033[1;30m";
const string C_RED = "\033[31m";
const string C_GREEN = "\033[32m";
const string C_YELLOW = "\033[33m";
const string C_BLUE = "\033[34m";
const string C_PURPLE = "\033[35m";
const string C_CYAN = "\033[36m";
const string C_WHITE = "\033[37m";
const string C_L_RED = "\033[1;31m";
const string C_L_GREEN = "\033[1;32m";
const string C_L_YELLOW = "\033[1;33m";
const string C_L_BLUE = "\033[34m";
const string C_L_PURPLE = "\033[35m";
const string C_L_CYAN = "\033[36m";
const string C_S_GREEN = "\033[7;32m";
const string C_S_RED = "\033[7;31m";

int main()
{
	printColorString("欢迎使用 QRSpeed_Text_Game_Converter",C_L_GREEN);
	wait(1000);
	while (true)
	{
		load();
		GUI_Main();
	}
	return 0;
}

void load()
{
	patch();
	getLoad(defaultPath,"Settings/defaultPath");
	getLoad(extraCommand,"Settings/extraCommand");
	getLoad(extraCommand,extraCommand);
	getLoad(beginCommand,"Settings/beginCommand");
	getLoad(continueCommand,"Settings/continueCommand");
	getLoad(mainCommand,"Settings/mainCommand");
	getLoad(defaultWorkPath,"Settings/defaultWorkPath");
	getLoad(dataFolder,"Settings/dataFolder");
}

void patch()
{
	mkdir("/storage/emulated/0/QR_TGC");
	mkdir("Settings");
}

void mkdir(string p)
{
	struct stat buffer;
	if (stat(p.c_str(),&buffer)!=0)
	{
		system(("mkdir -p "+p).c_str());
	}
}

void getLoad(string &var, string Path)
{
	struct stat buffer;
	if (stat(Path.c_str(),&buffer) == 0)
	{
		ifstream in;
		in.open(Path, ios::in);
		string v,a;
		if (in.good())
		{
			while (!in.eof())
			{
				getline(in,a);
				v = v + a + "\n";
			}
		}
		var = v.substr(0,v.length()-1);
		in.close();
	}
}

void save(string value, string Path)
{
	ofstream out;
	out.open(Path, ios::trunc | ios::out);
	out << value;
	out.close();
}

void clear()
{
	system("clear");
}

void wait(int s)//等待时间
{
	clock_t lastClock = clock();//记录当前系统时间
	clock_t targetClock = CLOCKS_PER_SEC / 1000 * s;//目标系统时间，CLOCKS_PER_SEC返回一秒包含的系统时间数，除以1000乘s就是要等待的系统时间0
	while (clock()-lastClock < targetClock);
}

void cinClear()//清除键盘缓冲序列
{
	cin.clear();
	while (cin.get() != '\n');
}

string zip(vector<string> v,string sp)
{
	string r;
	for (string s:v)
	{
		r = r+s+sp;
	}
	return r.substr(0,r.length()-sp.length());
}

void printColorString(string str,string color)//打印有颜色的字符串
{
	cout << color << str << "\033[0m" << flush;
}

//GUI

void GUI_Main()
{
	answer = GUI_startChoose();
	if (answer == "开始转换")
	{
		path =  defaultPath;
		printColorString("输入QTG文件路径：\n"+path,C_GRAY);
		getline(cin,answer);
		if (answer != "")
		{
			path += answer;
			lineFiles.clear();
			files.clear();
			eventName.clear();
			eventOption.clear();
			eventReturn.clear();
			eventAction.clear();
			eventCommand.clear();
			eventQuestion.clear();
			GUI_startConvert();
		}
	}
	else if (answer == "关于")
	{
		printColorString("QR_TGC V1.2.1 beta\nBy ScerX\n",C_L_CYAN);
		printColorString("由C++编写\n",C_YELLOW);
		printColorString("联系方式QQ:181275358\n\n",C_GRAY);
		printColorString("更新内容：\n",C_L_GREEN);
		printColorString("修复不能重复读取文件的bug\n添加新附属关键字“.all”",C_GREEN);
		printColorString("\n按下回车返回主界面",C_L_YELLOW);
		cin.get();
		//GUI_Main();
	}
	else if (answer == "设置")
	{
		answer = GUI_startSetting();
		if (answer == "默认读取目录")
		{
			printColorString("默认读取目录，指输入QTG文件路径时默认的目录\n默认为",C_GREEN);
			printColorString("/storage/emulated/0/\n",C_L_GREEN);
			printColorString("当前为",C_GREEN);
			printColorString(defaultPath+"\n\n",C_L_GREEN);
			printColorString("请输入新的默认读取路径（留空为退出）：",C_L_YELLOW);
			getline(cin,answer);
			if (answer.back() != '/')
			{
				answer += "/";
			}
			if (answer != "/")
			{
				save(answer,"Settings/defaultPath");
			}
		}
		else if (answer == "输出文件")
		{
			printColorString("输出文件，指程序输出文件的路径\n默认为",C_GREEN);
			printColorString("/storage/emulated/0/QR_TGC/dic.txt\n",C_L_GREEN);
			printColorString("当前为",C_GREEN);
			printColorString(defaultWorkPath+"\n\n",C_L_GREEN);
			printColorString("请输入新的输出文件（留空为退出）：",C_L_YELLOW);
			getline(cin,answer);
			if (answer != "")
			{
				save(answer,"Settings/defaultWorkPath");
			}
		}
		else if (answer == "\n附加指令")
		{
			printColorString("生成的qr代码前放入的指令，默认为",C_GREEN);
			printColorString("空\n\n",C_L_GREEN);
			printColorString("请输入存储附加指令的文件路径:\n"+defaultPath,C_L_YELLOW);
			getline(cin,answer);
			save(defaultPath+answer,"Settings/extraCommand");
		}
		else if (answer == "begin指令")
		{
			printColorString("修改默认的开始指令，默认为",C_GREEN);
			printColorString("&begin\n",C_L_GREEN);
			printColorString("当前为",C_GREEN);
			printColorString(beginCommand+"\n\n",C_L_GREEN);
			printColorString("请输入新的开始指令（留空为退出）：",C_L_YELLOW);
			getline(cin,answer);
			if (answer != "")
			{
				save(answer,"Settings/beginCommand");
			}
		}
		else if (answer == "continue指令")
		{
			printColorString("修改默认的继续指令，默认为",C_GREEN);
			printColorString("&continue\n",C_L_GREEN);
			printColorString("当前为",C_GREEN);
			printColorString(continueCommand+"\n\n",C_L_GREEN);
			printColorString("请输入新的继续指令（留空为退出）：",C_L_YELLOW);
			getline(cin,answer);
			if (answer != "")
			{
				save(answer,"Settings/continueCommand");
			}
		}
		else if (answer == "Main事件名称")
		{
			printColorString("修改默认开始时调用的事件名称，默认为",C_GREEN);
			printColorString("_begin\n",C_L_GREEN);
			printColorString("当前为",C_GREEN);
			printColorString(mainCommand+"\n\n",C_L_GREEN);
			printColorString("请输入新的开始指令（留空为退出）：",C_L_YELLOW);
			getline(cin,answer);
			if (answer != "")
			{
				save(answer,"Settings/mainCommand");
			}
		}
		else if (answer == "数据目录")
		{
			printColorString("更改用于存储qr数据的目录（文件夹），默认为",C_GREEN);
			printColorString("进程/%群号%/%QQ%/\n",C_L_GREEN);
			printColorString("当前为",C_GREEN);
			printColorString(dataFolder+"\n\n",C_L_GREEN);
			printColorString("请输入新的数据目录（留空为退出）",C_L_YELLOW);
			getline(cin,answer);
			if (answer.back() != '/')
			{
				answer += "/";
			}
			if (answer != "/")
			{
				save(answer,"Settings/dataFolder");
			}
			
		}
		if (answer!="退出")
		{
			clear();
			printColorString("修改完毕",C_L_GREEN);
			wait(1000);
		}
	}
	else
	{
		printColorString("没准备好",C_L_RED);
		wait(1000);
	}
}

string GUI_startChoose()
{
	int int_i,int_j{0};
	string options[] = {"开始转换","设置","关于"};
	clear();
	while (char(int_i) != '\n')
	{
		printColorString("主界面\n",C_L_GREEN);
		printColorString("(AD=左右，Enter=确认）\n\n",C_L_YELLOW);
		//判断输入行动
		if (char(int_i)=='a')
		{
			if (int_j > 0)
			{
				int_j --;
			}
			else
			{
				int_j = (sizeof(options)/sizeof(options[0]))-1;
			}
		}
		else if (char(int_i)=='d')
		{
			if (int_j < (sizeof(options)/sizeof(options[0]))-1)
			{
				int_j ++;
			}
			else
			{
				int_j = 0;
			}
		}
			
		//输出
		for (int for_i=0; for_i < (sizeof(options)/sizeof(options[0])); for_i++)
		{
			if (for_i == int_j)
			{
				printColorString(options[for_i],C_YELLOW);
			}
			else
			{
				printColorString(options[for_i],C_GRAY);
			}
			cout << " ";
		}
		int_i = getch();
		clear();
	}
	return options[int_j];
}

string GUI_startSetting()
{
	int int_i,int_j{0};
	string options[] = {"退出","默认读取目录","输出文件","\n附加指令","begin指令","continue指令","Main事件名称","数据目录"};
	string hint[] = {"退出","输入QTG文件路径时默认的目录","程序输出文件的路径","生成的qr代码前放入的指令","修改默认的开始指令","修改默认的继续指令","修改默认开始时调用的事件名称","更改用于存储qr数据的目录（文件夹）"};
	clear();
	while (char(int_i) != '\n')
	{
		printColorString("设置\n",C_L_GREEN);
		printColorString("(WS=上下，Enter=编辑）\n----------\n",C_L_YELLOW);
		//判断输入行动
		if (char(int_i)=='w')
		{
			if (int_j > 0)
			{
				int_j --;
			}
			else
			{
				int_j = (sizeof(options)/sizeof(options[0]))-1;
			}
		}
		else if (char(int_i)=='s')
		{
			if (int_j < (sizeof(options)/sizeof(options[0]))-1)
			{
				int_j ++;
			}
			else
			{
				int_j = 0;
			}
		}
			
		//输出
		for (int for_i=0; for_i < (sizeof(options)/sizeof(options[0])); for_i++)
		{
			if (for_i == int_j)
			{
				printColorString(options[for_i],C_YELLOW);
			}
			else
			{
				printColorString(options[for_i],C_GRAY);
			}
			cout << "\n";
		}
		printColorString("\n----------\n"+hint[int_j]+"\n",C_YELLOW);
		int_i = getch();
		clear();
	}
	return options[int_j];
}

void GUI_startConvert()
{
	//读取文件
	vector<string> v_str;
	string a;
	ifstream iFile;
	iFile.open(path,ios::in);
	if (!iFile.good())
	{
		clear();
		printColorString("文件不存在",C_RED);
		wait(1500);
	}
	else
	{
		while (!iFile.eof())
		{
			getline(iFile,a);
			lineFiles.push_back(a);
			raw_file += a+ "\n";
		}
		iFile.close();
		//转换
	
		clear();
		printColorString(path+"\n",C_L_GREEN);
		printColorString(extraCommand+"\n\n",C_GRAY);
		for (string print:lineFiles)
		{
			printColorString(print+"\n",C_GRAY);
		}
		printColorString("\n按下回车开始生成文件",C_L_YELLOW);
		cin.get();
		clear();
	
		//cout << raw_file << endl;
		for (string for_c:lineFiles)
		{
			if (int(for_c[0]) != 0)
			{
				v_str.push_back(for_c);
			}
			else
			{
				files.push_back(v_str);
				v_str.clear();
			}
		}
		files.push_back(v_str);
		for (vector<string> code:files)
		{
			process(code);
		}
		generate();
	}
}

void process(vector<string> code)//处理单事件
{
	int for_i{0},x_i{0},id=eventName.size();
	vector< vector<string> > o(1),r(1),a(1),c(1),q(1);
	vector<string> x_vs,eventAll,eventAllValue;
	x_vs.push_back(code[0]);
	eventName.push_back(x_vs);
	for_i ++;
	while (for_i < code.size())
	{
		if (code[for_i+1]=="{")
		{
			x_i = for_i + 2;
			x_vs.clear();
			while (code[x_i] != "}")
			{
				x_vs.push_back(replace(code[x_i],"\n","\\n"));
				x_i ++;
			}
			switch (code[for_i][0])
			{
				case 'o':
					o[0] = x_vs;
					eventOption.push_back(o[0]);
					break;
				case 'r':
					r[0] = x_vs;
					eventReturn.push_back(r[0]);
					break;
				case 'a':
					a[0] = x_vs;
					eventAction.push_back(a[0]);
					break;
				case 'c':
					c[0] = x_vs;
					eventCommand.push_back(c[0]);
					break;
				case 'q':
					q[0] = x_vs;
					eventQuestion.push_back(q[0]);
					break;
			}
			if (code[for_i].length() > 4 && code[for_i].substr(code[for_i].length()-4,code[for_i].length())==".all")
			{
				eventAll.push_back(code[for_i].substr(0,1));
				eventAllValue.push_back(zip(x_vs,"\\n"));
			}
			for_i = x_i;
		}
		for_i ++;
	}
	vector<int> sizes;
	int vectorMax;
	while (eventOption.size() < eventName.size())
	{
		eventOption.push_back({});
	}
	while (eventReturn.size() < eventName.size())
	{
		eventReturn.push_back({});
	}
	while (eventAction.size() < eventName.size())
	{
		eventAction.push_back({});
	}
	while (eventCommand.size() < eventName.size())
	{
		eventCommand.push_back({});
	}
	while (eventQuestion.size() < eventName.size())
	{
		eventQuestion.push_back({});
	}
	
	sizes.clear();
	sizes.push_back(eventOption[id].size());
	sizes.push_back(eventReturn[id].size());
	sizes.push_back(eventAction[id].size());
	sizes.push_back(eventCommand[id].size());
	sizes.push_back(eventQuestion[id].size());
	vectorMax = *max_element(sizes.begin(),sizes.end()); 
	while (eventOption[id].size() < vectorMax)
	{
		eventOption[id].push_back({""});
	}
	while (eventReturn[id].size() < vectorMax)
	{
		eventReturn[id].push_back({""});
	}
	while (eventAction[id].size() < vectorMax)
	{
		eventAction[id].push_back({""});
	}
	while (eventCommand[id].size() < vectorMax)
	{
		eventCommand[id].push_back({""});
	}
	while (eventQuestion[id].size() < vectorMax)
	{
		eventQuestion[id].push_back({""});
	}
	
	for (int for_s{0}; for_s < eventAll.size(); for_s ++)
	{
		for (int for_i = 0; for_i < vectorMax; for_i++)
		{
			if (eventAll[for_s] == "r")
			{
				eventReturn[id][for_i] = eventAllValue[for_s];
			}
			else if (eventAll[for_s] == "o")
			{
				eventOption[id][for_i] = eventAllValue[for_s];
			}
			else if (eventAll[for_s] == "a")
			{
				eventAction[id][for_i] = eventAllValue[for_s];
			}
			else if (eventAll[for_s] == "c")
			{
				eventCommand[id][for_i] = eventAllValue[for_s];
			}
			else if (eventAll[for_s] == "q")
			{
				eventQuestion[id][for_i] = eventAllValue[for_s];
			}
		}
	}
}

void generate()
{
	/*
	cout << eventName.size() << endl;
	cout << eventOption.size() << endl;
	cout << eventReturn.size() << endl;
	cout << eventAction.size() << endl;
	cout << eventCommand.size() << endl;
	cout << eventQuestion.size() << endl;
	cin.get();
	*/
	Gstr = extraCommand;
	Gstr += "\n\n"+beginCommand+"\n$写 "+dataFolder+"回答 a 1$\n$写 "+dataFolder+"进程 a "+mainCommand+"$\n$调用 0 0$\n\n";
	Gstr += continueCommand+"\n$写 "+dataFolder+"回答 a 0$\n$调用 0 0$\n\n";
	Gstr += "[\\d]*\n进:$读 "+dataFolder+"进程 a Null$\n答:$读 "+dataFolder+"回答 a 0$\n";
	for (int i=0; i<eventName.size(); i++)
	{
		for (int j=0; j<eventOption[i].size(); j++)
		{
			Gstr += "如果:%进%==" + eventName[i][0] + "&%答%==0\n";
			if (eventQuestion[i][j] != "")
			{
				Gstr += eventQuestion[i][j] + "\\n\n";
			}
			if (eventOption[i][j] != "")
			{
				for (int k=0; k<eventOption[i].size(); k++)
				{
					Gstr += to_string(k) + "." + eventOption[i][k] + "\\n\n";
				}
			}
			Gstr += "$写 "+dataFolder+"回答 a 1$\n返回\n如果尾\n如果:%进%==" + eventName[i][0] + "&%参数-1%==" + to_string(j) + "&%答%==1\n";
			Gstr += "$写 "+dataFolder+"回答 a 0$\n";
			if (eventReturn[i][j] != "")
			{
				Gstr += eventReturn[i][j] + "\n";
				Gstr += "$发送$\n";
			}
			if (eventCommand[i][j] != "")
			{
				Gstr += eventCommand[i][j] + "\n";
			}
			if (eventAction[i][j] != "")
			{
				Gstr += "$写 "+dataFolder+"进程 a "+eventAction[i][j]+"$\n";
				Gstr += "$调用 0 0$\n";
			}
			Gstr += "返回\n如果尾\n";
		}
	}
	clear();
	printColorString("生成文件中...\n",C_GREEN);
	ofstream oFile;
	oFile.open(defaultWorkPath, ios::trunc | ios::out);
	oFile << Gstr;
	oFile.close();
	clear();
	printColorString("生成成功！\n",C_L_GREEN);
	printColorString(defaultWorkPath+"\n",C_GREEN);
	wait(1000);
	clear();
	printColorString(Gstr+"\n",C_L_CYAN);
	printColorString("按下回车返回主界面",C_L_YELLOW);
	cin.get();
	clear();
	//GUI_Main();
}

string replace(string str1, string str2, string str3)
{
	int r{0};
	string ret = str1;
	while (ret.find(str2,r)!=string::npos)
	{
		r = ret.find(str2,r);
		if (r+str2.length() < ret.length())
		{
			ret.replace(r,str2.length(),str3);
		}
		r += str2.length();
	}
	return ret;
}