#include <stdio.h>

#include "minunit.h"

#include "param_manager.h"
#include "control_pot.h"
#include "keyboard.h"
#include "button.h"
#include "sequencer.h"
#include "led_control.h"
#include "ticker.h"

#define test_header(text) \
    printf("\n\n"); \
    printf("***********\n"); \
    printf(text);

MU_TEST(param_manager_test_bank_change) {
    ParamManager pm;
    param_manager_init(&pm);
    mu_assert_int_eq(0, pm.bank);
    param_manager_next_bank(&pm);
    mu_assert_int_eq(1, pm.bank);
    param_manager_next_bank(&pm);
    mu_assert_int_eq(2, pm.bank);
}

MU_TEST_SUITE(param_manager_suite) {
    test_header("Param Manager tests\n");
    MU_RUN_TEST(param_manager_test_bank_change);
}

MU_TEST(control_pot_test_locks) {
    ControlPot cp;
    control_pot_init(&cp);

    mu_check(cp.locked);
    mu_assert_int_eq(0, cp.value);

    for (uint8_t i = 0; i < CONTROL_POT_FILTER_SAMPLES; i++) {
      control_pot_update(&cp, 10);
    }

    mu_check(cp.locked);
    mu_assert_int_eq(10, cp.value);

    for (uint8_t i = 0; i < CONTROL_POT_FILTER_SAMPLES; i++) {
      control_pot_update(&cp, 100);
    }
    mu_check(!cp.locked);
    mu_assert_int_eq(100, cp.value);

    for (uint8_t i = 0; i < CONTROL_POT_FILTER_SAMPLES; i++) {
      control_pot_update(&cp, 101);
    }
    mu_check(!cp.locked);
    mu_assert_int_eq(101, cp.value);

    control_pot_lock(&cp);

    for (uint8_t i = 0; i < CONTROL_POT_FILTER_SAMPLES; i++) {
      control_pot_update(&cp, 102);
    }
    mu_check(cp.locked);
    mu_assert_int_eq(102, cp.value);

    for (uint8_t i = 0; i < CONTROL_POT_FILTER_SAMPLES; i++) {
      control_pot_update(&cp, 202);
    }
    mu_check(!cp.locked);
    mu_assert_int_eq(202, cp.value);

}

MU_TEST_SUITE(control_pot_suite) {
    test_header("Control Pot tests\n");
    MU_RUN_TEST(control_pot_test_locks);
}

MU_TEST(keyboard_test_debounce) {
    Keyboard k;
    keyboard_init(&k);

    for (int i = 0; i < (KEYBOARD_DEBOUNCE_SAMPLES - 1); i++) {
        keyboard_update(&k, 200);
        mu_check(keyboard_unstable(&k));
    }
    keyboard_update(&k, 200);
    mu_check(keyboard_stable(&k));
    mu_check(keyboard_key_pressed(&k));

    keyboard_update(&k, 200);
    mu_check(keyboard_stable(&k));
    mu_check(!keyboard_key_pressed(&k));
    mu_assert_int_eq(2, keyboard_get_key(&k));

    keyboard_update(&k, 100);
    mu_check(!keyboard_stable(&k));
    mu_check(keyboard_key_released(&k));
}

MU_TEST(keyboard_key_up) {
    Keyboard k;
    keyboard_init(&k);

    for (int i = 0; i < 3; i++) {
        keyboard_update(&k, 500);
        mu_check(keyboard_unstable(&k));
    }
    for (int i = 0; i < 10; i++) {
        keyboard_update(&k, 0);
        mu_check(keyboard_unstable(&k));
    }
}

MU_TEST_SUITE(keyboard_suite) {
    test_header("Keyboard tests\n");
    MU_RUN_TEST(keyboard_test_debounce);
    MU_RUN_TEST(keyboard_key_up);
}

MU_TEST(led_control_create) {
    Ticker ticker;
    ticker_init(&ticker);
    LEDControl led;
    led_control_init(&led, &ticker);

    mu_check(!led.flashing);
    mu_check(led_control_is_off(&led));
}

