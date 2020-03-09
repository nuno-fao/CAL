/*
 * Sudoku.cpp
 *
 */

#include "Sudoku.h"

/** Inicia um Sudoku vazio.
 */
Sudoku::Sudoku()
{
	this->initialize();
}

/**
 * Inicia um Sudoku com um conteúdo inicial.
 * Lança excepção IllegalArgumentException se os valores
 * estiverem fora da gama de 1 a 9 ou se existirem números repetidos
 * por linha, coluna ou bloc 3x3.
 *
 * @param nums matriz com os valores iniciais (0 significa por preencher)
 */
Sudoku::Sudoku(int nums[9][9])
{
	this->initialize();

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (nums[i][j] != 0)
			{
				int n = nums[i][j];
				numbers[i][j] = n;
				lineHasNumber[i][n] = true;
				columnHasNumber[j][n] = true;
				block3x3HasNumber[i / 3][j / 3][n] = true;
				countFilled++;
			}
		}
	}
}

void Sudoku::initialize()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			for (int n = 0; n < 10; n++)
			{
				numbers[i][j] = 0;
				lineHasNumber[i][n] = false;
				columnHasNumber[j][n] = false;
				block3x3HasNumber[i / 3][j / 3][n] = false;
			}
		}
	}

	this->countFilled = 0;
}

/**
 * Obtem o conteúdo actual (só para leitura!).
 */
int** Sudoku::getNumbers()
{
	int** ret = new int*[9];

	for (int i = 0; i < 9; i++)
	{
		ret[i] = new int[9];

		for (int a = 0; a < 9; a++)
			ret[i][a] = numbers[i][a];
	}

	return ret;
}

/**
 * Verifica se o Sudoku já está completamente resolvido
 */
bool Sudoku::isComplete()
{
	return countFilled == 9 * 9;
}



/**
 * Resolve o Sudoku.
 * Retorna indicação de sucesso ou insucesso (sudoku impossível).
 */
bool Sudoku::solve()
{
	int row, col;
	if(isComplete()){
	    return true;
	}
	if(!pickCell(&row,&col)){
	    return false;
	}
	for(int num = 1;num<=9;num++){
        if(isValid(num,row,col)){
            numbers[row][col]=num;
            print();
            columnHasNumber[col][num]=true;
            lineHasNumber[row][num]=true;
            block3x3HasNumber[row][col][num]=true;
            if(!solve()){
                numbers[row][col]=0;
                columnHasNumber[col][num]=false;
                lineHasNumber[row][num]=false;
                block3x3HasNumber[row][col][num]=false;
            }
        }
	}
	return true;
}



/**
 * Imprime o Sudoku.
 */
void Sudoku::print()
{
	for (int i = 0; i < 9; i++)
	{
		for (int a = 0; a < 9; a++)
			cout << this->numbers[i][a] << " ";

		cout << endl;
	}
}

bool Sudoku::isValid(int n, int row, int col) {
    if(numbers[row][col]!=0 || lineHasNumber[row][n] || columnHasNumber[n][col] || block3x3HasNumber[row/3][col/3][n]){
        return false;
    }
    return true;
}

bool Sudoku::pickCell(int *row, int *col) {
    int auxrow, auxcol, min=10, auxtotal;

    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++) {
            if (numbers[i][j] == 0) {
                auxtotal = minValues(i, j);
                if (auxtotal == 0) {
                    return false;
                }
                if(auxtotal<min){
                    min=auxtotal;
                    auxrow=i;
                    auxcol=j;
                }
            }
        }
    }
    *row=auxrow;
    *col=auxcol;
    return true;
}

int Sudoku::minValues(int row, int col) {
    int out=9;
    for(int n=1;n<10;n++){
        if(columnHasNumber[row][n] || lineHasNumber[col][n] || block3x3HasNumber[row/3][col/3][n]){
            out-=1;
        }
    }
    return out;
}
