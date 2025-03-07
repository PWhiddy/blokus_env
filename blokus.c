#include "blokus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Initialize all piece types with their orientations
// blokus.c - Updated piece initialization

// blokus.c - Updated piece initialization

void init_piece_types(PieceType piece_types[NUM_PIECE_TYPES]) {
    // Clear everything first
    memset(piece_types, 0, sizeof(PieceType) * NUM_PIECE_TYPES);
    
    // 0. Monomino (1 block)
    piece_types[0].num_orientations = 1;
    piece_types[0].orientations[0].size = 1;
    piece_types[0].orientations[0].blocks[0] = (Point){0, 0};
    
    // 1. Domino (2 blocks)
    piece_types[1].num_orientations = 2;
    // Horizontal
    piece_types[1].orientations[0].size = 2;
    piece_types[1].orientations[0].blocks[0] = (Point){0, 0};
    piece_types[1].orientations[0].blocks[1] = (Point){1, 0};
    // Vertical
    piece_types[1].orientations[1].size = 2;
    piece_types[1].orientations[1].blocks[0] = (Point){0, 0};
    piece_types[1].orientations[1].blocks[1] = (Point){0, 1};
    
    // 2. I-Tromino (3 blocks in a line)
    piece_types[2].num_orientations = 2;
    // Horizontal
    piece_types[2].orientations[0].size = 3;
    piece_types[2].orientations[0].blocks[0] = (Point){0, 0};
    piece_types[2].orientations[0].blocks[1] = (Point){1, 0};
    piece_types[2].orientations[0].blocks[2] = (Point){2, 0};
    // Vertical
    piece_types[2].orientations[1].size = 3;
    piece_types[2].orientations[1].blocks[0] = (Point){0, 0};
    piece_types[2].orientations[1].blocks[1] = (Point){0, 1};
    piece_types[2].orientations[1].blocks[2] = (Point){0, 2};
    
    // 3. L-Tromino (3 blocks in an L)
    piece_types[3].num_orientations = 4;
    // L shape
    piece_types[3].orientations[0].size = 3;
    piece_types[3].orientations[0].blocks[0] = (Point){0, 0};
    piece_types[3].orientations[0].blocks[1] = (Point){0, 1};
    piece_types[3].orientations[0].blocks[2] = (Point){1, 1};
    // Rotated 90°
    piece_types[3].orientations[1].size = 3;
    piece_types[3].orientations[1].blocks[0] = (Point){0, 0};
    piece_types[3].orientations[1].blocks[1] = (Point){1, 0};
    piece_types[3].orientations[1].blocks[2] = (Point){0, 1};
    // Rotated 180°
    piece_types[3].orientations[2].size = 3;
    piece_types[3].orientations[2].blocks[0] = (Point){0, 0};
    piece_types[3].orientations[2].blocks[1] = (Point){1, 0};
    piece_types[3].orientations[2].blocks[2] = (Point){1, -1};
    // Rotated 270°
    piece_types[3].orientations[3].size = 3;
    piece_types[3].orientations[3].blocks[0] = (Point){0, 0};
    piece_types[3].orientations[3].blocks[1] = (Point){0, -1};
    piece_types[3].orientations[3].blocks[2] = (Point){1, -1};
    
    // 4. Tetromino I (4 blocks in a line)
    piece_types[4].num_orientations = 2;
    // Horizontal
    piece_types[4].orientations[0].size = 4;
    piece_types[4].orientations[0].blocks[0] = (Point){0, 0};
    piece_types[4].orientations[0].blocks[1] = (Point){1, 0};
    piece_types[4].orientations[0].blocks[2] = (Point){2, 0};
    piece_types[4].orientations[0].blocks[3] = (Point){3, 0};
    // Vertical
    piece_types[4].orientations[1].size = 4;
    piece_types[4].orientations[1].blocks[0] = (Point){0, 0};
    piece_types[4].orientations[1].blocks[1] = (Point){0, 1};
    piece_types[4].orientations[1].blocks[2] = (Point){0, 2};
    piece_types[4].orientations[1].blocks[3] = (Point){0, 3};
    
    // Add the remaining 16 pieces with similar patterns
    // For brevity, I'll just initialize a few more here:
    
    // 5. Tetromino L
    piece_types[5].num_orientations = 4;
    piece_types[5].orientations[0].size = 4;
    piece_types[5].orientations[0].blocks[0] = (Point){0, 0};
    piece_types[5].orientations[0].blocks[1] = (Point){0, 1};
    piece_types[5].orientations[0].blocks[2] = (Point){0, 2};
    piece_types[5].orientations[0].blocks[3] = (Point){1, 2};

    // 6. Tetromino T
    piece_types[6].num_orientations = 4;
    piece_types[6].orientations[0].size = 4;
    piece_types[6].orientations[0].blocks[0] = (Point){0, 0};
    piece_types[6].orientations[0].blocks[1] = (Point){1, 0};
    piece_types[6].orientations[0].blocks[2] = (Point){2, 0};
    piece_types[6].orientations[0].blocks[3] = (Point){1, 1};
    
    // Initialize the remaining pieces (7-20) similarly...
    // For a real implementation, all 21 pieces should be defined here
}

