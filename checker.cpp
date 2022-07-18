#include <bits/stdc++.h>
using namespace std;

// ./spj input user_out answer score judge_message
ifstream input     ; // 题目的输入文件
ifstream output    ; // 用户输出
#define MAX (1024 + 64)

int T, m, n, leaves, ans;
std::vector <std::string> maze;
bool mark[MAX][MAX];
int rplus[4] = {1, -1, 0, 0}, cplus[4] = {0, 0, 1, -1};
struct node {
	node(int _r, int _c, int _pr, int _pc) {
		r = _r; c = _c; pr = _pr; pc = _pc;
	}
	int r, c, pr, pc;
};

bool inside(int r, int c) {
	return r >= 0 && r < m && c >= 0 && c < n;
}

bool leaf(int r, int c) {
	int adj = 0;
	for (int i = 0; i < 4; i++)
		if (inside(r + rplus[i], c + cplus[i]) && maze[r + rplus[i]][c + cplus[i]] == '.') adj++;
	return adj == 1;
}

std::vector<pair<int, int> > leafPos ;
void BFS(int r, int c) {
	queue <node> Q;
	Q.push(node(r, c, -1, -1));
	mark[r][c] = true;
	while (! Q.empty()) {
		node v = Q.front();
		Q.pop();
		if (leaf(v.r, v.c)) leaves++, leafPos.push_back (make_pair (v.r, v.c)) ;
		for (int i = 0; i < 4; i++) {
			int a = v.r + rplus[i], b = v.c + cplus[i];
			if (inside(a, b) && maze[a][b] == '.' && (a != v.pr || b != v.pc)) {
				if (mark[a][b]) throw ("Output has cycle");
				Q.push(node(a, b, v.r, v.c));
				mark[a][b] = true;
			}
		}
	}
}

void judge () {
    input >> T >> m >> n >> ans ;
	for (int i = 0; i < m; i++) {
        string instr, outstr ;
        input >> instr ;
        output >> outstr ;
        if (outstr.length() != n) throw ("maze wrong format.") ;
		for (int j = 0; j < n; j ++)
            if (outstr[j] != '.' && outstr[j] != '#' && outstr[j] != 'X')
                throw ("maze wrong format.") ; 
        maze.push_back (outstr) ;
        for (int j = 0; j < n; j ++)
            if (instr[j] != maze[i][j] && (instr[j] != '.' || maze[i][j] != 'X')) {
				printf ("error: Input and output maps are different at cell [%d, %d]", i, j) ;
				exit (0) ;
			}
    }

	// Check if output maze is a tree, and count its leaves
	int component = 0;
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			if (maze[i][j] == '.' && ! mark[i][j]) {
				if (component++ > 0) throw ("Output maze is not connected") ;
				BFS(i, j);
			}

	int curScore = int(leaves / (double)ans * 1000) ;

	for (pair<int, int> pos : leafPos) 
		maze[pos.first][pos.second] = 'O' ;

	printf("maze: (hiding places are marked as O)\n") ;
	for (int i = 0; i < m; i ++)
		cout << maze[i] << endl ;
	printf("\n") ;

	printf("number of hiding places: %d/%d\n", leaves, ans) ;
	printf("score: %d\n", curScore) ;
}

int main(int argc, char * argv[]) {
    try {
        input .open (argv[1]); // 题目的输入文件
        output    .open(argv[2]); // 用户输出

        try {
            judge () ;
        } catch (const char *s) {
            printf("error: %s\n", s) ;
        }

        input.close() ;
        output.close() ;
    } catch (...) {
        return -1;
    }
	
    return 0;
}
