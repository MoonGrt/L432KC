#include "mbed.h"

// Create a UnbufferedSerial object with a default baud rate.
static UnbufferedSerial pc(USBTX, USBRX);  // tx, rx
static UnbufferedSerial device(D1, D0);    // tx, rx
PwmOut motor1(D9); //A5
PwmOut motor2(D10);

void motor_drive(char motor1_pulse, char motor2_pulse)
{
    motor1 = (float)motor1_pulse/255;
    motor2 = (float)motor2_pulse/255;
}

void rx_irp()
{
    char c[2];

    // Read the data to clear the receive interrupt.
    if (device.read(&c, 2)) {
        // Echo the input back to the terminal.
        pc.write(&c, 2);
    }
    motor_drive(c[0], c[1]);
}

int main(void)
{
    // Set desired properties (9600-8-N-1).
    pc.baud(115200);
    pc.format(
        /* bits */ 8,
        /* parity */ SerialBase::None,
        /* stop bit */ 1
    );

    // Set desired properties (9600-8-N-1).
    device.baud(115200);
    device.format(
        /* bits */ 8,
        /* parity */ SerialBase::None,
        /* stop bit */ 1
    );
    // Register a callback to process a Rx (receive) interrupt.
    device.attach(&rx_irp, SerialBase::RxIrq);

    motor1.period(0.001f);
    motor1 = 0.0f;
    motor2.period(0.001f);
    motor2 = 0.0f;
}
