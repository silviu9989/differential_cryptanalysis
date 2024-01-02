#!/usr/bin/env python3

###########################################################
# calcul_probas.py : program to compute the probabilities #
# of the S-Boxes, needed to assess the quality of the     #
# differential paths                                      #
###########################################################

import sys


s = [0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7]

probas = []


def compute_probas():
    global probas, s



def print_probas():
    # Print the first line
    sys.stdout.write("   |")
    for delta_out in range(16):
        sys.stdout.write(" %x " % delta_out)
    sys.stdout.write("\n")

    sys.stdout.write("-" * (16 * 3 + 4))
    sys.stdout.write("\n")

    for delta_in in range(16):
        sys.stdout.write(" %x |" % delta_in)
        for delta_out in range(16):
            sys.stdout.write("%2d " % probas[delta_in][delta_out])
        sys.stdout.write("\n")


compute_probas()
print_probas()
