#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <stdexcept> 


//=======================================================================
// Exception class for part C, derived from std::exception
//=======================================================================


namespace mtm { 
    class Exception : public std::exception{};
    class GameException : public Exception{};
    
    class IllegalArgument : public GameException {
        public:
        const char* what() const noexcept override{ 
            return "A game related error has occurred: IllegalArgument";
        }
    };
    class IllegalCell : public GameException {
        public:
        const char* what() const noexcept override{
            return "A game related error has occurred: IllegalCell";
        }   
    };
    class CellEmpty : public GameException {
        public:
        const char* what() const noexcept override{
            return "A game related error has occurred: CellEmpty";
        }  
    };
    class MoveTooFar : public GameException {
        public:
        const char* what() const noexcept override{
            return "A game related error has occurred: MoveTooFar";
        } 
    };
    class CellOccupied : public GameException {
        public:
        const char* what() const noexcept override{
            return "A game related error has occurred: CellOccupied";
        }  
    };
    class OutOfRange : public GameException {
        public:
        const char* what() const noexcept override{
            return "A game related error has occurred: OutOfRange";
        }  
    };
    class OutOfAmmo : public GameException {
        public:
        const char* what() const noexcept override{
            return "A game related error has occurred: OutOfAmmo";
        }  
    };
    class IllegalTarget : public GameException {
        public:
        const char* what() const noexcept override{
            return "A game related error has occurred: IllegalTarget";
        }  
    };
}
#endif //EXCEPTIONS_H
