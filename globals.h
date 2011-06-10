#ifndef GLOBALS_H_
#define GLOBALS_H_

//#define NDEBUG
#define BITBOARD unsigned long long

#define WP 1
#define WN 2
#define WB 3
#define WR 4
#define WQ 5
#define WK 6
#define BP -1
#define BN -2
#define BB -3
#define BR -4
#define BQ -5
#define BK -6

#define A_FILE 0
#define B_FILE 1
#define C_FILE 2
#define D_FILE 3
#define E_FILE 4
#define F_FILE 5
#define G_FILE 6
#define H_FILE 7
#define RANK_1 0
#define RANK_2 1
#define RANK_3 2
#define RANK_4 3
#define RANK_5 4
#define RANK_6 5
#define RANK_7 6
#define RANK_8 7

#define MAX_PLY 99
#define MAX_MOVES 400
#define MAX_PIECE_MOVES 28

#define INFINITY 99999

#define DST_BLACK_KINGSIDE 62
#define DST_BLACK_QUEENSIDE 57
#define DST_WHITE_KINGSIDE 6
#define DST_WHITE_QUEENSIDE 1

#include <string>
#include <vector>
#include <cassert>

using namespace std;

extern bool cointoss; // do we need to initialize srand only at first program launch

struct move_t {
	// todo: consider making Move union instead of struct will it save memory?
    bool white_to_move;
    int src; // src square
    int dst; // dst square
    int piece; // piece
    int capture; // piece
    int ply;
    int move_no;
    int score;
	int epsquare; // what was the epsquare on the board prior to move
	bool white_can_castle_kingside; // state prior to move
	bool white_can_castle_queenside; // state prior to move
	bool black_can_castle_kingside; // state prior to move
	bool black_can_castle_queenside; // state prior to move
};

static string chessboard_squares[64] = { "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
							"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
							"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
							"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
							"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
							"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
							"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
							"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8" };

extern vector<move_t> current_variation;
extern vector<move_t> move_list;
extern move_t a_current_variation[MAX_PLY];
extern move_t a_move_list[MAX_PLY];

// game variables - reset each init
extern bool white_to_move;
extern bool enpassant;
extern int epsquare;
extern bool evaluating;
extern bool flip;
extern int ply;
extern int half_move;
extern int move_no;
extern int nodes;
extern int search_depth;
extern int debug;
extern int wtime;
extern int btime;
extern int winc;
extern int binc;
extern bool white_can_castle_kingside;
extern bool white_can_castle_queenside;
extern bool black_can_castle_kingside;
extern bool black_can_castle_queenside;

static string fen_start_pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

static string boardsquares[64] = {
	"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
	"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
	"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
	"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
	"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
	"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
	"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
	"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
};

static int rotate90[] = { 0,  8, 16, 24, 32, 40, 48, 56,
                          1,  9, 17, 25, 33, 41, 49, 57,
                          2, 10, 18, 26, 34, 42, 50, 58,                    
                          3, 11, 19, 27, 35, 43, 51, 59,
                          4, 12, 20, 28, 36, 44, 52, 60,
                          5, 13, 21, 29, 37, 45, 53, 61,
                          6, 14, 22, 30, 38, 46, 54, 62,
                          7, 15, 23, 31, 39, 47, 55, 63
                        };

static int rotate45l[] = { 15,22,29,36,43,50,57, 0,
                           23,30,37,44,51,58, 1, 8,
                           31,38,45,52,59, 2, 9,16,
                           39,46,53,60, 3,10,17,24,
                           47,54,61, 4,11,18,25,32,
                           55,62, 5,12,19,26,33,40,
                           63, 6,13,20,27,34,41,48, 
                           7,14,21,28,35,42,49,56 
                         };
