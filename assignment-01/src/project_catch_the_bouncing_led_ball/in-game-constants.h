// Game constants
enum c_difficulty
{
  seasy,
  easy,
  normal,
  medium,
  hard,
  nightmare,
  hell,
  inhuman,
};

static const char * const c_difficulties[] = {
  [seasy] = "Super Easy",
  [easy] = "Easy",
  [normal] = "Normal",
  [medium] = "Medium",
  [hard] = "Hard",
  [nightmare] = "Nightmare",
  [hell] = "Hell",
  [inhuman] = "Are you even human?"
};


static const String c_welcome = "Welcome to the Catch the Bouncing Led Ball Game. Press Key T1 to Start";
static const String c_score = "New point! Score: ";
static const String c_best_score = "Best score earned in ";
static const String c_game_starts = "Go!";
static const String c_game_over = "Game Over. Final Score: ";
static const String c_difficulty = "Difficulty changed to: ";
static const int c_difficulty_modes = 8;
static const int c_timer1 = 6;
static const int c_timer2 = 10;
static const int c_speed = 2;
static const int c_timer1_min = 4;
static const int c_timer2_min = 1;
static const float c_speed_min = 0.1;

// Hardware game constants
static const int c_tBlink = 2;
