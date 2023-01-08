#include "razclemba.h"

void Razclemba(unsigned long i_vrednost, boolean *o_RA1, boolean *o_RA2, boolean *o_RA3, int *o_zeljenavlaga1, int *o_zeljenavlaga2, int *o_zeljenavlaga3, int *o_zcas1,
 int *o_zcas2, int *o_zcas3, int *o_kcas1, int *o_kcas2, int *o_kcas3){

    unsigned long x = i_vrednost;

    //Prvo sporočilo
    if ((x / 1000000000) == 1)
    {
        x -= 1000000000;

        switch (x / 100000000)
        {
        case 1:
            x -= 100000000;
            *o_RA1 = manual_auto(x);
            *o_zeljenavlaga1 = zeljena_vaga(x);
            *o_zcas1 = cas_min(x);
            break;

        case 2:
            x -= 200000000;
            *o_RA2 = manual_auto(x);
            *o_zeljenavlaga2 = zeljena_vaga(x);
            *o_zcas2 = cas_min(x);
            break;

        case 3:
            x -= 300000000;
            *o_RA3 = manual_auto(x);
            *o_zeljenavlaga3 = zeljena_vaga(x);
            *o_zcas3 = cas_min(x);
            break;
        
        default:
            break;
        }
    }
    //Drugo sporočilo
    else if ((x / 1000000000) == 2)
    {
        x -= 2000000000;

        switch (x / 100000000)
        {
        case 1:
            x -= 100000000;
            *o_kcas1 = cas_min(x);
            break;

        case 2:
            x -= 200000000;
            *o_kcas2 = cas_min(x);
            break;

        case 3:
            x -= 300000000;
            *o_kcas3 = cas_min(x);
            break;
        
        default:
            break;
        }
    }
    
    

}

boolean manual_auto(unsigned long i_vrednost){
    if ((i_vrednost / 10000000) == 1)
    {
        return true;
    }
    else{
        return false;
    }
}

int zeljena_vaga(unsigned long i_vrednost){
    int x = i_vrednost / 10000;
    return(x % 1000);
}

int cas_min(unsigned long i_vrednost){
    int x = i_vrednost % 10000;
    x = (x / 100) * 60;
    int y = i_vrednost % 100;
    return(x + y);
}