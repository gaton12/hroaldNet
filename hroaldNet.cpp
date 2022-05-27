#include "hroaldNet.h"

netLayer::netLayer(matrix Ain, matrix Bin)
{
    A = Ain;
    B = Bin;
}

matrix netLayer::getA()
{
    return A;
}
matrix netLayer::getB()
{
    return B;
}

double netLayer::getAElement(int x, int y)
{
    return A.getElement(x, y);
}
double netLayer::getBElement(int x, int y)
{
    return B.getElement(x, y);
}

void netLayer::setAElement(int x, int y, double value)
{
    A.setElement(x, y, value);
}
void netLayer::setBElement(int x, int y, double value)
{
    B.setElement(x, y, value);
}

void netLayer::setWeights(double value)
{
    for (int x = 0; x < A.getRows(); x++)
    {
        for (int y = 0; y < A.getCols(); y++)
        {
            A.setElement(x, y, value /* + ((rand()%10 + 1)*(0.1 * value)) */);
        }
    }
    for (int x = 0; x < B.getRows(); x++)
    {
        for (int y = 0; y < B.getCols(); y++)
        {
            B.setElement(x, y, value /* + ((rand()%10 + 1)*(0.1 * value)) */);
        }
    }
    
}

//////////////////////////////////////////////////////////
neuralNet::neuralNet(int inputNodes, int hiddenNodes, int hiddenDepth, int outputNodes)
{
    inputLayer = matrix(inputNodes, 1);

    matrix A = {hiddenNodes, inputNodes};
    matrix B = {hiddenNodes, 1};
    netLayer hiddenLayer(A, B);

    for (int x = 0; x < hiddenDepth; x++)
    {
        hiddenLayers.push_back(hiddenLayer);
    }

    lastA = matrix(1, hiddenNodes);

    outputLayer = matrix(outputNodes, 1);
}

matrix neuralNet::getInputLayer()
{
    return inputLayer;
}

matrix* neuralNet::getHiddenLayerMatrix(int index, matrix* AandB) 
{
    AandB[0] = hiddenLayers[index].getA();
    AandB[1] = hiddenLayers[index].getB();
    return AandB;
}

void neuralNet::printHiddenLayerMatrix(int index)
{   
    std::cout << "A: " << std::endl;
    hiddenLayers[index].getA().printMatrix();
    std::cout << "B: " << std::endl;
    hiddenLayers[index].getB().printMatrix();
}

matrix neuralNet::getOutputLayer() 
{
    return outputLayer;
}

matrix neuralNet::getLastA()
{
    return lastA;
}

void neuralNet::setWeights(double value)
{
    srand(69);
    for (int i = 0; i < hiddenLayers.size() ; i++)
    {
        hiddenLayers[i].setWeights(value);
    }
    for (int x = 0; x < lastA.getRows(); x++)
    {
        for (int y = 0; y < lastA.getCols(); y++)
        {
            lastA.setElement(x, y, value /* + ((rand()%10 + 1)*(0.1*value)) */);
        }
    }
    
}

void neuralNet::updateMatrices(matrix newCoords)
{
    int counter = 0;
    for (int j = 0; j < hiddenLayers.size(); j++)
    {
        for (int x = 0; x < hiddenLayers[j].getA().getRows(); x++)
        {
            for (int y = 0; y < hiddenLayers[j].getA().getCols(); y++)
            {
                hiddenLayers[j].setAElement(x, y, hiddenLayers[j].getAElement(x, y) + newCoords.getElement(0, counter));
                counter++;
            }
        }
        for (int x = 0; x < hiddenLayers[j].getB().getRows(); x++)
        {   
            for (int y = 0; y < hiddenLayers[j].getB().getCols(); y++)
            {
                hiddenLayers[j].setBElement(x, y, hiddenLayers[j].getBElement(x, y) + newCoords.getElement(0, counter));
                //std::cout << "B: " << hiddenLayers[j].getBElement(x, y) + newCoords.getElement(0, counter) << std::endl;
                counter++;
            }
        }
        
    }

    for (int x = 0; x < lastA.getRows(); x++)
    {
        for (int y = 0; y < lastA.getCols(); y++)
        {
            lastA.setElement(x, y, lastA.getElement(x, y) + newCoords.getElement(0, counter));
            counter++;
        }
    }
    printHiddenLayerMatrix(0);
    //std::cout << counter << std::endl; 
}

