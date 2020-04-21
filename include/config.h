#define DEBUG true

#define ADC_SEED_PIN A0

// PINS definition
#define DOTMATRIX_DIN  16
#define DOTMATRIX_CS   14
#define DOTMATRIX_CLK  15
#define DOTMATRIX_DISPLAY_COUNT 1
#define BUZZER  10
#define LED_RED     3
#define LED_GREEN   5
#define LED_BLUE    6
#define LED_YELLOW  9
#define BUTTON_RED    2
#define BUTTON_GREEN  4
#define BUTTON_BLUE   7
#define BUTTON_YELLOW 8

// simon game definitions
#define CHOICE_OFF      0 //Used to control LEDs
#define CHOICE_NONE     0 //Used to check buttons
#define CHOICE_RED  (1 << 0)
#define CHOICE_GREEN    (1 << 1)
#define CHOICE_BLUE (1 << 2)
#define CHOICE_YELLOW   (1 << 3)

#define ROUNDS_TO_WIN      13 //Number of rounds to succesfully remember before you win. 13 is do-able.
#define ENTRY_TIME_LIMIT   3000 //Amount of time to press a button before game times out. 3000ms = 3 sec