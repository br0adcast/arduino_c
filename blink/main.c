#include <avr/io.h>
#include <util/delay.h>

#if 1
#include <util/setbaud.h>
#include <stdio.h>

void uart_init(void) {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
}

#if 0
void uart_putchar(char c) {
    loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
    UDR0 = c;
}
#endif

void uart_putchar(char c,  FILE *stream) {
    if (c == '\n') {
        uart_putchar('\r', stream);
    }
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
}

char uart_getchar(FILE *stream) {
    loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
    return UDR0;
}

FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);
#endif


int main() {
#if 1
    uart_init();
    stdout = &uart_output;
    stdin  = &uart_input;
#endif
#if 0
    PORTB = 0x00;
    DDRB = 0x08;
#endif
#if 0
    int counter = 0;
    DDRB = 0xff;
    while (1) {
        PORTB = 0xff;
        _delay_ms(200);
        PORTB = 0x00;
        _delay_ms(200);
    }
#endif
#if 0
    DDRB = 0xff;
    while (1) {
        PORTB = 0xff;
        _delay_ms(200);
        PORTB = 0x00;
        _delay_ms(200);
    }
#endif

#if 1
    /**
     * We will be using OCR1A as our PWM output which is the
     * same pin as PB1.
    */
    //TCNT1 = 0x1ff;
    DDRB |= _BV(PB1);
 

    TCCR1A |= _BV(COM1A1) | _BV(WGM10) | _BV(WGM11)/* | _BV(WGM12)*/;
    TCCR1B |= _BV(CS10) | _BV(WGM12);

    uint16_t pwm = 0x00;
    int up = 1;
    for(;;) {
 
        OCR1A = pwm;
 
        pwm += up ? 1 : -1;
        if (pwm == 0x3ff)
            up = 0;
        else if (pwm == 0x000)
            up = 1;
        //printf("pwm = %i\n", (int)pwm);
#if 0
        uart_putchar('p');
        uart_putchar(' ');
        uart_putchar('w');
        uart_putchar(' ');
        uart_putchar('m');
        uart_putchar('\n');
#endif
        _delay_ms(1);
    }
#endif

#if 0
    /**
     * We will be using OCR1A as our PWM output which is the
     * same pin as PD6.
     */
    DDRD |= _BV(PD6);
 
    /**
     * There are quite a number of PWM modes available but for the
     * sake of simplicity we'll just use the 8-bit Fast PWM mode.
     * This is done by setting the WGM00 and WGM01 bits.  The 
     * Setting COM0A1 tells the microcontroller to set the 
     * output of the OCR0A pin low when the timer's counter reaches
     * a compare value (which will be explained below).  CS00 being
     * set simply turns the timer on without a prescaler (so at full
     * speed).  The timer is used to determine when the PWM pin should be
     * on and when it should be off.
     */
    TCCR0A |= _BV(COM0A1) | _BV(WGM00) | _BV(WGM01);
    TCCR0B |= _BV(CS00);
 
    /**
     *  This loop is used to change the value in the OCR0A register.
     *  What that means is we're telling the timer waveform generator
     *  the point when it should change the state of the PWM pin.
     *  The way we configured it (with _BV(COM0A1) above) tells the
     *  generator to have the pin be on when the timer is at zero and then
     *  to turn it off once it reaches the value in the OCR0A register.
     *
     *  Given that we are using an 8-bit mode the timer will reset to zero
     *  after it reaches 0xff, so we have 255 ticks of the timer until it
     *  resets.  The value stored in OCR0A is the point within those 255
     *  ticks of the timer when the output pin should be turned off
     *  (remember, it starts on).
     *
     *  Effectively this means that the ratio of pwm / 255 is the percentage
     *  of time that the pin will be high.  Given this it isn't too hard
     *  to see what when the pwm value is at 0x00 the LED will be off
     *  and when it is 0xff the LED will be at its brightest.
     */
    uint8_t pwm = 0x00;
    int up = 1;
    for(;;) {
 
        OCR0A = pwm;
 
        pwm += up ? 1 : -1;
        if (pwm == 0xff)
            up = 0;
        else if (pwm == 0x00)
            up = 1;
 
        _delay_ms(2);
    }
#endif


    return 0;
}

