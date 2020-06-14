#include<iostream>
#include<string>
#include<cstring>
#include<cstdio>
using namespace std;

string cal[6] = { "", "+","+","-","*","/" };
int time[6] = { 0,1,1,1,5,10 };
int f[6] = { 0,0,2,4,6,8 };

int rat[6];

struct RS {
	string str1;
	string str2;
	int oper;
	int rs1;
	int rs2;
	int rd;
	int cycle;
}rs[6];

struct INST {
	string oper;
	int rs1;
	int rs2;
	int rd;
	int cycle;
}inst[10000];

int main() {
	string temp;
	int k = 1;
	for (int i = 0; i <= 5; i++) {
		rat[i] = 0;
		rs[i].cycle = 0;
		rs[i].oper = 0;
		rs[i].str1 = "0";
		rs[i].str2 = "0";
	}
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
	int cycle[6] = { 0 }, ct = 0;
	for (int i = 1; i <= k; i++) {
		int add = 0;
		int chk = 0;   //chk issue
		if (inst[i].oper == "ADDI" || inst[i].oper == "ADD" || inst[i].oper == "SUB") {
			while (true) {
				if (chk == 1)
					break;
				if (rs[1].oper == 0 || rs[2].oper == 0 || rs[3].oper == 0) { //���Ŧ�
					chk = 1;
					for (int j = 1; j <= 3; j++) {
						if (rs[j].oper == 0) {
							rs[j].rd = inst[i].rd;
							rs[j].rs1 = inst[i].rs1;
							rs[j].rs2 = inst[i].rs2;
							rs[j].cycle = i + add;  //issue cycle
							//cout << "Cycle: " << rs[j].cycle << endl;
							if (inst[i].oper == "ADDI") {
								rs[j].oper = 1;
								rat[inst[i].rd] = j;
								break;
							}
							else if (inst[i].oper == "ADD") {
								rs[j].oper = 2;
								rat[inst[i].rd] = j;
								break;
							}

							else if (inst[i].oper == "SUB") {
								rs[j].oper = 3;
								rat[inst[i].rd] = j;
								break;
							}

						}
					}
				}
				else {  //�S�Ŧ�
					add++;
				}
			}
		}
		if (inst[i].oper == "MUL" || inst[i].oper == "DIV") {
			while (true) {
				if (chk == 1)
					break;
				chk = 1;
				if (rs[4].oper == 0 || rs[5].oper == 0) {
					for (int j = 4; j <= 5; j++) {
						if (rs[j].oper == 0) {
							rs[j].rd = inst[i].rd;
							rs[j].rs1 = inst[i].rs1;
							rs[j].rs2 = inst[i].rs2;
							rs[j].cycle = i + add;
							//cout << "Cycle: " << rs[j].cycle << endl;
							if (inst[i].oper == "DIV") {
								rs[j].oper = 5;
								rat[inst[i].rd] = j;
								break;
							}
							else if (inst[i].oper == "MUL") {
								rs[j].oper = 4;
								rat[inst[i].rd] = j;
								break;
							}
						}
					}
				}
				else
					add++;
			}
		}

		string buf = "0";
		for (int j = 1; j <= 3; j++) {
			if (rat[rs[j].rs1] == 0 && rs[j].oper == 1 && rs[j].oper != 0) {
				if (cycle[rs[j].oper] + 1 == time[rs[j].oper])
					buf = "(RS" + to_string(j) + ") " + to_string(rs[j].rs1) + cal[rs[j].oper] + to_string(rs[j].rs2);

				if (cycle[1] == time[1]) {
					int value = f[rs[j].rs1] + rs[j].rs2;
					for (int p = 1; p <= 5; p++) {
						if (rat[p] == j) {  //wr rat
							f[rs[j].rd] = value;
							if ("RS" + to_string(rat[p]) == rs[p].str1) //wr rs
								rs[j].rs1 = value;
							if ("RS" + to_string(rat[p]) == rs[p].str2)
								rs[j].rs2 = value;

							rat[p] = 0;
						}
					}
					rs[j].oper = 0;  //rs out
				}
				else
					cycle[rs[j].oper]++;
				break;
			}
			if (rat[rs[j].rs1] == 0 && rat[rs[j].rs2] == 0 && rs[j].oper != 0) {
				if (cycle[rs[j].oper]+1 == time[rs[j].oper])
					buf = "(RS" + to_string(j) + ") " + to_string(rs[j].rs1) + cal[rs[j].oper] + to_string(rs[j].rs2);
				if (cycle[rs[j].oper] == time[rs[j].oper]) {
					int value = f[rs[j].rs1] + rs[j].rs2;
					for (int p = 1; p <= 5; p++) {
						if (rat[p] == j) {  //wr rat
							f[rs[j].rd] = value;
							if ("RS" + to_string(rat[p]) == rs[p].str1) //wr rs
								rs[j].rs1 = value;
							if ("RS" + to_string(rat[p]) == rs[p].str2)
								rs[j].rs2 = value;

							rat[p] = 0;
						}
					}
					rs[j].oper = 0;  //rs out
				}
				else
					cycle[rs[j].oper]++;
				break;
			}
		}


		cout << "Cycle: " << ++ct << endl << endl;
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
				if (rat[rs[j].rs1] != 0) {
					rs[j].str1 = "RS" + to_string(rat[rs[j].rs1]);
					if (rat[rs[j].rs2] != 0) {
						rs[j].str2 = "RS" + to_string(rat[rs[j].rs2]);
						cout << "RS" << to_string(j) << " |    " << cal[rs[j].oper] << " |  " << "RS" << rat[rs[j].rs1] << " |  " << "RS" << rat[rs[j].rs2] << " |\n";
					}
					else
						cout << "RS" << to_string(j) << " |    " << cal[rs[j].oper] << " |  " << "RS" << rat[rs[j].rs1] << " |    " << rs[j].rs2 << " |\n";
				}
				else {
					if (rat[rs[j].rs2] != 0) {
						rs[j].str2 = "RS" + to_string(rat[rs[j].rs2]);
						cout << "RS" << to_string(j) << " |    " << cal[rs[j].oper] << " |    " << rs[j].rs1 << " |  " << "RS" << rat[rs[j].rs2] << " |\n";
					}
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
		string buf2 = "0";
		for (int j = 4; j <= 5; j++) {
			if (rat[rs[j].rs1] == 0 && rat[rs[j].rs2] == 0 && rs[j].oper != 0) {
				if (cycle[rs[j].oper] + 1 == time[rs[j].oper])
					buf2 = "(RS" + to_string(j) + ") " + to_string(rs[j].rs1) + cal[rs[j].oper] + to_string(rs[j].rs2);
				if (cycle[rs[j].oper] == time[rs[j].oper]) {
					int value = f[rs[j].rs1] + rs[j].rs2;
					for (int p = 1; p <= 5; p++) {
						if (rat[p] == j) {  //wr rat
							f[rs[j].rd] = value;
							if ("RS" + to_string(rat[p]) == rs[p].str1) //wr rs
								rs[j].rs1 = value;
							if ("RS" + to_string(rat[p]) == rs[p].str2)
								rs[j].rs2 = value;

							rat[p] = 0;
						}
					}
					rs[j].oper = 0;  //rs out
				}
				else
					cycle[rs[j].oper]++;
				break;
			}
		}

		cout << "    _ RS _________________" << endl;
		for (int j = 4; j <= 5; j++) {
			if (rs[j].oper == 0)
				cout << "RS" << to_string(j) << " |      |      |      |\n";
			else {
				if (rat[rs[j].rs1] != 0) {
					rs[j].str1 = "RS" + to_string(rat[rs[j].rs1]);
					if (rat[rs[j].rs2] != 0) {
						rs[j].str2 = "RS" + to_string(rat[rs[j].rs2]);
						cout << "RS" << to_string(j) << " |    " << cal[rs[j].oper] << " |  " << "RS" << rat[rs[j].rs1] << " |  " << "RS" << rat[rs[j].rs2] << " |\n";
					}
					else
						cout << "RS" << to_string(j) << " |    " << cal[rs[j].oper] << " |  " << "RS" << rat[rs[j].rs1] << " |    " << rs[j].rs2 << " |\n";
				}
				else {
					if (rat[rs[j].rs2] != 0) {
						rs[j].str2 = "RS" + to_string(rat[rs[j].rs2]);
						cout << "RS" << to_string(j) << " |    " << cal[rs[j].oper] << " |    " << rs[j].rs1 << " |  " << "RS" << rat[rs[j].rs2] << " |\n";
					}
					else
						cout << "RS" << to_string(j) << " |    " << cal[rs[j].oper] << " |    " << rs[j].rs1 << " |    " << rs[j].rs2 << " |\n";
				}
			}
		}
		cout << "    ----------------------\n" << endl;
		cout << "BUFFER: ";
		if (buf2 == "0")
			cout << "empty\n\n";
		else
			cout << buf2 << endl << endl;
	}

			system("pause");
	}



