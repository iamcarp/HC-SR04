#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_LEN 80

int slobodno;
double IIRiterations[2], result[2];
double alpha = 1;
char c;
int iir_brojac= 0;
int brojac = 0;

double IIR(double );


int main()
{
    int file_desc;
    int a;
    char tmp[BUF_LEN];
    double udaljenost, filterUdaljenost;

    IIRiterations[0] = 0;
	result[0] = 0;
	result[1] = 0;

    printf("Pritisni enter za pocetak merenja\n");
    while(1)
    {
        c = getchar();
		if(c == 10)
			break;
    }

    while (1)
    {
        file_desc = open("/dev/gpio_driver", O_RDWR);
		if(file_desc < 0)
		{
			printf("Neuspesno otvaranje modula\n");
			return -1;
		}

		memset(tmp, 0, BUF_LEN);

        if(read(file_desc, tmp, BUF_LEN) != -1)
        {
            a = atoi(tmp);//vreme
            udaljenost = (double)a/58;//pretvaranje u cm
	
			brojac ++;
        }
        else
        {
            printf("File read error\n");
            return -1;
        }

		filterUdaljenost = IIR(udaljenost);
		close(file_desc);

		if(brojac == 400)
		{
			printf("Udaljenost: %.2lf cm\n", filterUdaljenost);
			brojac = 0;
		}

		usleep(200);
    }

    return 0;
}

double IIR(double d)
{
	if(iir_brojac!= 3)
		iir_brojac++;
	if((iir_brojac) == 1)
	{
		return d;
	}
	else if(iir_brojac== 2)
	{
		result[0] = d;
		IIRiterations[0] = d;
		return result[0];
	}
	IIRiterations[1] = d;
	result[1] = alpha * (result[0] + IIRiterations[1] - IIRiterations[0]);
    IIRiterations[0] = IIRiterations[1];
    result[0] = result[1];

    return result[1];
}

