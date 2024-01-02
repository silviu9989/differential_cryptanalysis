/*******************************************
 * minicipher.c : skeleton file for part 1 *
 ******************************************/



/**************
 * Encryption *
 **************/


/* S-Box Definition */
/********************/

int s[16] = {0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7};



/* Permutation Definition */
/**************************/

int perm[16] = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};



/* Execution of one encryption round (TODO) */
/********************************************/

int encrypt_round (int input __attribute__((unused)), int key __attribute__((unused)), int do_perm __attribute__((unused))) {
  /*
     Parameters
     ----------
     input   : the input block
     key     : the sub-key for this round
     do_perm : if True, perform the permutation (it is False for the last round)

     Output
     ------
     the output value for the round
  */


  // Subkey Addition
  int i = 0;
  int result = 0;

  for (i = 0; i <= 15; i++) 
  {
    if (((input & (1 << i))>>i == 1) && ((key & (1 << i))>>i == 0))  
    {
        result |= (1 << i);
    }

    if (((input & (1 << i))>>i == 0) && ((key & (1 << i))>>i == 1)) 
    {
        result |= (1 << i);
    }
  }


  // S-Box Application


    int part1 = (result >> 12) & 0xF;
    int part2 = (result >> 8) & 0xF;
    int part3 = (result >> 4) & 0xF;
    int part4 = result & 0xF;

    part1 = s[part1];
    part2 = s[part2];
    part3 = s[part3];
    part4 = s[part4];

    result = (part1 << 12) | (part2 << 8) | (part3 << 4) | part4;

  // If no permutation is performed, we can return the result early
  if(do_perm == 0)
    return result;
  else
  {
  // Else, we perform the permutation
    int permuted_result=0;
    for (i = 0; i <= 15; i++)
    {
      int bit_value = (result >> i) & 0x1;
      permuted_result |= (bit_value << perm[i]);
    }
    return permuted_result;
  }
  return -1;
}



/* Complete Encryption (TODO) */
/******************************/

int minicipher_encrypt (int plaintext __attribute__((unused)), int keys[] __attribute__((unused))) {
  /*
    Parameters
    ----------
    plaintext : input plaintext to encrypt
    keys      : array containing the 5 subkeys (i.e. the complete key)

    Output
    ------
    the encrypted value
  */
  int encrypted_value=0;
  int i=0, result=0;

  encrypted_value = encrypt_round(plaintext, keys[i], 1);
  for(i=1; i<=3; i++)
  {
    if(i != 3)
      encrypted_value = encrypt_round(encrypted_value, keys[i], 1);
    else encrypted_value = encrypt_round(encrypted_value, keys[i], 0);
  }
  
  for (i = 0; i <= 15; i++) 
  {
    if (((encrypted_value & (1 << i))>>i == 1) && ((keys[4] & (1 << i))>>i == 0)) 
    {
        result |= (1 << i);
    }

    if (((encrypted_value & (1 << i))>>i == 0) && ((keys[4] & (1 << i))>>i == 1)) 
    {
        result |= (1 << i);
    }
  }

  return result;
}






/**************
 * Decryption *
 **************/


/* Inverse operations for S-Box and Permutation */
/*************************************************/

int s_inv[16];
int perm_inv[16];


/* Compute the reverse S-Box and Permutation (TODO) */
/****************************************************/

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



/* One Decryption round (TODO) */
/*******************************/

int decrypt_round (int input __attribute__((unused)), int key __attribute__((unused)), int do_perm __attribute__((unused))) {
  /*
    Parameters
    ----------
    input   : the ciphertext block to decrypt
    key     : round subkey
    do_perm : if True, perform the permutation

    Output
    ------
    The decrypted plaintext value
  */

    int i=0;
    int result=0;
    int out=0;

    if(do_perm == 1)
    {
      for (i = 0; i <= 15; i++)
      {
        int bit_value = (input >> i) & 0x1;
        result |= (bit_value << perm_inv[i]);
      }
    }
    else
    {
      result=input;
    }

    int part1 = (result >> 12) & 0xF;
    int part2 = (result >> 8) & 0xF;
    int part3 = (result >> 4) & 0xF;
    int part4 = result & 0xF;

    part1 = s_inv[part1];
    part2 = s_inv[part2];
    part3 = s_inv[part3];
    part4 = s_inv[part4];

    result = (part1 << 12) | (part2 << 8) | (part3 << 4) | part4;


    for (i = 0; i <= 15; i++) 
    {
      if (((result & (1 << i))>>i == 1) && ((key & (1 << i))>>i == 0))  
      {
          out |= (1 << i);
      }

      if (((result & (1 << i))>>i == 0) && ((key & (1 << i))>>i == 1)) 
      {
          out |= (1 << i);
      }
    }

    return out;
}



/* Complete Decryption (TODO) */
/******************************/

int minicipher_decrypt (int ciphertext __attribute__((unused)), int keys[] __attribute__((unused))) {
  /*
    Parameters
    ----------
    ciphertext : ciphertext to decrypt
    keys       : array containing the 5 subkeys (i.e. the complete key)

    Output
    ------
    The decrypted plaintext
  */
  init_inverse_ops();

  int decrypted_value=0;
  int i=0, result=0;

  
  for (i = 0; i <= 15; i++) 
  {
    if (((ciphertext & (1 << i))>>i == 1) && ((keys[4] & (1 << i))>>i == 0)) 
    {
        result |= (1 << i);
    }

    if (((ciphertext & (1 << i))>>i == 0) && ((keys[4] & (1 << i))>>i == 1)) 
    {
        result |= (1 << i);
    }
  }

  decrypted_value = decrypt_round(result, keys[3], 0);
  for(i=2; i>=0; i--)
  {
    decrypted_value = decrypt_round(decrypted_value, keys[i], 1);
  }

  return decrypted_value;

}
