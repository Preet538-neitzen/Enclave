#include <openenclave/host.h>
#include "guessing_game_u.h"
#include <stdio.h>

// simul check
bool checkRun(int* argc, const char* argv[])
{
    for (int i = 0; i < *argc; i++)
    {
        if (strcmp(argv[i], "--simulate") == 0)
        {
            fprintf(stdout, "Running\n");
            memmove(&argv[i], &argv[i + 1], (*argc - i) * sizeof(char*));
            (*argc)--;
            return true;
        }
    }
    return false;
}

int main(int argc, const char* argv[])
{
    oe_result_t result;
    int ret = 1;
    oe_enclave_t* enclave = NULL;

    uint32_t flags = OE_ENCLAVE_FLAG_DEBUG;
    if (checkRun(&argc, argv))
    {
        flags |= OE_ENCLAVE_FLAG_SIMULATE;
    }

    if (argc != 2)
    {
        fprintf(
            stderr, "Usage: %s enclave_image_path [ --simulate  ]\n", argv[0]);
        goto exit;
    }

    result = oe_create_guessing_game_enclave(
        argv[1], OE_ENCLAVE_TYPE_AUTO, flags, NULL, 0, &enclave);
    if (result != OE_OK)
    {
        fprintf(
            stderr,
            "oe_create_guessing_game_enclave(): result=%u (%s)\n",
            result,
            oe_result_str(result));
        goto exit;
    }

    int curVal = -1;
    int maxVal = -1;
    printf("enter a max value\n");
    scanf("%d", &maxVal);
    printf("max value: %d\n", maxVal);
    result = findMaxi(enclave, maxVal);
        if (result != OE_OK)
        {
        fprintf(
            stderr,
            "calling failed: result=%u (%s)\n",
            result,
            oe_result_str(result));
        goto exit;
        }
    int* res = malloc(sizeof(int)); 
    int counter = 0;
     
    for ( counter=0; counter<maxVal; counter++){
    	printf("Make a guess\n");
        scanf("%d", &curVal);
        printf("You guess is: %d\n", curVal);
		
	result = checkStatus(enclave, curVal, res);
    	if (result != OE_OK)
    	{ 
	if (result!=OE_ENCLAVE_ABORTING){
        fprintf(
            stderr,
            "fail: result=%u (%s)\n",
            result,
            oe_result_str(result));
        goto exit;
	}
	else{
	fprintf(
            stderr,
            "Enclave stopped\nYou have lost the game");
	ret = 0;
	goto exit; 
	}
    	}
	if(*res){
		printf("You have won the game!\n"); 
		break;
	}
	else{
		printf("Wrong guess! Please try again! \n");
	}
    }
    ret = 0;
exit:
    if (enclave)
    // oe_terminate_enclave(enclave);
        oe_terminate_enclave(enclave);

    return ret;
}