static int rotate45r[] = {  0,57,50,43,36,29,22,15,
                            8, 1,58,51,44,37,30,23,
                           16, 9, 2,59,52,45,38,31,
                           24,17,10, 3,60,53,46,39,
                           32,25,18,11, 4,61,54,47,
                           40,33,26,19,12, 5,62,55,
                           48,41,34,27,20,13, 6,63, 
                           56,49,42,35,28,21,14, 7 
                         };
static int shift45[] = { 0, 0, 0, 0, 0, 0, 0, 0,
                         8, 8, 8, 8, 8, 8, 8,15,
                        16,16,16,16,16,16,22,22,
                        24,24,24,24,24,29,29,29,
                        32,32,32,32,36,36,36,36,
                        40,40,40,43,43,43,43,43,
                        48,48,50,50,50,50,50,50,
                        56,57,57,57,57,57,57,57 };
static int mask45[] = { 255,255,255,255,255,255,255,255,
                        127,127,127,127,127,127,127,  1,
                         63, 63, 63, 63, 63, 63,  3,  3,
                         31, 31, 31, 31, 31,  7,  7,  7,
                         15, 15, 15, 15, 15, 15, 15, 15,
                          7,  7,  7, 31, 31, 31, 31, 31,
                          3,  3, 63, 63, 63, 63, 63, 63,
                          1,127,127,127,127,127,127,127 
					  };

extern int chessboard[64];
extern string protocol;

// 0-7 = file1-file8
static BITBOARD bb_file[8] = {  0x0101010101010101,
								0x0202020202020202,
								0x0404040404040404,
								0x0808080808080808,
								0x1010101010101010,
								0x2020202020202020,
								0x4040404040404040,
								0x8080808080808080 };
// 0-7 = rank1-rank8
static BITBOARD bb_rank[8] = {  0x00000000000000ff,
							    0x000000000000ff00,
								0x0000000000ff0000,
								0x00000000ff000000,
								0x000000ff00000000,
								0x0000ff0000000000,
								0x00ff000000000000,
								0xff00000000000000 };

static BITBOARD bb_white_castle_kingside =  0x0000000000000060; 
static BITBOARD bb_white_castle_queenside = 0x0000000000000007; 
static BITBOARD bb_black_castle_kingside =  0x6000000000000000; 
static BITBOARD bb_black_castle_queenside =  0x0700000000000000;

//extern BITBOARD bbwhitepawnmoves[64];
extern BITBOARD bb_legal_pawn_moves[64];
extern BITBOARD bb_legal_pawn_moves2[64];
extern BITBOARD bb_legal_pawn_captures[64];
extern BITBOARD bb_legal_king_moves[64];
extern BITBOARD bb_legal_knight_moves[64];
extern BITBOARD bb_legal_slidingh_moves[64][256]; // Horizontal (rook/queen) moves
extern BITBOARD bb_legal_slidingv_moves[64][256]; // Vertical (rook/queen) moves
extern BITBOARD bb_legal_sliding45l_moves[64][256]; // left (bishop/queen) moves
extern BITBOARD bb_legal_sliding45r_moves[64][256]; // right (bishop/queen) moves
extern BITBOARD bb_squares[64];

extern BITBOARD bb_white_pawns;
extern BITBOARD bb_white_knights;
extern BITBOARD bb_white_bishops;
extern BITBOARD bb_white_rooks;
extern BITBOARD bb_white_queens;
extern BITBOARD bb_white_king;
extern BITBOARD bb_white_pieces;

extern BITBOARD bb_black_pawns;
extern BITBOARD bb_black_knights;
extern BITBOARD bb_black_bishops;
extern BITBOARD bb_black_rooks;
extern BITBOARD bb_black_queens;
extern BITBOARD bb_black_king;
extern BITBOARD bb_black_pieces;

extern BITBOARD bb_all_pieces;
extern BITBOARD bb_all_pieces90;
extern BITBOARD bb_all_pieces45l;
extern BITBOARD bb_all_pieces45r;

#endif //GLOBALS_H_