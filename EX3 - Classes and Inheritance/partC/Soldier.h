#ifndef SOLDIER_H
#define SOLDIER_H
#include "Auxiliaries.h"
#include <string>
#include <iostream>
#include "Character.h"
#include <memory>

/*=============================================================================*/
/* Soldier class                                                               */
/* --------------                                                              */
/* This interface exports the Soldier class, which is derived by Character     */
/*                                                                             */
/*                                                                             */
/* Declaration Contents:                                                       */
/* ---------------------                                                       */
/* 1. Private attributes.                                                      */
/* 2. Constructor                                                              */
/* 3. Overridden functions.                                                    */
/*                                                                             */
/*=============================================================================*/


namespace mtm { 
    class Soldier : public Character {
    private:

    //=======================================================================
    // 1. Private attributes. 
    //=======================================================================

        static const units_t STEPS = 3;
        static const units_t RELOAD_NUMBER = 3;
        static const char LETTER = 's';

    public:


    //=======================================================================
    // 2. Constructor. 
    //=======================================================================
    
        Soldier(units_t health, units_t ammo,units_t range, units_t power, Team team);

    //=======================================================================
    // 3. Overridden functions.. 
    //=======================================================================
    
        std::shared_ptr<Character> clone() const override;
        void reload() override;
        const units_t getSteps() const override;
        const char getLetter() const override;
        /**
        * Soldier::attack: Performs a custom attack of a soldier:
        * An attack will be carried out for a maximum distance of "range" at a straight line.
        * The attack can be on any target - opponent, teammate or empty spot.
        * A rival at the target point is damaged by a rate of power.
        * Rivals at upper integer value of range/3 distance have damage of power/2 upper integer value.
        * At the end of attack - the characters that were damaged have less of HP (by the power the attck)
        * if the HP is 0 - they will be removed from the board, ammo is reduced from the attacker
        * @param src_coordinates - coordinates of the attacker
        * @param dst_coordinates - coordinates of the target
        * @param board - the board in which the attack was made.
        * @return
        * 	none. 
        */
        void attack(const GridPoint & src_coordinates, 
        const GridPoint & dst_coordinates,Matrix<std::shared_ptr<Character>>& board) override;
    };
}
#endif //SOLDIER_H
