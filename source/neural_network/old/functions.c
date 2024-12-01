void init_weights(int row, int col, double **array);

void init_weights(int row, int col, double **array)
{

        for(int i = 0; i < row; i++)
        {
                for(int j = 0; j < col; j++)
                {
                        array[i][j] = RandFrom(-1, 1);
                }
        }
}

void init_biases(int length, double *array);

void init_biases(int length, double *array)
{
        for(int i = 0; i < length; i++)
        {
                array[i] = 0;
        }
}

double sigmoid(double x);

double sigmoid(double x)
{
        return 1 / (1 + exp(-x));
}

double dSigmoid(double x);

double dSigmoid(double x)
{
        return x * (1 - x);
}

int main()
{
        double training_inputs[numTrainingSets][numInputs] = {
                {0.0f, 0.0f},
                {0.0f, 1.0f},
                {1.0f, 0.0f},
                {1.0f, 1.0f}
        };
        // a transformer en une liste (A, B, C...)
        double training_output[numTrainingSets][numOutputs] = {
                {1.0f},
                {0.0f},
                {0.0f},
                {1.0f}
        };

        // Init of weights
        init_weights(numInputs, numHiddenNodes,
                    hiddenWeights);
        init_weights(numHiddenNodes, numOutputs,
                    outputWeights);
        // Init of bias
        init_biases(numOutputs, outputLayerBias);
        int numberOfEpochs = 100000;

        train(numberOfEpochs, training_inputs, training_output);
        return 0;
}