void neuralNet::train(std::vector<matrix> trainInput, std::vector<double> trainOutput, int iterations, double stepSize)
{
    std::vector<std::vector<matrix>> S;
    std::vector<double> delta  = {};
    std::vector<matrix> SPrime  = {};
    std::vector<std::vector<matrix>> newCoordsParts;
    std::vector<matrix> newCoords;
    std::vector<matrix> YPrimes;
    matrix Stemp = {};
    matrix yTemp = {};
    matrix SPtemp = {};
    matrix YPrime = {};
    matrix newCoordsTemp = {};
    matrix grad = {};
    double lossValue = 0;

    for (int iteration = 0; iteration < iterations; iteration++)
    {
        for (int j = 0; j < trainInput.size(); j++)
        {   
            S.push_back({}); 
            Stemp = sigmafy((hiddenLayers[0].getA() * trainInput[j]) + hiddenLayers[0].getB());
            //Stemp.printMatrix();
            S[j].push_back(Stemp);
            for (int i = 1; i < hiddenLayers.size(); i++)
            {
                Stemp = sigmafy((hiddenLayers[i].getA() * S[j][i-1]) + hiddenLayers[i].getB());
                //Stemp.printMatrix();
                S[j].push_back(Stemp);
            }
            yTemp = lastA * S[j][S[j].size() - 1];
            //yTemp.printMatrix();
            delta.push_back(yTemp.getElement(0, 0) - trainOutput[j]);

            //S.clear();
        }

        for (int i = 0; i < delta.size(); i++)
        {
            lossValue += (delta[i] * delta[i]);
        }
        lossValue = lossValue / delta.size();
        if (iteration % 1 == 0)
        {
            std::cout << "Iteration: " << iteration << " Loss: " << lossValue << std::endl;
        }
        
        for (int j = 0; j < trainInput.size(); j++)
        {
            newCoordsParts.push_back({});
            SPtemp = getPrimeMatrix(trainInput[j], hiddenLayers[0].getA());
            //SPtemp.printMatrix();
            //std::cout << "bajs" << std::endl;
            SPrime.push_back(SPtemp);
            for (int i = 1; i < hiddenLayers.size(); i++)
            {
                //std::cout << "bajsa" << std::endl;
                //S[j][i-1].printMatrix();
                SPtemp = getPrimeMatrix(S[j][i-1], hiddenLayers[i].getA());
                //SPtemp.printMatrix();
                //S[j][i-1].printMatrix();
                //std::cout << "bajsen" << std::endl;
                SPrime.push_back(SPtemp);
            }
            YPrime = S[j][S[j].size() - 1].transpose();
            //YPrime.printMatrix();
            YPrimes.push_back(YPrime);
            // TODO: fix this:
            for (int k = 1; k < hiddenLayers.size()+1; k++)
            {
                //std::cout << "bajs" << std::endl;
                newCoordsTemp = (lastA * S[j][S[j].size() - 1]);
                for (int i = hiddenLayers.size()-k; i > 0; i--)
                {
                    //std::cout << "korv" << std::endl;
                    newCoordsTemp *= hiddenLayers[hiddenLayers.size()-i].getA();
                    newCoordsTemp *= S[j][S[j].size()-1-i];
                }
                newCoordsTemp *= SPrime[k-1];
                //newCoordsTemp.printMatrix();
                newCoordsParts[j].push_back(newCoordsTemp);
            }
            SPrime.clear();
        }

        ///// PRINTING /////
        /* for (int i = 0; i < newCoordsParts.size(); i++)
        {
            for (int j = 0; j < newCoordsParts[i].size(); j++)
            {
                newCoordsParts[i][j].printMatrix();
            }
            std::cout << "----------" << std::endl;
            
        } */
        /////////////////////

        for (int i = 0; i < trainInput.size(); i++)
        {
            newCoords.push_back(newCoordsParts[i][0]);
            for (int j = 1; j < newCoordsParts[i].size(); j++)
            {
                newCoords[i] = newCoords[i].append(newCoordsParts[i][j]);
            }
            newCoords[i] = newCoords[i].append(YPrimes[i]);
            //newCoords[i].printMatrix();
        }

        grad = (newCoords[0]*delta[0]);
        for (int i = 1; i < trainInput.size(); i++)
        {
            grad += (newCoords[i] * delta[i]);
        }
        //grad.printMatrix();
        //std::cout << "Gradient: " << std::endl;
        //grad.printMatrix();

        //(grad * (-0.001)).printMatrix();
        //std::cout << (grad * (-0.001)).getCols() << std::endl;

        updateMatrices(grad * (-stepSize));

        lossValue = 0;
        grad = {};
        S.clear();
        delta.clear();
        SPrime.clear();
        newCoordsParts.clear();
        newCoords.clear();
        YPrimes.clear();
        Stemp = {};
        yTemp = {};
        SPtemp = {};
        YPrime = {};
        newCoordsTemp = {};
    }
    
}

matrix neuralNet::test(matrix testInput)
{
    std::vector<matrix> S;
    matrix Stemp = {};
    matrix yTemp = {};

    Stemp = sigmafy((hiddenLayers[0].getA() * testInput) + hiddenLayers[0].getB());
    S.push_back(Stemp);
    for (int i = 1; i < hiddenLayers.size(); i++)
        {
            Stemp = sigmafy((hiddenLayers[i].getA() * S[i-1]) + hiddenLayers[i].getB());
            S.push_back(Stemp);
        }
        yTemp = lastA * S[S.size() - 1];
    return yTemp;
}



//////////////////////////////////////////////////////////
matrix getPrimeMatrix(matrix input, matrix A) 
{
    matrix ANew = matrix(A.getRows(), A.getRows()*A.getRows());

    //std::cout << ANew.getCols() << std::endl;

    int counterY = 0;
    int counterX = 0;

    for (int j = 0; j < ANew.getRows(); j++)
    {
        for (int i = 0; i < input.getRows(); i++)
        {
            //std::cout << "bajs1" << std::endl;
            ANew.setElement(counterX, counterY, input.getElement(i, 0));
            counterY++;
            if (counterY == ANew.getCols())
            {
                counterY = 0;
                counterX++;
            }
            //std::cout << "bajs2" << std::endl;
        }
        if (j < ANew.getRows()-1)
        {
            //std::cout << "bajs3" << std::endl;
            for (int i = 0; i < ANew.getCols(); i++)
            {
                ANew.setElement(counterX, counterY, 0.0);
                counterY++;
                if (counterY == ANew.getCols())
                {
                    counterY = 0;
                    counterX++;
                }
            }
            //std::cout << "bajs4" << std::endl;
        }
    }

    matrix uniMatrix = unityMatrix(ANew.getRows(), ANew.getRows());

    matrix result = ANew.append(uniMatrix);

    return result;
}