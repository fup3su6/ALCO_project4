# README

## 1.Topic

Instruction Scheduling

## 2.Input description

1. 輸入Register File的值
2. 輸入不同Operator的Cycle time
3. 輸入n個instruction，以-1當作結束  

:warning: 每一個instruction前面不能有空格，輸入範例如下圖一  

:warning: 每個instruction要換行

![image](https://github.com/fup3su6/ALCO_project4/blob/master/input.JPG)

## 3.Output description

輸出第幾個Cycle 

輸出Register File、RAT、RS(加減)、RS(乘除)、Buffer

如下圖範例輸出

![image](https://github.com/fup3su6/ALCO_project4/blob/master/output.jpg)

## 4.Basic method

+ 利用struct建立每個instruction還有RS需要用到的元素
+ while(getline){ }將每一行instrucion讀入存起來  
+ 切割字串將需要用到的operator跟register存起來
+ 將instruction 依序Issue如果RS還有空
+ 判斷RS裡的register，如果都有值了就執行ALU
+ Write Result 將RAT與RS的值寫入

## 5.Code analysis

```c++
#include<iostream>
#include<string>
#include<cstring>
#include<cstdio>
using namespace std;
```
使用cin/cout，`include<iostream>`  

使用string取instruction，`include<string>`  


```c++
string cal[6] = { "", "+","+","-","*","/" };
int time[6];
int f[6];
int rat[6];
```
宣告`global cal array`存operator 

宣告`global int` `time`存operator的Cycle time  

宣告`global int` `f`存register file初始值
  
宣告`global int` `rat`存RAT

```c++
struct RS {
	string str1;
	string str2;
	int oper;
	int rs1;
	int rs2;
	int rd;
	int cycle;
}rs[6];

```
刻一個`struct RS`  

`string str1` 表 **RS的第一格**  

`string str2` 表 **RS的第二格**  

`int oper` 表示RS的operator  

`int rs1` 存第一個register  

`int rs2` 存第二個register  

`int rd` 存目標register  

`int cycle` 存Issue cycle  

```c++
struct INST {
	string oper;
	int rs1;
	int rs2;
	int rd;
	int cycle;
}inst[10000];
```
刻一個`struct INST`  

`string oper`表instruction的operator  

`int rs1` 存第一個register  

`int rs2` 存第二個register  

`int rd` 存目標register  

`int cycle` 存Issue cycle  

```c++
	for (int i = 1;i < 6;i++) {
		cout << "F" << i << ": ";
		cin >> f[i];
	}
	cout << endl;
	cout << "Cycle time" << endl;
	for (int i = 1;i < 6;i++) {	
		switch (i) {
			case 1:
				cout << "ADDI: ";
				cin >> time[i];
				break;
			case 2:
				cout << "ADD: ";
				cin >> time[i];
				break;
			case 3:
				cout << "SUB: ";
				cin >> time[i];
				break;
			case 4:
				cout << "MUL: ";
				cin >> time[i];
				break;
			case 5:
				cout << "DIV: ";
				cin >> time[i];
				break;
		}	
	}
```
跑for迴圈cin `register file`和`cycle time`  

```c++
	for (int i = 0; i <= 5; i++) {
		rat[i] = 0;
		rs[i].cycle = 0;
		rs[i].oper = 0;
		rs[i].str1 = "0";
		rs[i].str2 = "0";
	}
```
初始化數值

```c++
	cout << endl;
	cout << "Enter the code:" << endl<<endl;
	string temp;
	int k = 1;

	cin.ignore();
	while (getline(cin, temp)) {
		if (temp == "-1")
			break;

		int p = temp.find(" ");
		inst[k].oper = temp.substr(0, p);
		temp.erase(0, p + 1);

		int p1 = temp.find("F");
		int p2 = temp.find(",");
		inst[k].rd = stoi(temp.substr(p1 + 1, p2 - p1 - 1));
		temp.erase(0, p2 + 1);

		p1 = temp.find("F");
		p2 = temp.find(",");
		inst[k].rs1 = stoi(temp.substr(p1 + 1, p2 - p1 - 1));
		temp.erase(0, p2 + 1);
		if (inst[k].oper == "ADDI")
			inst[k].rs2 = stoi(temp);
		else {
			p1 = temp.find("F");
			p2 = temp.size();
			inst[k].rs2 = stoi(temp.substr(p1 + 1, p2));
		}

		k++;

	}
```
輸入code並用getline讀入  

如果輸入**-1**就停止  

找到空格切出operator存起來後把位子`0`到`p+1`刪除  

找到`F`的位子=`p1` ; `,`的位子=`p2`  

切割字串從`p1+1`到`p2-p1-1`並利用`stoi`轉成`int`存起來   

重複上面的方法存完所有需要用到的register 



# ALCO_project4