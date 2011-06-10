#ifndef PROTO_H_
#define PROTO_H_

void print_chessboard(); // utility.cpp
void print_vector(vector<move_t>); // utility.cpp
void print_variation(vector<move_t>); // utility.cpp
void print_movelist(); // utility.cpp
void print_bitboard(string, BITBOARD); // utility.cpp
void print_bitboard_long(string, BITBOARD); // utility.cpp
int square_to_int(string); // utility.cpp
void new_game(string); // slice.cpp
void process_moves(vector<string>); // slice.cpp
move_t movegen(int);  // search.cpp returns the recommended move
//vector<move_t> pawngenw(int); // mg_pawn.cpp can the white pawn make a move from square x
int pawngenw(int, move_t []); // mg_pawn.cpp can the white pawn make a move from square x
//vector<move_t> pawngenb(int); // mg_pawn.cpp can the black pawn make a move from square x
int pawngenb(int, move_t []); // mg_pawn.cpp can the black pawn make a move from square x
int gen_pawn_moves(move_t []); // mg_pawn.cpp
void takeback(move_t); // slice.cpp takeback the move
void generate_static_bitboards(); // slice.cpp generate all the static bitboards used for move generation
void makemove(move_t); // slice.cpp make the move
int coin_toss(void); // utility.cpp flip a coin
string move_to_long_algebraic(move_t); // utility.cpp 
void gen_static_squares_bitboards(void); // bitboards.cpp
void gen_static_king_bitboards(void); // bitboards.cpp
void gen_static_knight_bitboards(void); // bitboards.cpp
void gen_static_slidingh_bitboards(void); // bitboards.cpp
void gen_static_slidingv_bitboards(void); // bitboards.cpp
void gen_static_sliding45l_bitboards(void); // bitboards.cpp
void gen_static_sliding45r_bitboards(void); // bitboards.cpp
int gen_king_moves(move_t []); // mg_king.cpp
int gen_knight_moves(move_t []); // mg_knight.cpp
int gen_rook_moves(move_t []); // mg_rook.cpp
int gen_bishop_moves(move_t []); // mg_bishop.cpp
int bitScanForward(BITBOARD); // utility.cpp
int eval(void); // eval.cpp
int get_moves(move_t []); // search.cpp
int alphabeta(int, int, int); // search.cpp
#endif //PROTO_H_