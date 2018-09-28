#include "common/conv.h"
#include <strings.h>

 //Maximun size of a string containing a 32 bit number without signal
#define AUXCONV_UINT32_STR_MAX  (STRLEN("4294967295")+STRLEN("\0"))
//Maximun size of a string containing a 32 bit number with signal
#define AUXCONV_INT32_STR_MAX   (STRLEN("-2147483647")+STRLEN("\0"))
//Maximun size of a string containing a 64 bit number without signal
#define AUXCONV_UINT64_STR_MAX  (STRLEN("18446744073709551615")+STRLEN("\0"))

res_t Conv_IntToStr( int32_t pVal, char_t *pStrTo, uint8_t pStrMaxLen )
{
    char_t str[AUXCONV_INT32_STR_MAX];
    char_t *ptr;
    bool_t neg = false;

    if( pVal < 0 )
    {
        neg = true;
        pVal = ( -pVal );
    }

    ptr = (char_t*)(str + STRLEN(str));
    *ptr = 0;
    do
    {
        ptr--;
        *ptr = pVal%10 + '0';
    }while(pVal/=10);

    if( neg )
    {
        ptr--;
        *ptr = '-';
    }

    if( strlen(ptr) > pStrMaxLen )
    {
        return RES_ERR_OVERFLOW;
    }
    else
    {
        strcpy( pStrTo, ptr );
        return RES_SUCCESS;
    }
}
