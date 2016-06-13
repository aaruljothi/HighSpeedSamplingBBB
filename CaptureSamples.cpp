
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
// BUSY = GPIO1_13
// D0 = GPIO0_30
// D1 = GPIO0_31
// D2 = GPIO1_16
// d3 = GPIO0_5
// D4 = GPIO0_26
// D5 = GPIO0_3
// D6 = GPIO1_17
// D7 = GPIO0_12
// D8 = GPIO0_7
// D9 = GPIO0_13
// D10 = GPIO1_12
// D11 = GPIO0_20
// D12 = GPIO1_15
// D13 = GPIO1_28
// D14 = GPIO0_4
// D15 = GPIO0_27


//Function to write int to file using fwrite()


int main(){


   int samplenum = 3200000;

// map /dev/mem to access
    int fd = open("/dev/mem",O_RDWR | O_SYNC);

// connect pinconf1 to gpio1 (repeat for all four)
    ulong* pinconf0 =  (ulong*) mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO0_ADDR);
    ulong* pinconf1 =  (ulong*) mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO1_ADDR);

// Set CNVST to only output
    pinconf1[OE_ADDR/4] &= (0xFFFFFFFF ^ (1 << 18));

// Set CNVST to HIGH
    pinconf1[GPIO_DATAOUT/4]  |= (1 << 18);

// Set Output pin for GPIO0 as 29
    pinconf0[OE_ADDR/4] &= (0xFFFFFFFF ^ (1 << 29));

// Toggle CNVST to start
    
	unsigned long * gpio0Array;
	unsigned long * gpio1Array;

	gpio0Array = new unsigned long[samplenum]; //Creates array with 3.2 million samples
    	gpio1Array = new unsigned long[samplenum];

    int t = 0;

//    clock_t timer = clock();

    while(t < samplenum) //takes 320 million samples
    {
        pinconf1[GPIO_DATAOUT/4] ^= (1 << 18); //Pulse CVNST
        pinconf1[GPIO_DATAOUT/4] |= (1 << 18);

        //While the busy pin is high, do nothing
        while( (pinconf1[GPIO_DATAIN/4] & (1 << 13)));
	gpio0Array[t] = pinconf0[GPIO_DATAIN/4] ;
        gpio1Array[t] = pinconf1[GPIO_DATAIN/4];
	    t++;
    }

//   cout << "Total time taken for 320k samples" << endl;
//   cout << clock() - timer << endl;
//   cout << "Clock speed:" << endl;
//   cout <<  CLOCKS_PER_SEC << endl;


// open file to write
//    ofstream writeFile;   

    FILE *pFile;

    pFile = fopen("/media/card/rawdata.txt", "w");
    t = 0;
    while(t < samplenum){
	fprintf(pFile, "%lu", gpio0Array[t]);
	fprintf(pFile, "\n");
	fprintf(pFile, "%lu", gpio1Array[t]);
	fprintf(pFile, "\n");
    	t++;
    }
    fclose(pFile);
	delete [] gpio0Array;
	delete [] gpio1Array;
    return 0;
}

