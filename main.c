#include "blokus.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>  // For usleep

void print_piece(const BlokusEnv *env, int piece_idx, int orientation) {
    if (piece_idx < 0 || piece_idx >= NUM_PIECE_TYPES) {
        printf("Invalid piece index\n");
        return;
    }
    
    const PieceType *piece_type = &env->piece_types[piece_idx];
    if (orientation < 0 || orientation >= piece_type->num_orientations) {
        orientation = 0;
    }
    
    const PieceShape *shape = &piece_type->orientations[orientation];
    if (shape->size <= 0) {
        printf("Empty shape\n");
        return;
    }
    
    // Find the bounding box
    int min_x = shape->blocks[0].x;
    int min_y = shape->blocks[0].y;
    int max_x = shape->blocks[0].x;
    int max_y = shape->blocks[0].y;
    
    for (int i = 1; i < shape->size; i++) {
        if (shape->blocks[i].x < min_x) min_x = shape->blocks[i].x;
        if (shape->blocks[i].y < min_y) min_y = shape->blocks[i].y;
        if (shape->blocks[i].x > max_x) max_x = shape->blocks[i].x;
        if (shape->blocks[i].y > max_y) max_y = shape->blocks[i].y;
    }
    
    // Create a grid for the piece
    int width = max_x - min_x + 1;
    int height = max_y - min_y + 1;
    
    // Avoid potential buffer overflows
    if (width > 5 || height > 5) {
        printf("Piece too large to display\n");
        return;
    }
    
    char grid[5][5]; // 5x5 is enough for any Blokus piece
    
    // Initialize grid to spaces
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            grid[y][x] = '.';
        }
    }
    
    // Fill in the blocks
    for (int i = 0; i < shape->size; i++) {
        int x = shape->blocks[i].x - min_x;
        int y = shape->blocks[i].y - min_y;
        if (x >= 0 && x < width && y >= 0 && y < height) {
            grid[y][x] = '#';
        }
    }
    
    // Print the grid
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("%c ", grid[y][x]);
        }
        printf("\n");
    }
}

// Function to count remaining tiles for a player
int count_remaining_tiles(const BlokusEnv *env, int player) {
    int remaining = 0;
    for (int i = 0; i < NUM_PIECE_TYPES; i++) {
        if (env->state.players[player].available_pieces[i]) {
            // Add the size of this piece to the count
            remaining += env->piece_types[i].orientations[0].size;
        }
    }
    return remaining;
}

void display_available_pieces(const BlokusEnv *env, int player) {
    printf("\nAvailable pieces for Player %d:\n", player + 1);
    printf("-----------------------------\n");
    
    for (int i = 0; i < NUM_PIECE_TYPES; i++) {
        if (env->state.players[player].available_pieces[i]) {
            printf("Piece %d:\n", i);
            print_piece(env, i, 0);
            printf("\n");
        }
    }
    printf("-----------------------------\n");
}

// Display player scores including both tiles played and remaining
void display_scores(const BlokusEnv *env) {
    printf("\nCurrent Scores:\n");
    printf("---------------\n");
    for (int i = 0; i < NUM_PLAYERS; i++) {
        int tiles_remaining = count_remaining_tiles(env, i);
        printf("Player %d: %d points (Tiles placed), %d tiles remaining\n", 
               i + 1, env->state.players[i].score, tiles_remaining);
    }
    printf("---------------\n");
}

Action get_human_action(const BlokusEnv *env) {
    int player = env->state.current_player;
    Action action;
    
    while (1) {
        display_available_pieces(env, player);
        display_scores(env); // Show current scores including remaining tiles
        
        printf("Enter piece index, orientation, x, y (e.g., '3 0 5 7'): ");
        if (scanf("%hhd %hhd %hhd %hhd", &action.piece_idx, &action.orientation, 
                 &action.position.x, &action.position.y) != 4) {
            printf("Invalid input. Try again.\n");
            while (getchar() != '\n');  // Clear input buffer
            continue;
        }
        
        if (is_valid_action(env, player, &action)) {
            return action;
        } else {
            printf("Invalid move. Try again.\n");
        }
    }
}

