#include "AL.h"

Matrix BIRegressor::UCB(Matrix &data_points, double beta)
{   /* upper confidence bound */
    Matrix query_point(1, 3);
    int kill = -1;
    Matrix candidate_value;

    for (size_t i = 0; i < data_points.size1(); i++)
    {
        double candidate = prior.const_mean(data_points(i, 0), data_points(i, 1))
                        + prior.ARD(data_points(i, 0), data_points(i, 1),
                                    data_points(i, 0), data_points(i, 1)) * beta;
        if (kill == -1)
        {
             kill = i;
        }
    }

    return query_point;
};

double BIRegressor::predict(double rotate, double ray)
{
    int rows = memory.size1();
    Matrix C_block = prior.get_C_block(memory);
    Matrix K_block = prior.get_K_block(memory, rotate, ray);

    Matrix Y(rows, 1);
    for (int i = 0; i < rows; i++)
        Y(i, 0) = memory(i, 2);

    Matrix mu_x(rows, 1);
    for (int i = 0; i < rows; i++)
        mu_x(i, 0) = prior.const_mean(memory(i, 0), memory(i, 1));
    Matrix C_inverse = gjinverse(C_block, true);
    Matrix prod1 = prod(K_block, C_inverse);
    Matrix sub1 = Y - mu_x;
    Matrix ans = prod(prod1, sub1);
    return ans(0, 0) + prior.const_mean(rotate, ray); // fix
}

void BIRegressor::step(double rotate, double ray, double y)
{
    if (memory.size1() == 0)
    {
        memory.resize(1, 3);
        memory(0, 0) = rotate;
        memory(0, 1) = ray;
        memory(0, 2) = y;
        sigma = prior.get_K_scalar(rotate, ray);
        posterior_sigma = sigma;
        posterior_mu = 0;
    }
    else
    {
        counter++;
        int rows = memory.size1();
        Matrix C_block = prior.get_C_block(memory);
        Matrix K_block = prior.get_K_block(memory, rotate, ray);
        double K_scalar = prior.get_K_scalar(rotate, ray);

        Matrix Y(rows, 1);
        for (int i = 0; i < rows; i++)
            Y(i, 0) = memory(i, 2);

        Matrix mu_x(rows, 1);
        for (int i = 0; i < rows; i++)
            mu_x(i, 0) = prior.const_mean(memory(i, 0), memory(i, 1));

        //K_block @ np.linalg.inv(C_block) @ (Y - mu_x)

        Matrix C_inverse = gjinverse(C_block, true);
        Matrix prod1 = prod(K_block, C_inverse);
        Matrix sub1 = Y - mu_x;
        Matrix ans1 = prod(prod1, sub1);
        posterior_mu = prior.const_mean(rotate, ray) + ans1(0, 0);
        Matrix trans = transpose(K_block);
        Matrix ans2 = prod(prod1, trans);
        posterior_sigma = K_scalar - ans2(0, 0);


        sigma = posterior_sigma;
        mu = posterior_mu;
        memory.resize(rows + 1, 3, true);
        memory(rows, 0) = rotate;
        memory(rows, 1) = ray;
        memory(rows, 2) = y;
    }
};

void BIRegressor::train(Matrix& data_points)
{
    // qcb optimizer
    //UCB
    for (size_t i = 0; i < data_points.size1(); i++)
        step(data_points(i, 0), data_points(i, 1), data_points(i, 2));

}
