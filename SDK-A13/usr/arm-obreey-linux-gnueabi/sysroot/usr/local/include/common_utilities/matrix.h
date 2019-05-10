#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cmath>

namespace pocketbook {
namespace math {
namespace matrix {

template <typename T> class Matrix;
template <typename T> class SquareMatrix;
template <typename T> std::ostream & operator<<(std::ostream &, const Matrix<T> &);

template <typename T>
class Matrix
{
public:
  Matrix(size_t rows, size_t cols) : _m(rows, std::vector<T>(cols)) { }
  Matrix(size_t rows, size_t cols, const T & initvalue) : _m(rows, std::vector<T>(cols, initvalue)) { }
  Matrix(const Matrix & m) : _m(m._m) { }

  template <typename S> operator Matrix<S>() const;

  Matrix transpose() const;
  Matrix minor_matrix(size_t row, size_t col) const;

  Matrix gauss() const;
  Matrix gauss_nonreduced(int &) const;
  Matrix gauss_jordan() const;

  size_t rows() const { return _m.size(); }
  size_t cols() const { if (_m.size()) return _m[0].size(); else return 0; }

  // Swapping rows or columns is done on the matrix itself.
  void swap_rows(size_t r1, size_t r2) { _m[r1].swap(_m[r2]); }
  void swap_cols(size_t c1, size_t c2) { for (int i = 0; i < rows(); i++) std::swap(_m[i][c1], _m[i][c2]); }

  // Removing rows or columns is done on the matrix itself.
  void remove_row(size_t row) { _m.erase(_m.begin() + row); }
  void remove_column(size_t col) {  for (int i = 0; i < _m.size(); i++) _m[i].erase(_m[i].begin() + col); }

  const T& operator()(size_t i, size_t j) const { return _m[i][j]; }
  T& operator()(size_t i, size_t j) { return _m[i][j]; }

  Matrix<T> operator+(const Matrix<T> &) const;
  Matrix<T> operator*(const T &) const;
  Matrix<T> operator*(const Matrix<T> &) const;

  // A portable absolute value.
  T myabs(const T &) const;

protected:
  std::vector< std::vector<T> > _m;

  friend std::ostream & operator<< <T> (std::ostream &, const Matrix<T> &);
  friend class SquareMatrix<T>;
};


template<typename T> T Matrix<T>::myabs(const T & x) const { return fabs(x); }
template<> int Matrix<int>::myabs(const int & x) const { return abs(x); }
template<> long int Matrix<long int>::myabs(const long int & x) const { return labs(x); }
//template<> std::complex<double> Matrix<std::complex<double> >::myabs(const std::complex<double> & z) const { return abs(z); }

/* Matrix transpose */
template <typename T>
Matrix<T> Matrix<T>::transpose() const
{
  Matrix<T> m(cols(), rows());
  for (size_t i = 0; i < _m.size(); i++)
    for (size_t j = 0; j < _m[i].size(); j++)
      m(j,i) = _m[i][j];
  return m;
}

/* Matrix minor */
template <typename T>
Matrix<T> Matrix<T>::minor_matrix(size_t row, size_t col) const
{
  Matrix<T> m(*this);
  m.remove_row(row);
  m.remove_column(col);
  return m;
}

/* Partial Gaussian elimination, returns upper-triangular form. */
template <typename T>
Matrix<T> Matrix<T>::gauss_nonreduced(int & swapcount) const
{
  swapcount = 0;
  Matrix<T> m(*this);
  size_t i = 0;
  size_t j = 0;
  while(i < m.rows() && j < m.cols())
  {
    size_t maxi = i;
    for (size_t k = i + 1; k < m.rows(); k++)
      if (myabs(m(k,j)) > myabs(m(maxi,j)))
        maxi = k;
    if (m(maxi, j) != 0)
    {
      if (i != maxi)
      {
        m.swap_rows(i, maxi);
        swapcount++;
      }

      T divisor = m(i,j);
      for (size_t k = i + 1; k < rows(); k++)
      {
        T tmp = m(k, j);
        for (size_t l = 0; l < cols();  l++)
          m(k, l) -= (tmp/divisor * m(i, l));
      }
      ++i;
    }
    ++j;
  }
  return m;
}

/* Gaussian elimination, returns a row-echelon form with "1" along the diagonal. */
template <typename T>
Matrix<T> Matrix<T>::gauss() const
{
  //std::cerr << "Now eliminating Gauss." << std::endl;
  Matrix<T> m(*this);
  size_t i = 0;
  size_t j = 0;
  while(i < m.rows() && j < m.cols())
  {
    size_t maxi = i;
    for (size_t k = i + 1; k < m.rows(); k++)
      if (myabs(m(k,j)) > myabs(m(maxi,j)))
        maxi = k;
    if (m(maxi, j) != 0)
    {
      m.swap_rows(i, maxi);

      // Divide each entry in row i by m(i,j)
      T divisor = m(i,j);
      for (size_t l = 0; l < cols();  l++)
        m(i, l) /= divisor;

      for (size_t k = i + 1; k < rows(); k++)
      {
        T tmp = m(k, j);
        for (size_t l = 0; l < cols();  l++)
          m(k, l) -= (tmp * m(i, l));
      }
      ++i;
    }
    ++j;
  }
  return m;
}

/* Fake Gauss-Jordan elimination, returns the reduced row-echolon form by backsubstition in gauss(). */
template <typename T>
Matrix<T> Matrix<T>::gauss_jordan() const
{
  //std::cerr << "Now eliminating Gauss-Jordan: First... ";
  Matrix<T> m(this->gauss());
  //std::cerr << "... then back-substituting." << std::endl;

  for (int i = rows() - 2; i >= 0; i--)
    for (int j = rows() - 1; j > i; j--)
    {
      // subtract m(i,j)*row(j) from row(i)
      T tmp = m(i,j);
      for (int l = 0; l < cols(); l++)
        m(i, l) -= (tmp * m(j, l));
    }
  return m;
}

/* Cast operator */
template <typename T>
template <typename S>
Matrix<T>::operator Matrix<S>() const
{
  Matrix<S> m(rows(), cols());
  for (int i = 0; i < _m.size(); i++)
    for (int j = 0; j < _m[i].size(); j++)
      m(i, j) = S(_m[i][j]);
  return m;
}

/* Matrix addition */
template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> & m) const
{
  if (!(rows() == m.rows() && cols() == m.cols())) throw std::domain_error("Trying to add matrices of different dimension!");

  Matrix<T> out(rows(), cols());
  for (size_t i = 0; i < rows(); i++)
    for (size_t j = 0; j < cols(); j++)
      out(i, j) = (*this)(i, j) + m(i, j);
  return out;
}

/* Scaling */
template <typename T>
Matrix<T> Matrix<T>::operator*(const T & s) const
{
  Matrix<T> out(rows(), cols());
  for (size_t i = 0; i < rows(); i++)
    for (size_t j = 0; j < cols(); j++)
      out(i, j) = (*this)(i, j) * s;
  return out;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> & s) const
{
  Matrix<T> out(rows(), s.cols());
  if (cols() != s.rows()) return out;
  for (size_t i = 0; i < rows(); i++)
    for (size_t j = 0; j < s.cols(); j++)
        for (size_t k = 0; k < cols(); ++k)
            out(i, j) += (*this)(i, k) * s(k, j);
  return out;
}

/* OStream output */
template <typename T>
std::ostream & operator<<(std::ostream & os, const Matrix<T> & m)
{
  os << "[";
  for (size_t i = 0; i < m._m.size(); i++)
  {
    os << "[";
    for (size_t j = 0; j < m._m[i].size() - 1; j++)
    {
      os << m._m[i][j] << ", ";
    }
    os << m._m[i][m._m[i].size() - 1] << "]";
  }
  os << "]";
  return os;
}




template <typename T>
class SquareMatrix : public Matrix<T>
{
public:
  explicit SquareMatrix(size_t dim) : Matrix<T>(dim, dim) { }
  SquareMatrix(size_t dim, const T & initvalue) : Matrix<T>(dim, dim, initvalue) { }
  SquareMatrix(const SquareMatrix & m) : Matrix<T>(m) { }
  SquareMatrix(const Matrix<T> & m) : Matrix<T>(m)
  {
    if (m.rows() != m.cols())
      throw std::domain_error("Trying to construct square matrix from non-square matrix!");
    Matrix<T>::_m = m._m;
  }

