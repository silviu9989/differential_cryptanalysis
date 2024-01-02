############################################
# minicipher.py : skeleton file for part 1 #
############################################


##############
# Encryption #
##############


# S-Box Definition
s = [0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7]


# Permutation Definition
perm = [0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15]


# Execution of one encryption round (TODO)
def encrypt_round(input, key, do_perm):
    """encrypt_round
    Parameters
    ----------
    input   : the input block
    key     : the sub-key for this round
    do_perm : if True, perform the permutation (it is False for the last round)

    Output
    ------
    the output value for the round
    """

    # Subkey Addition

    # S-Box Application

    # If no permutation is performed, we can return the result early

    # Else, we perform the permutation



# Complete Encryption (TODO)
def encrypt(plaintext, keys):
    """encrypt
    Parameters
    ----------
    plaintext : input plaintext to encrypt
    keys      : array containing the 5 subkeys (i.e. the complete key)

    Output
    ------
    the encrypted value
    """




##############
# Decryption #
##############


# Inverse operations for S-Box and Permutation
s_inv = [0] * 16
perm_inv = [0] * 16


# Compute the reverse S-Box and Permutation (TODO)


def init_inverse_ops():
    """This function populates  s_inv et perm_inv to make decryption possible.
    Of course, it should be called BEFORE any decryption operation.
    """



# One Decryption round (TODO)
def decrypt_round(input, key, do_perm):
    """decrypt_round
    Parameters
    ----------
    input   : the ciphertext block to decrypt
    key     : round subkey
    do_perm : if True, perform the permutation

    Output
    ------
    The decrypted plaintext value
    """



# Complete Decryption (TODO)
def decrypt(ciphertext, keys):
    """decrypt
    Parameters
    ----------
    ciphertext : ciphertext to decrypt
    keys       : array containing the 5 subkeys (i.e. the complete key)

    Output
    ------
    The decrypted plaintext
    """

