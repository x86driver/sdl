#include <stdlib.h>
#include <stdio.h>

long big5(long Code)
{
    long Offset, LowCode;
    LowCode = Code & 0xff;
    if(LowCode > 0x80) LowCode = (0x7e -0x40) + (LowCode -0xA1+1);
    else LowCode -= 0x40;
    Offset = (((Code>>8) - 0xA4) * (0x7E - 0x40 + 1 + 0xfe - 0xa1 + 1) + LowCode)*30;
    return(Offset);
}

long big5_spc(long Code)
{
    long Offset, LowCode;
    LowCode = Code & 0xff;
    if(LowCode > 0x80) LowCode = (0x7e -0x40) + (LowCode -0xA1+1);
    else LowCode -= 0x40;
    Offset = (((Code>>8) - 0xA1) * (0x7E - 0x40 + 1 + 0xfe - 0xa1 + 1) + LowCode)*30;
    return(Offset);
}

void draw_16x15(unsigned char buf[30])
{
    int i, len;
    for (len = 0; len < 15; ++len) {
	for (i = 0; i < 8; ++i) {
	    if ((buf[len*2] & 0x80) != 0) {
		printf("██");
	    } else {
		printf("　");
	    }
	    buf[len*2] <<= 1;
	}

	
	for (i = 0; i < 8; ++i) {
	    if ((buf[len*2+1] & 0x80) != 0) {
		printf("██");
	    } else {
		printf("　");
	    }
	    buf[len*2+1] <<= 1;
	}
	
	printf("\n");
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    long offset = 0;
    if (argc != 2) {
	printf("Usage: %s offset\n", argv[0]);
	return 0;
    }

    unsigned char buf[30] = {0};
    FILE *fp = fopen("SPCFSUPP.15", "rb");
    //offset = big5_spc(atoi(argv[1]));
    offset = atoi(argv[1]);
    fseek(fp, offset, SEEK_SET);
    fread(&buf, 30, 1, fp);
    fclose(fp);

    printf("Offset: %u\n", offset);

    draw_16x15(buf);
    return 0;
}
