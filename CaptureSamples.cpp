#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>
#include <fstream>
#include <bitset>
#include <time.h>


#define OE_ADDR 0x134
#define GPIO_DATAOUT 0x13C
#define GPIO_DATAIN 0x138
#define GPIO0_ADDR 0x44E07000
#define GPIO1_ADDR 0x4804C000
//#define GPIO2_ADDR 0x481AC000
//#define GPIO3_ADDR 0x481AE000




using namespace std;

// PIN NAME & PINLOCATION
// CNVST = GPIO1_18
// BUSY = GPIO0_11
// D0 = GPIO0_30
// D1 = GPIO0_31
// D2 = GPIO0_15
// D3 = GPIO0_5
// D4 = GPIO0_26
// D5 = GPIO0_3
// D6 = GPIO0_2
// D7 = GPIO0_12
// D8 = GPIO0_7
// D9 = GPIO0_13
// D10 = GPIO0_10
// D11 = GPIO0_20
// D12 = GPIO0_9
// D13 = GPIO0_8
// D14 = GPIO0_4
// D15 = GPIO0_27


//Function to write int to file using fwrite()
void writeInt(FILE * out, int num)
{
    if (NULL == out){
	cout << "EXIT_FAILURE";
    }
    fwrite(&num, sizeof(int), 1, out);
}


int main(){

   int samplenum = 320000;



// map /dev/mem to access
    int fd = open("/dev/mem",O_RDWR | O_SYNC);



// connect pinconf1 to gpio1 (repeat for all four)
    unsigned long* pinconf0 =  (unsigned long*) mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO0_ADDR);
    unsigned long* pinconf1 =  (unsigned long*) mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO1_ADDR);

// Set CNVST to only output
    pinconf1[OE_ADDR/4] &= (0xFFFFFFFF ^ (1 << 18));

// Set CNVST to HIGH
    pinconf1[GPIO_DATAOUT/4]  |= (1 << 18);

// Set Output pin for GPIO0 as 29
    pinconf0[OE_ADDR/4] &= (0xFFFFFFFF ^ (1 << 29));

// Toggle CNVST to start
    
    unsigned long gpio0Array[samplenum]; //Creates array with 3.2 million samples

    int t = 0;

    while(t < samplenum) //takes 320 million samples
    {
        pinconf1[GPIO_DATAOUT/4] ^= (1 << 18); //Pulse CVNST
        pinconf1[GPIO_DATAOUT/4] |= (1 << 18);

        //While the busy pin is high, do nothing
        while( (pinconf1[GPIO_DATAIN/4] & (1 << 13)));
	gpio0Array[t] = pinconf0[GPIO_DATAIN/4];
	    t++;
    }


// open file to write

    FILE *pFile;
    
    
    pFile = fopen("baseline2_dump.txt", "w");
    t = 0; // Sample counter

    while(t < samplenum){
    	fprintf(pFile, "%lu", gpio0Array[t]);
    	fprintf(pFile, "\n");
    	t++;
    }

// Close write file
    fclose(pFile);
    return 0;
}

