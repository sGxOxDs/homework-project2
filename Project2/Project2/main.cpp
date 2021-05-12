#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

void outputallentry();

//vector<string> output;
vector<string> jumplist;

struct Entry
{
	int history = 0;
	int bc2[4] = { 0 };
};
vector<Entry> entries;

//pc
int pc = 0;

//register
int reg[32] = { 0 };
/*
//binary轉換器 int轉string
string binary(long long n, int bits, bool sign = false)
{
	string a;
	bool s = false;
	if (sign && n < 0)
	{
		n = -n;
		s = !s;
	}

	for (int j = 0; j < bits; j++)   //迴圈過程的始終，判斷n是否能被2除盡
	{
		a += n % 2 + '0';   //用陣列儲存每次除以2之後的餘數，即斷定奇偶性，對應二進位制位上數值
		n = n / 2;
	}
	if (sign && s)
		a[bits - 1] = '1';
	else if (sign)
		a[bits - 1] = '0';
	reverse(a.begin(), a.end());
	return a;
}
*/
int label(string temp)
{
	for (int i = 0; i < jumplist.size(); i++)
		if (jumplist[i] == temp)
			return i;
	return -1;
}
/*
string type_i_offset(stringstream& ss, string opcode, string f3)
{
	string temp, rd, rs1, imm;

	ss >> temp;

	getline(ss, temp, ',');
	temp.erase(temp.find_first_of('x'), 1);
	rd = binary(atoi(temp.c_str()), 5);

	getline(ss, temp, '(');
	imm = binary(atoi(temp.c_str()), 12, true);

	getline(ss, temp, ')');
	temp.erase(temp.find_first_of('x'), 1);
	rs1 = binary(atoi(temp.c_str()), 5);

	return imm + rs1 + f3 + rd + opcode;
}

string type_i(stringstream& ss, string opcode, string f3)
{
	string temp, rd, rs1, imm;

	ss >> temp;

	getline(ss, temp, ',');
	temp.erase(temp.find_first_of('x'), 1);
	rd = binary(atoi(temp.c_str()), 5);

	getline(ss, temp, ',');
	temp.erase(temp.find_first_of('x'), 1);
	rs1 = binary(atoi(temp.c_str()), 5);

	ss >> temp;
	imm = binary(atoi(temp.c_str()), 12, true);

	return imm + rs1 + f3 + rd + opcode;
}

string type_sb(stringstream& ss, string opcode, string f3, int i)
{
	string temp, rs1, rs2, imm;

	ss >> temp;

	getline(ss, temp, ',');
	temp.erase(temp.find_first_of('x'), 1);
	rs1 = binary(atoi(temp.c_str()), 5);

	getline(ss, temp, ',');
	temp.erase(temp.find_first_of('x'), 1);
	rs2 = binary(atoi(temp.c_str()), 5);

	ss >> temp;
	imm = binary(label(jumplist, temp, i), 13, true);

	string imm1, imm2;
	{
		imm1 += imm[0];
		imm1 += imm[2];
		imm1 += imm[3];
		imm1 += imm[4];
		imm1 += imm[5];
		imm1 += imm[6];
		imm1 += imm[7];

		imm2 += imm[8];
		imm2 += imm[9];
		imm2 += imm[10];
		imm2 += imm[11];
		imm2 += imm[1];
	}

	return imm1 + rs2 + rs1 + f3 + imm2 + opcode;
}

string type_s(stringstream& ss, string opcode, string f3)
{
	string temp, rs1, rs2, imm;

	ss >> temp;

	getline(ss, temp, ',');
	temp.erase(temp.find_first_of('x'), 1);
	rs2 = binary(atoi(temp.c_str()), 5);

	getline(ss, temp, '(');
	imm = binary(atoi(temp.c_str()), 12, true);

	getline(ss, temp, ')');
	temp.erase(temp.find_first_of('x'), 1);
	rs1 = binary(atoi(temp.c_str()), 5);

	string imm1, imm2;
	{
		imm1 += imm[0];
		imm1 += imm[1];
		imm1 += imm[2];
		imm1 += imm[3];
		imm1 += imm[4];
		imm1 += imm[5];
		imm1 += imm[6];

		imm2 += imm[7];
		imm2 += imm[8];
		imm2 += imm[9];
		imm2 += imm[10];
		imm2 += imm[11];
	}

	return imm1 + rs2 + rs1 + f3 + imm2 + opcode;
}

string type_r(stringstream& ss, string opcode, string f3, string f2, string f5)
{
	string temp, rd, rs1, rs2;

	ss >> temp;

	getline(ss, temp, ',');
	temp.erase(temp.find_first_of('x'), 1);
	rd = binary(atoi(temp.c_str()), 5);

	getline(ss, temp, ',');
	temp.erase(temp.find_first_of('x'), 1);
	rs1 = binary(atoi(temp.c_str()), 5);

	ss >> temp;
	if (temp.find('x') != -1)
	{
		temp.erase(temp.find_first_of('x'), 1);
		rs2 = binary(atoi(temp.c_str()), 5);
	}
	else
	{
		rs2 = binary(atoi(temp.c_str()), 5);
	}
	return f5 + f2 + rs2 + rs1 + f3 + rd + opcode;
}
*/
void history_predictor(bool taken)
{
	int this_entry = pc % entries.size();
	entries[this_entry].history = ((entries[this_entry].history << 1) & 3);

	if (taken)
		entries[this_entry].history++;
}

