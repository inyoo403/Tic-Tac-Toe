# Tic-Tac-Toe

ImGui-based Tic-Tac-Toe game with AI opponent.

## How to Play

1. Click an empty square to place X
2. AI automatically places O
3. First to complete 3 in a row/column/diagonal wins

## Features

### Game Implementation
- 3x3 grid board setup
- Player turn management
- Win detection (rows, columns, diagonals)
- Draw detection
- State serialization/deserialization

### Game Status Window
- Current player information
- Turn count
- Board state string
- Last move information
- Game over status with winner/draw indication
- Reset game button

### Game Log Window
- Real-time move logging with player and position
- Win/draw detection logging
- Color-coded log entries (Green: wins, Yellow: draws, Blue: moves)
- Auto-scroll to latest entries
- Clear log and copy log functionality
- Maximum 100 log entries

### Player Move Tracking
- Automatic detection of player moves
- Logs include player number, symbol (X/O), and board coordinates
- Tracks game state changes between turns

## AI Implementation

### Algorithm
Negamax with Alpha-Beta Pruning

### Core Concepts
- AI = +1, Human = -1 (sign determines maximizing/minimizing)
- Searches on string copy of board (GUI independent)
- Reuses `actionForEmptyHolder()` so AI places pieces same way as human

### Key Functions

**updateAI()**
- Tries each empty square virtually, scores with negamax
- Selects highest scoring move

**negamax(state, depth, alpha, beta, playerColor)**
- Returns evaluation at terminal state
- Recursive call: `-negamax(newState, depth+1, -beta, -alpha, -playerColor)`
- Prunes when `alpha >= beta`

### Evaluation
- AI wins: +1, Human wins: -1, Draw: 0
- Multiplied by `(10 - depth)` to prefer faster wins

### Board Representation
9-char string: `'0'`=empty, `'1'`=X, `'2'`=O