MU_TEST(led_control_flash) {
    Ticker ticker;
    ticker_init(&ticker);
    LEDControl led;
    led_control_init(&led, &ticker);

    led_control_flash_start(&led, 2, 10);
    mu_check(led.flashing);
    mu_check(led_control_is_on(&led));

    ticker_increment(&ticker, 1000);
    led_control_update(&led);
    ticker_increment(&ticker, 1000);
    led_control_update(&led);

    ticker_increment(&ticker, 500);
    led_control_update(&led);
    mu_check(led.flashing);
    mu_check(led_control_is_on(&led));
    mu_assert_int_eq(3, led.changes_remaining);

    ticker_increment(&ticker, 100);
    led_control_update(&led);
    mu_check(led.flashing);
    mu_check(led_control_is_off(&led));
    mu_assert_int_eq(2, led.changes_remaining);

    ticker_increment(&ticker, 2600);
    led_control_update(&led);
    mu_check(led.flashing);
    mu_check(led_control_is_on(&led));
    mu_assert_int_eq(1, led.changes_remaining);

    ticker_increment(&ticker, 2600);
    led_control_update(&led);
    mu_check(!led.flashing);
    mu_check(led_control_is_off(&led));
    mu_assert_int_eq(0, led.changes_remaining);

    ticker_increment(&ticker, 2600);
    led_control_update(&led);
    mu_check(!led.flashing);
    mu_check(led_control_is_off(&led));
    mu_assert_int_eq(0, led.changes_remaining);
}

MU_TEST_SUITE(led_control_suite) {
    test_header("LED Control tests\n");
    MU_RUN_TEST(led_control_create);
    MU_RUN_TEST(led_control_flash);
}

MU_TEST(button_pressing) {
    Button button;
    Ticker t;
    ticker_init(&t);
    button_init(&button, &t);

    mu_check(button.current_state == BUTTON_UP);
    mu_check(button.previous_state == BUTTON_UP);

    for (int i = 0; i < BUTTON_DEBOUNCE_SAMPLES - 1; i += 1) {
        button_update(&button, BUTTON_PRESSED);
        mu_check(!button_just_pressed(&button));
    }
    button_update(&button, BUTTON_PRESSED);
    mu_check(button.current_state == BUTTON_PRESSED);
    mu_check(button.previous_state == BUTTON_UP);
    mu_check(button_just_pressed(&button));

    for (int i = 0; i < (BUTTON_DEBOUNCE_SAMPLES - 1); i += 1) {
        button_update(&button, BUTTON_UP);
        mu_check(!button_just_released(&button));
    }
    button_update(&button, BUTTON_UP);
    mu_check(button.current_state == BUTTON_UP);
    mu_check(button.previous_state == BUTTON_PRESSED);
    mu_check(button_just_released(&button));
}

MU_TEST(button_long_hold) {
    Button button;
    Ticker t;
    ticker_init(&t);
    button_init(&button, &t);

    for (int i = 0; i < BUTTON_DEBOUNCE_SAMPLES; i += 1) {
        button_update(&button, BUTTON_PRESSED);
    }
    mu_check(button_just_pressed(&button));

    ticker_set(&t, 7424); // 29 << 8
    button_update(&button, BUTTON_PRESSED);
    mu_assert_int_eq(29, button.time_pressed);
    mu_check(!button_just_pressed(&button));
    mu_check(!button_is_held(&button));

    ticker_set(&t, 9000);
    button_update(&button, BUTTON_PRESSED);
    mu_assert_int_eq(35, button.time_pressed);
    mu_check(button_is_held(&button));
    mu_check(!button_just_pressed(&button));

    ticker_set(&t, 9100);
    button_update(&button, BUTTON_UP);
    // still held until button up is debounced
    mu_check(button_is_held(&button));
    for (int i = 0; i < (BUTTON_DEBOUNCE_SAMPLES - 1); i += 1) {
        button_update(&button, BUTTON_UP);
    }

    mu_assert_int_eq(0, button.time_pressed);
    mu_check(!button_is_held(&button));
    mu_check(!button_just_pressed(&button));
    mu_check(button_just_released(&button));
}

