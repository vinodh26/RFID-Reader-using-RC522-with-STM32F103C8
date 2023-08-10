#include "stm32f1xx_hal.h" // Include the necessary STM32 HAL libraries

// RC522 Commands
#define RC522_CMD_IDLE         0x00
#define RC522_CMD_AUTHENT      0x0E
#define RC522_CMD_RECEIVE      0x08
#define RC522_CMD_TRANSMIT     0x04
#define RC522_CMD_TRANSCEIVE   0x0C
#define RC522_CMD_SOFT_RESET   0x0F

// RC522 Register Addresses
#define RC522_REG_COMMAND      0x01
#define RC522_REG_FIFO_DATA    0x09
#define RC522_REG_FIFO_LEVEL   0x0A

// RC522 Function Prototypes
void RC522_WriteRegister(uint8_t addr, uint8_t val);
uint8_t RC522_ReadRegister(uint8_t addr);
void RC522_SetBitMask(uint8_t reg, uint8_t mask);
void RC522_ClearBitMask(uint8_t reg, uint8_t mask);
void RC522_Reset();
void RC522_Init();
uint8_t RC522_Request(uint8_t* tagType);
uint8_t RC522_AntiCollision(uint8_t* serial);
void RC522_ReadCardSerial(uint8_t* serial);

// SPI Handler
SPI_HandleTypeDef hspi1;

int main(void)
{
    // Initialize the HAL library
    HAL_Init();

    // Initialize the RC522 module
    RC522_Init();

    // Variables to store tag information
    uint8_t tagType[2];
    uint8_t serial[5];

    while (1)
    {
        // Wait for a tag to be detected
        if (RC522_Request(tagType) == HAL_OK)
        {
            // Anti-collision to get the serial number of the tag
            if (RC522_AntiCollision(serial) == HAL_OK)
            {
                // Print the detected tag's serial number
                printf("Tag UID: %02X %02X %02X %02X %02X\r\n",
                       serial[0], serial[1], serial[2], serial[3], serial[4]);
            }
        }
    }
}


// Function to write data to RC522 register
void RC522_WriteRegister(uint8_t addr, uint8_t val)
{
    // Create a buffer to hold the data to be transmitted
    uint8_t data[2];
    data[0] = (addr << 1) & 0x7E; // Address + Write command (MSB = 0)
    data[1] = val;                // Value to be written

    // Select the RC522 chip by pulling its chip select (CS) pin low
    HAL_GPIO_WritePin(RC522_CS_GPIO_Port, RC522_CS_Pin, GPIO_PIN_RESET);

    // Send the data buffer over SPI
    HAL_SPI_Transmit(&hspi1, data, sizeof(data), HAL_MAX_DELAY);

    // Deselect the RC522 chip by pulling its chip select (CS) pin high
    HAL_GPIO_WritePin(RC522_CS_GPIO_Port, RC522_CS_Pin, GPIO_PIN_SET);
}


// Function to read data from RC522 register
uint8_t RC522_ReadRegister(uint8_t addr)
{
    // Create a buffer to hold the command and the received data
    uint8_t txData[1];
    uint8_t rxData[1];

    txData[0] = ((addr << 1) & 0x7E) | 0x80; // Address + Read command (MSB = 1)

    // Select the RC522 chip by pulling its chip select (CS) pin low
    HAL_GPIO_WritePin(RC522_CS_GPIO_Port, RC522_CS_Pin, GPIO_PIN_RESET);

    // Send the address with Read command and receive the data over SPI
    HAL_SPI_TransmitReceive(&hspi1, txData, rxData, sizeof(txData), HAL_MAX_DELAY);

    // Deselect the RC522 chip by pulling its chip select (CS) pin high
    HAL_GPIO_WritePin(RC522_CS_GPIO_Port, RC522_CS_Pin, GPIO_PIN_SET);

    return rxData[0];
}


// Function to set bits in RC522 register
void RC522_SetBitMask(uint8_t reg, uint8_t mask)
{
    uint8_t tmp;
    tmp = RC522_ReadRegister(reg);
    RC522_WriteRegister(reg, tmp | mask);
}

// Function to clear bits in RC522 register
void RC522_ClearBitMask(uint8_t reg, uint8_t mask)
{
    uint8_t tmp;
    tmp = RC522_ReadRegister(reg);
    RC522_WriteRegister(reg, tmp & (~mask));
}

// Function to reset the RC522 module
void RC522_Reset()
{
    RC522_WriteRegister(RC522_REG_COMMAND, RC522_CMD_SOFT_RESET);
    HAL_Delay(50);
}

// Function to initialize the RC522 module
void RC522_Init()
{
    // Initialize your SPI peripheral and other GPIO pins here
    // For example, configure the SPI GPIO pins and initialize the SPI peripheral
    // For this example, let's assume you have configured the SPI as hspi1

    // SPI configuration structure
    hspi1.Instance = SPI1;  // Use SPI1 peripheral
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT; // RC522 uses software-controlled chip select
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4; // Adjust as needed
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        // Error handling if SPI initialization fails
        // Add your error handling code here
    }

    // Initialize the GPIO pin for the RC522 chip select (CS) pin
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = RC522_CS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(RC522_CS_GPIO_Port, &GPIO_InitStruct);

    // Perform a soft reset on the RC522 module
    RC522_Reset();
}

// Function to request a tag to be detected by the RC522
uint8_t RC522_Request(uint8_t* tagType)
{
    uint8_t status;
    uint8_t buffer[2];

    buffer[0] = RC522_CMD_REQA; // Request command: 0x26 (REQA)

    // Select the RC522 chip by pulling its chip select (CS) pin low
    HAL_GPIO_WritePin(RC522_CS_GPIO_Port, RC522_CS_Pin, GPIO_PIN_RESET);

    // Send the request command over
}
