#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <memory>
#include <set>
#include <algorithm>

using namespace std;

typedef pair<int, int> Cell;

//Constraint
struct Cons {
	unsigned target_sum;
	shared_ptr<vector<Cell>> pCons_cells;

	Cons(unsigned target_sum, shared_ptr<vector<Cell>> pCons_cells) {
		this->target_sum = target_sum;
		this->pCons_cells = pCons_cells;
	}

	bool operator ==(const Cons& ano) const {
		if (this->target_sum == ano.target_sum && this->pCons_cells == ano.pCons_cells)
			return true;
		return false;
	}
};


vector<Cons> conses;
set<Cell> cells;
map<Cell, vector<bool>> cell_curdom;
map<Cell, bool> assigned;
map<Cell, int> value;
list<Cons> GAC_Queue;

size_t board_size_row;
size_t board_size_col;

void GAC();

bool GAC_Enforce();

bool find_support(const Cons& cons, const Cell& assgined_cell, const unsigned assgined_cell_value);

void backtrack(const vector<Cell>& unassigned_cells, unsigned cursum, unsigned target_sum, size_t cur_index, bool& flag);

void change_other_cells_curdom(const Cell& assigned_cell, unsigned assigned_cell_value);

void init_board();

void show();

int main()
{
	init_board();
	GAC();

	system("pause");
    return 0;
}

void GAC()
{
	map<Cell, bool>::iterator assg_it = assigned.begin();
	bool all_assigned = true;
	Cell unassigned_cell(-1, -1);
	for (; assg_it != assigned.end(); assg_it++) {
		if (assg_it->second == false) {
			unassigned_cell = assg_it->first;
			all_assigned = false;
			break;
		}
	}
	if (all_assigned) {
		show();
		return;
	}

	assigned[unassigned_cell] = true;
	for (size_t i = 0; i < 9; i++) {
		if (cell_curdom[unassigned_cell][i]) {
			value[unassigned_cell] = i + 1;
			map<Cell, vector<bool>> dom_for_restore = cell_curdom;
			//TODO: change other vars' curdom
			change_other_cells_curdom(unassigned_cell, i);
			/* Prune all values of V ≠ d from CurDom[V] */
			for (size_t j = 0; j < 9; j++) {
				cell_curdom[unassigned_cell][j] = i == j ? true : false;
			}

			/* for each constraint C whose scope contains V, Put C on GACQueue */
			vector<Cons>::iterator cons_it = conses.begin();
			while (cons_it != conses.end()) {
				vector<Cell>::iterator cons_contain_cell_it = cons_it->pCons_cells->begin();
				while (cons_contain_cell_it != cons_it->pCons_cells->end()) {
					if (*cons_contain_cell_it == unassigned_cell) {
						GAC_Queue.push_back(*cons_it);
						break;
					}
					cons_contain_cell_it++;
				}
				cons_it++;
			}
			/* if(GAC_Enforce() != DWO) */
			if (GAC_Enforce()) {
				GAC();
			}
			cell_curdom = dom_for_restore;
		}
	}
	assigned[unassigned_cell] = false;
	return;
}

bool GAC_Enforce()
{
	while (!GAC_Queue.empty()) {
		Cons con = GAC_Queue.front();
		GAC_Queue.pop_front();
		vector<Cell>::iterator cell_it = con.pCons_cells->begin();
		while (cell_it != con.pCons_cells->end()) {
			for (size_t i = 0; i < cell_curdom[*cell_it].size(); i++) {
				if (cell_curdom[*cell_it][i]) {
					bool support_found = find_support(con, *cell_it, i + 1);
					if (!support_found) {
						cell_curdom[*cell_it][i] = false;

						/* Determine if CurDom[V] = empty set */
						bool all_false = true;
						vector<bool>::iterator curdom_it = cell_curdom[*cell_it].begin();
						while (curdom_it != cell_curdom[*cell_it].end()) {
							if (*curdom_it) {
								all_false = false;
								break;
							}
							curdom_it++;
						}
						if (all_false) {
							GAC_Queue.clear();
							return false;
						}
						else {
							/* push all constraints C' such that V belongs to scope(C') and C' not belongs to GACQueue */
							vector<Cons>::iterator cons_it = conses.begin();
							while (cons_it != conses.end()) {
								vector<Cell>::iterator cons_contain_cell_it = cons_it->pCons_cells->begin();
								while (cons_contain_cell_it != cons_it->pCons_cells->end()) {
									if (*cons_contain_cell_it == *cell_it) { // find a cons containing the cell
										list<Cons>::iterator find_cons_it = find(GAC_Queue.begin(), GAC_Queue.end(), con);
										if (find_cons_it == GAC_Queue.end()) // Not found
											GAC_Queue.push_back(*cons_it);
										break;  // Stop checking other cells of the cons
									}
									cons_contain_cell_it++;
								}
								cons_it++;
							}
						}
					}
				}
			}
			cell_it++;
		}
	}
	return true;
}

