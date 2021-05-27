#include "Medic.h"

namespace mtm { 
Medic::Medic(units_t health, units_t ammo,units_t range, units_t power, Team team) : Character(health,ammo,range,power,team){}
std::shared_ptr<Character> Medic::clone() const 
{
    return std::shared_ptr<Character>(new Medic(health,ammo,range,power,team));
}
void Medic::reload()
{
    ammo+=RELOAD_NUMBER;
}

const units_t Medic::getSteps() const{
    return STEPS;
}

const char Medic::getLetter() const{
    if (team==CPP){
        return toupper(LETTER);
    }
    return LETTER;
}

void Medic::attack(const GridPoint & src_coordinates, 
    const GridPoint & dst_coordinates,Matrix<std::shared_ptr<Character>>& board)
    {
        if(GridPoint::distance(src_coordinates,dst_coordinates)>range){
            throw OutOfRange();
        }
        std::shared_ptr<Character> target(getPoint(dst_coordinates,board));
        if((src_coordinates==dst_coordinates)||(target==nullptr)){
            throw IllegalTarget();
        }
        if ((team!=target->getTeam())&&(ammo==0)){
            throw OutOfAmmo();
        }
        // give HP to team member
        if (team==target->getTeam()){
            target->changeHP(-power);
        }
        else // attack other team
        {
            target->changeHP(power);
            if (target->getHealth()<=0){
                getPoint(dst_coordinates,board)=nullptr;    
            }
            reduceAmmo(); // reduce ammo only on actual attack       
        }
    }
}