#include <stdint.h>

#include "tm4c_cmsis.h"
#include "delay.h"

#define LED_RED   (1U << 1)
#define LED_BLUE  (1U << 2)
#define LED_GREEN (1U << 3)

//static int16_t x = -1;
//static uint32_t y = LED_RED | LED_GREEN;

//static int16_t sqr[] = {
//    1*1,
//    2*2,
//    3*3,
//    4*4
//};

typedef struct {
    uint8_t y;
    uint16_t x;
} Point;

static Point p1 = {
    123U,
    0x1234U
};
static Point p2;

typedef struct {
    Point top_left;
    Point bottom_right;
} Window;

typedef struct {
    Point corners[3];
} Triangle;

static Window w = {
    { 123U, 0x1234U },
    { 234U, 0x6789U }
};
static Window w2;
static Triangle t;

int main(void) {
    Point *pp;
    Window *wp;

    p1.x = sizeof(Point);
    p1.y = 0xAAU;

    w.top_left.x = 1U;
    w.bottom_right.y = 2U;

    t.corners[0].x = 1U;
    t.corners[2].y = 2U;

    p2 = p1;
    w2 = w;

    pp = &p1;
    wp = &w2;

    (*pp).x = 1U;

    (*wp).top_left = *pp;

    pp->x = 1U;
    wp->top_left = *pp;

    SYSCTL->RCGC2 |= (1U << 5);  /* enable clock for GPIOF */
    SYSCTL->GPIOHSCTL |= (1U << 5); /* enable AHB for GPIOF */

    /* make sure the Run Mode and AHB-enable take effects
    * before accessing the peripherals
    */
    __ISB(); /* Instruction Synchronization Barrier */
    __DSB(); /* Data Memory Barrier */

    GPIOF_HS->DIR |= (LED_RED | LED_BLUE | LED_GREEN);
    GPIOF_HS->DEN |= (LED_RED | LED_BLUE | LED_GREEN);

    /* turn all LEDs off */
    GPIOF_HS->DATA_Bits[LED_RED | LED_BLUE | LED_GREEN] = 0U;

    GPIOF_HS->DATA_Bits[LED_BLUE] = LED_BLUE;
    while (1) {
        GPIOF_HS->DATA_Bits[LED_RED] = LED_RED;
        delay(500000);

        GPIOF_HS->DATA_Bits[LED_RED] = 0;

        delay(500000);
    }
    //return 0; // unreachable code
}

void Q_onAssert(char const *module, int loc) {
    /* TBD: damage control */
    (void)module; /* avoid the "unused parameter" compiler warning */
    (void)loc;    /* avoid the "unused parameter" compiler warning */
    NVIC_SystemReset();
}

