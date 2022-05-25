#ifndef AL_H
#define AL_H
#include "model.h"




//Matrix get_data_placeholder()
//{
//    Matrix data(7, 3);
//    data(0, 0) = 0.0; data(0, 1) = 2.0; data(0, 2) = 4.0;
//    data(1, 0) = 129; data(1, 1) = 2.0; data(1, 2) = 5.22;
//    data(2, 0) = 140; data(2, 1) = 2.0; data(2, 2) = 4.94;
//    data(3, 0) = 120; data(3, 1) = 2.0; data(3, 2) = 5.47;
//    data(4, 0) = 131; data(4, 1) = 2.0; data(4, 2) = 5.17;
//    data(5, 0) = 141; data(5, 1) = 2.0; data(5, 2) = 4.91;
//    data(6, 0) = 116; data(6, 1) = 2.0; data(6, 2) = 5.59;
//    return data;
//}

class MVN
{

public:
    double (MVN::*mean)();
    double (MVN::*kernel)();
    MVN(double (MVN::*kernel_func)() = nullptr, double (MVN::*mean_func)() = nullptr)
    {
        //if (kernel_func == nullptr)
        //    kernel = &MVN::ARD;
    };

    int const_mean(double rotate, double ray)
    {
        return 0;
    }

    double ARD(double rotate_a, double ray_a, double rotate_b, double ray_b, double sigma = 2.5, const vector& l = vector(2, 100.0))
    {   /* Automatic Relevance Determination */
        double sigma_suqared = std::pow(sigma, 2);
        double cumulative = 0;
        cumulative += std::pow( (rotate_a - rotate_b) / l[0], 2);
        cumulative += std::pow( (ray_a - ray_b) / l[1], 2);
        return sigma_suqared * std::exp(-0.5 * cumulative);
    }

    Matrix get_C_block(Matrix& data_points)
    {
        int rows = data_points.size1();
        Matrix C_block(rows, rows);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < rows; j++)
                C_block(i, j) = ARD(data_points(i, 0), data_points(i, 1),
                                    data_points(j, 0), data_points(j, 1));
        return C_block;
    }

    Matrix get_K_block(Matrix& data_points, double rotate, double ray)
    {
        Matrix K_block(1, data_points.size1());
        for (size_t i = 0; i < data_points.size1(); i++)
            K_block(0, i) = ARD(data_points(i, 0), data_points(i, 1),
                                    rotate, ray);
        return K_block;
    }

    double get_K_scalar(double rotate, double ray)
    {
        return ARD(rotate, ray, rotate, ray);
    }

};

/*
Cb ------ Kb
|		  |
|		  |  <----- Cov_matrix
Kb.T ---- Ks
*/

class BIRegressor
{
    MVN& prior;
    double sigma;
    double mu;
    double posterior_sigma;
    double posterior_mu;
    Matrix memory;
    int counter;

public:
    Process model;

    BIRegressor(MVN& prior): prior(prior), sigma(0), mu(0), counter(0) {};

    Matrix UCB(Matrix& data_points, double beta = 0.001);

    double predict(double rotate, double ray);

    void step(double rotate, double ray, double y);

    void train(Matrix& data_points);



};

/*
Choose the point of having the highest uncertainty
Add the point to train set
Train on the new train set
Go to 1 till convergence or budget elapsed
*/

//int main()
//{
//    Matrix data_points = get_data_placeholder();
//    MVN prior;
//    BIRegressor BI(prior);
//    BI.train(data_points);
//    std::cout << "prediction: " << BI.predict(117, 2) << '\n';
//    return 0;
//}

#endif // AL_H
