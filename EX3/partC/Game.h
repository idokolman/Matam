#ifndef GAME_H
#define GAME_H
#include "Auxiliaries.h"
#include "Matrix.h"
#include <iostream>
#include <memory>
#include "Character.h"
#include "Soldier.h"
#include "Sniper.h"
#include "Medic.h"

/*=============================================================================*/
/* Game class                                                                  */
/* --------------                                                              */
/* This interface exports the Game class, which provides an easy, simple,      */
/* and convenient way for the staff of the "MATAM" course to settle their      */
/* argument about which programming language is better, C++ or Python.         */
/* The Game class is supported by the Character class.                         */
/* The available characters in the game are: Soldier, Medic and Sniper.        */
/*                                                                             */
/*                                                                             */
/* Declaration Contents:                                                       */
/* ---------------------                                                       */
/* 1. Private attributes.                                                      */
/* 2. Private functions.                                                       */
/* 3. Constructors, destructor and assignment operator.                        */
/* 4. Characters functions.                                                    */
/* 5. Gameplay functions.                                                      */
/* 6. Printing and game ending.                                                */
/*                                                                             */
/*=============================================================================*/

namespace mtm { 
    class Game {
        private: 

            //=======================================================================
            // 1. Private attributes. 
            //=======================================================================

            Matrix<std::shared_ptr<Character>> board;

            //=======================================================================
            // 2. Private functions.
            //=======================================================================

            /**
            * Game::checkLimits: Check if given coordinates are in boundaries.
            * @param coordinates - The row and columns to check.
            * @return
            * 	true - if in bounds.
            *   else - if out of bounds. 
            */
            bool checkLimits(const GridPoint& coordinates) const;

            /**
            * Game::getPoint: returns the shared_ptr in the place on the board matching the coordinates
            * @param coordinates - The row and columns to check.
            * @return
            * 	shared_ptr to the desired place in the game. 
            */
            std::shared_ptr<Character>& getPoint(const GridPoint& coordinates);

            /**
            * Game::validateDim: Check if given height and width are positive.
            * @param height - The number of rows in the board.
            * @param width - The number of columns in the board.
            * @return
            * 	Dimensions of the desired size.
            */
            static Dimensions validateDim(int height,int width);

        public:

            //=======================================================================
            // 3. Constructors, destructor and assignment operator.
            //=======================================================================

            // Constrcutor that receives 2 paramets, which define the size of the game board.
            Game(int height, int width);
            // Copy Constructor that does deep copy of the other Game.  
            Game(const Game& other);
            // The attribute of game is board, and is freed by default since board is a matrix.
            ~Game()=default;
            // Assignment operator that does deep copy of the other game.
            Game& operator=(const Game& other); 

            //=======================================================================
            // 4. Characters functions.
            //=======================================================================

            /**
            * Game::addCharacter: Adds a chacracter to the game.
            * @param coordinates - The coordiantes of where we want to add the character to.
            * @param character - The character we want to add to the game board.
            * @return
            * 	none.
            */
            void addCharacter(const GridPoint& coordinates, std::shared_ptr<Character> character);

            /**
            * Game::makeCharacter: Creates a new chacracter for the game.
            * @param type - The type of the new character.
            * @param team - The team of the new character.
            * @param health - The health of the new character.
            * @param ammo - The ammo of the new character.
            * @param range - The range of the new character.
            * @param power - The power of the new character.
            * @return
            * 	A shared_ptr to the newly created character.
            */
            static std::shared_ptr<Character> makeCharacter(CharacterType type, Team team,
            units_t health, units_t ammo, units_t range, units_t power); 

            //=======================================================================
            // 5. Gameplay functions.
            //=======================================================================

            /**
            * Game::move: Moves a chacracter from one place to another on the game board.
            * @param src_coordinates - The coordiantes of character we want to move.
            * @param dst_coordinates - The coordiantes of the place we want to move the character to.
            * @return
            * 	none.
            */
            void move(const GridPoint & src_coordinates, const GridPoint & dst_coordinates); 

            /**
            * Game::attack: Makes the chacracter from the desired place attack the target place.
            * @param src_coordinates - The coordiantes of the character that attacks.
            * @param dst_coordinates - The coordiantes of the place we want the character to attack.
            * @return
            * 	none.
            */
            void attack(const GridPoint & src_coordinates, const GridPoint & dst_coordinates);

            /**
            * Game::reload: Makes the chacracter from the desired place to reload and add ammo to itself.
            * @param coordinates - The coordiantes of the character that reload.
            * @return
            * 	none.
            */
            void reload(const GridPoint & coordinates); 

            //=======================================================================
            // 6. Printing and game ending.
            //=======================================================================

            /**
            * Game::operator<<: Prints the current state of the board. 
            * @param os - The ostream variable where we print to
            * @param game - the game whose current state we want to print.
            * @return
            * 	prints the current state of the board, ' ' for empty spot,
            *   CPP team: 'S' for soldier, 'M' for medic, 'N' for sniper.
            *   PYTHON team: 's' for soldier, 'm' for medic, 'n' for sniper.
            */
            friend std::ostream& operator<<(std::ostream& os, const Game& game);

            /**
            * Game::isOver: Checks if there is a winner, and returns the winning team if wanted.
            * @param winningTeam - The pointer where we save the winning team if the user wants us to,
            *                      and only saves there if there is a winning team and pointer is not NULL.
            * @return
            * 	true - if there is only one team on the board.
            *   false - if there are 0 or 2 teams on the board.
            */
            bool isOver(Team* winningTeam=NULL) const; 
    };
    std::ostream& operator<<(std::ostream& os, const Game& game);
}
#endif //GAME_H
