#include <iostream>
#include <string>
#include "HashSet.h"
using namespace std;

void runShell();

int main(int, char**) {
    cout << "starting test\n";
	runShell();
	cout << "finished test\n";
}

#define numCmd(command) if(cmd.substr(0, string(#command).size()) == #command){ \
	cin >> num;\
	cout << #command ": " << num << endl;\
	set.command(num);\
	}

void runShell(){
	cout << "input max rank: ";
	int max_rank = 0;
	cin >> max_rank;
	HashSet set(max_rank, true);
	cout << "initialized HashSet with rank: " << max_rank << endl;
	set.printLayout();
	while(true){
		cout << "HashSet> ";
		string cmd;
		int num = 0;
		cin >> cmd;
		if(cmd == "end")
			return;
		
		numCmd(insert)
		else numCmd(remove)
		else numCmd(layout)
	}
}