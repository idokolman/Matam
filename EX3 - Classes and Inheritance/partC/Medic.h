#ifndef MEDIC_H
#define MEDIC_H
#include "Auxiliaries.h"
#include <string>
#include <iostream>
#include "Character.h"
#include <memory>


/*=============================================================================*/
/* Medic class                                                                 */
/* --------------                                                              */
/* This interface exports the Medic class, which is derived by Character       */
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
    class Medic : public Character {
    private:

    //=======================================================================
    // 1. Private attributes. 
    //=======================================================================

        static const units_t STEPS = 5;
        static const units_t RELOAD_NUMBER = 5;
        static const char LETTER = 'm';
    public:

    //=======================================================================
    // 2. Constructor. 
    //=======================================================================
    
        Medic(units_t health, units_t ammo,units_t range, units_t power, Team team);

    //=======================================================================
    // 3. Overridden functions.. 
    //=======================================================================
    
        virtual std::shared_ptr<Character> clone() const override;
        virtual void reload() override;
        const units_t getSteps() const override;
        const char getLetter() const override;
        /**
        * Medic::attack: Performs a custom attack of a Medic:
        * An attack will be carried out for a maximum distance of "range" at any path.
        * The attack can be on non empty any target - opponent/teammate, but not itself.
        * A rival at the target point is damaged by a rate of power and ammo is reduced from the attacker
        * A teammate at the target point is givven HP by a rate of power.
        * At the end of attack - the character that was damaged have less/more of HP (by the power the attck)
        * if the HP is 0 - they will be removed from the board.  
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

#endif //MEDIC_H
