// PWM Tone App
//
// Use PWM to play a tone over the speaker

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_pwm.h"

#include "microbit_v2.h"
#include "gpio.h"
#include "led_matrix.h"


#define ROW_KEY EDGE_P20
#define COL_KEY EDGE_P16

// PWM configuration
static const nrfx_pwm_t PWM_INST = NRFX_PWM_INSTANCE(0);

// Holds duty cycle values to trigger PWM toggle
nrf_pwm_values_common_t sequence_data[1] = {0};

// Sequence structure for configuring DMA
nrf_pwm_sequence_t pwm_sequence = {
  .values.p_common = sequence_data,
  .length = 1,
  .repeats = 0,
  .end_delay = 0,
};


static void pwm_init(void) {
  // Initialize the PWM
  // SPEAKER_OUT is the output pin, mark the others as NRFX_PWM_PIN_NOT_USED
  // Set the clock to 500 kHz, count mode to Up, and load mode to Common
  // The Countertop value doesn't matter for now. We'll set it in play_tone()
  nrfx_pwm_config_t pwm_config = {
    .output_pins = {SPEAKER_OUT, NRFX_PWM_PIN_NOT_USED, NRFX_PWM_PIN_NOT_USED, NRFX_PWM_PIN_NOT_USED},
    .base_clock = NRF_PWM_CLK_500kHz,
    .count_mode = NRF_PWM_MODE_UP,
    .load_mode = NRF_PWM_LOAD_COMMON,
    .step_mode = NRF_PWM_STEP_AUTO
  };
  nrfx_pwm_init(&PWM_INST, &pwm_config, NULL);
}

static void play_tone(uint16_t frequency) {
  // Stop the PWM (and wait until its finished)
  nrfx_pwm_stop(&PWM_INST, true);

  // Set a countertop value based on desired tone frequency
  // You can access it as NRF_PWM0->COUNTERTOP
  NRF_PWM0->COUNTERTOP = 500000 / frequency;

  // Modify the sequence data to be a 25% duty cycle
  sequence_data[0] = (500000 / 2) / frequency;

  // Start playback of the samples and loop indefinitely
  nrfx_pwm_simple_playback(&PWM_INST, &pwm_sequence, 1, NRFX_PWM_FLAG_LOOP);
}

uint16_t NOTES_ARRAY[] = {261.6256, 261.6256, 391.9954, 391.9954, 440, 440, 391.9954};
uint16_t BUTTONS_ARRAY[] = {0, 0, 4, 4, 5, 5, 4};
uint16_t ASCII_ARRAY[] = {67, 67, 71, 71, 65, 65, 71};
char CHAR_ARRAY[] = {'C', 'C', 'G', 'G', 'A', 'A', 'G'};
bool key_pressed[32];

/*
while(1){
  read all keys;
  if key is pressed, put true in the key_pressed array;
 }
*/


//initialize led matrix?
int la1[1];
int la2[1];
//map each piano key to a number 0 thru 5
// see what key the user pressed and compare it to BUTTONS_ARRAY[i]
// create function to map buttons to nums 0-5

  /*
while(1):
  microbit plays current note
  delay
  if user plays current note:
      pop current note
  delay
  

play_next_note():
pops first note in notes linked list
  */
  
int main(void) {
  printf("Board started!\n");

  // initialize PWM
  pwm_init();

  uint i  = 0;
  bool played_once = false;
  gpio_config(14, 0);
  gpio_config(23, 0);
  led_matrix_init();

  nrf_gpio_pin_dir_set(ROW_KEY, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(COL_KEY,  NRF_GPIO_PIN_DIR_OUTPUT);

  nrf_gpio_pin_write(ROW_KEY, 1);
  nrf_gpio_pin_write(COL_KEY, 1);
 


  while(1){

    while(i<sizeof(NOTES_ARRAY)/2){

    uint32_t row_key_value = nrf_gpio_pin_read(ROW_KEY);
    uint32_t col_key_value = nrf_gpio_pin_read(COL_KEY);
    printf("%ld\n", row_key_value);
    printf("%ld\n", col_key_value);
    printf("after\n");
  
      
      disp_char(CHAR_ARRAY[i]);
      if (!played_once | !gpio_read(23)){
	
        play_tone(NOTES_ARRAY[i]);
	nrf_delay_ms(1000);
	nrfx_pwm_stop(&PWM_INST, true);
	played_once = true;
	//if (!gpio_read(23)){
	//  disp_char('X');
	//}
	}
      nrf_delay_ms(100);
      printf("%d", !gpio_read(14));

      if (!gpio_read(14)){
      	played_once = false;
	i++;
      }

      if (i==sizeof(NOTES_ARRAY)/2){
	nrfx_pwm_stop(&PWM_INST, true);
      }
     

    
  }
  }

  /*
  for(int i = 0; i < sizeof(NOTES_ARRAY)/2; ++i){
    play_tone(NOTES_ARRAY[i]);
    nrf_delay_ms(500);
    }*/

  /*

  // C4 (Middle C)
  play_tone(261.6256);
  nrf_delay_ms(1000);

  // D4
  play_tone(293.6648);
  nrf_delay_ms(1000);

  // E4
  play_tone(329.6276);
  nrf_delay_ms(1000);

  //F4
  play_tone(349.2282);
  nrf_delay_ms(1000);

  //G4
  play_tone(391.9954);
  nrf_delay_ms(1000);

  //A4
  play_tone(440);
  nrf_delay_ms(1000);

  //B4
  play_tone(493.8833);
  nrf_delay_ms(1000);
  
  //C5
  play_tone(523.2511);
  nrf_delay_ms(1000);

  */

  


  // Stop all noises
  nrfx_pwm_stop(&PWM_INST, true);
}