bool guess_predictor()
{
	int this_entry = pc % entries.size();

	bool guess = (entries[this_entry].bc2[entries[this_entry].history] >= 2);
	return guess;
}

void edit_predictor(bool taken)
{
	int this_entry = pc % entries.size();
	int this_history = entries[this_entry].history;
	
	bool guess = (entries[this_entry].bc2[this_history] >= 2);

	if ((guess == taken))
	{
		if (entries[this_entry].bc2[this_history] == 2)
			entries[this_entry].bc2[this_history] = 3;
		else if (entries[this_entry].bc2[this_history] == 1)
			entries[this_entry].bc2[this_history] = 0;
	}
	else
	{
		if (guess)
			entries[this_entry].bc2[this_history]--;
		else
			entries[this_entry].bc2[this_history]++;
	}

	//cout << "line:" << pc << "\tguess: " << ((guess) ? "T" : "N") << "\tfact: " << ((taken) ? "T" : "N") << endl;
	//outputallentry();
}

void li(stringstream& ss)
{
	string temp, rd, imm;
	ss >> temp;

	getline(ss, temp, ',');
	temp.erase(temp.find_first_of('R'), 1);
	rd = temp;

	ss >> temp;
	imm = temp;

	reg[atoi(rd.c_str())] = atoi(imm.c_str());
}

void addi(stringstream& ss)
{
	string temp, rd, rs1, imm;
	ss >> temp;

	getline(ss, temp, ',');
	temp.erase(temp.find_first_of('R'), 1);
	rd = temp;

	getline(ss, temp, ',');
	temp.erase(temp.find_first_of('R'), 1);
	rs1 = temp;

	ss >> temp;
	imm = temp;

	reg[atoi(rd.c_str())] = reg[atoi(rs1.c_str())] + atoi(imm.c_str());
}

void andi(stringstream& ss)
{
	string temp, rd, rs1, imm;
	ss >> temp;

	getline(ss, temp, ',');
	temp.erase(temp.find_first_of('R'), 1);
	temp.erase(temp.find_first_of(' '), 1);
	rd = temp;

	getline(ss, temp, ',');
	temp.erase(temp.find_first_of('R'), 1);
	rs1 = temp;

	ss >> temp;
	imm = temp;
	reg[atoi(rd.c_str())] = reg[atoi(rs1.c_str())] & atoi(imm.c_str());
}

void add(stringstream& ss)
{
	string temp, rd, rs1, rs2;
	ss >> temp;

	getline(ss, temp, ',');
	temp.erase(temp.find_first_of('R'), 1);
	rd = temp;

	getline(ss, temp, ',');
	temp.erase(temp.find_first_of('R'), 1);
	rs1 = temp;

	ss >> temp;
	temp.erase(temp.find_first_of('R'), 1);
	rs2 = temp;

	reg[atoi(rd.c_str())] = reg[atoi(rs1.c_str())] + reg[atoi(rs2.c_str())];
}

void beq(stringstream& ss)
{
	string temp, rs1, rs2;
	ss >> temp;

	getline(ss, temp, ',');
	temp.erase(temp.find_first_of('R'), 1);
	rs1 = temp;

	getline(ss, temp, ',');
	temp.erase(temp.find_first_of('R'), 1);
	rs2 = temp;

	ss >> temp;

	bool taken = reg[atoi(rs1.c_str())] == reg[atoi(rs2.c_str())];
	cout << "\tfact:" << ((taken) ? "T" : "N") << endl;
	edit_predictor(taken);
	history_predictor(taken);
	if (taken)
	{
		pc = label(temp);
		pc--;
	}
}

