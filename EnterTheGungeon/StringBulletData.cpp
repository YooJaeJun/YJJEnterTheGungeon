#include "stdafx.h"

namespace Gungeon
{
    void StringBulletData::SetStringBullet()
    {
        midForTargetFactor = 0.0f + inputString.size() * 0.1f;

        for (int i = 0; i < inputString.size(); i++)
        {
            switch (inputString[i])
            {
            case 'A':  copy(&A[0][0], &A[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'B':  copy(&B[0][0], &B[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'C':  copy(&C[0][0], &C[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'D':  copy(&D[0][0], &D[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'E':  copy(&E[0][0], &E[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'F':  copy(&F[0][0], &F[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'G':  copy(&G[0][0], &G[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'H':  copy(&H[0][0], &H[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'I':  copy(&I[0][0], &I[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'J':  copy(&J[0][0], &J[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'K':  copy(&K[0][0], &K[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'L':  copy(&L[0][0], &L[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'M':  copy(&M[0][0], &M[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'N':  copy(&N[0][0], &N[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'O':  copy(&O[0][0], &O[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'P':  copy(&P[0][0], &P[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'Q':  copy(&Q[0][0], &Q[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'R':  copy(&R[0][0], &R[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'S':  copy(&S[0][0], &S[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'T':  copy(&T[0][0], &T[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'U':  copy(&U[0][0], &U[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'V':  copy(&V[0][0], &V[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'W':  copy(&W[0][0], &W[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'X':  copy(&X[0][0], &X[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'Y':  copy(&Y[0][0], &Y[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'Z':  copy(&Z[0][0], &Z[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'a':  copy(&A[0][0], &A[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'b':  copy(&B[0][0], &B[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'c':  copy(&C[0][0], &C[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'd':  copy(&D[0][0], &D[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'e':  copy(&E[0][0], &E[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'f':  copy(&F[0][0], &F[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'g':  copy(&G[0][0], &G[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'h':  copy(&H[0][0], &H[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'i':  copy(&I[0][0], &I[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'j':  copy(&J[0][0], &J[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'k':  copy(&K[0][0], &K[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'l':  copy(&L[0][0], &L[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'm':  copy(&M[0][0], &M[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'n':  copy(&N[0][0], &N[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'o':  copy(&O[0][0], &O[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'p':  copy(&P[0][0], &P[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'q':  copy(&Q[0][0], &Q[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'r':  copy(&R[0][0], &R[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 's':  copy(&S[0][0], &S[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 't':  copy(&T[0][0], &T[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'u':  copy(&U[0][0], &U[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'v':  copy(&V[0][0], &V[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'w':  copy(&W[0][0], &W[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'x':  copy(&X[0][0], &X[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'y':  copy(&Y[0][0], &Y[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case 'z':  copy(&Z[0][0], &Z[0][0] + 25, &outputAlphbets[i][0][0]);           break;
            case '0':  copy(&num0[0][0], &num0[0][0] + 25, &outputAlphbets[i][0][0]);     break;
            case '1':  copy(&num1[0][0], &num1[0][0] + 25, &outputAlphbets[i][0][0]);     break;
            case '2':  copy(&num2[0][0], &num2[0][0] + 25, &outputAlphbets[i][0][0]);     break;
            case '3':  copy(&num3[0][0], &num3[0][0] + 25, &outputAlphbets[i][0][0]);     break;
            case '4':  copy(&num4[0][0], &num4[0][0] + 25, &outputAlphbets[i][0][0]);     break;
            case '5':  copy(&num5[0][0], &num5[0][0] + 25, &outputAlphbets[i][0][0]);     break;
            case '6':  copy(&num6[0][0], &num6[0][0] + 25, &outputAlphbets[i][0][0]);     break;
            case '7':  copy(&num7[0][0], &num7[0][0] + 25, &outputAlphbets[i][0][0]);     break;
            case '8':  copy(&num8[0][0], &num8[0][0] + 25, &outputAlphbets[i][0][0]);     break;
            case '9':  copy(&num9[0][0], &num9[0][0] + 25, &outputAlphbets[i][0][0]);     break;
            case ' ':  copy(&space[0][0], &space[0][0] + 25, &outputAlphbets[i][0][0]);   break;
                // case '¤»': copy(&kik[0][0], &kik[0][0] + 25, &outputAlphbets[i][0][0]);       break;
                // case '¤¾': copy(&hih[0][0], &hih[0][0] + 25, &outputAlphbets[i][0][0]);       break;
            }
        }
    }
}