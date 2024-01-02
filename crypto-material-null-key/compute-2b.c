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
    fisier1 = fopen("pairs-k3_delta_in_0220.txt", "r");
    fseek(fisier1, 0, 0);

    int i=0; 
    int a=0, b=0;
    int k3_delta_in_0220[1500][2];

    fscanf (fisier1, "%x", &i);
    k3_delta_in_0220[0][0] = i;

    while (!feof (fisier1))
    {  
        b=1-b;
        if(b == 0) a++;
        fseek(fisier1, 1, SEEK_CUR);
        fscanf (fisier1, "%x", &i);
        k3_delta_in_0220[a][b] = i;
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
    int aux=0;
    int k5=0x0000; //to modify in the non null folder
    int k4=0x0000;
    part1=0; i=0;
    int file=0;

    while(k<=0x5555)
    {
      aux=k>>8;
      line= (aux&0x1) + ((aux&0x4)>>1) + ((aux&0x10)>>2) + ((aux&0x40)>>3);
      aux=k%0x100;
      file=(aux&0x1) + ((aux&0x4)>>1) + ((aux&0x10)>>2) + ((aux&0x40)>>3);

        for(a=0; a < 1600; a++)
        {
            T=k3_delta_in_0220[a][0] ^ k5;
            Tprime=k3_delta_in_0220[a][1] ^ k5;

            part1 = (T >> 12) & 0xF;
            part2 = (T >> 8) & 0xF;
            part3 = (T >> 4) & 0xF;
            part4 = T & 0xF;
            part1 = s_inv[part1];
            part2 = s_inv[part2];
            part3 = s_inv[part3 ];
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

            U = U^k4;
            Uprime = Uprime ^ k4;

            T=0;
            for (i = 0; i <= 15; i++)
            {
              int bit_value = (U >> i) & 0x1;
              T |= (bit_value << perm_inv[i]);
            }

            Tprime=0;
            for (i = 0; i <= 15; i++)
            {
              int bit_value = (Uprime >> i) & 0x1;
              Tprime |= (bit_value << perm_inv[i]);
            }

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

            U = U ^ k;
            Uprime = Uprime ^ k;

            T=0;
            for (i = 0; i <= 15; i++)
            {
              int bit_value = (U >> i) & 0x1;
              T |= (bit_value << perm_inv[i]);
            }

            Tprime=0;
            for (i = 0; i <= 15; i++)
            {
              int bit_value = (Uprime >> i) & 0x1;
              Tprime |= (bit_value << perm_inv[i]);
            }

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
                mat_prob[line][file]++;
            }
        }

        k++;
        while((k & 0xaaaa) != 0) k++;
    }

    int max=0, row, column;
    for(i=0; i<16;i++)
    {
        for(part1=0; part1<16; part1++)
        {
            printf("%i ", mat_prob[i][part1]); //PRINT LE MATRIX OF POINTS
            
            if(max < mat_prob[i][part1])
            {
                row=i;
                column=part1;
                max=mat_prob[i][part1];
            }
        }
        printf("\n");
    }

    int guessed_k=(row<<8)+column;
    printf("After the first step, the guess is that k=%x. Keep in mind, we only know the value of the 0:3 and 8:11 bits. The rest are 0, for now...\n", guessed_k);


    int k3_delta_in_1010[8000][2];
    FILE* fisier2;
    fisier2 = fopen("pairs-k3_delta_in_1010.txt", "r"); 
    fscanf (fisier2, "%x", &i);
    k3_delta_in_1010[0][0] = i;
    a=0;
    b=0;

    while (!feof (fisier2))
    {  
        b=1-b;
        if(b == 0) a++;
        fseek(fisier2, 1, SEEK_CUR);
        fscanf (fisier2, "%x", &i);
        k3_delta_in_1010[a][b] = i;
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
    aux=0;
    part1=0; i=0;
    file=0;
    k=0; line=0;

    while(k<=0xaaaa)
    {
        for(a=0; a < 8000; a++)
        {
          aux=k>>8;
          line= ((aux&0x2)>>1) + ((aux&0x8)>>2) + ((aux&0x20)>>3) + ((aux&0x80)>>4);
          aux=k%0x100;
          file= ((aux&0x2)>>1) + ((aux&0x8)>>2) + ((aux&0x20)>>3) + ((aux&0x80)>>4);
                
            T=k3_delta_in_1010[a][0] ^ k5;
            Tprime=k3_delta_in_1010[a][1] ^ k5;

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

            U = U ^ k;
            Uprime = Uprime ^ k;

            T=0;
            for (i = 0; i <= 15; i++)
            {
              int bit_value = (U >> i) & 0x1;
              T |= (bit_value << perm_inv[i]);
            }

            Tprime=0;
            for (i = 0; i <= 15; i++)
            {
              int bit_value = (Uprime >> i) & 0x1;
              Tprime |= (bit_value << perm_inv[i]);
            }

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
                mat_prob2[line][file]++;
            }
        }

        k++;
        while((k & 0x5555) != 0) k++;
    }
    
    max=0;
    for(i=0; i<16; i++)
    {
        for(part1=0; part1<16; part1++)
        {
            printf("%i ", mat_prob2[i][part1]); //PRINT LE MATRIX OF POINTS
            
            if(max < mat_prob2[i][part1])
            {
                row=i;
                column=part1;
                max=mat_prob2[i][part1];
            }
        }
        printf("\n");
    }

    int guessed_k2=(row<<12)+(column<<4);
    printf("After the second step, the guess is that k=%x.\n", guessed_k2);
    int k3=guessed_k+guessed_k2;
    printf("Adding them up, the guess is that k3=%x.\n", k3);

    return 0;
}