MU_TEST(button_long_hold_reset) {
    Button button;
    Ticker t;
    ticker_init(&t);
    button_init(&button, &t);

    for (int i = 0; i < BUTTON_DEBOUNCE_SAMPLES; i += 1) {
        button_update(&button, BUTTON_PRESSED);
    }
    mu_check(button_just_pressed(&button));

    ticker_set(&t, 7424); // 29 << 8
    button_update(&button, BUTTON_PRESSED);
    mu_assert_int_eq(29, button.time_pressed);
    mu_check(!button_just_pressed(&button));
    mu_check(!button_is_held(&button));

    ticker_set(&t, 9000);
    button_update(&button, BUTTON_PRESSED);
    mu_assert_int_eq(35, button.time_pressed);
    mu_check(button_is_held(&button));
    mu_check(!button_just_pressed(&button));

    button_reset(&button);

    mu_check(!button_is_held(&button));
    mu_check(!button_just_pressed(&button));
    mu_check(!button_just_released(&button));

}

MU_TEST_SUITE(button_suite) {
    test_header("Button tests\n");
    MU_RUN_TEST(button_pressing);
    MU_RUN_TEST(button_long_hold);
    MU_RUN_TEST(button_long_hold_reset);
}

MU_TEST(sequencer_run) {
    Sequencer s;
    Ticker t;
    ticker_init(&t);
    sequencer_init(&s, &t);
    mu_check(!sequencer_is_running(&s));

    sequencer_set_step_length(&s, 200);
    sequencer_set_note_length(&s, 500);

    sequencer_add_trig_step(&s, 0);
    mu_assert_int_eq(0, sequencer_get_step_value(&s, 0));
    mu_assert_int_eq(1, s.step_count);

    sequencer_add_trig_step(&s, 1);
    mu_assert_int_eq(1, sequencer_get_step_value(&s, 1));
    mu_assert_int_eq(2, s.step_count);

    sequencer_add_trig_step(&s, 2);
    mu_assert_int_eq(2, sequencer_get_step_value(&s, 2));
    mu_assert_int_eq(3, s.step_count);

    sequencer_add_trig_step(&s, 3);
    mu_assert_int_eq(3, sequencer_get_step_value(&s, 3));
    mu_assert_int_eq(4, s.step_count);

    mu_check(s.note_state == SEQUENCER_NOTE_STATE_OFF);

    sequencer_start(&s);

    for (int i = 0; i < 9; i++) {

        mu_check(sequencer_is_running(&s));
        mu_check(s.note_state == SEQUENCER_NOTE_STATE_ON);
        mu_check(sequencer_note_started(&s));
        mu_assert_int_eq(i % s.step_count, s.current_step);
        mu_assert_int_eq(i % s.step_count, sequencer_current_step_value(&s));

        ticker_increment(&t, 100);
        sequencer_tick(&s);
        mu_check(!sequencer_note_started(&s));

        ticker_increment(&t, 1400);
        sequencer_tick(&s);
        mu_check(sequencer_is_running(&s));
        mu_check(s.note_state == SEQUENCER_NOTE_STATE_OFF);
        mu_check(sequencer_note_finished(&s));
        mu_assert_int_eq(i % s.step_count, s.current_step);

        ticker_increment(&t, 100);
        sequencer_tick(&s);
        mu_check(!sequencer_note_finished(&s));

        ticker_increment(&t, 1000);
        sequencer_tick(&s);
    }
}

