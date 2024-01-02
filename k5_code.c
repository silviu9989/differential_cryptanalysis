#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int s[16] = {0xe,0x4,0xd,0x1,0x2,0xf,0xb,0x8,
	     0x3,0xa,0x6,0xc,0x5,0x9,0x0,0x7};

int perm[16] = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};

int probas[16][16];

int s_inv[16];
int perm_inv[16];


void compute_probas () {
  int x, deltaIN, deltaOUT;

  for(deltaIN=0; deltaIN<=15; deltaIN++)
  {
    for(deltaOUT=0; deltaOUT<=15; deltaOUT++)
    {
      probas[deltaIN][deltaOUT]=0;
    }
  }

  for(deltaIN=0; deltaIN<=15; deltaIN++)
  {
    for(x=0; x<=15; x++)
    {
      deltaOUT=s[x] ^ s[x ^ deltaIN];
      probas[deltaIN][deltaOUT]++;
    }
  }
}


void print_probas () {
  int i, delta_in,delta_out;
  char ligne[16*3 + 4 + 1];

  // Print the first line
  printf ("   |");
  for (delta_out=0; delta_out<16; delta_out++)
    printf (" %x ", delta_out);
  printf ("\n");

  for (i=0; i<16*3 + 4; i++)
    ligne[i]='-';
  ligne[16*3 + 4] = 0;
  printf ("%s\n", ligne);
  
  for (delta_in=0; delta_in<16; delta_in++) {
    printf (" %x |", delta_in);
    for (delta_out=0; delta_out<16; delta_out++)
      printf ("%2d ", probas[delta_in ][delta_out]);
    printf ("\n");
  }
}

void init_inverse_ops () {
  // This function populates  s_inv et perm_inv to make decryption possible.
  // Of course, it should be called BEFORE any decryption operation.

  int i;
  for(i=0; i<=15; i++)
  {
    s_inv[s[i]]=i;
    perm_inv[perm[i]]=i;
  }
}