bool find_support(const Cons & cons, const Cell & assgined_cell, const unsigned assgined_cell_value)
{
	vector<Cell> vec_ano_var;    //vector for cells of the cons except the assgined_cell
	vector<Cell>::iterator cell_it = cons.pCons_cells->begin();
	while (cell_it != cons.pCons_cells->end()) {
		if (*cell_it != assgined_cell)	vec_ano_var.push_back(*cell_it);
		cell_it++;
	}
	unsigned remain_sum = cons.target_sum - assgined_cell_value;
	bool is_support_found = false;
	backtrack(vec_ano_var, 0, remain_sum, 0, is_support_found);
	return is_support_found;
}

void backtrack(const vector<Cell>& unassigned_cells, const unsigned cursum, const unsigned target_sum, const size_t cur_index, bool & flag)
{
	if (cur_index == unassigned_cells.size() - 1) {  //last var
		for (size_t i = 0; i < 9; i++) {
			if (cell_curdom[unassigned_cells[cur_index]][i]) {
				if (i + 1 + cursum == target_sum) {
					flag = true;
					return;
				}
			}
		}
		return;
	}
	for (size_t i = 0; i < 9; i++) {
		if (cell_curdom[unassigned_cells[cur_index]][i]) {
			if (i + 1 + cursum < target_sum) {
				map<Cell, vector<bool>> dom_for_restore = cell_curdom;
				change_other_cells_curdom(unassigned_cells[cur_index], i);
				backtrack(unassigned_cells, i + 1 + cursum, target_sum, cur_index + 1, flag);
				cell_curdom = dom_for_restore;
			}
		}
	}
	return;
}

void change_other_cells_curdom(const Cell & assigned_cell, unsigned assigned_cell_value)
{
	/* find cons containing the cell */
	vector<Cons>::iterator cons_it = conses.begin();
	while (cons_it != conses.end()) {
		vector<Cell>::iterator cons_contain_cell_it = cons_it->pCons_cells->begin();
		while (cons_contain_cell_it != cons_it->pCons_cells->end()) {
			if (*cons_contain_cell_it == assigned_cell) { // found a cons containing the cell
				/* Copy the vector of the cons' cells, and delete the assigned_cell from the copy */
				vector<Cell> vec = *(cons_it->pCons_cells);
				vector<Cell>::iterator vec_it = find(vec.begin(), vec.end(), assigned_cell);
				vec.erase(vec_it);
				/* Change other cells' curdom */
				for (vec_it = vec.begin(); vec_it != vec.end(); vec_it++) {
					cell_curdom[*vec_it][assigned_cell_value] = false;
				}
			}
			cons_contain_cell_it++;
		}
		cons_it++;
	}
	return;
}

