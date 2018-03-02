#define STATE_DEBUG -1
#define STATE_STARTUP 0
#define STATE_NAME_ENTRY 1
#define STATE_GAME 2
#define STATE_GAMEOVER 3
#define STATE_SCOREBOARD 4

extern void tick_go(void);
extern void tick_ne(void);
extern int state;
