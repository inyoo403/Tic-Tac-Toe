#include "../Application.h"
#include "TicTacToe.h"

const int AI_PLAYER = 1;
const int HUMAN_PLAYER = 0;

TicTacToe::TicTacToe() { _aiMoveMade = false; }
TicTacToe::~TicTacToe() {}

Bit* TicTacToe::PieceForPlayer(const int playerNumber)
{
    Bit *bit = new Bit();
    bit->LoadTextureFromFile(playerNumber == 0 ? "x.png" : "o.png");
    bit->setOwner(getPlayerAt(playerNumber));
    return bit;
}

void TicTacToe::setUpBoard()
{
    setNumberOfPlayers(2);
    setAIPlayer(AI_PLAYER);
    _gameOptions.rowX = 3;
    _gameOptions.rowY = 3;
    const float squareSize = 100.0f;
    const float spacing = 10.0f;
    const float startX = 50.0f;
    const float startY = 50.0f;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            ImVec2 position(startX + x * (squareSize + spacing), startY + y * (squareSize + spacing));
            _grid[y][x].initHolder(position, "square.png", x, y);
            _grid[y][x].setSize(squareSize, squareSize);
        }
    }
    startGame();
    _aiMoveMade = false;
}

bool TicTacToe::actionForEmptyHolder(BitHolder *holder)
{
    if (!holder || holder->bit() != nullptr) return false;
    Player* currentPlayer = getCurrentPlayer();
    if (!currentPlayer) return false;
    int currentPlayerIndex = currentPlayer->playerNumber();
    Bit* newBit = PieceForPlayer(currentPlayerIndex);
    newBit->setPosition(holder->getPosition());
    holder->setBit(newBit);
    ClassGame::lastMovedPlayer = currentPlayerIndex;
    _aiMoveMade = false;
    return true;
}

bool TicTacToe::canBitMoveFrom(Bit *bit, BitHolder *src) { return false; }
bool TicTacToe::canBitMoveFromTo(Bit* bit, BitHolder*src, BitHolder*dst) { return false; }

void TicTacToe::stopGame()
{
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            _grid[y][x].destroyBit();
        }
    }
    _aiMoveMade = false;
}

Player* TicTacToe::ownerAt(int index ) const
{
    Bit* bit = _grid[index / 3][index % 3].bit();
    return bit ? bit->getOwner() : nullptr;
}

Player* TicTacToe::checkForWinner()
{
    int wins[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    for (int i = 0; i < 8; i++) {
        Player* o1 = ownerAt(wins[i][0]);
        Player* o2 = ownerAt(wins[i][1]);
        Player* o3 = ownerAt(wins[i][2]);
        if (o1 && o1 == o2 && o2 == o3) return o1;
    }
    return nullptr;
}

bool TicTacToe::checkForDraw()
{
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (_grid[y][x].bit() == nullptr) return false;
        }
    }
    return true;
}

std::string TicTacToe::initialStateString() { return "000000000"; }

std::string TicTacToe::stateString() const
{
    std::string state = "";
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            Bit* bit = _grid[y][x].bit();
            if (bit == nullptr) state += '0';
            else state += std::to_string(bit->getOwner()->playerNumber() + 1);
        }
    }
    return state;
}

void TicTacToe::setStateString(const std::string &s)
{
    for (int i = 0; i < 9; i++) {
        int pNum = s[i] - '0';
        if (pNum == 0) _grid[i/3][i%3].destroyBit();
        else {
            Bit* bit = PieceForPlayer(pNum - 1);
            bit->setPosition(_grid[i/3][i%3].getPosition());
            _grid[i/3][i%3].setBit(bit);
        }
    }
}

bool TicTacToe::aiTestForTerminalState(const std::string& state)
{
    int wins[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    for (int i = 0; i < 8; i++) {
        char c = state[wins[i][0]];
        if (c != '0' && c == state[wins[i][1]] && c == state[wins[i][2]]) return true;
    }
    for (int i = 0; i < 9; i++) {
        if (state[i] == '0') return false;
    }
    return true;
}

int TicTacToe::aiBoardEval(const std::string& state)
{
    int wins[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    for (int i = 0; i < 8; i++) {
        char c = state[wins[i][0]];
        if (c != '0' && c == state[wins[i][1]] && c == state[wins[i][2]]) {
            return (c == '2') ? 1 : -1;
        }
    }
    return 0;
}

int TicTacToe::negamax(const std::string& state, int depth, int alpha, int beta, int playerColor)
{
    if (aiTestForTerminalState(state)) {
        int eval = aiBoardEval(state);
        return (eval == 0) ? 0 : (eval * playerColor) * (10 - depth);
    }

    int bestValue = -10000;
    char mark = (playerColor == 1) ? '2' : '1';
    
    for (int i = 0; i < 9; i++) {
        if (state[i] == '0') {
            std::string newState = state;
            newState[i] = mark;
            int value = -negamax(newState, depth + 1, -beta, -alpha, -playerColor);
            if (value > bestValue) bestValue = value;
            if (value > alpha) alpha = value;
            if (alpha >= beta) break;
        }
    }
    return bestValue;
}

void TicTacToe::updateAI()
{
    if (_aiMoveMade) return;
    
    std::string currentState = stateString();
    int bestMove = -1;
    int bestValue = -10000;
    
    for (int i = 0; i < 9; i++) {
        if (currentState[i] == '0') {
            std::string newState = currentState;
            newState[i] = '2';
            int value = -negamax(newState, 1, -10000, 10000, -1);
            if (value > bestValue) {
                bestValue = value;
                bestMove = i;
            }
        }
    }
    
    if (bestMove != -1) {
        BitHolder& holder = _grid[bestMove / 3][bestMove % 3];
        if (actionForEmptyHolder(&holder)) {
            _aiMoveMade = true;
            endTurn();
        }
    }
}