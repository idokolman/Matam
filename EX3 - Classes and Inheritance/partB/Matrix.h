#ifndef EX3_MATRIX_H
#define EX3_MATRIX_H
#include <string>
#include <iostream>
#include "Exceptions.h"
#include "Auxiliaries.h"

/*=============================================================================*/
/* Matrix template class                                                       */
/* --------------                                                              */
/* This interface exports the Matrix template class, which provides an         */
/* efficient, safe, convenient use for a two dimensional array type in C++.    */
/*                                                                             */
/* Contents:                                                                   */
/* 1. Private attributes                                                       */
/* 2. Static private functions                                                 */
/* 3. Constructors, destructor and assignment operator.                        */
/* 4. Matrix info                                                              */
/* 5. Matrix creating functions.                                               */
/* 6. Member operators.                                                        */
/* 7. Logical comparison operators.                                            */
/* 8. Exception classes                                                        */
/* 9. Matrix non-member operators                                              */
/* 10. Boolean grouping.                                                       */
/* 11. Declaration of iterator class.                                          */
/* 12. Declaration of Const_Iterator class.                                    */
/*                                                                             */
/*=============================================================================*/

namespace mtm {
    template <class T>
    class Matrix{
    private:

        //=======================================================================
        // 1. Private attributes 
        //=======================================================================

        Dimensions dim;
        T* data;

        //=======================================================================
        // 2. Static private functions
        //=======================================================================

        /**
        * validateDim: chek if dim_t is legal for initialization 
        * @param matrix - dim_t to check.
        * @return
        * 	the dim_t if exception isn't thrown
        */        
        static Dimensions validateDim(Dimensions dim_t);
        //Functor for fliping every instance of true to false and vice versa
        class boolNot { 
        public: 
            bool operator()(bool val){ 
                return !val; 
            } 
        }; 

    public:

        //=======================================================================
        // 3. Constructors, destructor and assignment operator.
        //=======================================================================
        //assumptions: T has operator=, T has non-parameter constructor
        Matrix(Dimensions dim_t, T initial_value=T()); // initial_value is by the deafult c'tor of T
        //assumptions: T has operator= T has non-parameter constructor
        Matrix(const Matrix& copy_matrix); 
        //assumptions: T has operator= T has non-parameter constructor 
        Matrix& operator=(const Matrix& other_matrix);
        //assumptions: d'tor for T
        ~Matrix(); 

        //=======================================================================
        // 4. Matrix info 
        //=======================================================================
        
        // Returns the number of rows in the matrix.
        int height() const
        {
            return dim.getRow();
        }

        // Returns the number of columns in the matrix.
        int width() const
        {
            return dim.getCol();
        } 

        // Returns the total size of the matrix.
        int size() const
        {
            return dim.getCol()*dim.getRow();
        }

        //=======================================================================
        // 5. Matrix creating functions. 
        //=======================================================================
        /** Returns a new Matrix of size_n value on the diagonal, 0 otherwise
        * assumptions: T has operator =
        */
        static Matrix Diagonal(int size_n,const T& value);
        /** Returns a new Matrix with rows and columns flipped compared to the matrix 
        * assumptions: T has operator =
        */
        Matrix transpose() const;
        /**
        * apply: apply the functor f on each value of the matrix
        * assumptions: f() operates on T and returns T, and T has operator =
        * @param f - functor with operator().
        * @return
        * 	new matrix of the values of this after operation of f
        */
        template <class Function>
        Matrix apply(Function f) const;

        //=======================================================================
        // 6. Member operators.
        //=======================================================================
        /** operator-: returns a new matrix of -value of each value
         * assumptions: T has unary operator-, T has operator=
         */
        Matrix operator-() const;
        /** operator+=: addes the object to each value of the matrix. 
         * assumptions: T has operator+=
         */
        Matrix& operator+=(const T object);
        // const operator(): returns the value in the row and col by reference as a const
        const T& operator()(int row, int col) const;
        // operator(): returns the value in the row and col by reference  
        T& operator()(int row, int col); 

