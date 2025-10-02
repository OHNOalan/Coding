#include <iostream>
#include <iomanip>
using namespace std;

const int SIZE = 625;

const int ROCK = -1;
const int EMPTY = 0;

const int EAST = 0;
const int NORTH = 1;
const int SOUTH = 2;
const int WEST = 3;
const char dName[4] = {'E', 'N', 'S', 'W'};

int board[SIZE+2][SIZE+2];

int bestVal, row, col, dir;

void printBoard(int m, int n)
{
	for(int i=1; i<=m; i++) {
		for(int j=1; j<=n; j++)
			cout << setw(3) << board[i][j];
	cout << endl;
	}
}

void makePath(int r, int c, int rInc, int cInc, int len, int startR, int startC, int startD)
{
	if (board[r][c] != EMPTY) {
		if(len > bestVal) {
			bestVal = len;
			row = startR;
			col = startC;
			dir = startD;
/*/
cout << "at " << r << ',' << c << ", len = " << len << endl;
cout << startR << ',' << startC << ',' << startD << endl;
printBoard(5, 5);
/**/
		}
	}
	else {
		int savr = r;
		int savc = c;
		while(board[r][c] == EMPTY) {
			board[r][c] = ++len;
			r += rInc;
			c += cInc;
		}
		r -= rInc;
		c -= cInc;
		makePath(r+cInc, c+rInc, cInc, rInc, len, startR, startC, startD);
		makePath(r-cInc, c-rInc, -cInc, -rInc, len, startR, startC, startD);
		while (r != savr || c != savc) {
			board[r][c] = EMPTY;
			r -= rInc;
			c -= cInc;
		}
		board[r][c] = EMPTY;
	}
}
	
int main()
{
	int m, n, rocks;
	int icase=0;
	cin >> m >> n;
	while (m > 0) {
		icase++;
		for(int i=0; i<m+2; i++)
			for(int j=0; j<n+2; j++)
				board[i][j] = EMPTY;
		cin >> rocks;
		for(int i=0; i<rocks; i++) {
			int r, c;
			cin >> r >> c;
			board[r+1][c+1] = ROCK;
		}
		for(int i=0; i<m+2; i++)
			board[i][0] = board[i][n+1] = ROCK;
		for(int j=0; j<n+2; j++)
			board[0][j] = board[m+1][j] = ROCK;

		bestVal = 0;
		for(int i=1; i<=m; i++) {
			for(int j=1; j<=n; j++) {
				if (board[i][j] == ROCK)
					continue;
				board[i][j] = 1;
				makePath(i, j+1, 0, 1, 1, i, j, EAST);
				makePath(i-1, j, -1, 0, 1, i, j, NORTH);
				makePath(i+1, j, 1, 0, 1, i, j, SOUTH);
				makePath(i, j-1, 0, -1, 1, i, j, WEST);
				board[i][j] = EMPTY;
			}
		}
/*
		cout << "Case " << icase << ": " << row-1 << " " << col-1 << " " << dName[dir] << endl;
*/
		cout << "Case " << icase << ": "<< bestVal << " " << row-1 << " " << col-1 << " " << dName[dir] << endl;
		cin >> m >> n;
	}
	
	return 0;
}