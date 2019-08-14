#include "mbed.h"
#include "Servo/Servo.h"
#include "PwmIn/PwmIn.h"
#include "QEI\QEI.h"

#define RANGE 0.01

Servo servo(PD_14);
PwmIn RPI(PD_10);
PwmIn RPIengine(PD_12);
DigitalOut Forward(PD_1);
DigitalOut Backward(PD_3);
QEI encoder(PG_11, PC_14, PF_2, 2084, QEI::X4_ENCODING);
PwmOut engine(PC_8);
DigitalIn direction(PC_12);
InterruptIn User(USER_BUTTON);

static bool ActionCounter = true;

void intreruptHandler()
{
    if (ActionCounter)
        ActionCounter = false;
    else
        ActionCounter = true;
}

int main()
{
    float servoAngle = 0;
    engine.period(0.0002f);
    float speed = 0.0f;
    engine.write(speed);
    
    User.rise(&intreruptHandler);

    while (true) 
    {
        if (ActionCounter)
        {
            printf("Rotile sunt la pozitia: %f\r\n", servo.read());
            printf("Motorul are viteza: %f\r\n", engine.read());

            /* DIRECTIE */
            if (direction)
            {
                Forward = 1;
                Backward = 0;
            }
            else
            {
                Forward = 0;
                Backward = 1;
            }
            wait(0.1);

            /* MOTOR */
            speed = RPIengine.dutycycle();
            if ( speed < 0.2 || speed > 0.75)
            {
                printf("Valoarea furnizata nu este optima pentru deplasare!\r\n");
            }
            else
            {
                //encoder.reset();
                engine.write(speed);
                printf("Viteza de deplasare este: %f\r\n", speed);
            }

            /* SERVO */
            if (RPI.period() == RPI.pulsewidth())
            {
                continue;
            }

            servoAngle = RPI.dutycycle();
            if (servoAngle < 0.16 || servoAngle > 0.95)
            {
                printf("Unghiul de la servo nu este optim pentru rotatie!\r\n");
            }
            else
            {
                servo.write(servoAngle);
                printf("Valoarea pentru servo este: %f\r\n", servoAngle);
            }
            printf("PULSURI: %d \t\t\t REVOLUTIE: %d \t\t\t INDEX: %d\r\n", encoder.getPulses(), encoder.getRevolutions(), encoder.getIndex());
        }
        else
        {
            servo = 0.5;
            engine = 0;
        }
    }
}
