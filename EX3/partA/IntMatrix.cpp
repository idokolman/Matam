#include "IntMatrix.h"

/*=============================================================================*/
/*                                                                             */
/* Contents:                                                                   */
/*                                                                             */
/* 1. Constructors, destructor and assignment operator.                        */
/* 2. Matrix info                                                              */
/* 3. Member operators.                                                        */
/* 4. Logical comparison operators.                                            */
/* 5. Matrix creating functions.                                               */
/* 6. Matrix non-member operators                                              */
/* 7. Boolean grouping.                                                        */
/* 8. Implementation of iterator class.                                        */
/* 9. Implementation of Const_Iterator class.                                  */
/*                                                                             */
/*=============================================================================*/

namespace mtm { 

    //=======================================================================
    // 1. Constructors, destructor and assignment operator.
    //=======================================================================

    IntMatrix::IntMatrix(Dimensions dim_t, int initial_value) :
    dim(dim_t), array(new int[dim_t.getCol()*dim_t.getRow()])
    {
        for (int i = 0; i < size(); i++)
        {
            array[i]=initial_value;
        }
    }

    IntMatrix::IntMatrix(const IntMatrix& copy_matrix) : dim(copy_matrix.dim), array(new int[copy_matrix.size()])
    {
        for (int i = 0; i < size(); i++)
        {
            array[i]=copy_matrix.array[i];
        }
    }

    IntMatrix& IntMatrix::operator=(const IntMatrix& other_matrix)
    {
        if (this==&other_matrix){ // self =
        return *this;
        }
        dim=other_matrix.dim;
        delete[] array;
        array=new int[other_matrix.size()];
        for (int i = 0; i < size(); i++)
        {
            array[i]=other_matrix.array[i];
        }
        return *this;
    }

    IntMatrix::~IntMatrix() 
    {
        delete[] array;
    }

    //=======================================================================
    // 2. Matrix info 
    //=======================================================================

    int IntMatrix::height() const
    {
        return dim.getRow();
    }
    int IntMatrix::width() const
    {
        return dim.getCol();
    }
    int IntMatrix::size() const
    {
        return dim.getCol()*dim.getRow();
    }

    //=======================================================================
    // 3. Member operators.
    //=======================================================================

    IntMatrix& IntMatrix::operator+=(const int scalar)
    {
        for (int i=0; i<size();i++){
            array[i]+=scalar;
        }
        return *this;
    }

    IntMatrix IntMatrix::operator-() const
    {
        IntMatrix minus_matrix(dim);
        for (int i=0; i<size();i++){
            minus_matrix.array[i]=-array[i];
        }
        return minus_matrix;
    }

    const int& IntMatrix::operator()(int row, int col) const
    {
        return array[row*width()+col];
    }
    
    int& IntMatrix::operator()(int row, int col)
    {
        return array[row*width()+col];
    }

    //=======================================================================
    // 4. Logical comparison operators. 
    //=======================================================================

    IntMatrix IntMatrix::operator==(const int number) const
    {
        return(this->operator<=(number))-(this->operator<(number));
    }
    IntMatrix IntMatrix::operator!=(const int number) const
    {
        return(this->operator>(number))+(this->operator<(number));
    }
    IntMatrix IntMatrix::operator<=(const int number) const
    {
        return(this->operator<(number+1)); //smaller than n => equel and smaller than n+1
    }
    IntMatrix IntMatrix::operator<(const int number) const
    {
        IntMatrix result(dim,0);
        for (int i = 0; i < size(); i++)
        {
            if (array[i]<number)
            {
               result.array[i]=1;
            }
        }
        return result;       
    }
    IntMatrix IntMatrix::operator>=(const int number) const
    {
        return(this->operator>(number-1)); //bigger than n => equel and bigger than n-1
    }
    IntMatrix IntMatrix::operator>(const int number) const
    {
        return (-*this)<(-number); // minus arguments provide same expression
    }

    //=======================================================================
    // 5. Matrix creating functions. 
    //=======================================================================
    
    IntMatrix IntMatrix::Identity(int size_n)
    {
        Dimensions identity_dim(size_n,size_n);
        IntMatrix identity(identity_dim);
        for (int i=0; i<size_n;i++){
            identity(i,i)=1;
        }
        return identity;
    }

