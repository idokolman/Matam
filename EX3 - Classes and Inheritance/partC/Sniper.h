#ifndef SNIPER_H
#define SNIPER_H
#include "Auxiliaries.h"
#include <string>
#include <iostream>
#include "Character.h"
#include <memory>

/*=============================================================================*/
/* Sniper class                                                                */
/* --------------                                                              */
/* This interface exports the Sniper class, which is derived by Character      */
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
    class Sniper : public Character {
    private:

    //=======================================================================
    // 1. Private attributes. 
    //=======================================================================

        units_t counter_triple;
        static const units_t STEPS = 4;
        static const units_t RELOAD_NUMBER = 2;
        static const char LETTER = 'n';
    public:

    //=======================================================================
    // 2. Constructor. 
    //=======================================================================
    
        Sniper(units_t health, units_t ammo,units_t range, units_t power, Team team);

    //=======================================================================
    // 3. Overridden functions.. 
    //=======================================================================
    
        virtual std::shared_ptr<Character> clone() const override;
        virtual void reload() override;
        const units_t getSteps() const override;
        const char getLetter() const override;
        /**
        * Sniper::attack: Performs a custom attack of a Sniper:
        * An attack will be carried out for distance of "range"/2 upper integer value to "range" at any path.
        * The attack can be on opponent only.
        * A rival at the target point is damaged by a rate of power and ammo is reduced from the attacker
        * for every 3rd attack of a sniperm, the power of the attack is 3 times harder.
        * At the end of attack - the character that was damaged have less of HP (by the power the attck)
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
#endif //SNIPER_H
