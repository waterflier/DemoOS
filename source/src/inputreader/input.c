
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "./InputReader.h"



int main(int argc,char** argv)
{
    InputReader* pReader = CreateInputReader();
    InputReaderStart(pReader);

    sleep(1000);

    ReleaseInputReader(pReader);

    return 0;
}