void bne(stringstream& ss)
{
	string temp, rs1, rs2;
	ss >> temp;

	getline(ss, temp, ',');
	temp.erase(temp.find_first_of('R'), 1);
	rs1 = temp;

	getline(ss, temp, ',');
	temp.erase(temp.find_first_of('R'), 1);
	rs2 = temp;

	ss >> temp;

	bool taken = reg[atoi(rs1.c_str())] != reg[atoi(rs2.c_str())];
	cout << "\tfact:" << ((taken) ? "T" : "N") << endl;
	edit_predictor(taken);
	history_predictor(taken);
	if (taken)
	{
		pc = label(temp);
		pc--;
	}
}

void outputallentry()
{
	for (int i = 0; i < entries.size(); i++)
	{
		cout << "entries[" << i << "] = (";
		if (entries[i].history == 0)
			cout << "00";
		else if (entries[i].history == 1)
			cout << "01";
		else if (entries[i].history == 2)
			cout << "10";
		else if (entries[i].history == 3)
			cout << "11";
		for (int j = 0; j < 4; j++)
		{
			cout << ", ";
			if (entries[i].bc2[j] == 0)
				cout << "SN";
			else if (entries[i].bc2[j] == 1)
				cout << "WN";
			else if (entries[i].bc2[j] == 2)
				cout << "WT";
			else if (entries[i].bc2[j] == 3)
				cout << "ST";
		}
		cout << ")" << endl;
	}
	cout << endl;
}