void play_auto_game() {

    bool fast = false;

    for (int i = 0; i < 100; i++) {

        BlokusEnv env;
        init_env(&env);
        
        int turn_count = 0;
        
        while (!env.state.game_over) {
            if (!fast) {
                // Clear screen (ANSI escape code)
                printf("\033[2J\033[H");
                
                // Show turn number
                printf("Turn: %d\n", turn_count++);
                
                // Show the game board
                display_board(&env.state);
                
                // Show current scores including remaining tiles
                display_scores(&env);
            }
            Action action;
            int player = env.state.current_player;
            
            // Check if any actions are possible
            Action possible_actions[1];
            int num_actions = get_valid_actions(&env, player, possible_actions, 1);
            
            if (num_actions == 0) {
                env.state.current_player = (env.state.current_player + 1) % NUM_PLAYERS;
                continue;
            }
            
            // All players are random agents
            action = get_random_action(&env);
            
            float reward;
            bool game_over = step(&env, &action, &reward, fast);
            
            if (game_over) {
                env.state.game_over = true;
            }
            
            // Sleep for 250ms (4 moves per second)
            if (!fast) {
                usleep(250000);
            }
            //usleep(250000);
        }
        if (!fast) {
            // Game over
            printf("\033[2J\033[H");  // Clear screen
            printf("Final board state:\n");
            display_board(&env.state);
            
            printf("\n===== GAME OVER =====\n\n");
            for (int i = 0; i < NUM_PLAYERS; i++) {
                int tiles_remaining = count_remaining_tiles(&env, i);
                printf("Player %d: %d points (Tiles placed), %d tiles remaining\n", 
                    i + 1, env.state.players[i].score, tiles_remaining);
            }
            
            // Find winner(s)
            int max_score = -1;
            for (int i = 0; i < NUM_PLAYERS; i++) {
                if (env.state.players[i].score > max_score) {
                    max_score = env.state.players[i].score;
                }
            }
            
            printf("\nWinner(s): ");
            bool first_winner = true;
            for (int i = 0; i < NUM_PLAYERS; i++) {
                if (env.state.players[i].score == max_score) {
                    if (!first_winner) printf(", ");
                    printf("Player %d", i + 1);
                    first_winner = false;
                }
            }
            printf("\n");
        }
    }
}

void play_game() {
    
    BlokusEnv env;

    init_env(&env);

    while (!env.state.game_over) {
        display_board(&env.state);
        
        // Show current scores including remaining tiles
        display_scores(&env);
        
        Action action;
        int player = env.state.current_player;
        
        // Check if any actions are possible
        Action possible_actions[1];
        int num_actions = get_valid_actions(&env, player, possible_actions, 1);
        
        if (num_actions == 0) {
            printf("Player %d has no valid moves and passes.\n", player + 1);
            env.state.current_player = (env.state.current_player + 1) % NUM_PLAYERS;
            continue;
        }
        
        if (player == 0) {  // Human player
            printf("\nYour turn (Player 1)\n");
            action = get_human_action(&env);
        } else {  // Random agent
            action = get_random_action(&env);
            printf("\nPlayer %d plays piece %d (orientation %d) at (%d, %d)\n", 
                player + 1, action.piece_idx, action.orientation,
                action.position.x, action.position.y);
        }
        
        float reward;
        bool game_over = step(&env, &action, &reward, false);
        
        if (game_over) {
            env.state.game_over = true;
        }
    }
    
    // Game over
    display_board(&env.state);
    
    printf("\n===== GAME OVER =====\n\n");
    for (int i = 0; i < NUM_PLAYERS; i++) {
        int tiles_remaining = count_remaining_tiles(&env, i);
        printf("Player %d: %d points (Tiles placed), %d tiles remaining\n", 
               i + 1, env.state.players[i].score, tiles_remaining);
    }
    
    // Find winner(s)
    int max_score = -1;
    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (env.state.players[i].score > max_score) {
            max_score = env.state.players[i].score;
        }
    }
    
    printf("\nWinner(s): ");
    bool first_winner = true;
    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (env.state.players[i].score == max_score) {
            if (!first_winner) printf(", ");
            printf("Player %d", i + 1);
            first_winner = false;
        }
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    srand(time(NULL));  // Initialize random seed
    
    // Check for --auto flag
    bool auto_mode = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--auto") == 0) {
            auto_mode = true;
            break;
        }
    }
    
    if (auto_mode) {
        printf("Running in auto mode with 4 random agents (4 moves per second).\n");
        printf("Press Ctrl+C to exit.\n\n");
        play_auto_game();
    } else {
        printf("Welcome to Blokus!\n");
        printf("You are Player 1 (Blue), playing against 3 random agents.\n");
        printf("Let's begin!\n\n");
        play_game();
    }
    
    return 0;
}