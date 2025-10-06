import pyfirmata
import time

TRIG_PIN = 3
ECHO_PIN = 2
RED_LED_PIN = 8
GREEN_LED_PIN = 9
BUZZER_PIN = 10

board = pyfirmata.Arduino('COM3')

it = pyfirmata.util.Iterator(board)
it.start()

board.digital[TRIG_PIN].mode = pyfirmata.OUTPUT
board.digital[ECHO_PIN].mode = pyfirmata.INPUT
board.digital[RED_LED_PIN].mode = pyfirmata.OUTPUT
board.digital[GREEN_LED_PIN].mode = pyfirmata.OUTPUT
board.digital[BUZZER_PIN].mode = pyfirmata.OUTPUT


def measure_distance():
    board.digital[TRIG_PIN].write(0)
    time.sleep(0.000002)
    board.digital[TRIG_PIN].write(1)
    time.sleep(0.00001)
    board.digital[TRIG_PIN].write(0)

    start_time = time.time()
    end_time = time.time()

    while board.digital[ECHO_PIN].read() == 0:
        start_time = time.time()

    while board.digital[ECHO_PIN].read() == 1:
        end_time = time.time()

    duration = end_time - start_time
    distance = (duration * 34300) / 2
    return distance


def play_buzzer(frequency, duration):
    period = 1.0 / frequency
    half_period = period / 2
    end_time = time.time() + duration

    while time.time() < end_time:
        board.digital[BUZZER_PIN].write(1)
        time.sleep(half_period)
        board.digital[BUZZER_PIN].write(0)
        time.sleep(half_period)


try:
    while True:
        distance = measure_distance()
        print(f"Distance: {distance:.2f} cm")

        if distance < 5:
            play_buzzer(1000, 0.1)
            board.digital[RED_LED_PIN].write(1)
            board.digital[GREEN_LED_PIN].write(0)
        else:
            board.digital[RED_LED_PIN].write(0)
            board.digital[GREEN_LED_PIN].write(1)

        time.sleep(0.1)

except KeyboardInterrupt:
    board.digital[BUZZER_PIN].write(0)
    board.digital[RED_LED_PIN].write(0)
    board.digital[GREEN_LED_PIN].write(0)
    board.exit()
