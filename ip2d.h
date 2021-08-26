/* ip2d.h
   -------------------------------------------------------------------------
    (inverse) tabelinterpolatie in 2 dimensies
    
    Een te gebruiken tabel heeft rij- en kolomkoppen, en voldoet aan:
    .a De eerste rij bevat de kolomkoppen, per kolom oplopend/ongelijk 0,
        en wordt afgesloten met een 0 als extra kolom.
    .b Gegevensrijen eindigen met een kop als extra kolom, per rij oplopend.
    .c De tabel wordt afgesloten met een 0.
    NB: de eerste gegevensrij bevat geen waarde 0
    
    Voorbeeldtabel:

       | 5  | 10 |
    ---+----+----+
     2 | 14 | 21 |
     4 | 8  |  6 |
    ---+----+----+
    
    In het geheugen zou deze als volgt staan:
    {5, 10, 0, 14, 21, 2, 8, 6, 4, 0}, te lezen als:
    kolommen K=3, waarvan 1 als rijkoppen
    rijkoppen 2+3R 2 en 4, daarna eindigt tabel 


    Interpolatie wordt gedaan door:
    1. het aantal kolommen te bepalen
    2. in de rijen te interpoleren
    3. yx2z: in de kolommen te interpoleren en gegevenswaarde te bepalen
       yz2x: in de gegevens te interpoleren en de kolomwaarde te bepalen
        .c De tabel wordt afgesloten met een 0.
   ---------------------------------------------------------------------- */
#ifndef IP2D_H_
#define IP2D_H_

#include <stdint.h>
typedef int32_t     s32;
typedef uint16_t    u16;

u16 yx2z(u16 y, u16 x, u16 const *const tab);
u16 yz2x(u16 y, u16 z, u16 const *const tab);

#endif//IP2D_H_
