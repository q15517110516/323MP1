#include <stdio.h>
#include <assert.h>

const int GRIDSIZE = 3;
const int GRID_SQUARED = GRIDSIZE*GRIDSIZE;
const int ALL_VALUES = (1<<GRID_SQUARED)-1;

int board[GRID_SQUARED][GRID_SQUARED];

char *easy_board_init[GRID_SQUARED] = {"******3**", 
"*8***3**9", 
"*24*7**86", 
"9****54*8", 
"***4*2***", 
"1*69****5", 
"56**3*14*", 
"4**5***9*", 
"**7******"};

char *hard_board_init[GRID_SQUARED] = {"*94******", 
"1**79****", 
"**3*86**1", 
"92*****3*", 
"****7****", 
"*8*****25", 
"4**52*9**", 
"****61**3", 
"******64*"};

int bit_count(int value);
int get_nth_set_bit(int value, int index);
bool singleton(int value); 
int get_singleton(int value);
bool rule2();

// Use the "board_init" variable to initialize the bitmasks for each square.
void
init_board(char *board_init[GRID_SQUARED]) {
	for (int i = 0 ; i < GRID_SQUARED ; ++ i) {
		for (int j = 0 ; j < GRID_SQUARED ; ++ j) {
			char c = board_init[i][j];
			if (c == '*') {
				board[i][j] = ALL_VALUES;
			} else {
		  board[i][j] = 1 << (c - '0' - 1);  //  DO YOU UNDERSTAND WHAT THIS DOES?  YOU SHOULD.
		}
	}
}
}

// A board is done if all squares are singletons.
bool
board_done(int board[GRID_SQUARED][GRID_SQUARED]) {
	for (int i = 0 ; i < GRID_SQUARED ; ++ i) {
		for (int j = 0 ; j < GRID_SQUARED ; ++ j) {
			if (!singleton(board[i][j])) {
				return false;
			}
		}
	}
	return true;
}

// Print the board with one value per square.
void
print_board(int board[GRID_SQUARED][GRID_SQUARED]) {
	for (int i = 0 ; i < GRID_SQUARED ; ++ i) {
		for (int j = 0 ; j < GRID_SQUARED ; ++ j) {
			int value = board[i][j];
			char c = '*';
			if (singleton(value)) {
				c = get_singleton(value) + '1';
			}
			printf("%c", c);
		}
		printf("\n");
	}
}

// Print the board with up to 9 values per square, based on what
// the possibilities are.
void
print_board_verbose() {
	for (int i = 0 ; i < GRID_SQUARED ; ++ i) {
		for (int j = 0 ; j < GRID_SQUARED ; ++ j) {
			int value = board[i][j];
			char c = ' ';
			if (singleton(value)) {
				c = '*';
			}
			for (int k = 0 ; k < GRID_SQUARED ; ++ k) {
				char cc = (value & (1<<k)) ? ('1'+k) : c;
				printf("%c", cc);
			}
			printf("%c", (j == (GRID_SQUARED-1)) ? '\n' : '|');
		}
	}
}

int 
get_square_begin(int index) {
	return (index/GRIDSIZE) * GRIDSIZE;
}

bool
rule1() {
	bool changed = false;
	for (int i = 0 ; i < GRID_SQUARED ; ++ i) {  
		for (int j = 0 ; j < GRID_SQUARED ; ++ j) {
		// for every square, if the value is a singleton, remove it from the row, column, and square.
			int value = board[i][j];
			if (singleton(value)) {
				for (int k = 0 ; k < GRID_SQUARED ; ++ k) {
			 // eliminate from row
					if (k != j) {
						if (board[i][k] & value) {
							changed = true;
						}
						board[i][k] &= ~value;
					}
			 // eliminate from column
					if (k != i) {
						if (board[k][j] & value) {
							changed = true;
						}
						board[k][j] &= ~value;
					}
				}

		  // elimnate from square
				int ii = get_square_begin(i);
				int jj = get_square_begin(j);
				for (int k = ii ; k < ii + GRIDSIZE ; ++ k) {
					for (int l = jj ; l < jj + GRIDSIZE ; ++ l) {
						if ((k == i) && (l == j)) {
							continue;
						}
						if (board[k][l] & value) {
							changed = true;
						}
						board[k][l] &= ~value;
					}
				}
			}
		}
	}
	return changed;
}

bool 
solve(char *board_init[GRID_SQUARED]) {
	init_board(board_init);
	bool changed;
	do {
		changed = rule1();
		changed |= rule2();
	} while (changed);
	if (board_done(board)) {
		printf("SUCCESS!\n");
		print_board(board);
		printf("\n");
		return true;
	} else {
		printf("FAILED!\n");
		print_board_verbose();
		printf("\n");
		return false;
	}
}