  size_t dim() const { return Matrix<T>::_m.size(); }
  
  /**
   * @brief inverse - function used only for matrix (2, 2)
   * @return 
   */
  SquareMatrix inverse() const;
  SquareMatrix transpose() const;
  T determinant() const;
};

template <typename T>
SquareMatrix<T> SquareMatrix<T>::inverse() const
{
  SquareMatrix<T> m(dim());
  if (Matrix<T>::rows() != 2 || Matrix<T>::cols() != 2) return m;
  T det = determinant();
  m(0, 0) = Matrix<T>::_m[1][1] / det;
  m(0, 1) = -Matrix<T>::_m[1][0] / det;
  m(1, 0) = -Matrix<T>::_m[0][1] / det;
  m(1, 1) = Matrix<T>::_m[0][0] / det;
  
  return m;
}

/* Specialised square matrix transpose */
template <typename T>
SquareMatrix<T> SquareMatrix<T>::transpose() const
{
  SquareMatrix<T> m(dim());
  for (size_t i = 0; i < Matrix<T>::_m.size(); i++)
    for (size_t j = 0; j < Matrix<T>::_m[i].size(); j++)
      m(j, i) = Matrix<T>::_m[i][j];
  return m;
}

/* Determinant */
template <typename T>
T SquareMatrix<T>::determinant() const
{
  /* Gauss elimination followed by multiplying up the diagonal. */
  int swapcount;
  SquareMatrix<T> gaussed = this->gauss_nonreduced(swapcount);
  T det(1);
  for (size_t i = 0; i < dim(); i++)
    det *= gaussed(i, i);
  return (swapcount % 2  ?  -det  :  det);
}



template <typename T>
SquareMatrix<T> vandermonde(const std::vector<T> & data)
{
  //std::cerr << "Vandermonde is dealing with type " << typeid(T).name() << std::endl;
  SquareMatrix<T> m(data.size());
  for (size_t i = 0; i < data.size(); i++)
    for (size_t j = 0; j < data.size(); j++)
      m(i, j) = pow(data[i], j);
  return m;
}

}  // namespace pocketbook
}  // namespace math
}  // namespace matrix

#endif // MATRIX_H
