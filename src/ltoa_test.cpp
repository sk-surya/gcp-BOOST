#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
 
using Eigen::MatrixXd;
using Eigen::Matrix2d;
using namespace std;

int main()
{
  MatrixXd m(2,2);
  m(0,0) = 3;
  m(1,0) = 2.5;
  m(0,1) = -1;
  m(1,1) = m(1,0) + m(0,1);
  std::cout << m << std::endl;

  Matrix2d ones = MatrixXd::Ones(2,2);
  Eigen::SelfAdjointEigenSolver<MatrixXd> es(ones);
  cout << "The eigenvalues of the 3x3 matrix of ones are:" 
     << endl << es.eigenvalues() << endl;

}
