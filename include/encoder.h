#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

//Rotary encoder rotation status
typedef enum {
    Encoder_Rotate_Increment,
    Encoder_Rotate_Decrement,
    Encoder_Rotate_Nothing    
} Encoder_Rotation_t;

//Mode for direction when it will be increment or decrement
typedef enum {
    Encoder_Mode_Normal, //Default
    Encoder_Mode_Reverse
} Encoder_Mode_t;

//The main encoder working structure
typedef struct {
    int32_t RE_Count;           //Temporary variable to store data between rotation and user check
    Encoder_Rotation_t Rotation;       //Rotation status
    Encoder_Mode_t Mode;             //Rotary encoder mode
    GPIO_TypeDef* GPIO_A;       //Pointer to GPIO port for encoder A pin
    GPIO_TypeDef* GPIO_B;       //Pointer to GPIO port for encoder B pin
    uint16_t GPIO_PIN_A;        //GPIO pin for rotary encoder A pin
    uint16_t GPIO_PIN_B;        //GPIO pin for rotary encoder B pin
} Encoder_t;

//Initiate the *data rotary encoder
void Encoder_Init(Encoder_t* data, 
                GPIO_TypeDef* GPIO_A_Port, 
                uint16_t GPIO_A_Pin, 
                GPIO_TypeDef* GPIO_B_Port, 
                uint16_t GPIO_B_Pin);

//Set the normal or reversed direction encoder mode
void Encoder_SetMode(Encoder_t* data, Encoder_Mode_t mode);

//Checks and gets the new values of rotary encoder
Encoder_Rotation_t Encoder_Get(Encoder_t* data);

//Process function. Call it frequently
void Encoder_Process(Encoder_t* data);