#ifndef DIMACS
#include <cryptominisat5/cryptominisat.h>
#else
#include <fstream>
#include <vector>
#endif

namespace sat {

#ifndef DIMACS

CMSat::SATSolver solver;
std::vector<CMSat::Lit> lit_buf;

void Init(int n) { solver.new_vars(n + 1); }

void AddClause(std::vector<int> v) {
	lit_buf.clear();
	lit_buf.reserve(v.size());
	for (int x : v) lit_buf.emplace_back(abs(x), x < 0);
	solver.add_clause(lit_buf);
}

bool Solve() { return solver.solve() == CMSat::l_True; }

int GetResult(int id) {
	auto r = solver.get_model()[id];
	return r == CMSat::l_True ? 1 : r == CMSat::l_False ? -1 : 0;
}

#else

std::vector<std::vector<int>> clauses;
int n_vars;

void Init(int n) { n_vars = n; }

void AddClause(std::vector<int> v) { clauses.emplace_back(std::move(v)); }

bool Solve() {
	std::fstream fs("out.dimacs", fs.trunc | fs.out);
	fs << "p cnf " << n_vars << ' ' << clauses.size() << '\n';
	for (auto &v : clauses) {
		for (auto x : v) fs << x << ' ';
		fs << "0\n";
	}
	fs.close();
	exit(0);
}

int GetResult(int id) { return 0; }

#endif // DIMACS

}  // namespace sat


#include <stdio.h>
#include <stdlib.h>

void get_edges_index(int r, int c, int i, int j, int e[]){
	e[0] = (i - 1) * (2 * c + 1) + j;
	e[1] = e[0] + c;
	e[2] = e[0] + c + 1;
	e[3] = e[0] + 2 * c + 1;
	return;
}

void zero_clause(int e[]){
	
}

void one_clause(int e[]){
	
}

void two_clause(int e[]){
	
}

void three_clause(int e[]){
	
}

int main(void){
	int r, c;
	scanf("%d%d", &r, &c);
	char b;
	int e[4], length = 2 * r * c + r + c;
	sat::Init(length);
	for (int i = 1; i <= r; i++){
		for (int j = 1; j <= c; j++){
			scanf("%c", &b);
			if (b != '.'){
				get_edges_index(r, c, i, j, e);
				switch (b){
					case '0' :
					zero_clause(e);
					break;
					case '1' :
					one_clause(e);
					break;
					case '2' :
					two_clause(e);
					break;
					case '3' :
					three_clause(e);
					break;
				}
			}
		}
		scanf("%c", &c);//scan '\n'
	}
	sat::Solve();
	for (int i = 1; i <= length; i++){
		if (sat::GetResult(i))
			putchar('1');
		else
			putchar('0');
	}
	return 0;
}