int main()
{
	string tempin;
	vector<string> input;
	int tmpentriessize;
	cout << "enter the X entries ( X > 0 ) " << endl;
	cin >> tmpentriessize;
	entries.resize(tmpentriessize);
	cout << "plz enter all inst. ( end of EOF ) " << endl;

	while (getline(cin, tempin))
	{
		input.push_back(tempin);
	}

	//output.resize(input.size());
	jumplist.resize(input.size());

	//jump label
	for (int i = 0; i < input.size(); i++)
	{
		//string opcode, rd, rs1, rs2, imm, f3, f2, f5;
		string temp, c_input;
		stringstream ss;
		c_input = input[i];
		ss << c_input;

		if (c_input.find(":") != -1)
		{
			ss >> temp;
			temp.erase(temp.find_first_of(':'), 1);
			jumplist[i] = temp;
		}
	}

	for (int i = 0; i < input.size(); i = ++pc)
	{
		string opcode, rd, rs1, rs2, imm, f3, f2, f5, temp, c_input;
		stringstream ss;
		c_input = input[i];
		ss << c_input;
		
		cout << "line:" << pc << "\tentry:" << pc % entries.size() << "\tguess:" << ((guess_predictor()) ? "T" : "N");

		if (c_input.find(":") != -1)
			ss >> temp;
		//li
		if (c_input.find("li") != -1)
		{
			li(ss);
		}
		else if (c_input.find("addi") != -1)
		{
			addi(ss);
		}
		else if (c_input.find("add") != -1)
		{
			add(ss);
		}
		else if (c_input.find("beq") != -1)
		{
			beq(ss);
			outputallentry();
			continue;
		}
		else if (c_input.find("bne") != -1)
		{
			bne(ss);
			outputallentry();
			continue;
		}
		else if (c_input.find("andi") != -1)
		{
			andi(ss);
		}
		history_predictor(false);
		cout << "\tnot branch!" << endl;
		outputallentry();
		/*
		//type_i_offset
		else if (c_input.find("lb") != -1 || c_input.find("lh") != -1 || c_input.find("lw") != -1)
		{
			opcode = "0000011";

			if (c_input.find("lbu") != -1)
				f3 = "100";
			else if (c_input.find("lhu") != -1)
				f3 = "101";
			else if (c_input.find("lb") != -1)
				f3 = "000";
			else if (c_input.find("lh") != -1)
				f3 = "001";
			else if (c_input.find("lw") != -1)
				f3 = "010";

			//output[i] = imm + " " + rs1 + " " + f3 + " " + rd + " " + opcode;
			//output[i] = type_i_offset(ss, opcode, f3);
		}
		//addi type_i
		else if (c_input.find("addi") != -1 || c_input.find("slti") != -1 || c_input.find("ori") != -1 || c_input.find("andi") != -1)
		{
			opcode = "0010011";

			if (c_input.find("sltiu") != -1)
				f3 = "011";
			else if (c_input.find("xori") != -1)
				f3 = "100";
			else if (c_input.find("addi") != -1)
				f3 = "000";
			else if (c_input.find("slti") != -1)
				f3 = "010";
			else if (c_input.find("ori") != -1)
				f3 = "110";
			else if (c_input.find("andi") != -1)
				f3 = "111";

			//output[i] = imm + " " + rs1 + " " + f3 + " " + rd + " " + opcode;
			//output[i] = type_i(ss, opcode, f3);
		}
		//type_s
		else if (c_input.find("sb") != -1 || c_input.find("sh") != -1 || c_input.find("sw") != -1)
		{
			opcode = "0100011";

			if (c_input.find("sb") != -1)
				f3 = "000";
			else if (c_input.find("sh") != -1)
				f3 = "001";
			else if (c_input.find("sw") != -1)
				f3 = "010";

			//output[i] = type_s(ss, opcode, f3);
		}
		//type_r
		else if (c_input.find("sl") != -1 || c_input.find("sr") != -1 || c_input.find("add") != -1 ||
			c_input.find("sub") != -1 || c_input.find("or") != -1 || c_input.find("and") != -1)
		{
			if (c_input.find("slli") != -1 || c_input.find("srli") != -1 || c_input.find("srai") != -1)
				opcode = "0010011";
			else
				opcode = "0110011";

			f2 = "00";

			if (c_input.find("sll") != -1)
				f3 = "001";
			else if (c_input.find("sr") != -1)
				f3 = "101";
			else if (c_input.find("add") != -1 || c_input.find("sub") != -1)
				f3 = "000";
			else if (c_input.find("sltu") != -1)
				f3 = "011";
			else if (c_input.find("slt") != -1)
				f3 = "010";
			else if (c_input.find("xor") != -1)
				f3 = "100";
			else if (c_input.find("or") != -1)
				f3 = "110";
			else if (c_input.find("and") != -1)
				f3 = "111";

			if (c_input.find("srai") != -1 || c_input.find("sub") != -1 || c_input.find("sra") != -1)
				f5 = "01000";
			else
				f5 = "00000";

			//output[i] = f5 + " " + f2 + " " + rs2 + " " + rs1 + " " + f3 + " " + rd + " " + opcode;
			//output[i] = type_r(ss, opcode, f3, f2, f5);
		}
		//beq type_sb
		else if (c_input.find("beq") != -1 || c_input.find("bne") != -1 || c_input.find("blt") != -1 || c_input.find("bge") != -1)
		{
			opcode = "1100011";

			if (c_input.find("beq") != -1)
				f3 = "000";
			else if (c_input.find("bne") != -1)
				f3 = "001";
			else if (c_input.find("bltu") != -1)
				f3 = "110";
			else if (c_input.find("bgeu") != -1)
				f3 = "111";
			else if (c_input.find("blt") != -1)
				f3 = "100";
			else if (c_input.find("bge") != -1)
				f3 = "101";

			//output[i] = imm1 + " " + rs2 + " " + rs1 + " " + f3 + " " + imm2 + " " + opcode;
			//output[i] = type_sb(ss, opcode, f3, i);
		}*/
		//system("pause");
	}
	/*for (int i = 0; i < output.size(); i++)
	{
		cout << output[i] << endl;
	}*/
	return 0;
}

/*add x2,x2,x23
addi x24,x24,2
L2: sw x1,0(x2)
beq x0,x0,L1
L1: add x2,x2,x23
beq x0,x0,L2*/

/* A版
li R1,0
li R2,4
Loop: beq R1,R2,End
addi R2,R2,-1
beq R0,R0,Loop
End:
*/

/* B版
li R2,0
li R3,16
li R4,0
LoopI: beq R4,R3,EndLoopI
li R5,0
LoopJ: beq R5,R3,EndLoopJ
add R6,R5,R4
andi R6,R6,3
bne R6,R0,Endif
add R2,R2,R5
Endif: addi R5,R5,1
beq R0,R0,LoopJ
EndLoopJ: addi R4,R4,1
beq R0,R0,LoopI
EndLoopI:
*/