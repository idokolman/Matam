#include <stdexcept> 
#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

class Exception : public std::exception{};
class InvalidInput : public Exception{
    public:
    const char* what() const noexcept{ 
        return "Error: entered an illegal input";
    }
};

class IllegalName: public Exception{
    public:
    const char* what() const noexcept{ 
        return "Error: entered an illegal name";
    }
};

class IllegalArgument:public Exception{
    public:
    const char* what() const noexcept{ 
        return "Error: Illegal number of arguments";
    }
};
class NoFile:public Exception{
    public:
    const char* what() const noexcept{ 
        return "Error: can't open file";
    }
};

class LoadProblem:public Exception{
    public:
    const char* what() const noexcept{ 
        return "Error: couldn't load from file";
    }
};

class SaveProblem:public Exception{
    public:
    const char* what() const noexcept{ 
        return "Error: couldn't save to file";
    }
};

class AlreadyExist: public Exception{
    public:
    const char* what() const noexcept{ 
        return "Error: it already exists";
    }
};

class InvalidEdge: public Exception{
    public:
    const char* what() const noexcept{ 
        return "Error: entered an illegal edge";
    }
};

class InvalidVertex: public Exception{
    public:
    const char* what() const noexcept{ 
        return "Error: entered an illegal vertex";
    }
};

class InvalidGraph: public Exception{
    public:
    const char* what() const noexcept{ 
        return "Error: entered an illegal graph";
    }
};

#endif