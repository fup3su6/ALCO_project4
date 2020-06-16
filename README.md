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

![image](https://github.com/fup3su6/ALCO_project4/blob/master/output.JPG)

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
void print(int ct);
```
宣告print funtion  

```c++
string cal[6] = { "", "+","+","-","*","/" };
int time[6];
int f[6];
int rat[6];
int pp = 0;
int pp2 = 0;
int k = 1;
int i = 0;
```
宣告`global cal array`存operator 

宣告`global int` `time`存operator的Cycle time  

宣告`global int` `f`存register file初始值
  
宣告`global int` `rat`存RAT  

宣告`global int` `pp`存上一個write result的rs  

宣告`global int` `pp2`暫存  

宣告`global int` `k`存instrution數量  

宣告`global int` `i`跑目前的instrution     
 
```c++
struct RS {
	string str1;
	string str2;
	int oper;
	int rs1;
	int rs2;
	int rd;
	int cycle;
	int pwr;
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

`int pwr` 存前一個write result的cycle 

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
void input(int& k) {
	for (int i = 1; i < 6; i++) {
		cout << "F" << i << ": ";
		cin >> f[i];
	}
	cout << endl;
	cout << "Cycle time" << endl;
	for (int i = 1; i < 6; i++) {
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
		rs[i].pwr = 0;
	}
```
初始化數值

```c++
	cout << endl;
	cout << "Enter the code:" << endl << endl;
	string temp;
	for (int i = 0; i <= 5; i++) {
		rat[i] = 0;
		rs[i].cycle = 0;
		rs[i].oper = 0;
		rs[i].str1 = "0";
		rs[i].str2 = "0";
		rs[i].pwr = 0;
	}
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
}
```
輸入code並用getline讀入  

如果輸入**-1**就停止  

找到空格切出operator存起來後把位子`0`到`p+1`刪除  

找到`F`的位子=`p1` ; `,`的位子=`p2`  

切割字串從`p1+1`到`p2-p1-1`並利用`stoi`轉成`int`存起來   

重複上面的方法存完所有需要用到的register 

```c++
int main() {
	input(k);

	int ct = 0, n = 1;
	for (i = 1; n = 1; i++) { //如果rs空,inst空就停止
		int yy = 0;
		for (int j = 1; j <= 5; j++) {
			if (rs[j].oper == 0)
				yy++;
		}
		if (yy == 5 && i > k)
			return 0;


		int add = 0;
		int chk = 0;   //chk issue
```
如果`rs`全部為空,`i`又大於`k`代表全部issue完,結束程式  

`chk`檢查issue過了沒  

```c++
		if (i <= k) {  //issue
			if (inst[i].oper == "ADDI" || inst[i].oper == "ADD" || inst[i].oper == "SUB") {
				while (true) {
					if (chk == 1)
						break;
					if (rs[1].oper == 0 || rs[2].oper == 0 || rs[3].oper == 0) { //如果有位子   issue
						chk = 1;
						ct++;
						for (int j = 1; j <= 3; j++) {//issue
							if (rs[j].oper == 0) {
								rs[j].rd = inst[i].rd;//放rd

								if (rat[inst[i].rs1] == 0)//放rs1
									rs[j].rs1 = f[inst[i].rs1];
								else
									rs[j].str1 = "RS" + to_string(rat[inst[i].rs1]);

								if (inst[i].oper == "ADDI") {//放rs2
									rs[j].rs2 = inst[i].rs2;
								}
								else {
									if (rat[inst[i].rs2] == 0)
										rs[j].rs2 = f[inst[i].rs2];
									else
										rs[j].str2 = "RS" + to_string(rat[inst[i].rs2]);
								}

								rs[j].cycle = i + add;  //issue cycle
								rat[inst[i].rd] = j;
								if (inst[i].oper == "ADDI") {
									rs[j].oper = 1;
									print(ct);
									break;
								}
								else if (inst[i].oper == "ADD") {
									rs[j].oper = 2;
									print(ct);
									break;
								}

								else if (inst[i].oper == "SUB") {
									rs[j].oper = 3;
									print(ct);
									break;
								}

							}
						}
					}
					else {  //沒有空位
						ct++;
						add++;
						print(ct);
					}
				}
			}
```
判斷如果issue為`ADDI/ADD/SUB`且`rs`有空位,將值存入rs  

issue完呼叫`print funtion`把output印出  

```c++
			if (inst[i].oper == "MUL" || inst[i].oper == "DIV") {
				while (true) {
					if (chk == 1)
						break;
					if (rs[4].oper == 0 || rs[5].oper == 0) {
						chk = 1;
						ct++;
						for (int j = 4; j <= 5; j++) {
							if (rs[j].oper == 0) {
								rs[j].rd = inst[i].rd; //放rd

								if (rat[inst[i].rs1] == 0) //放rs1
									rs[j].rs1 = f[inst[i].rs1];
								else
									rs[j].str1 = "RS" + to_string(rat[inst[i].rs1]);

								if (rat[inst[i].rs2] == 0) {  //放rs2
									rs[j].rs2 = f[inst[i].rs2];
									if (rs[j].rs2 == 0) {
										cout << "Exception:  divide by 0 !!!" << endl;
										return 0;
									}

								}
								else
									rs[j].str2 = "RS" + to_string(rat[inst[i].rs2]);

								rs[j].cycle = i + add;
								rat[inst[i].rd] = j;
								if (inst[i].oper == "DIV") {
									rs[j].oper = 5;
									print(ct);
									break;
								}
								else if (inst[i].oper == "MUL") {
									rs[j].oper = 4;
									print(ct);
									break;
								}
							}
						}
					}
					else {
						add++;
						ct++;
						print(ct);
					}

				}
			}
		}
		else
			print(ct++);
	}
}
```
判斷如果issue為`MUL/DIV`且`rs`有空位,將值存入rs  

**注意**如果DIV的**除數為0**,跳出例外狀況結束程式    

issue完呼叫`print funtion`把output印出 

 ```c++
void print(int ct) {
	string buf = "0";
	int value;
	int prev = 0, prev2 = 0;

	for (int j = 1; j <= 3; j++) {
		if (rs[j].oper == 1 || rs[j].oper == 2)
			value = rs[j].rs1 + rs[j].rs2;
		if (rs[j].oper == 3)
			value = rs[j].rs1 - rs[j].rs2;
		if (rs[j].oper == 4)
			value = rs[j].rs1 * rs[j].rs2;
		if (rs[j].oper == 5)
			value = rs[j].rs1 / rs[j].rs2;
```
`string buf` 存buffer要印出的字串   
 
`int value` 存寫回的值  

`int prev` 存上一個write result的cycle  

跑for迴圈計算value的值  

```c++
		if (rs[j].str1 == "0" && rs[j].str2 == "0" && rs[j].oper != 0) {	//exec
			if (rs[j].cycle + 1 == ct) {
				buf = "(RS" + to_string(j) + ") " + to_string(rs[j].rs1) + cal[rs[j].oper] + to_string(rs[j].rs2);
				pp2 = j;
			}
			if (rs[j].cycle + 1 + time[rs[j].oper] == ct) { //wr
				f[rs[j].rd] = value;
				rs[j].oper = 0;  //rs out
				int a = 0;
				for (int p = 1; p <= 5; p++) {
					for (int q = 1; q <= 3; q++) {
						if (rs[q].str1 == "RS" + to_string(j) || rs[q].str2 == "RS" + to_string(j)) {
							if ("RS" + to_string(j) == rs[q].str1) { //wr rs{
								rs[q].rs1 = value;
								rs[q].str1 = "0";
							}
							if ("RS" + to_string(j) == rs[q].str2) {
								rs[q].rs2 = value;
								rs[q].str2 = "0";
							}
						}
						if (rat[p] == j)
							rat[p] = 0;
					}

				}
			}
```	
第一個if判斷等到值時在對應的cycle time execute  

 `if (rs[j].cycle + 1 == ct)`  放入buf 表示ALU正在執行  
 
 `pp2 = j;`將目前的register 存入暫存  
 
 `if (rs[j].cycle + 1 + time[rs[j].oper] == ct)`  判斷execute完後 write back  
 
 `f[rs[j].rd] = value;` value 寫入RF ; `rs[j].oper = 0;` free RS  
 
 `if (rs[q].str1 == "RS" + to_string(j) || rs[q].str2 == "RS" + to_string(j))` dispatch ,結束後裡面write back 回rs  
 
`if (rat[p] == j) rat[p] = 0;`  erase RAT    

```c++
	string buf2 = "0";
	for (int j = 4; j <= 5; j++) {
		if (rs[j].oper == 1 || rs[j].oper == 2)
			value = rs[j].rs1 + rs[j].rs2;
		if (rs[j].oper == 3)
			value = rs[j].rs1 - rs[j].rs2;
		if (rs[j].oper == 4)
			value = rs[j].rs1 * rs[j].rs2;
		if (rs[j].oper == 5)
			value = rs[j].rs1 / rs[j].rs2;
```
 `string buf2` 存buffer要印出的字串    
 
 跑for迴圈計算value的值   
 
 ```c++
		if (rs[j].str1 == "0" && rs[j].str2 == "0" && rs[j].oper != 0) {//exec 
			if ((rs[j].cycle + 1 <= ct && prev == 0) || (prev < ct && prev != 0)) {
				buf2 = "(RS" + to_string(j) + ") " + to_string(rs[j].rs1) + cal[rs[j].oper] + to_string(rs[j].rs2);
				if (rs[j].oper != 0)
					prev = rs[j].cycle + time[rs[j].oper];
				else
					prev = 0;
			}

			if (pp == 0) {
				if (((rs[j].cycle + time[rs[j].oper] + 1 == ct && rs[j].pwr == 0) || (rs[j].pwr + time[rs[j].oper] == ct && rs[j].pwr != 0))) { //wr	
					f[rs[j].rd] = value;
					rs[j].oper = 0;  //rs out
					int a = 0;
					for (int p = 1; p <= 5; p++) {
						for (int q = 1; q <= 3; q++) {
							if (rs[q].str1 == "RS" + to_string(j) || rs[q].str2 == "RS" + to_string(j)) {
								cout << "p: " << p << "  j: " << j << "  q: " << q << endl << endl;
								if ("RS" + to_string(j) == rs[q].str1) { //wr rs
									rs[q].rs1 = value;
									rs[q].str1 = "0";
								}
								if ("RS" + to_string(j) == rs[q].str2) {
									rs[q].rs2 = value;
									rs[q].str2 = "0";
								}
							}
							if (rat[p] == j)
								rat[p] = 0;
						}
					}
					pp = j;
					rs[j].pwr = ct;
				}


			}
		}
```
 `if (rs[j].str1 == "0" && rs[j].str2 == "0" && rs[j].oper != 0)` exec  

 `if ((rs[j].cycle + 1 <= ct && prev == 0) || (prev < ct && prev != 0))` prev紀錄上一次的結束後cycle time 判斷是否可execute下一個  
 
 `if (pp == 0)` 如果是第一次被execute  
 
 `if (((rs[j].cycle + time[rs[j].oper] + 1 == ct && rs[j].pwr == 0) || (rs[j].pwr + time[rs[j].oper] == ct && rs[j].pwr != 0)))` execute完後 write back, `pwr`存上一個write back的cycle time  
 
 `pp = j;` 存write back 的register  ,`rs[j].pwr = ct; `  紀錄write back cycle time  
 
 ```c++
	cout << "Cycle: " << ct << endl << endl;
	cout << "     _ RF __" << endl;
	cout << "  F1 |   " << f[1] << " |" << endl;
	cout << "  F2 |   " << f[2] << " |" << endl;
	cout << "  F3 |   " << f[3] << " |" << endl;
	cout << "  F4 |   " << f[4] << " |" << endl;
	cout << "  F5 |   " << f[5] << " |" << endl;
	cout << "     -------" << endl;

	cout << "     _ RAT __" << endl;
	for (int j = 1; j <= 5; j++) {
		if (rat[j] == 0)
			cout << "  F" + to_string(j) + " |       |" << endl;
		else
			cout << "  F" + to_string(j) + " |  " << " RS" << rat[j] << " |" << endl;
	}
	cout << "     -------\n" << endl;

	cout << "    _ RS _________________" << endl;
	for (int j = 1; j <= 3; j++) {
		if (rs[j].oper == 0)
			cout << "RS" << to_string(j) << " |      |      |      |\n";
		else {
			if (rs[j].str1 != "0") {
				if (rs[j].str2 != "0")
					cout << "RS" << to_string(j) << " |    " << cal[rs[j].oper] << " |  " << rs[j].str1 << " |  " << rs[j].str2 << " |\n";
				else
					cout << "RS" << to_string(j) << " |    " << cal[rs[j].oper] << " |  " << rs[j].str1 << " |    " << rs[j].rs2 << " |\n";
			}
			else {
				if (rs[j].str2 != "0")
					cout << "RS" << to_string(j) << " |    " << cal[rs[j].oper] << " |    " << rs[j].rs1 << " |  " << rs[j].str2 << " |\n";

				else
					cout << "RS" << to_string(j) << " |    " << cal[rs[j].oper] << " |    " << rs[j].rs1 << " |    " << rs[j].rs2 << " |\n";
			}
		}
	}
	cout << "    ----------------------\n" << endl;

	cout << "BUFFER: ";
	if (buf == "0")
		cout << "empty\n\n";
	else {
		cout << buf << endl << endl;
	}

	cout << "    _ RS _________________" << endl;
	for (int j = 4; j <= 5; j++) {
		if (rs[j].oper == 0)
			cout << "RS" << to_string(j) << " |      |      |      |\n";
		else {
			if (rs[j].str1 != "0") {
				if (rs[j].str2 != "0")
					cout << "RS" << to_string(j) << " |    " << cal[rs[j].oper] << " |  " << rs[j].str1 << " |  " << rs[j].str2 << " |\n";
				else
					cout << "RS" << to_string(j) << " |    " << cal[rs[j].oper] << " |  " << rs[j].str1 << " |    " << rs[j].rs2 << " |\n";
			}
			else {
				if (rs[j].str2 != "0")
					cout << "RS" << to_string(j) << " |    " << cal[rs[j].oper] << " |    " << rs[j].rs1 << " |  " << rs[j].str2 << " |\n";
				else
					cout << "RS" << to_string(j) << " |    " << cal[rs[j].oper] << " |    " << rs[j].rs1 << " |    " << rs[j].rs2 << " |\n";
			}
		}
	}
	cout << "    ----------------------\n" << endl;

	int check = 0;
	for (int c = 1; c <= 5; c++)
		if (rs[c].oper == 0)
			check++;
	if (check == 5 && i > k) {
		buf = "0";
		buf2 = "0";
	}

	cout << "BUFFER: ";
	if (buf2 == "0")
		cout << "empty\n\n";
	else
		cout << buf2 << endl << endl;
}
```
剩下cout依照題目要求格式輸出