    IntMatrix IntMatrix::transpose() const
    {
        Dimensions transpose_dim(dim.getCol(),dim.getRow());
        IntMatrix transpose(transpose_dim);
        for (int i=0; i<height(); i++){
            for (int j=0; j<width(); j++){
                transpose(j,i)=this->operator()(i,j);
            }
        }
        return transpose;
    }

    //=======================================================================
    // 6. Matrix non-member operators
    //=======================================================================

    std::ostream& operator<<(std::ostream& os, const IntMatrix& matrix)
    {
        os << printMatrix(matrix.array,matrix.dim);
        return os;
    }

    IntMatrix operator-(const IntMatrix& matrix1, const IntMatrix& matrix2) 
    {
        return matrix1+(-matrix2);
    }

    IntMatrix operator+(const IntMatrix& matrix1, const IntMatrix& matrix2) 
    {
        IntMatrix result=matrix1;
        IntMatrix::const_iterator it2=matrix2.begin();
        for (IntMatrix::iterator it1=result.begin(); it1!=result.end();++it1,++it2)
        {
            *it1+=*it2;
        }
        return result;
    }

    IntMatrix operator+(const IntMatrix& matrix,const int scalar)
    {
        IntMatrix result=matrix;
        return result+=scalar;
    }
    IntMatrix operator+(const int scalar,const IntMatrix& matrix)
    {
        return (matrix+scalar);
    }

    //=======================================================================
    // 7. Boolean grouping.
    //=======================================================================

    bool all(const IntMatrix& matrix)
    {
        for (IntMatrix::const_iterator it=matrix.begin(); it!=matrix.end();it++){
            if (*it==0){
                return false;
            }
        }
        return true;
    }

    bool any(const IntMatrix& matrix)
    {
        for (IntMatrix::const_iterator it=matrix.begin(); it!=matrix.end();++it)
        {
            if (*it!=0){
                return true;
            }
        }
        return false;
    }

    //=======================================================================
    // 8. Implementation of iterator class.
    //=======================================================================

    IntMatrix::iterator::iterator(IntMatrix* int_matrix, int index): int_matrix(int_matrix), index(index){}

    IntMatrix::iterator IntMatrix::begin()
    {
        return iterator(this,0);
    }

    IntMatrix::iterator IntMatrix::end()
    {
        return iterator(this,size());
    }

    int& IntMatrix::iterator::operator*() const
    {
        return(int_matrix->array[index]);
    }

    IntMatrix::iterator& IntMatrix::iterator::operator++()
    {
        index++;
        return *this;
    }

    IntMatrix::iterator IntMatrix::iterator::operator++(int)
    {
        iterator result=*this;
        ++*this;
        return result;
    }

    bool IntMatrix::iterator::operator==(const iterator& it) const
    {
        return (int_matrix==it.int_matrix) && (index==it.index);
    }

    bool IntMatrix::iterator::operator!=(const iterator& it) const
    {
        return !(*this==it);
    }
    
    //=======================================================================
    // 9. Implementation of const_iterator class.
    //=======================================================================

    IntMatrix::const_iterator::const_iterator(const IntMatrix* int_matrix, int index):
    int_matrix(int_matrix), index(index){}

    IntMatrix::const_iterator IntMatrix::begin() const
    {
        return const_iterator(this,0);
    }

    IntMatrix::const_iterator IntMatrix::end() const
    {
        return const_iterator(this,size());
    }

    const int& IntMatrix::const_iterator::operator*() const
    {
        return(int_matrix->array[index]);
    }

    IntMatrix::const_iterator& IntMatrix::const_iterator::operator++()
    {
        index++;
        return *this;
    }

    IntMatrix::const_iterator IntMatrix::const_iterator::operator++(int)
    {
        const_iterator result=*this;
        ++*this;
        return result;
    }

    bool IntMatrix::const_iterator::operator==(const const_iterator& it) const
    {
        return (int_matrix==it.int_matrix) && (index==it.index);
    }

    bool IntMatrix::const_iterator::operator!=(const const_iterator& it) const
    {
        return !(*this==it);
    }

}