MU_TEST(sequencer_add_steps) {
    Sequencer s;
    Ticker t;
    sequencer_init(&s, &t);

    sequencer_set_step_length(&s, 200);
    sequencer_set_note_length(&s, 500);

    sequencer_add_trig_step(&s, 120);
    sequencer_add_hold_step(&s, 80);
    sequencer_add_rest_step(&s, 60);

    sequencer_start(&s);

    for (int i = 0; i < 10; i++) {
        ticker_increment(&t, 1000);
        sequencer_tick(&s);

        ticker_increment(&t, 1500);
        sequencer_tick(&s);
        mu_assert_int_eq(i % s.step_count, s.current_step);
        switch (i % s.step_count) {
            case 0:
                mu_check(sequencer_current_step_type(&s) == SEQUENCER_NOTE_TYPE_TRIG);
                break;
            case 1:
                mu_check(sequencer_current_step_type(&s) == SEQUENCER_NOTE_TYPE_HOLD);
                break;
            case 2:
                mu_check(sequencer_current_step_type(&s) == SEQUENCER_NOTE_TYPE_REST);
                break;
        }
    }

    mu_assert_int_eq(9 % s.step_count, s.current_step);

    sequencer_add_trig_step(&s, 180);

    sequencer_reset(&s);
    mu_check(s.note_state == SEQUENCER_NOTE_STATE_ON);
    mu_check(sequencer_note_started(&s));

    sequencer_add_trig_step(&s, 30);

    ticker_increment(&t, 1000);
    sequencer_tick(&s);
    mu_check(s.note_state == SEQUENCER_NOTE_STATE_ON);
    mu_check(!sequencer_note_started(&s));
    mu_assert_int_eq(0, s.current_step);

    ticker_increment(&t, 1500);
    sequencer_tick(&s);
    mu_check(s.note_state == SEQUENCER_NOTE_STATE_OFF);
    mu_check(sequencer_note_finished(&s));
    mu_assert_int_eq(0, s.current_step);

    for (int i = 1; i < 7; i++) {
        ticker_increment(&t, 1000);
        sequencer_tick(&s);
        mu_check(sequencer_is_running(&s));
        if (sequencer_current_step_type(&s) != SEQUENCER_NOTE_TYPE_REST)  {
            mu_check(s.note_state == SEQUENCER_NOTE_STATE_ON);
            mu_check(sequencer_note_started(&s));
        }
        mu_assert_int_eq(i % s.step_count, s.current_step);

        switch (i % s.step_count) {
            case 0:
                mu_check(sequencer_current_step_type(&s) == SEQUENCER_NOTE_TYPE_TRIG);
                break;
            case 1:
                mu_check(sequencer_current_step_type(&s) == SEQUENCER_NOTE_TYPE_HOLD);
                break;
            case 2:
                mu_check(sequencer_current_step_type(&s) == SEQUENCER_NOTE_TYPE_REST);
                break;
        }

        ticker_increment(&t, 1<<4);
        sequencer_tick(&s);
        mu_check(!sequencer_note_started(&s));

        ticker_increment(&t, 1500);
        sequencer_tick(&s);
        mu_check(sequencer_is_running(&s));
        if (sequencer_current_step_type(&s) != SEQUENCER_NOTE_TYPE_HOLD)  {
            mu_check(s.note_state == SEQUENCER_NOTE_STATE_OFF);
            mu_check(sequencer_note_finished(&s));
        }
        mu_assert_int_eq(i % s.step_count, s.current_step);

        ticker_increment(&t, 1<<4);
        sequencer_tick(&s);
        mu_check(!sequencer_note_finished(&s));
    }

}

MU_TEST_SUITE(sequencer_suite) {
    test_header("Sequencer tests\n");
    MU_RUN_TEST(sequencer_run);
    MU_RUN_TEST(sequencer_add_steps);
}

int main(int argc, char *argv[]) {
    test_header("Yorick tests\n");
    MU_RUN_SUITE(param_manager_suite);
    MU_RUN_SUITE(control_pot_suite);
    MU_RUN_SUITE(keyboard_suite);
    MU_RUN_SUITE(led_control_suite);
    MU_RUN_SUITE(sequencer_suite);
    MU_RUN_SUITE(button_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
