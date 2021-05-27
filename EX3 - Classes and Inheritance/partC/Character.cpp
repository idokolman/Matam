#include "Character.h"

namespace mtm { 
    Character::Character(units_t health, units_t ammo,units_t range, units_t power, Team team):
    health(health), ammo(ammo), range(range), power(power), team(team){}

    void Character::changeHP(units_t health_point_change)
    {
        health-=health_point_change;
    }

    void Character::reduceAmmo()
    {
        ammo--;
    }
    
    std::shared_ptr<Character>& Character::getPoint(const GridPoint& coordinates,
        Matrix<std::shared_ptr<Character>>& board)
    {
        return board(coordinates.row,coordinates.col);
    }

    const Team Character::getTeam() const{
    return team;
    }

    const units_t Character::getHealth() const{
    return health;
    }

}

