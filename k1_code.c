#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int s[16] = {0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8,
             0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7};

int perm[16] = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};

int probas[16][16];

int s_inv[16];
int perm_inv[16];

void compute_probas()
{
  int x, deltaIN, deltaOUT;

  for (deltaIN = 0; deltaIN <= 15; deltaIN++)
  {
    for (deltaOUT = 0; deltaOUT <= 15; deltaOUT++)
    {
      probas[deltaIN][deltaOUT] = 0;
    }
  }

  for (deltaIN = 0; deltaIN <= 15; deltaIN++)
  {
    for (x = 0; x <= 15; x++)
    {
      deltaOUT = s[x] ^ s[x ^ deltaIN];
      probas[deltaIN][deltaOUT]++;
    }
  }
}

void print_probas()
{
  int i, delta_in, delta_out;
  char ligne[16 * 3 + 4 + 1];

  // Print the first line
  printf("   |");
  for (delta_out = 0; delta_out < 16; delta_out++)
    printf(" %x ", delta_out);
  printf("\n");

  for (i = 0; i < 16 * 3 + 4; i++)
    ligne[i] = '-';
  ligne[16 * 3 + 4] = 0;
  printf("%s\n", ligne);

  for (delta_in = 0; delta_in < 16; delta_in++)
  {
    printf(" %x |", delta_in);
    for (delta_out = 0; delta_out < 16; delta_out++)
      printf("%2d ", probas[delta_in][delta_out]);
    printf("\n");
  }
}

void init_inverse_ops()
{
  // This function populates  s_inv et perm_inv to make decryption possible.
  // Of course, it should be called BEFORE any decryption operation.

  int i;
  for (i = 0; i <= 15; i++)
  {
    s_inv[s[i]] = i;
    perm_inv[perm[i]] = i;
  }
}

int main()
{
  compute_probas();
  // print_probas ();

  init_inverse_ops();

  FILE *fisier1;
  fisier1 = fopen("plaintext-ciphertext.txt", "r");
  fseek(fisier1, 0, 0);

  int i = 0;
  int k1_delta_in[1][2];

  fscanf(fisier1, "%x", &i);
  k1_delta_in[0][0] = i;
  fseek(fisier1, 1, SEEK_CUR);
  fscanf(fisier1, "%x", &i);
  k1_delta_in[0][1] = i;

  fclose(fisier1);

  int T, Tprime, Uprime;
  int part1, part2, part3, part4;

  int k5 = 0x5d8e; // to modify in the non null folder
  int k4 = 0xd6bf;
  int k3 = 0x7c76;
  int k2 = 0x14dc;

  part1 = 0;
  i = 0;

  T = k1_delta_in[0][0];
  Tprime = k1_delta_in[0][1] ^ k5;

  part1 = (Tprime >> 12) & 0xF;
  part2 = (Tprime >> 8) & 0xF;
  part3 = (Tprime >> 4) & 0xF;
  part4 = Tprime & 0xF;
  part1 = s_inv[part1];
  part2 = s_inv[part2];
  part3 = s_inv[part3];
  part4 = s_inv[part4];
  Uprime = (part1 << 12) | (part2 << 8) | (part3 << 4) | part4;

  Uprime = Uprime ^ k4;

  Tprime = 0;
  for (i = 0; i <= 15; i++)
  {
    int bit_value = (Uprime >> i) & 0x1;
    Tprime |= (bit_value << perm_inv[i]);
  }

  part1 = (Tprime >> 12) & 0xF;
  part2 = (Tprime >> 8) & 0xF;
  part3 = (Tprime >> 4) & 0xF;
  part4 = Tprime & 0xF;
  part1 = s_inv[part1];
  part2 = s_inv[part2];
  part3 = s_inv[part3];
  part4 = s_inv[part4];
  Uprime = (part1 << 12) | (part2 << 8) | (part3 << 4) | part4;

  Uprime = Uprime ^ k3;

  Tprime = 0;
  for (i = 0; i <= 15; i++)
  {
    int bit_value = (Uprime >> i) & 0x1;
    Tprime |= (bit_value << perm_inv[i]);
  }

  part1 = (Tprime >> 12) & 0xF;
  part2 = (Tprime >> 8) & 0xF;
  part3 = (Tprime >> 4) & 0xF;
  part4 = Tprime & 0xF;
  part1 = s_inv[part1];
  part2 = s_inv[part2];
  part3 = s_inv[part3];
  part4 = s_inv[part4];
  Uprime = (part1 << 12) | (part2 << 8) | (part3 << 4) | part4;

  Uprime = Uprime ^ k2;

  Tprime = 0;
  for (i = 0; i <= 15; i++)
  {
    int bit_value = (Uprime >> i) & 0x1;
    Tprime |= (bit_value << perm_inv[i]);
  }


  part1 = (Tprime >> 12) & 0xF;
  part2 = (Tprime >> 8) & 0xF;
  part3 = (Tprime >> 4) & 0xF;
  part4 = Tprime & 0xF;
  part1 = s_inv[part1];
  part2 = s_inv[part2];
  part3 = s_inv[part3];
  part4 = s_inv[part4];
  Uprime = (part1 << 12) | (part2 << 8) | (part3 << 4) | part4;

  int k1;
  k1 = T ^ Uprime;
  printf("After the first step, the guess is that k1=%x\n", k1);

  return 0;
}