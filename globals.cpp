#include <string>
#include <vector>
#include "globals.h"

using namespace std;

bool cointoss = true; // do we need to initialize srand only at first program launch
// game variables - reset each init
bool white_to_move;
bool enpassant;
int epsquare;
bool evaluating;
bool flip;
int ply;
int half_move;
int move_no;
int nodes;
int search_depth;
int chessboard[64];
string protocol;
int debug;
int wtime;
int btime;
int winc;
int binc;
bool white_can_castle_kingside = true;
bool white_can_castle_queenside = true;
bool black_can_castle_kingside = true;
bool black_can_castle_queenside = true;

vector<move_t> current_variation;
vector<move_t> move_list;
move_t a_current_variation[MAX_PLY];
move_t a_move_list[MAX_PLY];

BITBOARD bb_legal_pawn_moves[64];
BITBOARD bb_legal_pawn_moves2[64];
BITBOARD bb_legal_pawn_captures[64];
BITBOARD bb_legal_king_moves[64];
BITBOARD bb_legal_knight_moves[64];
BITBOARD bb_legal_slidingh_moves[64][256]; // Horizontal (rook/queen) moves
BITBOARD bb_legal_slidingv_moves[64][256]; // Vertical (rook/queen) moves
BITBOARD bb_legal_sliding45l_moves[64][256]; // left (bishop/queen) moves
BITBOARD bb_legal_sliding45r_moves[64][256]; // right (bishop/queen) moves
BITBOARD bb_squares[64];

BITBOARD bb_white_pawns;
BITBOARD bb_white_knights;
BITBOARD bb_white_bishops;
BITBOARD bb_white_rooks;
BITBOARD bb_white_queens;
BITBOARD bb_white_king;
BITBOARD bb_white_pieces;

BITBOARD bb_black_pawns;
BITBOARD bb_black_knights;
BITBOARD bb_black_bishops;
BITBOARD bb_black_rooks;
BITBOARD bb_black_queens;
BITBOARD bb_black_king;
BITBOARD bb_black_pieces;

BITBOARD bb_all_pieces;
BITBOARD bb_all_pieces90;
BITBOARD bb_all_pieces45l;
BITBOARD bb_all_pieces45r;
