#include "joystick.h"

void adc_init(joystick_cfg_t* cfg) {
    // --- 1. Enable clocks ---
    RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;   // Enable ADC clock

    // Enable required GPIO ports dynamically
    for (uint8_t i = 0; i < config->num_pins; i++) {
        if (config->pins[i].port == GPIOA) RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
        if (config->pins[i].port == GPIOB) RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
        if (config->pins[i].port == GPIOC) RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
        // (extend for GPIOD/E if needed)

        // Configure pin as Analog mode
        config->pins[i].port->MODER |= (3U << (config->pins[i].pin * 2));
        config->pins[i].port->PUPDR &= ~(3U << (config->pins[i].pin * 2));
    }

    // --- 2. ADC clock source (SYSCLK for simplicity) ---
    RCC->CCIPR &= ~RCC_CCIPR_ADCSEL;  // 00: SYSCLK

    // --- 3. Calibration ---
    if (ADC1->CR & ADC_CR_ADEN) {
        ADC1->CR |= ADC_CR_ADDIS;         // Disable if enabled
    }
    while (ADC1->CR & ADC_CR_ADEN);

    ADC1->CR |= ADC_CR_ADCAL;             // Start calibration
    while (ADC1->CR & ADC_CR_ADCAL);

    // --- 4. Enable ADC regulator ---
    ADC1->CR &= ~ADC_CR_ADVREGEN;
    ADC1->CR |= ADC_CR_ADVREGEN_0;
    for (volatile int i = 0; i < 1000; i++);

    // --- 5. Enable ADC ---
    ADC1->ISR |= ADC_ISR_ADRDY;
    ADC1->CR |= ADC_CR_ADEN;
    while (!(ADC1->ISR & ADC_ISR_ADRDY));

    // Default config: 12-bit, single conversion
    ADC1->CFGR &= ~(ADC_CFGR_CONT);
}

uint16_t adc_read(joystick_cfg_t* cfg, GPIO_Pin_t pin) {    
  // Configure sequence for 1 channel
  ADC1->SQR1 = 0;
  ADC1->SQR1 |= (channel << 6);  // SQ1 = channel
  ADC1->SQR1 |= (0U << 0);       // L=0 → 1 conversion

  // Start conversion
  ADC1->CR |= ADC_CR_ADSTART;
  while (!(ADC1->ISR & ADC_ISR_EOC));
  return (uint16_t)ADC1->DR;
}

Direction joystick_get_direction(){
    float angle = joystick_get_angle();  // 0 to 360, -1 for centred

    Direction d;
    // partition 360 into segments and check which segment the angle is in
    if (angle < 0.0f) {
        d = CENTRE;   // check for -1.0 angle
    } else if (angle < 22.5f) {  // then keep going in 45 degree increments
        d = N;
    } else if (angle < 67.5f) {
        d = NE;
    } else if (angle < 112.5f) {
        d = E;
    } else if (angle < 157.5f) {
        d = SE;
    } else if (angle < 202.5f) {
        d = S;
    } else if (angle < 247.5f) {
        d = SW;
    } else if (angle < 292.5f) {
        d = W;
    } else if (angle < 337.5f) {
        d = NW;
    } else {
        d = N;
    }
    return d;
}

// this method gets the magnitude of the joystick movement
float joystick_get_mag(){
    Polar p = joystick_get_polar();
    return p.mag;
}

// this method gets the angle of joystick movement (0 to 360, 0 North)
float joystick_get_angle(){
    Polar p = joystick_get_polar();
    return p.angle;
}

// get raw joystick coordinate in range -1 to 1
// Direction (x,y)
// North     (0,1)
// East      (1,0)
// South     (0,-1)
// West      (-1,0)
Vector2D joystick_get_coord(){
    // read() returns value in range 0.0 to 1.0 so is scaled and centre value
    // substracted to get values in the range -1.0 to 1.0
    float x = 2.0f*( horiz->read() - _x0 );
    float y = 2.0f*( vert->read() - _y0 );

    // Note: the values are negated so postive is up (y).
    Vector2D coord = {x,-y};
    return coord;
}

// This maps the raw x,y coord onto a circular grid.
// See:  http://mathproofs.blogspot.co.uk/2005/07/mapping-square-to-circle.html
Vector2D joystick_get_mapped_coord(){
    Vector2D coord = joystick_get_coord();

    // do the transformation
    float x = coord.x*sqrt(1.0f-pow(coord.y,2.0f)/2.0f);
    float y = coord.y*sqrt(1.0f-pow(coord.x,2.0f)/2.0f);

    Vector2D mapped_coord = {x,y};
    return mapped_coord;
}

// this function converts the mapped coordinates into polar form
Polar joystick_get_polar(){
    // get the mapped coordinate
    Vector2D coord = joystick_get_mapped_coord();

    // at this point, 0 degrees (i.e. x-axis) will be defined to the East.
    // We want 0 degrees to correspond to North and increase clockwise to 359
    // like a compass heading, so we need to swap the axis and invert y
    float x = coord.y;
    float y = coord.x;

    float mag = sqrt(x*x+y*y);  // pythagoras
    float angle = RAD2DEG*atan2(y,x);
    // angle will be in range -180 to 180, so add 360 to negative angles to
    // move to 0 to 360 range
    if (angle < 0.0f) {
        angle+=360.0f;
    }

    // the noise on the ADC causes the values of x and y to fluctuate slightly
    // around the centred values. This causes the random angle values to get
    // calculated when the joystick is centred and untouched. This is also when
    // the magnitude is very small, so we can check for a small magnitude and then
    // set the angle to -1. This will inform us when the angle is invalid and the
    // joystick is centred

    if (mag < TOL) {
        mag = 0.0f;
        angle = -1.0f;
    }

    Polar p = {mag,angle};
    return p;
}