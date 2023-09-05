#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>

enum class GameState
{
    Menu,
    BotMove,
    PlayerMove
};
enum class CellState
{
    Empty,
    X,
    O
};

bool GameRunning{ false };
int score{};

GameState gameState{ GameState::Menu };
CellState player{CellState::X};
CellState bot{CellState::O};

int generateRandomNumber(int minRange, int maxRange) {
    // Generate a random number within the given range
    return minRange + std::rand() % (maxRange - minRange + 1);
}

void setPlayerCharacter()
{
    int input{};
    while (true)
    {
        std::cout << "\nWhat character would you like to play for?" << std::endl;
        std::cout << "   1. X\n   2. O" << std::endl;
        std::cin >> input;

        if (input == 1 || input == 2) break;
            
        std::cout << "Please, enter valid input." << std::endl;
    }
    if (input == 1)
    {
        player = CellState::X;
        bot = CellState::O;
        // X plays first
        gameState = GameState::PlayerMove;
    }
    else
    {
        player = CellState::O;
        bot = CellState::X;
        gameState = GameState::BotMove;
    }
}


void initGame(CellState (&board)[3][3], std::vector<unsigned>& availableCells)
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board[i][j] = CellState::Empty;
        }
    }
    availableCells.assign({ 1, 2, 3, 4, 5, 6, 7, 8, 9 });
    setPlayerCharacter();

    GameRunning = true;
}

void makeMove(CellState(&board)[3][3], std::vector<unsigned>& availableCells, int cell, CellState& state)
{
    int row{ (cell - 1) / 3 };
    int col{ (cell - 1) % 3 };

    board[row][col] = state;

    // remove from available cells
    size_t i{};
    for (; i < availableCells.size(); ++i)
    {
        if (availableCells[i] == cell) break;
    }
    availableCells.erase(availableCells.begin() + i);
}

void playerMove(CellState(&board)[3][3], std::vector<unsigned>& availableCells)
{
    int input{};
    while (true)
    {
        std::cout << "\nEnter the cell number, to make a move: ";
        std::cin >> input;

        if (input < 1 || input > 9)
            std::cout << "Please, enter valid input." << std::endl;
        else if (std::count(availableCells.begin(), availableCells.end(), input))
            break;
        else
            std::cout << "This cell is not available!" << std::endl;
    }

    makeMove(board, availableCells, input, player);
}


void botMove(CellState(&board)[3][3], std::vector<unsigned>& availableCells)
{
    int move{ generateRandomNumber(0, availableCells.size() - 1) };

    makeMove(board, availableCells, static_cast<int>(availableCells[move]), bot);
}

char getCellText(CellState state)
{
    if (state == CellState::Empty) return '.';
    else if (state == CellState::X) return 'X';
    else return 'O';
}

void draw(const CellState (&board)[3][3])
{
    system("cls");

    const int dimensions{ 12 };

    for (int i{}, k{}; i <= dimensions; ++i)
    {
        std::cout << "\t";
        if (i == 0 || i == 4 || i == 8 || i == 12)
        {
            for (int j{}; j <= dimensions; ++j)
            {
                std::cout << "-";
            }
            std::cout << std::endl;
        }
        else if (i == 2 || i == 6 || i == 10)
        {
            std::cout << "| " << getCellText(board[k][0]) << " | " << getCellText(board[k][1]) << " | " << getCellText(board[k][2]) << " |" << std::endl;
            k++;
        }
        else
        {
            int bar{ dimensions / 3 };
            for (int j{}; j <= dimensions; ++j)
            {
                if (j % bar == 0) std::cout << "|";
                else std::cout << " ";
            }
            std::cout << std::endl;
        }
    }
}

void displayResults(CellState winner)
{
    std::cout << std::endl;
    if (winner == player)
    {
        // player has won
        score += 10;
        std::cout << "\tCongratulations! You have WON! Your score is " << score << std::endl;
    }
    else if (winner == bot)
    {
        // the bot has won
        std::cout << "\tYou have LOST! Your score is " << score << std::endl;
    }
    else
    {
        // draw
        std::cout << "\tThe game is a DRAW! Your score is " << score << std::endl;
    }
}

void update(CellState(&board)[3][3], std::vector<unsigned>& availableCells)
{
    // Check if anybody has won the game
    const int n = 3;

    CellState winner{ CellState::Empty };

    // Check rows and columns if they contain the same symbol sequence
    for (int i{}; i < n; ++i)
    {
        CellState temp_hor = board[i][0];
        CellState temp_ver = board[0][i];
        if (temp_hor != CellState::Empty && temp_hor == board[i][1] && temp_hor == board[i][2])
        {
            winner = temp_hor;
            break;
        }
        if (temp_ver != CellState::Empty && temp_ver == board[1][i] && temp_ver == board[2][i])
        {
            winner = temp_ver;
            break;
        }
    }
    // If not found, then check diagonally
    if (winner == CellState::Empty)
    {
        CellState temp = board[1][1];
        if (temp != CellState::Empty && 
            ((temp == board[0][2] && temp == board[2][0]) || (temp == board[0][0] && temp == board[2][2])))
        {
            winner = temp;
        }
    }

    if (winner != CellState::Empty)
    {
        displayResults(winner);
        GameRunning = false;
    }
    else if (availableCells.size() == 0)
    {
        displayResults(CellState::Empty);
        GameRunning = false;
    }
}

int main()
{
    // Set the seed number for srand
    std::srand(static_cast<unsigned>(std::time(NULL)));
    
    CellState board[3][3];
    std::vector<unsigned> availableCells;

    while (true)
    {
        initGame(board, availableCells);
        draw(board);
        
        while (GameRunning)
        {
            if (gameState == GameState::BotMove)
            {    
                botMove(board, availableCells);
                gameState = GameState::PlayerMove;
            }
            else if (gameState == GameState::PlayerMove)
            {
                playerMove(board, availableCells);
                gameState = GameState::BotMove;
            }

            draw(board);
            update(board, availableCells);
        }

        char input;

        std::cout << "\nEnter 'q' to quit or any other symbol to play again." << std::endl;
        std::cin >> input;
        if (std::tolower(input) == 'q') return 0;
    }
}