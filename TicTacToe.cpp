// TicTacToe.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>

enum class CellState
{
    Empty,
    X,
    O
};

std::string getCellText(CellState state)
{
    if (state == CellState::Empty) return ".";
    else if (state == CellState::X) return "X";
    else return "O";
}

void draw(const CellState (&board)[3][3])
{
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

int main()
{
    
    CellState board[3][3];

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board[i][j] = CellState::Empty;
        }
    }
    board[1][1] = CellState::X;
    board[0][2] = CellState::O;

    draw(board);
}