//
// input.c
//
//  test of read input
//


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "./InputReader.h"



int main(int argc,char** argv)
{
    InputReader* pReader = CreateInputReader();
    InputReaderStart(pReader);
    ReleaseInputReader(pReader);

	return 0;
}

