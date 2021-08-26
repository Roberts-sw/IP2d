/* ip2d.c
   -------------------------------------------------------------------------
Auteur: R. van Lierop
Wijzigingen:
    RvL 26 aug 2021 * opzet
   ---------------------------------------------------------------------- */
#include "ip2d.h"

    /* -----------------------------------------------------
    private
    ----------------------------------------------------- */
#define mdr(x,y,z)  ((x)*(y)+(z)/2)/(z)
static u16 kolommen (u16 const *const tab)
{   if(!tab || !*tab)   return 0;
    u16 *_r=(u16*)tab, k0, k1;
    for(k0=*_r++; k1=*_r++; k0=k1)      //.a: oplopend per kolom en ongelijk 0
        if(k0>=k1)      return 0;
    return *_r ? _r-tab : 0;            // >= 2 of .c: geen gegevens
}

    /* -----------------------------------------------------
    public
    ----------------------------------------------------- */
u16 yx2z (u16 y, u16 x, u16 const *const tab)
{   //1. bepaal aantal kolommen, rij met y<rijkop, positie tov. rij:
    u16 K=kolommen(tab);
    if(!K)              return 0;
    u16 *_r=(u16*)tab+K-1;              //.b laatste kolom
    do  _r+=K;                          //(volgende rij)
    while(_r[1] && _r[K]<=y);           //.c: test op tabeleinde + volgende rij
    u16 dr=_r[1] && y<_r[K] ? mdr((s32)y-_r[0], 65536, _r[K]-_r[0]) : 0;

    //2. interpoleer in gegevenskolommen:
    for(_r-=K-1, y=0; y<K-2 && tab[y+1]<=x; ++y)
        ++_r;
    u16 k0=_r[0]+(s32)(_r[K+0]-_r[0])*dr/65536;
    if(tab[y]<=x && y<K-2)
    {   u16 k1=_r[1]+(s32)(_r[K+1]-_r[1])*dr/65536;
        y=k0+mdr((s32)x-tab[y+0], k1-k0, tab[y+1]-tab[y+0]);
    } else y=k0;
    return y;
}

u16 yz2x (u16 y, u16 z, u16 const *const tab)
{   //1. bepaal aantal kolommen, rij met y<rijkop, positie tov. rij:
    u16 K=kolommen(tab);
    if(!K)              return 0;
    u16 *_r=(u16*)tab+K-1;              //.b laatste kolom
    do  _r+=K;                          //(volgende rij)
    while(_r[1] && _r[K]<=y);           //.c: test op tabeleinde + volgende rij
    u16 dr=_r[1] && y<_r[K] ? mdr((s32)y-_r[0], 65536, _r[K]-_r[0]) : 0;

    //2. zoek in de gegevenstabel de aangrenzende kolommen:
    u16 k0, k1=0;
    _r-=K-1; y=0;  k0=_r[0]+(s32)(_r[K+0]-_r[0])*dr/65536;
    for(; y<K-2 &&(k1=_r[1]+(s32)(_r[K+1]-_r[1])*dr/65536)<=z; ++y)
        k0=k1, ++_r;
    dr=(k0<z && z<k1) ? mdr((s32)(z-k0), 65536, k1-k0) : 0;
    
    //3. bereken resultaat ahv. kolomkoppen:
    y=tab[y]+(s32)(tab[y+1]-tab[y])*dr/65536;
    return y;
}
