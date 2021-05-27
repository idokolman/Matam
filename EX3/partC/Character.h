#ifndef CHARACTER_H
#define CHARACTER_H
#include "Auxiliaries.h"
#include "Matrix.h"
#include "Exceptions.h"
#include <string>
#include <iostream>
#include <cmath>
#include <memory>

/*=============================================================================*/
/* Character class                                                             */
/* --------------                                                              */
/* This interface exports the Character class, which supports the Game class.  */
/* The Character class is a polymorphic class,                                 */
/* and it's childs are: Soldier, Medic and Sniper.                               */
/*                                                                             */
/*                                                                             */
/* Declaration Contents:                                                       */
/* ---------------------                                                       */
/* 1. Private attributes.                                                      */
/* 2. Private functions.                                                       */
/* 3. Constructor and destructor.                                              */
/* 4. Pure virtual functions.                                                  */
/* 5. Non-virtual functions.                                                   */
/*                                                                             */
/*=============================================================================*/

namespace mtm { 
    class Character {
    protected :

        //=======================================================================
        // 1. Private attributes. 
        //=======================================================================

        units_t health, ammo, range, power;
        Team team;

        //=======================================================================
        // 2. Private functions.
        //=======================================================================

        /**
            * Character::getPoint: returns the shared_ptr in the place on the board matching the coordinates
            * @param coordinates - The row and columns to check.
            * @param board - the board in which we want the shared_ptr.
            * @return
            * 	shared_ptr to the desired place in the game. 
            */
        static std::shared_ptr<Character>& getPoint(const GridPoint& coordinates,
        Matrix<std::shared_ptr<Character>>& board);

    public:

        //=======================================================================
        // 3. Constructor and destructor.
        //=======================================================================

        // Constructor for character, only 5 parameters constrcutor exists
        Character(units_t health, units_t ammo,units_t range, units_t power, Team Team);

        // Destructor is by default so no attribute is allocated
        ~Character() = default;

        //=======================================================================
        // 4. Pure virtual functions.
        //=======================================================================

        // Returns a pointer to a copy of "this". 
        virtual std::shared_ptr<Character> clone() const = 0;

        // Adds ammo to the character, amount is different for each child class 
        virtual void reload() = 0;

        // Returns the number of steps the character can do
        virtual const units_t getSteps() const = 0;

        // Returns the letter the defines the character, letter is different for each child class and each team
        virtual const char getLetter() const = 0;

        /**
            * Character::attack: deals damage(power) from the attacker to the target, changes for each child class.
            * @param src_coordinates - The coordinates of the attacker.
            * @param dst_coordinates - The coordinates of the target.
            * @param board - The board of the game in which we want the attack to happen.
            * @return
            * 	none. 
            */
        virtual void attack(const GridPoint & src_coordinates,
        const GridPoint & dst_coordinates, Matrix<std::shared_ptr<Character>>& board) = 0;

        //=======================================================================
        // 5. Non-virtual functions.
        //=======================================================================

        // Returns the team of the character.
        const Team getTeam() const;

        // Returns the health of the character.
        const units_t getHealth() const;

        // Reduced the amount of ammo attribute by 1.
        void reduceAmmo();

        // Decreases the health of the character by the amount received.
        void changeHP(units_t health_point_change);
    };
}

#endif //CHARACTER_H
