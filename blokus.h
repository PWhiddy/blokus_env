#ifndef BLOKUS_H
#define BLOKUS_H

#include <stdbool.h>
#include <stdint.h>

#define BOARD_SIZE 20
#define NUM_PLAYERS 4
#define NUM_PIECE_TYPES 21
#define MAX_PIECE_SIZE 5
#define MAX_ORIENTATIONS 8  // Maximum orientations per piece
#define MAX_ACTIONS 1000    // Maximum possible actions for a player

// Represents a point on the board
typedef struct {
    int8_t x;
    int8_t y;
} Point;

// Represents a piece shape in a specific orientation
typedef struct {
    int8_t size;                    // Number of blocks in the piece
    Point blocks[MAX_PIECE_SIZE];   // Coordinates of blocks relative to origin
} PieceShape;

// Represents a piece type with all possible orientations
typedef struct {
    int8_t num_orientations;
    PieceShape orientations[MAX_ORIENTATIONS];
} PieceType;

// Represents a player's state
typedef struct {
    bool available_pieces[NUM_PIECE_TYPES];  // Available pieces
    int score;
    bool first_move;                        // Whether it's the player's first move
} PlayerState;

// Represents an action
typedef struct {
    int8_t piece_idx;    // Which piece to place
    int8_t orientation;  // Which orientation of the piece
    Point position;      // Where to place the piece
} Action;

// Represents the game state
typedef struct {
    uint8_t board[BOARD_SIZE][BOARD_SIZE];  // 0 for empty, 1-4 for player colors
    PlayerState players[NUM_PLAYERS];
    int8_t current_player;
    bool game_over;
} GameState;

// Blokus environment for RL
typedef struct {
    GameState state;
    PieceType piece_types[NUM_PIECE_TYPES];
} BlokusEnv;

// Core game functions
void init_game(GameState *state);
bool is_valid_action(const BlokusEnv *env, int player, const Action *action);
void apply_action(BlokusEnv *env, const Action *action);
int get_valid_actions(const BlokusEnv *env, int player, Action *actions, int max_actions);
bool is_game_over(const BlokusEnv *env);
void calculate_scores(GameState *state);

// RL environment functions
void init_env(BlokusEnv *env);
bool step(BlokusEnv *env, const Action *action, float *reward);
void reset_env(BlokusEnv *env);
Action get_random_action(const BlokusEnv *env);

// Utility functions
void display_board(const GameState *state);
void init_piece_types(PieceType piece_types[NUM_PIECE_TYPES]);

#endif // BLOKUS_H
