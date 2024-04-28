#ifndef GENERATE_H
#define GENERATE_H

#include "state.h"

int gen(state_t *s, move_s *moves);
int gen_quiet(state_t *s, move_s *moves);
int gen_captures(state_t *s, move_s *moves);
int gen_good_checks(state_t *s, move_s *moves);
int gen_evasions(state_t *s, move_s *moves, const int checker);
move_s* add_move(move_s *move, int Pfrom, int Ptarget, int Ppromoted);
move_s* add_capture(move_s *move, int Pfrom, int Ptarget, int Pcaptured,
		 int Ppromoted, int Pep);
move_s* add_king_castle (move_s *move, int Pfrom, int Ptarget, int Pcastle_type);

#endif
