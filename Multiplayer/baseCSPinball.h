#ifndef BASECSPINBALL_H
#define	BASECSPINBALL_H

#define STD_PORT 49876
#define MAX_LENGTH_NAME_PLAYER 20

enum class STATE_OF_GAME {
  FINISH,
  START,
  ACTIVE
};

struct PairPlayers {
  int game_id;
  int playerOne;
  int playerTwo;
  STATE_OF_GAME state;
};


#endif	/* BASECSPINBALL_H */

