#include "ip2d.h"

u16 const TAB[] = {
    1019,   1488,   2008,   2507,   0,  //.a
    200,    234,    280,    325,    300,//.b
    185,    215,    256,    298,    600,//.b
    150,    170,    190,    210,    700,//.b
    0                                   //.c
};

int main (void)
{   u16 x = 1834;
    u16 z = yx2z(500, x, TAB);
    x = yz2x(500, z, TAB);
    return 0;
}
