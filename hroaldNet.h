#include "matrix.h"
#include <stdlib.h>

class netLayer
{   
    matrix A;
    matrix B;
    public:
        netLayer(matrix Ain, matrix Bin);
        matrix getA();
        matrix getB();
        double getAElement(int x, int y);
        double getBElement(int x, int y);

        void setAElement(int x, int y, double value);
        void setBElement(int x, int y, double value);

        void setWeights(double value);
};

class neuralNet 
{   
    matrix inputLayer;
    std::vector<netLayer> hiddenLayers;
    matrix outputLayer;
    matrix lastA;

    public:
        neuralNet(int inputNodes, int hiddenNodes, int hiddenDepth, int outputNodes);

        matrix getInputLayer();
        matrix* getHiddenLayerMatrix(int index, matrix* AandB);
        void printHiddenLayerMatrix(int index);
        matrix getOutputLayer();
        matrix getLastA();

        void setWeights(double value);

        void updateMatrices(matrix newCoords);
        void train(std::vector<matrix> trainInput, std::vector<double> trainOutput, int iterations, double stepSize);
        matrix test(matrix testInput);
};

matrix getPrimeMatrix(matrix input, matrix A);