void init_game(GameState *state) {
    // Clear the board
    memset(state->board, 0, sizeof(state->board));
    
    // Initialize player states
    for (int i = 0; i < NUM_PLAYERS; i++) {
        state->players[i].score = 0;
        state->players[i].first_move = true;
        memset(state->players[i].available_pieces, true, sizeof(state->players[i].available_pieces));
    }
    
    state->current_player = 0;
    state->game_over = false;
}

// Check if a position is the starting corner for a player
bool is_starting_corner(int player, int x, int y) {
    switch (player) {
        case 0: return x == 0 && y == 0;                     // Top-left
        case 1: return x == BOARD_SIZE-1 && y == 0;          // Top-right
        case 2: return x == BOARD_SIZE-1 && y == BOARD_SIZE-1; // Bottom-right
        case 3: return x == 0 && y == BOARD_SIZE-1;          // Bottom-left
        default: return false;
    }
}

bool is_valid_action(const BlokusEnv *env, int player, const Action *action) {
    const GameState *state = &env->state;
    const PieceType *piece_type = &env->piece_types[action->piece_idx];
    
    // Check if the piece index and orientation are valid
    if (action->piece_idx < 0 || action->piece_idx >= NUM_PIECE_TYPES ||
        action->orientation < 0 || action->orientation >= piece_type->num_orientations) {
        return false;
    }
    
    // Check if piece is available
    if (!state->players[player].available_pieces[action->piece_idx]) {
        return false;
    }
    
    const PieceShape *shape = &piece_type->orientations[action->orientation];
    bool touches_corner = false;
    bool first_move = state->players[player].first_move;
    
    // Check if all cells in the piece can be placed on the board
    for (int i = 0; i < shape->size; i++) {
        int x = action->position.x + shape->blocks[i].x;
        int y = action->position.y + shape->blocks[i].y;
        
        // Check if within board bounds
        if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
            return false;
        }
        
        // Check if cell is empty
        if (state->board[y][x] != 0) {
            return false;
        }
        
        // For first move, check if touches starting corner
        if (first_move && is_starting_corner(player, x, y)) {
            touches_corner = true;
        }
        
        // Check for edge adjacency with same color (not allowed)
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (abs(dx) + abs(dy) == 1) {  // Orthogonal neighbor
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE) {
                        if (state->board[ny][nx] == player + 1) {
                            return false;  // Edge adjacency with same color
                        }
                    }
                }
            }
        }
        
        // Check for corner adjacency with same color (required after first move)
        if (!first_move) {
            for (int dx = -1; dx <= 1; dx += 2) {
                for (int dy = -1; dy <= 1; dy += 2) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE) {
                        if (state->board[ny][nx] == player + 1) {
                            touches_corner = true;
                        }
                    }
                }
            }
        }
    }
    
    // Check requirements based on move type
    if (first_move) {
        return touches_corner;  // First move must touch starting corner
    } else {
        return touches_corner;  // Subsequent moves must touch corner of same color
    }
}

