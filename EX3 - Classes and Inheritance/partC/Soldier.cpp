#include "Soldier.h"

namespace mtm { 
    Soldier::Soldier(units_t health, units_t ammo, units_t range, units_t power, Team team) : Character(health,ammo,range,power,team){}
    std::shared_ptr<Character> Soldier::clone() const 
    {
        return std::shared_ptr<Character>(new Soldier(health,ammo,range,power,team));
    }
    void Soldier::reload()
    {
        ammo+=RELOAD_NUMBER;
    }

    const units_t Soldier::getSteps() const{
        return STEPS;
    }

    const char Soldier::getLetter() const{
        if (team==CPP){
            return toupper(LETTER);
        }
        return LETTER;
    }

    void Soldier::attack(const GridPoint & src_coordinates, 
    const GridPoint & dst_coordinates,Matrix<std::shared_ptr<Character>>& board)
    {
        if(GridPoint::distance(src_coordinates,dst_coordinates)>range){
            throw OutOfRange();
        }
        if (ammo==0){
            throw OutOfAmmo();
        }
        if((src_coordinates.col!=dst_coordinates.col)&&(src_coordinates.row!=dst_coordinates.row)){
            throw IllegalTarget(); 
        }
        //effects on the target
        std::shared_ptr<Character> target(getPoint(dst_coordinates,board));
        if((target!=nullptr) && (team!=target->getTeam()))
        {
            target->changeHP(power);
            if (target->getHealth()<=0){
                getPoint(dst_coordinates,board)=nullptr;    
            }
        }
        //effects on the target surroundings (other team only)
        for (int i=0; i<board.height(); i++)
        {
            for (int j=0; j<board.width(); j++)
            {
                if((board(i,j)!=nullptr) && (team!=board(i,j)->getTeam()))
                {
                    GridPoint temp_point(i,j);
                    int distance=GridPoint::distance(temp_point,dst_coordinates);
                    if (distance!=0 && distance<=(ceil((double)range/3))){
                        std::shared_ptr<Character> target2(getPoint(temp_point,board));
                        board(i,j)->changeHP(ceil((double)power/2));
                        if (board(i,j)->getHealth()<=0){
                            std::shared_ptr<Character> target2(getPoint(temp_point,board));
                            board(i,j)=nullptr;   
                        }
                    }
                }
            }
        }
        reduceAmmo();        
    }
    
}