void init_board()
{
	/* 输入所有约束，检查输入的合法性；同时建立所有Cell的集合 */
	bool valid_input_size = false;
	while (!valid_input_size) {
		cout << "请输入游戏规模（如，8 * 9 的方格，输入 8 9）>>> ";
		board_size_row = 0, board_size_col = 0;
		cin >> board_size_row >> board_size_col;
		if (board_size_row <= 1 || board_size_col <= 1) {
			cout << "规模过小，请重新输入：" << endl;
			continue;
		}
		valid_input_size = true;

		bool valid_input_cons_cnt = false;
		while (!valid_input_cons_cnt) {
			cout << "请输入约束的总数量 >>> ";
			size_t cons_cnt = 0;
			cin >> cons_cnt;
			if (cons_cnt >= board_size_row * board_size_col) {
				cout << "约束总数量超过最大数量，请重新输入：" << endl;
				continue;
			}
			valid_input_cons_cnt = true;
			cout << "请按照示例输入所有的约束" << endl;
			cout << "输入示例 : " << endl;
			cout << "-----------------\n";
			cout << "|   |   | 8 |   |\n";
			cout << "-----------------\n";
			cout << "| 7 | * | * |   |\n";
			cout << "-----------------\n";
			cout << "|   |   | * |   |\n";
			cout << "-----------------\n";
			cout << "图中的一个约束应输入 >>> 7\n2\nr\n1 1\n";
			cout << "这表示，该约束的目标和是 7，其约束域为 2 个格子，约束方向为横向(r, 代表row；约束方向为纵向时输入c，代表column）；";
			cout << "其约束作用域的起始坐标为(1, 1)；目标和为8的约束的作用域的起始坐标为(1, 2)；";
			cout << "行数和列数均从0开始计。\n" << endl;

			for (size_t i = 0; i < cons_cnt; i++) {
				bool valid_input_cons = false;
				while (!valid_input_cons) {
					cout << "第 " << i + 1 << " 个约束的目标和 >>> ";
					int target_sum = 0, cell_cnt = 0;
					cin >> target_sum;
					cout << "第 " << i + 1 << " 个约束所约束的格子数量 >>> ";
					cin >> cell_cnt;
					char dir = 'a';
					cout << "第 " << i + 1 << " 个约束的约束方向(r or c) >>> ";
					cin >> dir;
					cout << "第 " << i + 1 << " 个约束的起始坐标 >>> ";
					int start_row = -1, start_col = -1;
					cin >> start_row >> start_col;
					if (target_sum <= 0 || cell_cnt <= 0 || cell_cnt >= max(board_size_row, board_size_col)) {
						cout << "目标和或格子数输入有误，请重新输入：" << endl;
						continue;
					}
					valid_input_cons = true;

					Cons new_cons(target_sum, make_shared<vector<Cell>>());
					/*
					for (size_t j = 0; j < cell_cnt; j++) {
						bool valid_input_cell = false;
						while (!valid_input_cell) {
							cout << "第 " << i + 1 << " 个约束所要约束的第 " << j + 1 << " 个格子的坐标 >>> ";
							int cell_row = -1, cell_col = -1;
							cin >> cell_row >> cell_col;
							if (cell_row < 0 && cell_col < 0) {
								cout << "输入有误，请重新输入：" << endl;
								continue;
							}
							valid_input_cell = true;
							Cell cell(cell_row, cell_col);
							new_cons.pCons_cells->push_back(cell);
							cells.insert(cell);
						}
					}
					*/
					for (size_t j = 0; j < cell_cnt; j++) {
						int cell_row = start_row;
						int cell_col = start_col;
						cell_row += dir == 'c' ? j : 0;
						cell_col += dir == 'r' ? j : 0;
						Cell cell(cell_row, cell_col);
						new_cons.pCons_cells->push_back(cell);
						cells.insert(cell);
					}
					conses.push_back(new_cons);
				}
			}
		}
	}

	/* 初始化所有Cell的Domain和assgined */
	set<Cell>::iterator set_it = cells.begin();
	for (; set_it != cells.end(); set_it++) {
		vector<bool> vec;
		vec.resize(9, true);
		cell_curdom[*set_it] = vec;
		assigned[*set_it] = false;
		value[*set_it] = -1;
	}

	return;
}

void show()
{
	cout << "该CSP问题的解为：\n" << endl;
	map<Cell, int>::iterator it = value.begin();
	for (size_t i = 0; i < board_size_col; i++)
		cout << "----";
	cout << "-" << endl;
	for (size_t i = 0; i < board_size_row; i++) {
		cout << "|";
		for (size_t j = 0; j < board_size_col; j++) {
			if (i == it->first.first && j == it->first.second) {
				cout << " " << it->second << " |";
				it++;
			}
			else  cout << "   |";
		}
		cout << endl;
		for (size_t k = 0; k < board_size_col; k++)
			cout << "----";
		cout << "-" << endl;
	}
	return;
}
