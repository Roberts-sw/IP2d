/* ip2d.c
   -------------------------------------------------------------------------
Auteur: R. van Lierop
Wijzigingen:
    RvL 26 aug 2021 * opzet
    RvL 27 aug 2021 * vereenvoudiging
   ---------------------------------------------------------------------- */
#include "ip2d.h"

    /* -----------------------------------------------------
    private
    ----------------------------------------------------- */
#define mdr(x,y,z)  ((x)*(y)+(z)/2)/(z)
static u16 y2_rdr (u16 *y, u16 **_t)
{   if(!_t || !*_t)     return 0;
    u16 K=0, dr=0, k0, k1, *_r=*_t;
    for(k0=*_r++; k1=*_r++; k0=k1)      //.a: oplopend per kolom en ongelijk 0
        if(k0>=k1)      return 0;
    if(!*_r)            return 0;       //.c: geen gegevens
    K=_r-*_t;                           //K >= 2
    _r+=K-1;                            //.b laatste kolom
    while(_r[1])                        //.c: test op tabeleinde
        if(*y<_r[K])                    //... + volgende rij
        {   dr=mdr((s32)*y-_r[0], 65536, _r[K]-_r[0]);
		    break;
        } else _r+=K;                   //(ga naar volgende rij)
    return *_t=_r, *y=dr, K;
}

    /* -----------------------------------------------------
    public
    ----------------------------------------------------- */
u16 yx2z (u16 y, u16 x, u16 const *const tab)
{   //1. bepaal aantal kolommen, rij met y<rijkop, positie tov. rij:
    u16 *_r=(u16*)tab;
    u16 K=y2_rdr(&y, &_r);
    if(!K)              return 0;

    //2. interpoleer in gegevenskolommen:
    u16 k;
    for(k=0, _r-=K-1; k<K-2 && tab[k+1]<=x; ++k)
        ++_r;
    u16 k0=_r[0]+(s32)(_r[K+0]-_r[0])*y/65536;
    if(k<K-2 && tab[k]<=x)
    {   u16 k1=_r[1]+(s32)(_r[K+1]-_r[1])*y/65536;
        k=k0+mdr((s32)x-tab[k+0], k1-k0, tab[k+1]-tab[k+0]);
    } else k=k0;
    return k;
}

u16 yz2x (u16 y, u16 z, u16 const *const tab)
{   //1. bepaal aantal kolommen, rij met y<rijkop, positie tov. rij:
    u16 *_r=(u16*)tab;
    u16 K=y2_rdr(&y, &_r);
    if(!K)              return 0;

    //2. zoek in de gegevenstabel de aangrenzende kolommen:
    u16 k, k0, k1=0;
    _r-=K-1;k0=_r[0]+(s32)(_r[K+0]-_r[0])*y/65536;
    for(k=0; k<K-2; ++k, k0=k1, ++_r)
        if((k1=_r[1]+(s32)(_r[K+1]-_r[1])*y/65536)>z)
            break;

    //3. interpoleer resultaat ahv. kolomkoppen:
    if(k0<z && z<k1)
    {   y=mdr((s32)(z-k0), 65536, k1-k0);
        y=(s32)(tab[k+1]-tab[k])*y/65536;
    } else y=0;
    return tab[k]+y;
}
