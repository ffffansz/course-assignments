#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

namespace BnUtil {
/* beginning of namespace BnUtil */
	
class Node {
private:
	string name_;
	vector<string> varlist_;
	map<map<string, string>, double> cpt_;
public:
	Node(const string& name, const vector<string>& varlist) 
		:name_(name), varlist_(varlist)
	{		
		cpt_ = map<map<string, string>, double>();
	}

	void setCpt(const map<map<string, string>, double>& cpt) {
		cpt_ = cpt;
		return;
	}

	void printInfo() {
		cout << "Name = " << name_ << endl;
		cout << "	vars " << "[";
		for (size_t i = 0; i < varlist_.size(); i++)
			cout << varlist_[i] << ", ";
		cout << "\b\b]" << endl;

		for (map<map<string, string>, double>::iterator rowit = cpt_.begin(); rowit != cpt_.end(); rowit++) {
			cout << "		key: ";
			for (map<string, string>::iterator it = rowit->first.begin(); it != rowit->first.end(); it++) {

			}
			cout << "val: " << it->second << endl;
		}
		cout << endl;
			
		return;
	}

	/* function that restricts a variable to some value in a given factor */
	void restrict(const string& var, const string& val) {
		vector<string>::iterator varIt = find(varlist_.begin(), varlist_.end(), var);
		if (varIt == varlist_.end()) {
			cout << "The variable is not in the node's varlist." << endl;
			return;
		}

		map<map<string, string>, double> newCpt();
		for (map<map<map<string, string>, double>, double>::iterator it = cpt_.begin(); it != cpt_.end(); it++) {
			if (it->first)
		}
		return;
	}

	/* function that sums out a variable given a factor */
	void sumout(const string& var) {
		return;
	}

	/* function that multiplies with another factor */
	void multiply(const Node& factor) {
		return;
	}
};

/* end of namespace BnUtil */
}