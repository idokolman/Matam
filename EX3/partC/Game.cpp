#include "Game.h"
#include <math.h>
#include <vector>

/*=============================================================================*/
/*                                                                             */
/* Game class Implementation:                                                  */
/* --------------------------                                                  */
/* 1. Private functions.                                                       */
/* 2. Constructors, destructor and assignment operator.                        */
/* 3. Characters functions.                                                    */
/* 4. Gameplay functions.                                                      */
/* 5. Printing and game ending.                                                */
/*                                                                             */
/*=============================================================================*/

namespace mtm 
{ 
    //=======================================================================
    // 1. Private functions.
    //=======================================================================

    Dimensions Game::validateDim(int height,int width)
    {
        if (height<=0||width<=0){
            throw IllegalArgument();
        }
        Dimensions result(height,width);
        return result;
    }

    bool Game::checkLimits(const GridPoint& coordinates) const
    {
        bool row_ok=(coordinates.row<board.height())&&(coordinates.row>=0);
        bool col_ok=(coordinates.col<board.width())&&(coordinates.col>=0);
        return (row_ok&&col_ok);
    }

    std::shared_ptr<Character>& Game::getPoint(const GridPoint& coordinates)
    {
        return board(coordinates.row,coordinates.col);
    }

    //=======================================================================
    // 2. Constructors, destructor and assignment operator.
    //=======================================================================

    // checks the dimensions received in the validateDim, initializes the board to only have nullptrs
    Game::Game(int height, int width):
    board(Matrix<std::shared_ptr<Character>>(validateDim(height,width),nullptr)){}

    Game::Game(const Game& other):
    board(Matrix<std::shared_ptr<Character>>(Dimensions(other.board.height(),other.board.width()),nullptr))
    {
        typename Matrix<std::shared_ptr<Character>>::iterator it1=board.begin();
        for (typename Matrix<std::shared_ptr<Character>>::const_iterator it2 = other.board.begin();
        it2 != other.board.end(); it1++,it2++)
        {
            if(*it2!=nullptr){
                *it1=(*it2)->clone();
            }
        }
    }

    Game& Game::operator=(const Game& other)
    {
        Matrix<std::shared_ptr<Character>> temp(Dimensions(other.board.height(),other.board.width()),nullptr);
        typename Matrix<std::shared_ptr<Character>>::iterator it1=temp.begin();
        for (typename Matrix<std::shared_ptr<Character>>::const_iterator it2 = other.board.begin();
        it2 != other.board.end(); it1++,it2++)
        {
            if(*it2!=nullptr){
                *it1=(*it2)->clone();
            }
        }
        board=temp;
        return *this;
    }

    //=======================================================================
    // 3. Characters functions.
    //=======================================================================

    std::shared_ptr<Character> Game::makeCharacter(CharacterType type, Team team,
    units_t health, units_t ammo, units_t range, units_t power)
    {
        if(health<=0||ammo<0||range<0||power<0)
        {
            throw IllegalArgument();
        }
        if (type==SOLDIER)
        {
            std::shared_ptr<Character> new_character(new Soldier(health,ammo,range,power,team));
            return new_character;
        }
        if (type==MEDIC) 
        {
            std::shared_ptr<Character> new_character(new Medic(health,ammo,range,power,team));
            return new_character;
        }
        if (type==SNIPER)
        {
            std::shared_ptr<Character> new_character(new Sniper(health,ammo,range,power,team));
            return new_character;

        }
        return nullptr; // should not reach here
    }

    void Game::addCharacter(const GridPoint& coordinates, std::shared_ptr<Character> character)
    {
        if (!checkLimits(coordinates))
        {
            throw IllegalCell();
        }
        if (board(coordinates.row,coordinates.col)!=nullptr)
        {
            throw CellOccupied();
        }
        board(coordinates.row,coordinates.col)=character; //where we add the character to the desired place
    }

    //=======================================================================
    // 4. Gameplay functions.
    //=======================================================================

    void Game::attack(const GridPoint & src_coordinates, const GridPoint & dst_coordinates)
    {
        if(!(checkLimits(src_coordinates))||!(checkLimits(dst_coordinates))){
            throw IllegalCell();
        }
        if (getPoint(src_coordinates)==nullptr){
            throw CellEmpty();
        }//sends the attack to the character functions, to handle the attack and it's consequences.
        getPoint(src_coordinates)->attack(src_coordinates,dst_coordinates,board); 
    }

    void Game::reload(const GridPoint & coordinates)
    {
        if (!checkLimits(coordinates))
        {
            throw IllegalCell();
        }
        if (getPoint(coordinates)==nullptr)
        {
            throw CellEmpty();
        }
        getPoint(coordinates)->reload();
    }

    void Game::move(const GridPoint & src_coordinates, const GridPoint & dst_coordinates)
    {
        if (!(checkLimits(src_coordinates))||!(checkLimits(dst_coordinates))){
            throw IllegalCell();
        }
        if (getPoint(src_coordinates)==nullptr){
            throw CellEmpty();
        }
        if (GridPoint::distance(src_coordinates, dst_coordinates)>getPoint(src_coordinates)->getSteps()){
            throw MoveTooFar();
        }
        if (getPoint(dst_coordinates)!=nullptr){
            throw CellOccupied();
        }// swaping between two shared_ptr:
        getPoint(src_coordinates).std::shared_ptr<Character>::swap(getPoint(dst_coordinates));
    }

    //=======================================================================
    // 5. Printing and game ending.
    //=======================================================================

    std::ostream& operator<<(std::ostream& os, const Game& game)
    {
        std::vector<char> charBoard(game.board.size(),' ');
        int i=0;
        for (typename Matrix<std::shared_ptr<Character>>::const_iterator it2 = game.board.begin();
        it2 != game.board.end(); i++,it2++)
        {
            if(*it2!=nullptr){
                charBoard[i]=(*it2)->getLetter();
            }
        }// sends the ostream variable, the start of the charBoard, it's end, and the width of the board
        return printGameBoard(os,charBoard.data(),charBoard.data()+game.board.size(),game.board.width());
    }

    bool Game::isOver(Team* winningTeam) const
    {
        bool python_win = false, cpp_win=false;
        for (typename Matrix<std::shared_ptr<Character>>::const_iterator it = board.begin();
        it != board.end(); it++)
        {
            if (*it==nullptr){
                continue;
            }
            if ((*it)->getTeam()==PYTHON){
                python_win=true;
            }
            if ((*it)->getTeam()==CPP){
                cpp_win=true;
            }
        }
        if (cpp_win&&python_win){
                return false;
            }
        if (cpp_win==false&&python_win==false){
            return false;
        }
        if (cpp_win==false&&python_win==true){
            if (winningTeam!=NULL&&winningTeam!=nullptr){
                *winningTeam=PYTHON;
            }
            return true;
        }
        if (winningTeam!=NULL&&winningTeam!=nullptr){ //only option left by this point is cpp winning
                *winningTeam=CPP;
            }
        return true;
    }
}