int
main() {
	printf("bit_count(0x34) = %d\n", bit_count(0x34));
	printf("get_nth_set_bit(0x34, 1) = %d\n", get_nth_set_bit(0x34, 1));
	printf("singleton(0x40) = %s\n", singleton(0x40) ? "TRUE" : "FALSE");
	printf("get_singleton(0x40) = %d\n\n", get_singleton(0x40));

	printf("EASY BOARD:\n");
	bool solved = solve(easy_board_init);
	if (solved) {
		printf("HARD BOARD:\n");
		solve(hard_board_init);
	}
	return 0;
}

// ALL your code goes below this line.
// 
// ---------------------------------------------------------------------

/*
 * This function takes an integer and returns the count of the
number of bits set to 1
 */
int 
bit_count(int value) {
	int count = 0;							// initialize count to 0
	for (int index = 0; index < 9; index++) {	
		count += (value>>index) & 0b1;		// increment count by the value at each index (+0 or +1)
	}
	return count;							// return count (# of bits set to 1 in value)
}

/*
 * This function takes an integer and an index; it iterates
through the bits of the integer (from LSB to MSB) counting set bits until
it comes to the “index”th set bit. The current bit position is returned.
 */
int 
get_nth_set_bit(int value, int index) {
	int countdown = index + 1;			// Set the value of countdown to index +1
	for (int i = 0; i < 9; i++) {		
		countdown -= (value>>i)&0b1;	// subtarct bit at index i in value form countdown (-0 or -1)
		if (!countdown) {				// When countdown == 0 then we have found the index
			return i;					// return the index value
		}
	}
	return -1;							// if the index is not found retur -1
}

/*
 * This function returns a bool (a type which can hold either true
of false) based on whether a single bit is set.
 * Use this to determine if the square can only hold 1 value
 * Singleton == TRUE if ((value-1)&value) = 0 
 * Singleton == TRUE if (value!=0)
 * Singleton == FALSE
 *  if either above statement is false 
 */
bool 
singleton(int value) {  
	return !((value-1)&value) && (value != 0);	
}

/*
 * Get the position of the set bit for a singleton
 */
int 
get_singleton(int value) {			
	int countdown = 1;				
	for (int i = 0; i < 9; i++) {
		countdown -= (value>>i)&0b1;// Find if there is a single 1-bit
		if (!countdown) {
			return i;				// return index if there is a single 1-bit
		}
	}
	return -1;						// return -1 if 0 or 2+ 1-bits 	
}

/*
 * Rule 2: If there is only one square in a given row, column or 3*3 square
that can hold a given value, then it must be the one that holds that value.
All other possibilities can be eliminated from that square.
 */
bool
rule2() {
	changed = false;								// Set changed to FALSE
	for (int i = 0; i < 9; i++) {					// For each column
		for (int j = 0; i < 9; j++) {				// For each row
			//Row									
			int result = 0;							// Set result to 0
			for (int row = 0; row < 9; row++) {
				if (row != j) {						// for all non-j rows
					result |= board[i][row];		// add all entered values in column[i]
				}
			}
			// NOTE: ~0b0001 --> 0b1110 (~ invereses bits)
			if (singleton(~result)) {		// if the inverse reult is a singleton 
				board[i][j] = ~result;		// set the square value at [i][j] to ~result
				changed = true;				// set changed to TRUE
				break;
			}
			//Column
			result = 0;								// reset result to 0
			for (int col = 0; col < 9; col++) {		
				if (col != i) {						// for all non-i columns
					result |= board[col][j];		// add all entered values in row[j]
				}
			}
			if (singleton(~result)) {		// If inverse result is a singleton
				board[i][j] = ~result;		// Set the square at [i][j] to ~result
				changed = true;				// set changed to TRUE
				break;
			}
			//Square
		//  i=0 ----------------->i=8
		//	[][][] | [][][] | [][][]	} j = 0/col = 0
		//	[][][] | [][][] | [][][]	} j = 1/col = 1
		//__[][][]_|_[][][]_|_[][][]__  } j = 2
		//	[][][] | [][][] | [][][]	...
		//	[][][] | [][][] | [][][]	...
		//__[][][]_|_[][][]_|_[][][]__
		//	[][][] | [][][] | [][][]
		//	[][][] | [][][] | [][][]
		//  [][][] | [][][] | [][][]
			result = 0;
			int x = (i/3) * 3					// determine which 3x3 [i] is in
			int maxx = x + 3;					// maximum [i] in 3x3 containing [i][j]
			int y = (j/3) * 3					// determine which 3x3 [j]
			int maxy = y + 3;					// maximumu [j] in 3x3 containing [i][j]
			for (x; x < maxx; x++) {		
				for (y; y < maxy; y++) {		// beginning at the top-left corner of 3x3
					if (x != i && y != j) {
						result |= board[x][y];	// add all [!=i][!=j] squares in 3x3
					}
				}
			}
			if (singleton(~result)) {			// if the inverse result is a singleton
				board[i][j] = ~result;			// set [i][j] to ~result
				changed = true;					// set changed to TRUE
				break;
			}
		}
	}
	return changed;								// return TRUE or FALSE
}

