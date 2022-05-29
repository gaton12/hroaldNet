#include <iostream>
#include "hroaldNet.h"

using namespace std;

int main() 
{   

    cout.precision(17);

    neuralNet nn(3, 3, 3, 1);

    vector<matrix> trainInput = {
        matrix{3,1, {1, 1, 1}},
        matrix{3,1, {2, 2, 1}}
        /* {3,1, {3, 2, 0}},
        {3,1, {10, 1, 0}}, */
    };
    vector<double> trainOutput = {
        double(2.0),
        double(4.0)
        /* 1,
        9, */
    };

    //matrix AandB[2] = {};
    nn.setWeights(0.1);

    //nn.getInputLayer().printMatrix();
    //nn.getHiddenLayerMatrix(2, AandB)[1].printMatrix();
    //nn.printHiddenLayerMatrix(0);
    //nn.getOutputLayer().printMatrix();

    //nn.getLastA().printMatrix();

    nn.train(trainInput, trainOutput, 500, 0.005);
    /* nn.printHiddenLayerMatrix(0);
    nn.printHiddenLayerMatrix(1);
    nn.printHiddenLayerMatrix(2);
    nn.getLastA().printMatrix(); */

    matrix testData = matrix(3,1, {20, 32, 1});
    matrix output = nn.test(testData);
    output.printMatrix();

    std::cout << "END" << std::endl;
    return 0;
}