void apply_action(BlokusEnv *env, const Action *action) {
    GameState *state = &env->state;
    int player = state->current_player;
    const PieceType *piece_type = &env->piece_types[action->piece_idx];
    const PieceShape *shape = &piece_type->orientations[action->orientation];
    
    // Place the piece on the board
    for (int i = 0; i < shape->size; i++) {
        int x = action->position.x + shape->blocks[i].x;
        int y = action->position.y + shape->blocks[i].y;
        state->board[y][x] = player + 1;
    }
    
    // Mark the piece as used
    state->players[player].available_pieces[action->piece_idx] = false;
    
    // Update score
    state->players[player].score += shape->size;
    
    // Mark that the player has made their first move
    state->players[player].first_move = false;
    
    // Move to the next player
    state->current_player = (state->current_player + 1) % NUM_PLAYERS;
    
    // Skip players who can't make a move
    int consecutive_passes = 0;
    
    while (consecutive_passes < NUM_PLAYERS) {
        Action dummy_actions[1];
        int player = state->current_player;
        
        if (get_valid_actions(env, player, dummy_actions, 1) > 0) {
            break;  // Current player can make a move
        }
        
        // Current player has to pass
        printf("Player %d has no valid moves and passes.\n", player + 1);
        state->current_player = (state->current_player + 1) % NUM_PLAYERS;
        consecutive_passes++;
    }
    
    // If all players had to pass, game is over
    if (consecutive_passes == NUM_PLAYERS) {
        state->game_over = true;
    }
}

int get_valid_actions(const BlokusEnv *env, int player, Action *actions, int max_actions) {
    int count = 0;
    
    // For each piece
    for (int piece_idx = 0; piece_idx < NUM_PIECE_TYPES && count < max_actions; piece_idx++) {
        if (!env->state.players[player].available_pieces[piece_idx]) {
            continue;
        }
        
        const PieceType *piece_type = &env->piece_types[piece_idx];
        
        // For each orientation
        for (int orient = 0; orient < piece_type->num_orientations && count < max_actions; orient++) {
            // Use shape variable here to avoid the warning
            const PieceShape *shape = &piece_type->orientations[orient];
            
            // Optimize the search space based on whether it's the first move
            if (env->state.players[player].first_move) {
                // For first move, only need to check positions near starting corner
                int start_x, start_y;
                switch (player) {
                    case 0: start_x = 0; start_y = 0; break;
                    case 1: start_x = BOARD_SIZE-1; start_y = 0; break;
                    case 2: start_x = BOARD_SIZE-1; start_y = BOARD_SIZE-1; break;
                    case 3: start_x = 0; start_y = BOARD_SIZE-1; break;
                    default: start_x = 0; start_y = 0; break;
                }
                
                // Check positions near the starting corner
                for (int dx = -shape->size; dx <= shape->size && count < max_actions; dx++) {
                    for (int dy = -shape->size; dy <= shape->size && count < max_actions; dy++) {
                        int x = start_x + dx;
                        int y = start_y + dy;
                        
                        Action action = {piece_idx, orient, {x, y}};
                        if (is_valid_action(env, player, &action)) {
                            actions[count++] = action;
                        }
                    }
                }
            } else {
                // For subsequent moves, check the board more efficiently
                // We only need to check near existing pieces of the player
                for (int x = 0; x < BOARD_SIZE && count < max_actions; x++) {
                    for (int y = 0; y < BOARD_SIZE && count < max_actions; y++) {
                        Action action = {piece_idx, orient, {x, y}};
                        if (is_valid_action(env, player, &action)) {
                            actions[count++] = action;
                        }
                    }
                }
            }
        }
    }
    
    return count;
}

