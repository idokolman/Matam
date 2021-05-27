#include "Sniper.h"

namespace mtm { 
Sniper::Sniper(units_t health, units_t ammo,units_t range, units_t power, Team team)
: Character(health,ammo,range,power,team), counter_triple(1){}

std::shared_ptr<Character> Sniper::clone() const 
{
    return std::shared_ptr<Character>(new Sniper(health,ammo,range,power,team));
}

void Sniper::reload()
{
    ammo+=RELOAD_NUMBER;
}

const units_t Sniper::getSteps() const{
    return STEPS;
}

const char Sniper::getLetter() const{
    if (team==CPP){
        return toupper(LETTER);
    }
    return LETTER;
}

void Sniper::attack(const GridPoint & src_coordinates, 
    const GridPoint & dst_coordinates,Matrix<std::shared_ptr<Character>>& board)
    {
        int distance=GridPoint::distance(src_coordinates,dst_coordinates);
        if((distance>range)||(distance<(ceil(range/2)))){
            throw OutOfRange();
        }
        std::shared_ptr<Character> target(getPoint(dst_coordinates,board));
        if((target==nullptr)||(team==target->getTeam())){
            throw IllegalTarget();
        }
        if (ammo==0){
            throw OutOfAmmo();
        }
        // 3rd attack 
        if (counter_triple==3){
            target->changeHP(power*2);
            if (target->getHealth()<=0){
                getPoint(dst_coordinates,board)=nullptr;   
            }
            counter_triple=1;
        }
        else // non 3rd attack
        {
            target->changeHP(power);
            if (target->getHealth()<=0){
                getPoint(dst_coordinates,board)=nullptr;   
            }
            counter_triple++;
        }
        reduceAmmo();      
    }
}