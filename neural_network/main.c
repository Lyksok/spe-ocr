#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "convertion.h"
#include "neural_letter.h"

int main()
{
	/*
	 * FOR TESTING PURPOSES
	 * I need a SDL_Surface
	 * of a specific height and width
	 * (here 16 x 16)
	 * */
	/*int size = 16;
        SDL_Surface* new = SDL_CreateRGBSurface(0, size, size, 8, 0, 0, 0, 0);
	if (new == NULL)
        {
                printf("Failure to create the surface : ");
                printf("%s", SDL_GetError());
        }

	int len = size * size;
	int *list = calloc(len, sizeof(int));
	SDL_to_list(new, len, &list);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			printf("%d ", list[i * size + j]);
		}
		printf("\n");
	}
	free(list);*/
    double training_inputs[numTrainingSets][numInputs] = //TODO
    // a transformer en une liste (A, B, C...)
    double training_output[26][numOutputs] = //TODO
    
    char alphabet[26] = "abcdefghijklmnopqrstuvwxyz";
    // Init of weights
    init_weights(numInputs, numHiddenNodes,
                    hiddenWeights);
    init_weights(numHiddenNodes, numOutputs,
                    outputWeights);
    // Init of bias
    for(int i = 0; i < numOutputs; i++)
    {
        outputLayerBias[i] = random_w_b();
    }

    int numberOfEpochs = 100000;
    
    train(numberOfEpochs, training_inputs, training_output);

	return 0;
}