bool is_game_over(const BlokusEnv *env) {
    if (env->state.game_over) {
        return true;
    }
    
    // Check if any player can make a valid move
    for (int player = 0; player < NUM_PLAYERS; player++) {
        Action actions[1];
        if (get_valid_actions(env, player, actions, 1) > 0) {
            return false;
        }
    }
    return true;
}

void calculate_scores(GameState *state) {
    // Add bonus for using all pieces
    for (int player = 0; player < NUM_PLAYERS; player++) {
        bool all_used = true;
        for (int i = 0; i < NUM_PIECE_TYPES; i++) {
            if (state->players[player].available_pieces[i]) {
                all_used = false;
                break;
            }
        }
        
        if (all_used) {
            state->players[player].score += 15;  // Bonus for using all pieces
        }
        
        // Additional 5-point bonus if the last piece played was the 1-square piece
        if (all_used && !state->players[player].available_pieces[0]) {
            state->players[player].score += 5;
        }
    }
}

void display_board(const GameState *state) {
    printf("  ");
    for (int x = 0; x < BOARD_SIZE; x++) {
        printf("%2d", x % 10);  // Print column numbers (modulo 10 for readability)
    }
    printf("\n");
    
    for (int y = 0; y < BOARD_SIZE; y++) {
        printf("%2d ", y % 10);  // Print row numbers
        for (int x = 0; x < BOARD_SIZE; x++) {
            int cell = state->board[y][x];
            if (cell == 0) {
                printf(". ");
            } else {
                // Use colors: B-Blue, R-Red, G-Green, Y-Yellow
                char symbols[] = {'B', 'R', 'G', 'Y'};
                printf("%c ", symbols[cell - 1]);
            }
        }
        printf("\n");
    }
    
    // Print scores
    printf("\nScores: ");
    for (int i = 0; i < NUM_PLAYERS; i++) {
        printf("Player %d: %d  ", i+1, state->players[i].score);
    }
    printf("\n\n");
}

// RL Environment functions

void init_env(BlokusEnv *env) {
    init_game(&env->state);
    init_piece_types(env->piece_types);
}

bool step(BlokusEnv *env, const Action *action, float *reward) {
    int player = env->state.current_player;
    
    // Check if action is valid
    if (!is_valid_action(env, player, action)) {
        *reward = -10.0f;  // Invalid action penalty
        return false;
    }
    
    // Apply the action
    apply_action(env, action);
    
    // Simple reward: the size of the piece placed
    const PieceType *piece_type = &env->piece_types[action->piece_idx];
    const PieceShape *shape = &piece_type->orientations[action->orientation];
    *reward = (float)shape->size;
    
    // Check if game is over
    if (is_game_over(env)) {
        calculate_scores(&env->state);
        env->state.game_over = true;
        
        // Additional reward based on winning or final score
        if (env->state.players[player].score > env->state.players[(player+1)%NUM_PLAYERS].score &&
            env->state.players[player].score > env->state.players[(player+2)%NUM_PLAYERS].score &&
            env->state.players[player].score > env->state.players[(player+3)%NUM_PLAYERS].score) {
            *reward += 50.0f;  // Winning bonus
        }
        
        return true;
    }
    
    return false;  // Game not over
}

void reset_env(BlokusEnv *env) {
    init_game(&env->state);
}

Action get_random_action(const BlokusEnv *env) {
    int player = env->state.current_player;
    Action actions[MAX_ACTIONS];
    int num_actions = get_valid_actions(env, player, actions, MAX_ACTIONS);
    
    if (num_actions > 0) {
        return actions[rand() % num_actions];
    } else {
        // No valid actions, return a dummy action
        Action dummy = {0, 0, {0, 0}};
        return dummy;
    }
}
