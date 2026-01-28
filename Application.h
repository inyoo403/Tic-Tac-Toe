#pragma once
#include <vector>
#include <string>

namespace ClassGame {
    void GameStartUp();
    void RenderGame();
    void EndOfTurn();
    void AddLog(const std::string& message);
    void ClearLog();
    void ResetGame();
    
    extern int lastMovedPlayer;
}