int main () {
    compute_probas ();
    //print_probas ();

    init_inverse_ops();

    FILE* fisier1;
    fisier1 = fopen("pairs-k5_0b00_0606.txt", "r");
    fseek(fisier1, 0, 0);

    int i=0; 
    int a=0, b=0;
    int k5_0b00_0606[4000][2];

    fscanf (fisier1, "%x", &i);
    k5_0b00_0606[0][0] = i;

    while (!feof (fisier1))
    {  
        b=1-b;
        if(b == 0) a++;
        fseek(fisier1, 1, SEEK_CUR);
        fscanf (fisier1, "%x", &i);
        k5_0b00_0606[a][b] = i;
    } 
    fclose(fisier1);

    int k=0x0000;
    int T, Tprime, U, Uprime;
    int line=0;
    int part1, part2, part3, part4;
    int mat_prob[16][16];
    for(i=0; i<16; i++)
    {
        for(part1=0; part1<16; part1++)
        {
            mat_prob[i][part1]=0;
        }
    }
    part1=0; i=0;

    while(k<=0x0f0f)
    {
        for(a=0; a < 4000; a++)
        {
            T=k5_0b00_0606[a][0] ^ k;
            Tprime=k5_0b00_0606[a][1] ^ k;

            part1 = (T >> 12) & 0xF;
            part2 = (T >> 8) & 0xF;
            part3 = (T >> 4) & 0xF;
            part4 = T & 0xF;
            part1 = s_inv[part1];
            part2 = s_inv[part2];
            part3 = s_inv[part3];
            part4 = s_inv[part4];
            U = (part1 << 12) | (part2 << 8) | (part3 << 4) | part4;

            part1 = (Tprime >> 12) & 0xF;
            part2 = (Tprime >> 8) & 0xF;
            part3 = (Tprime >> 4) & 0xF;
            part4 = Tprime & 0xF;
            part1 = s_inv[part1];
            part2 = s_inv[part2];
            part3 = s_inv[part3];
            part4 = s_inv[part4];
            Uprime = (part1 << 12) | (part2 << 8) | (part3 << 4) | part4;

            if(((U ^ Uprime) & 0x0f0f) == 0x0606)
            {
                mat_prob[line][k%0x0010]++;
            }
        }

        k++;
        if(k%0x0010 == 0) {k=k+0x00f0; line++;}

    }

    int max=0, row, column;
    for(i=0; i<16;i++)
    {
        for(part1=0; part1<16; part1++)
        {
            //printf("%i ", mat_prob[i][part1]); //PRINT LE MATRIX OF POINTS
            
            if(max < mat_prob[i][part1])
            {
                row=i;
                column=part1;
                max=mat_prob[i][part1];
            }
        }
        //printf("\n");
    }

    int guessed_k=(row<<8)+column;
    printf("After the first step, the guess is that k=%x. Keep in mind, we only know the value of the 0:3 and 8:11 bits. The rest are 0, for now...\n", guessed_k);


    int k5_000d_a0a0[4000][2];
    FILE* fisier2;
    fisier2 = fopen("pairs-k5_000d_a0a0.txt", "r"); 
    fscanf (fisier2, "%x", &i);
    k5_000d_a0a0[0][0] = i;
    a=0;
    b=0;

    while (!feof (fisier2))
    {  
        b=1-b;
        if(b == 0) a++;
        fseek(fisier2, 1, SEEK_CUR);
        fscanf (fisier2, "%x", &i);
        k5_000d_a0a0[a][b] = i;
    } 
    fclose(fisier2);

    int mat_prob2[16][16];
    for(i=0; i<16; i++)
    {
        for(part1=0; part1<16; part1++)
        {
            mat_prob2[i][part1]=0;
        }
    }
    part1=0; i=0;

    k=0; line=0;
    while(k<=0xf0f0)
    {
        for(a=0; a < 4000; a++)
        {
            T=k5_000d_a0a0[a][0] ^ k;
            Tprime=k5_000d_a0a0[a][1] ^ k;

            part1 = (T >> 12) & 0xF;
            part2 = (T >> 8) & 0xF;
            part3 = (T >> 4) & 0xF;
            part4 = T & 0xF;
            part1 = s_inv[part1];
            part2 = s_inv[part2];
            part3 = s_inv[part3];
            part4 = s_inv[part4];
            U = (part1 << 12) | (part2 << 8) | (part3 << 4) | part4;

            part1 = (Tprime >> 12) & 0xF;
            part2 = (Tprime >> 8) & 0xF;
            part3 = (Tprime >> 4) & 0xF;
            part4 = Tprime & 0xF;
            part1 = s_inv[part1];
            part2 = s_inv[part2];
            part3 = s_inv[part3];
            part4 = s_inv[part4];
            Uprime = (part1 << 12) | (part2 << 8) | (part3 << 4) | part4;

            if(((U ^ Uprime) & 0xf0f0) == 0xa0a0)
            {
                mat_prob2[line][(k/0x10)%0x010]++;
            }
        }

        k=k+0x0010;
        if(k%0x0100 == 0) {k=k+0x0f00; line++;}
    }
    
    max=0;
    for(i=0; i<16; i++)
    {
        for(part1=0; part1<16; part1++)
        {
            //printf("%i ", mat_prob2[i][part1]); //PRINT LE MATRIX OF POINTS
            
            if(max < mat_prob2[i][part1])
            {
                row=i;
                column=part1;
                max=mat_prob2[i][part1];
            }
        }
        //printf("\n");
    }

    int guessed_k2=(row<<12)+(column<<4);
    printf("After the second step, the guess is that k=%x.\n", guessed_k2);
    int k5=guessed_k+guessed_k2;
    printf("Adding them up, the guess is that k5=%x.\n", k5);

    return 0;
}
