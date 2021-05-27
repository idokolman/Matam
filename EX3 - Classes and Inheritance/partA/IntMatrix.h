#ifndef EX3_INTMATRIX_H
#define EX3_INTMATRIX_H
#include "Auxiliaries.h"
#include <string>
#include <iostream>

/*=============================================================================*/
/* IntMatrix class                                                             */
/* --------------                                                              */
/* This interface exports the IntMatrix class, which provides an efficient,    */
/* safe, convenient use for a two dimensional array of ints in C++.            */
/*                                                                             */
/* Contents:                                                                   */
/* 1. Private attributes                                                       */
/* 2. Constructors, destructor and assignment operator.                        */
/* 3. Matrix info                                                              */
/* 4. Member operators.                                                        */
/* 5. Logical comparison operators.                                            */
/* 6. Matrix creating functions.                                               */
/* 7. Matrix non-member operators                                              */
/* 8. Boolean grouping.                                                        */
/* 9. Declaration of iterator class.                                           */
/* 10. Declaration of Const_Iterator class.                                    */
/*                                                                             */
/*=============================================================================*/

namespace mtm { 
    class IntMatrix {

        //=======================================================================
        // 1. Private attributes 
        //=======================================================================
        Dimensions dim;
        int* array;

    public:

        //=======================================================================
        // 2. Constructors, destructor and assignment operator.
        //=======================================================================
        explicit IntMatrix(Dimensions dim_t,  int initial_value=0); 
        IntMatrix(const IntMatrix& copy_matrix); 
        IntMatrix& operator=(const IntMatrix& other_matrix);
        ~IntMatrix(); 

        //=======================================================================
        // 3. Matrix info 
        //=======================================================================
        // height: Returns the number of rows in the matrix.
        int height() const;
        //width: Returns the number of columns in the matrix.
        int width() const; 
        //size: Returns the total size of the matrix.
        int size() const; 

        //=======================================================================
        // 4. Member operators.
        //=======================================================================
        // operator+=: addes the scaler to each value of the matrix. 
        IntMatrix& operator+=(const int scalar);
        // operator-: returns a new matrix of -value of each value
        IntMatrix operator-() const;
        // const operator(): returns the value in the row and col by reference as a const
        const int& operator()(int row, int col) const;
        // operator(): returns the value in the row and col by reference  
        int& operator()(int row, int col); 

        //=======================================================================
        // 5. Logical comparison operators.returns an intMatrix
        // returns an intMatrix of the result of Comparison, 1 meets the condition, 0 does not
        //=======================================================================
        IntMatrix operator==(const int number) const; 
        IntMatrix operator!=(const int number) const;
        IntMatrix operator<=(const int number) const;
        IntMatrix operator<(const int number) const;
        IntMatrix operator>=(const int number) const;
        IntMatrix operator>(const int number) const;

        //=======================================================================
        // 6. Matrix creating functions. 
        //=======================================================================
        // Returns an identity matrix with size equal to: size_n x size_n 
        static IntMatrix Identity(int size_n); 
        // Returns a new IntMatrix with rows and columns flipped compared to the matrix 
        IntMatrix transpose() const; 

        /***** Friend functions *****/
        // returns ostream value with the the correct form of printing IntMatrix  
        friend std::ostream& operator<<(std::ostream& os, const IntMatrix& matrix); 

        /***** Iterator classes *****/
        // read and write iterator
        class iterator; 
        // returns iteraor to the begin of the IntMatrix 
        iterator begin();
        // returns iteraor to one place after the end of the IntMatrix  
        iterator end();
        // read only iterator
        class const_iterator;
        // returns const_iterator to the begin of the IntMatrix 
        const_iterator begin() const;
        // returns const_iterator to one place after the end of the IntMatrix  
        const_iterator end() const;
    };
    
    //=======================================================================
    // 7. Matrix non-member operators
    //=======================================================================
    // Returns the result of adding value by value of 2 IntMatrix
    IntMatrix operator+(const IntMatrix& matrix1,const IntMatrix& matrix2);
    // Returns the result of adding scaler to each value of IntMatrix in symetric way
    IntMatrix operator+(const IntMatrix& matrix,const int scalar);
    IntMatrix operator+(const int scalar,const IntMatrix& matrix);
    // Returns the result of deduction value by value of 2 IntMatrix
    IntMatrix operator-(const IntMatrix& matrix1, const IntMatrix& matrix2);
    // returns ostream value with the the correct form of printing IntMatrix  
    std::ostream& operator<<(std::ostream& os, const IntMatrix& matrix);

    //=======================================================================
    // 8. Boolean grouping.
    //=======================================================================
    /**
    * all: chek the value the matrix compare to 0.
    * @param matrix - The IntMatrix to check.
    * @return
    * 	bool parameter - true if all values of the matrix are differnt then 0, false otherwise
    */
    bool all(const IntMatrix& matrix);
    /**
    * any: chek the value the matrix compare to 0.
    * @param matrix - The IntMatrix to check.
    * @return
    * 	bool parameter - true if at least one of the values of the matrix are differnt then 0, false otherwise
    */
    bool any(const IntMatrix& matrix);

    //=======================================================================
    // 9. Declaration of iterator class.  
    //=======================================================================
    class IntMatrix::iterator {
        //attributes of iterator
        IntMatrix* int_matrix;
        int index; 
        //constructor of iterator
        iterator(IntMatrix* matrix, int index); 
        friend class IntMatrix;
    public:
        //returns the value the iterator points on
        int& operator*() const;
        //post and prefix ways of promoting the iterator
        iterator& operator++();
        iterator operator++(int);
        //checking equality between operators, return true if equal, false otherwise
        bool operator==(const iterator& it) const;
        //checks operator inequality, return true if unequal, false otherwise
        bool operator!=(const iterator& it) const;
        //default copy constructor
        iterator(const iterator& it)=default;
        //default operator=
        iterator& operator=(const iterator& it) = default;
    };   
    
    //=======================================================================
    // 10. Declaration of Const_Iterator class. 
    //=======================================================================
    class IntMatrix::const_iterator {
        //attributes of const_iterator
        const IntMatrix* int_matrix;
        int index; 
        //constructor of const_iterator
        const_iterator(const IntMatrix* matrix, int size);
        friend class IntMatrix;
    public:
        //returns the value the iterator points on as a const
        const int& operator*() const;
        //post and prefix ways of promoting the const_iterator
        const_iterator& operator++();
        const_iterator operator++(int);
        //checking equality between operators, return true if equal, false otherwise
        bool operator==(const const_iterator& it) const;
        //checks operator inequality, return true if unequal, false otherwise
        bool operator!=(const const_iterator& it) const;
        //default copy constructor
        const_iterator(const const_iterator& it) = default;
        //default operator=
        const_iterator& operator=(const const_iterator& it) = default; 
    };      
}


#endif //EX3_INTMATRIX_H
