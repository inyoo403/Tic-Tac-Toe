#include "Application.h"
#include "imgui/imgui.h"
#include "classes/TicTacToe.h"
#include <sstream>
#include <iomanip>

namespace ClassGame {
        //
        // our global variables
        //
        TicTacToe *game = nullptr;
        bool gameOver = false;
        int gameWinner = -1;
        std::vector<std::string> gameLog;
        int turnCount = 0;
        std::string lastMove = "";
        std::string previousBoardState = "000000000";
        int lastMovedPlayer = -1;

        //
        // game starting point
        // this is called by the main render loop in main.cpp
        //
        void GameStartUp() 
        {
            game = new TicTacToe();
            game->setUpBoard();
            ClearLog();
            turnCount = 0;
            gameOver = false;
            gameWinner = -1;
            previousBoardState = "000000000";
            lastMove = "";
            lastMovedPlayer = -1;
            AddLog("=== Game Started ===");
            AddLog("Player 0 (X) turn.");
        }

        //
        // game render loop
        // this is called by the main render loop in main.cpp
        //
        void RenderGame() 
        {
                ImGui::DockSpaceOverViewport();

                //ImGui::ShowDemoWindow();

                if (!game) return;
                if (!game->getCurrentPlayer()) return;
                
                ImGui::Begin("Game Status");
                
                int currentPlayerNum = game->getCurrentPlayer()->playerNumber();
                const char* playerSymbol = (currentPlayerNum == 0) ? "X" : "O";
                ImGui::Text("Current Player: Player %d (%s)", currentPlayerNum, playerSymbol);
                ImGui::Text("Turn Count: %d", turnCount);
                ImGui::Separator();
                
                ImGui::Text("Board State: %s", game->stateString().c_str());
                
                if (!lastMove.empty()) {
                    ImGui::Text("Last Move: %s", lastMove.c_str());
                }
                
                ImGui::Separator();
                
                if (gameOver) {
                    if (gameWinner == -1) {
                        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Game Over: Draw!");
                    } else {
                        const char* winnerSymbol = (gameWinner == 0) ? "X" : "O";
                        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Game Over: Player %d (%s) Wins!", gameWinner, winnerSymbol);
                    }
                    ImGui::Spacing();
                    if (ImGui::Button("Reset Game", ImVec2(-1.0f, 0.0f))) {
                        ResetGame();
                    }
                } else {
                    if (ImGui::Button("Reset Game", ImVec2(-1.0f, 0.0f))) {
                        ResetGame();
                    }
                }
                
                ImGui::End();

                ImGui::Begin("Game Log");
                
                if (ImGui::Button("Clear Log")) {
                    ClearLog();
                }
                ImGui::SameLine();
                if (ImGui::Button("Copy Log")) {
                    std::string allLogs = "";
                    for (const auto& log : gameLog) {
                        allLogs += log + "\n";
                    }
                    ImGui::SetClipboardText(allLogs.c_str());
                }
                
                ImGui::Separator();
                
                ImGui::BeginChild("LogScroll", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
                
                for (const auto& log : gameLog) {
                    if (log.find("Wins") != std::string::npos || log.find("Wins!") != std::string::npos) {
                        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s", log.c_str());
                    } else if (log.find("Draw") != std::string::npos) {
                        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", log.c_str());
                    } else if (log.find("Player") != std::string::npos && log.find("placed") != std::string::npos) {
                        ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "%s", log.c_str());
                    } else {
                        ImGui::Text("%s", log.c_str());
                    }
                }
                
                if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 1.0f) {
                    ImGui::SetScrollHereY(1.0f);
                }
                
                ImGui::EndChild();
                ImGui::End();

                ImGui::Begin("GameWindow");
                game->drawFrame();
                ImGui::End();
        }

        //
        // end turn is called by the game code at the end of each turn
        // this is where we check for a winner
        //
        void EndOfTurn() 
        {
            if (gameOver) return;
            
            std::string currentBoardState = game->stateString();
            
            int movePosition = -1;
            
            for (int i = 0; i < 9; i++) {
                if (previousBoardState[i] == '0' && currentBoardState[i] != '0') {
                    movePosition = i;
                    break;
                }
            }
            
            if (movePosition != -1 && lastMovedPlayer != -1) {
                int row = movePosition / 3;
                int col = movePosition % 3;
                const char* playerSymbol = (lastMovedPlayer == 0) ? "X" : "O";
                std::stringstream moveLog;
                moveLog << "Player " << lastMovedPlayer << " (" << playerSymbol << ") placed at (" 
                        << row << ", " << col << ").";
                lastMove = moveLog.str();
                AddLog(lastMove);
            }
            
            turnCount++;
            previousBoardState = currentBoardState;
            lastMovedPlayer = -1;
            
            Player *winner = game->checkForWinner();
            if (winner)
            {
                gameOver = true;
                gameWinner = winner->playerNumber();
                const char* winnerSymbol = (gameWinner == 0) ? "X" : "O";
                std::stringstream ss;
                ss << "[Turn " << turnCount << "] Player " << gameWinner << " (" << winnerSymbol << ") Wins!";
                AddLog(ss.str());
                AddLog("=== Game Over ===");
            }
            else if (game->checkForDraw()) {
                gameOver = true;
                gameWinner = -1;
                std::stringstream ss;
                ss << "[Turn " << turnCount << "] Draw! Board is full.";
                AddLog(ss.str());
                AddLog("=== Game Over ===");
            }
            else {
                int nextPlayerNum = game->getCurrentPlayer()->playerNumber();
                const char* nextSymbol = (nextPlayerNum == 0) ? "X" : "O";
                std::stringstream ss;
                ss << "Player " << nextPlayerNum << " (" << nextSymbol << ") turn.";
                AddLog(ss.str());
            }
        }
        
        void AddLog(const std::string& message)
        {
            std::stringstream ss;
            ss << "[" << std::setfill('0') << std::setw(3) << turnCount << "] " << message;
            gameLog.push_back(ss.str());
            
            if (gameLog.size() > 100) {
                gameLog.erase(gameLog.begin());
            }
        }
        
        void ClearLog()
        {
            gameLog.clear();
        }
        
        void ResetGame()
        {
            if (game) {
                game->stopGame();
                game->setUpBoard();
            }
            gameOver = false;
            gameWinner = -1;
            turnCount = 0;
            lastMove = "";
            previousBoardState = "000000000";
            lastMovedPlayer = -1;
            ClearLog();
            AddLog("=== Game Started ===");
            AddLog("Player 0 (X) turn.");
        }
}
