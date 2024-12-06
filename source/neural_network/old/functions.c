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

void RecoverBiases(Layer l, int nodes, double arr[nodes]);

void RecoverBiases(Layer l, int nodes, double arr[nodes])
{
        for (int n = 0; n < nodes ; n++)
        {
                l.neurons[n].bias = arr[n];
        }
        return;
}

void RecoverWeigths(Layer l, int rows, int cols, double mat[rows][cols]);

void RecoverWeigths(Layer l, int rows, int cols, double mat[rows][cols])
{
        for (int r = 0; r < rows ; r++)
        for (int c = 0; c < cols ; c++)
        {
                l.weights[r][c] = mat[r][c];
        }
        return;
}

Layer RecoverFirstLayer();

Layer RecoverFirstLayer()
{
        Layer layer;
        layer.numNeurons = nNodes;
        layer.neurons = malloc(nNodes * sizeof(Neuron));
        layer.inputs = calloc(nInputs, sizeof(double));
        RecoverBiases(layer, nNodes, HiddenBias);
        layer.numWeights = nInputs;
        RecoverWeigths(layer, nNodes, nInputs, HiddenWeight);
        layer.prev = NULL;
        layer.next = NULL;
        return layer;
}

void RecoverSecondLayer(Layer *l);

void RecoverSecondLayer(Layer *l)
{
        Layer layer;
        layer.numNeurons = nOut;
        layer.neurons = malloc(nOut * sizeof(Neuron));
        layer.inputs = calloc(nNodes, sizeof(double));
        layer.numWeights = nNodes;
        RecoverBiases(layer, nOut, OutputBias);
        RecoverWeigths(layer, nOut, nNodes, OutputWeight);

        layer.prev = l;
        l->next = &layer;
        layer.next = NULL;
        return;
}

void Softmax(int length, double **mat);

//void softmax(int K, double mat[1][K])
void Softmax(int length, double **mat)
{
        double sum = 0;
        for (int n = 0; n < length; n++)
        {
                mat[0][n] = exp(mat[0][n]);
                sum += mat[0][n];
        }
        for (int n = 0; n < length; n++)
        {
                mat[0][n] = mat[0][n] / sum;
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

void forward(int i, double **training_inputs);

void forward(int i, double **training_inputs)
{
    for(int j = 0; j < numHiddenNodes; j++)
    {
        double activation = hiddenLayerBias[j];

        for(int k = 0; k < numInputs; k++)
        {
            activation += training_inputs[i][k] * hiddenWeights[k][j];
        }

        hiddenLayer[j] = sigmoid(activation);

    }

    for(int j = 0; j < numOutputs; j++)
    {
        double activation = hiddenLayerBias[j];
        for(int k = 0; k < numHiddenNodes; k++)
        {
            activation += hiddenLayer[k] * outputWeights[k][j];
        }
        outputLayer[j] = sigmoid(activation);
    }
}

void backward(int i, const double lr, double **training_output);

void backward(int i, const double lr, double **training_output)
{

    double deltaOutput[numOutputs];
    for(int j = 0; j < numOutputs; j++)
    {
        double error = (training_output[i][j] - outputLayer[j]);
            deltaOutput[j] = error * dSigmoid(outputLayer[j]);
    }

    //Compute change in hidden weights
    double deltaHidden[numHiddenNodes];
    for(int j = 0; j < numHiddenNodes; j++)
    {
        double error = 0.0f;
        for(int k = 0; k < numOutputs; k++)
        {
            error += deltaOutput[k] * outputWeights[j][k];
        }
        deltaHidden[j] = error * dSigmoid(hiddenLayer[j]);
    }
    change_weights(lr, deltaOutput, deltaHidden);
}

void change_weights(const double lr,
		double deltaOutput[numOutputs],
		double deltaHidden[numHiddenNodes]);

void change_weights(const double lr,
		double deltaOutput[numOutputs],
		double deltaHidden[numHiddenNodes])
{

    for(int j = 0; j < numOutputs; j++)
    {
        outputLayerBias[j] += deltaOutput[j] * lr;
        for(int k = 0; k < numHiddenNodes; k++)
        {
            outputWeights[k][j] += hiddenLayer[k] *
                    deltaOutput[j] * lr;
        }
    }

    for(int j = 0; j < numHiddenNodes; j++)
    {
        hiddenLayerBias[j] += deltaHidden[j] * lr;
        for(int k = 0; k < numInputs; k++)
        {
            hiddenWeights[k][j] += hiddenLayer[k] *
                    deltaHidden[j] * lr;
        }
    }
}

void print_res(double **training_inputs,
                double **training_output);

void print_res(double **training_inputs,
                double **training_output)
{

     for(int i = 0; i < numTrainingSets; i++)
     {
        printf("Input: [%g, %g] Output: %g Predicted Output: %g \n",
            training_inputs[i][0], training_inputs[i][1],
            outputLayer[0], training_output[i][0]);
    }
}

void train(int numEpochs, double **training_inputs,
                double **training_output);

void train(int numEpochs, double **training_inputs,
                double **training_output)
{
    const double lr = 0.1f;

    for (int epoch = 0; epoch < numEpochs; epoch++)
    {
        for (int i = 0; i < numTrainingSets; i++)
        {
            forward(i, training_inputs);
            backward(i, lr, training_output);
        }
    }
    print_res(training_inputs, training_output);
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