        //=======================================================================
        // 7. Logical comparison operators.
        // returns a bool Matrix of the result of Comparison, true meets the condition, false does not 
        //=======================================================================
        //assumptions: T has operator==
        Matrix<bool> operator==(const T object) const; 
        //assumptions: T has < operator , T has operator-, bool has + operator for different values (true and false)
        Matrix<bool> operator!=(const T object) const;
        //assumptions: T has == and < operator
        Matrix<bool> operator<=(const T object) const;
        //assumptions: T has < operator
        Matrix<bool> operator<(const T object) const;
        //assumptions: T has < and == operator 
        Matrix<bool> operator>=(const T object) const;
        //assumptions: T has < and == operator 
        Matrix<bool> operator>(const T object) const;
        
        //Iterator classes
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

        //=======================================================================
        // 8. Exception classes
        //=======================================================================
        class AccessIllegalElement : public Exception {
            public:
            const char* what() const noexcept{
                return "Mtm matrix error: An attempt to access an illegal element";
            }  
        };

        class IllegalInitialization : public Exception {
            public:
            const char* what() const noexcept{
                return "Mtm matrix error: Illegal initialization values";
            } 
        };

        class DimensionMismatch : public Exception {
            public:
            Dimensions dim1;
            Dimensions dim2;
            std::string message;
            DimensionMismatch(Matrix mat1,Matrix mat2) : dim1(mat1.dim), dim2(mat2.dim) {
                std::string left_dim=dim1.toString();
                std::string right_dim=dim2.toString();
                message="Mtm matrix error: Dimension mismatch: " + left_dim + " " + right_dim;
            }
            const char* what() const noexcept 
            {
                return message.c_str();
            }
        };
    };
    //=======================================================================
    // 9. Matrix non-member operators
    //=======================================================================
    template <class T>
    /** Returns the result of adding value by value of 2 Matrices
    * assumptions: T has += operator, T has oprator=
    */
    Matrix<T> operator+(const Matrix<T>& matrix1,const Matrix<T>& matrix2);
    template <class T>
    /** Returns the result of deduction value by value of 2 Matrices
    * assumptions: T has unary operator-, T has operator=, T has += operator
    */
    Matrix<T> operator-(const Matrix<T>& matrix1,const Matrix<T>& matrix2);
    template <class T>
    /** Returns the result of adding object to each value of Matrix in symetric way
    * assumptions: T has operator+=, T has operator=
    */
    Matrix<T> operator+(const Matrix<T>& matrix,const T object);
    template <class T>
    Matrix<T> operator+(const T object,const Matrix<T>& matrix);
    template <class T>
    /** returns ostream value with the the correct form of printing IntMatrix 
    * assumptions: T has operator<< 
    */
    std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix);

    //=======================================================================
    // 10. Boolean grouping.
    //=======================================================================
    template <class T>
    /**
    * all: chek the values of the matrix compare to true (when converted to bool).
    * assumptions: T has (any kind of) cast to bool 
    * @param matrix - The IntMatrix to check.
    * @return
    * 	bool parameter - true if all values of the matrix are true (when converted to bool), false otherwise
    */
    bool all(const Matrix<T>& matrix);
    template <class T>
    /**
    * any: chek the values of the matrix compare to true (when converted to bool).
    * assumptions: T has (any kind of) cast to bool 
    * @param matrix - The IntMatrix to check.
    * @return
    * 	bool parameter - true if at least one of the values of the matrix are true (when converted to bool), false otherwise
    */
    bool any(const Matrix<T>& matrix);


    //=======================================================================
    // 11. Declaration of iterator class.  
    //=======================================================================
    template <class T>
    class Matrix<T>::iterator {
        //attributes of iterator
        Matrix* matrix;
        int index; 
        //constructor of iterator
        iterator(Matrix* matrix, int index);
        friend class Matrix;
    public:
        //returns the value the iterator points on
        T& operator*() const;
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
    // 12. Declaration of Const_Iterator class. 
    //=======================================================================

    template <class T>
    class Matrix<T>::const_iterator {
        //attributes of const_iterator
        const Matrix* matrix;
        int index; 
        //constructor of const_iterator
        const_iterator(const Matrix* matrix, int index);
        friend class Matrix;
    public:
        //returns the value the iterator points on as a const
        const T& operator*() const;
        //post and prefix ways of promoting the const_iterator
        const_iterator& operator++();
        const_iterator operator++(int);
        //checking equality between operators, return true if equal, false otherwise
        bool operator==(const const_iterator& it) const;
        //checks operator inequality, return true if unequal, false otherwise
        bool operator!=(const const_iterator& it) const;
        //default copy constructor
        const_iterator(const const_iterator& it)=default;
        //default operator=
        const_iterator& operator=(const const_iterator& it) = default;
    }; 

    /*=============================================================================*/
    /*                                                                             */
    /* Implementation of Matrix template class:                                    */
    /*                                                                             */
    /* 1. Static private functions                                                 */
    /* 2. Constructors, destructor and assignment Operator.                        */
    /* 3. Matrix creating functions.                                               */
    /* 4. Operators.                                                               */
    /* 5. Logical comparison operators.                                            */
    /* 6. Boolean grouping.                                                        */
    /* 7. Implementation of iterator class.                                        */
    /* 8. Implementation of const_Iterator class.                                  */
    /*                                                                             */
    /*=============================================================================*/

    //=======================================================================
    // 1. Static private functions
    //=======================================================================

    template <class T>
    Dimensions Matrix<T>::validateDim(Dimensions dim_t)
    {
        if (dim_t.getCol()<=0 or dim_t.getRow()<=0){
            throw IllegalInitialization();
        }
        return dim_t;
    }

    //=======================================================================
    // 2. Constructors, destructor and assignment Operator. 
    //=======================================================================

    template <class T>
    Matrix<T>::Matrix(Dimensions dim_t, T initial_value) :
    dim(validateDim(dim_t)), data(new T[dim_t.getCol()*dim_t.getRow()])
    {
        try{
            for (int i = 0; i < size(); i++)
            {
                data[i]=initial_value;
            }
        }catch(...){
            delete[] data;
            throw;
        }
    }

    template <class T>
    Matrix<T>::Matrix(const Matrix& copy_matrix) : dim(copy_matrix.dim), data(new T[copy_matrix.size()])
    {
        try{
            for (int i = 0; i < size(); i++)
            { 
                data[i]=copy_matrix.data[i];
            }
        }catch(...){
            delete[] data;
            throw;
        }
    }

    template <class T>
    Matrix<T>& Matrix<T>::operator=(const Matrix& other_matrix)
    {
        T* temp=new T[other_matrix.size()]; 
        try{
            for (int i = 0; i < other_matrix.size(); i++)
            {
                temp[i]=other_matrix.data[i];
            }
        }catch(...){
            delete[] temp;
            throw;
        }
        dim=other_matrix.dim;
        delete[] data;
        data=temp;
        return *this;
    }

    template <class T>
    Matrix<T>::~Matrix() {
        delete[] data;
    }


    //=======================================================================
    // 3. Matrix creating functions.  
    //=======================================================================

    template <class T>
    Matrix<T> Matrix<T>::Diagonal(int size_n,const T& value)
    {
        if (size_n<=0) {
            throw IllegalInitialization();
        }
        Dimensions diagonal_dim(size_n,size_n);
        Matrix Diagonal(diagonal_dim);
        for (int i=0; i<size_n;i++){
                Diagonal(i,i)=value;
            }
        return Diagonal;
    }

    template <class T>
    Matrix<T> Matrix<T>::transpose() const
    {
        Dimensions transpose_dim(dim.getCol(),dim.getRow());
        Matrix transpose(transpose_dim);
        for (int i=0; i<height(); i++){
            for (int j=0; j<width(); j++){
                transpose(j,i)=this->operator()(i,j);
            }
        }
        return transpose;
    }

    template <class T>
    template <class Function>
    Matrix<T> Matrix<T>::apply(Function f) const
    {
        Matrix result=*this; 
        for (Matrix::iterator it=result.begin(); it!=result.end();it++){
        *it=f(*it);
        }
        return result;
    }

    //=======================================================================
    // 4. Operators.  
    //=======================================================================

    template <class T>
    Matrix<T> operator+(const Matrix<T>& matrix1,const Matrix<T>& matrix2)
    {
        if ((matrix1.height()!=matrix2.height()) || (matrix1.width()!=matrix2.width())){
            throw typename Matrix<T>::DimensionMismatch(matrix1, matrix2);
        }
        Matrix<T> result=matrix1;
        typename Matrix<T>::const_iterator it2=matrix2.begin();
        for (typename Matrix<T>::iterator it1=result.begin(); it1!=result.end();++it1,++it2)
        {
            *it1+=*it2;
        }
        return result;
    }

    template <class T>
    Matrix<T> Matrix<T>::operator-() const
    {
        Matrix minus_matrix(dim);
        for (int i=0; i<size();i++){
            minus_matrix.data[i]=-data[i];
        }
        return minus_matrix;
    }

    template <class T>
    Matrix<T>& Matrix<T>::operator+=(const T object)
    {
        for (int i=0; i<size();i++){
            data[i]+=object;
        }
        return *this;
    }

    template <class T>
    Matrix<T> operator-(const Matrix<T>& matrix1,const Matrix<T>& matrix2)
    {

        if ((matrix1.height()!=matrix2.height()) || (matrix1.width()!=matrix2.width())){
            throw typename Matrix<T>::DimensionMismatch(matrix1, matrix2);
        }
        return matrix1+(-matrix2);
    }

    template <class T>
    const T& Matrix<T>::operator()(int row, int col) const
    {
        if ((row<0||col<0)||(col>=width())||(row>=height())){
            throw AccessIllegalElement();
        }
        return data[row*width()+col];
    }

    template <class T>
    T& Matrix<T>::operator()(int row, int col)
    {
        if ((row<0||col<0)||(col>=width())||(row>=height())){
            throw AccessIllegalElement();
        }
        return data[row*width()+col];
    }

    template <class T>
    std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix)
    {
        typename Matrix<T>::const_iterator it_begin=matrix.begin();
        typename Matrix<T>::const_iterator it_end=matrix.end();
        int width=matrix.width();
        return printMatrix(os, it_begin, it_end, width);
    }

    template <class T>
    Matrix<T> operator+(const Matrix<T>& matrix, const T object)
    {
        Matrix<T> result=matrix;
        return result+=object;
    }

    template <class T>
    Matrix<T> operator+(const T object, const Matrix<T>& matrix)
    {
        Matrix<T> result=matrix;
        for (typename Matrix<T>::iterator it=result.begin(); it!=result.end();++it)
        {
            *it=object+*it;
        }
        return (result);
    }

    //=======================================================================
    // 5. Logical comparison operators. 
    //=======================================================================

    template <class T>
    Matrix<bool> Matrix<T>::operator==(const T object) const
    {
        Matrix<bool> result(dim,false);
        Matrix<bool>::iterator it=result.begin();
        for (int i = 0; i < size(); i++, it++)
        {
            if (data[i]==object)
            {
                *it=true;
            }
        }
        return result;
    }

    template <class T>
    Matrix<bool> Matrix<T>::operator!=(const T object) const
    {
        return(*this==(object)).apply(boolNot());
    }

    template <class T>
    Matrix<bool> Matrix<T>::operator<=(const T object) const
    {
        return (*this==(object))+(*this<(object));
    }

    template <class T>
    Matrix<bool> Matrix<T>::operator<(const T object) const
    {
        Matrix<bool> result(dim,false);
        Matrix<bool>::iterator it=result.begin();
        for (int i = 0; i < size(); i++, it++)
        {
            if (data[i]<object)
            {
                *it=true;
            }
        }
        return result;
    }

    template <class T>
    Matrix<bool> Matrix<T>::operator>=(const T object) const
    {
        return (*this==(object))+(*this>(object));
    }

    template <class T>
    Matrix<bool> Matrix<T>::operator>(const T object) const
    {
        return(*this<=(object)).apply(boolNot());
    }  

    //=======================================================================
    // 6. Boolean grouping.
    //=======================================================================

    template <class T>
    bool all(const Matrix<T>& matrix)
    {
        for (typename Matrix<T>::const_iterator it=matrix.begin(); it!=matrix.end();++it){
            if (static_cast<bool>(*it)==false){
                return false;
            }
        }
        return true;
    }

    template <class T>
    bool any(const Matrix<T>& matrix)
    {
        for (typename Matrix<T>::const_iterator it=matrix.begin(); it!=matrix.end();++it){
            if (static_cast<bool>(*it)==true){
                return true;
            }
        }
        return false;
    }

    //=======================================================================
    // 7. Implementation of iterator class.
    //=======================================================================

    template <class T>
    Matrix<T>::iterator::iterator(Matrix* matrix, int index): matrix(matrix), index(index){}

    template <class T>
    typename Matrix<T>::iterator Matrix<T>::begin()
    {
        return iterator(this,0);
    }

    template <class T>
    typename Matrix<T>::iterator Matrix<T>::end()
    {
        return iterator(this,size());
    }

    template <class T>
    T& Matrix<T>::iterator::operator*() const
    {
        if (index>=matrix->size()){
            throw AccessIllegalElement();
        }
        return(matrix->data[index]);
    }

    template <class T>
    typename Matrix<T>::iterator& Matrix<T>::iterator::operator++()
    {
        index++;
        return *this;
    }

    template <class T>
    typename Matrix<T>::iterator Matrix<T>::iterator::operator++(int)
    {
        iterator result=*this;
        ++*this;
        return result;
    }

    template <class T>
    bool Matrix<T>::iterator::operator==(const iterator& it) const
    {
        return (matrix==it.matrix) && (index==it.index);
    }

    template <class T>
    bool Matrix<T>::iterator::operator!=(const iterator& it) const
    {
        return !(*this==it);
    }

    //=======================================================================
    // 8. Implementation of const_iterator class.
    //=======================================================================

    template <class T>
    Matrix<T>::const_iterator::const_iterator(const Matrix* matrix, int index): matrix(matrix), index(index){}

    template <class T>
    typename Matrix<T>::const_iterator Matrix<T>::begin() const
    {
        return const_iterator(this,0);
    }

    template <class T>
    typename Matrix<T>::const_iterator Matrix<T>::end() const
    {
        return const_iterator(this,size());
    }

    template <class T>
    const T& Matrix<T>::const_iterator::operator*() const
    {
        if (index>=matrix->size()){
            throw AccessIllegalElement();
        }
        return(matrix->data[index]);
    }

    template <class T>
    typename Matrix<T>::const_iterator& Matrix<T>::const_iterator::operator++()
    {
        index++;
        return *this;
    }

    template <class T>
    typename Matrix<T>::const_iterator Matrix<T>::const_iterator::operator++(int)
    {
        const_iterator result=*this;
        ++*this;
        return result;
    }

    template <class T>
    bool Matrix<T>::const_iterator::operator==(const const_iterator& it) const
    {
        return (matrix==it.matrix) && (index==it.index);
    }

    template <class T>
    bool Matrix<T>::const_iterator::operator!=(const const_iterator& it) const
    {
        return !(*this==it);
    }

}
#endif //EX3_MATRIX_H
