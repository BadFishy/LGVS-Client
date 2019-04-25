#include "jiami.h"
#include<cstring>

jiami::jiami(char* ch, int t)
{
    for (int i=0;i<strlen(ch);i++){
        ch[i]=~(ch[i]-t);
    }

}

jiemi::jiemi(char* ch, int t)
{
    for (int i=0;i<strlen(ch);i++){
        ch[i]=~ch[i]+t;
    }
}

