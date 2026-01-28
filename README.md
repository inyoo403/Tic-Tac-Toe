# Tic-Tac-Toe Game

A complete implementation of Tic-Tac-Toe game with AI opponent using ImGui.

## Features

### Game Implementation
  - Board setup with 3x3 grid
  - Player turn management
  - Win condition detection (rows, columns, diagonals)
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
- Color-coded log entries:
  - Green for wins
  - Yellow for draws
  - Blue for player moves
- Auto-scroll to latest entries
- Clear log and copy log functionality
- Maximum 100 log entries

### Player Move Tracking
- Automatic detection of player moves
- Logs include player number, symbol (X/O), and board coordinates
- Tracks game state changes between turns

### AI Opponent
- Negamax algorithm with Alpha-Beta Pruning
- String-based state evaluation for optimal performance
- Minimizes memory allocation overhead
- AI player automatically makes moves when it's their turn

## How to Play

1. Click on an empty square to place your piece (X)
2. AI will automatically place its piece (O) after your move
3. First player to get three in a row wins
4. If all squares are filled with no winner, it's a draw
5. Use "Reset Game" button to start a new game

## Technical Details

- **AI Algorithm**: Negamax with Alpha-Beta Pruning
- **State Representation**: String-based (9 characters: '0' for empty, '1' for player 0, '2' for player 1)
