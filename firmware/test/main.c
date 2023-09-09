#include <stdio.h>

#include "minunit.h"

#include "param_manager.h"
#include "button.h"
#include "sequencer.h"
#include "led_flasher.h"
#include "ticker.h"

#define test_header(text) \
    printf("\n\n"); \
    printf("***********\n"); \
    printf(text);

MU_TEST(param_manager_test_bank_change) {
    ParamManager pm;
    param_manager_init(&pm);
    mu_check(pm.bank == 0);
    param_manager_next_bank(&pm);
    mu_check(pm.bank == 1);
    param_manager_next_bank(&pm);
    mu_check(pm.bank == 2);
}

MU_TEST(param_manager_test_locks) {
    ParamManager pm;
    param_manager_init(&pm);

    ParamType first = param_manager_current(&pm, 0);
    mu_check(pm.pot_change_locked[first]);
    mu_check(pm.previous_pot_value[first] == 0);

    bool locked;

    locked = param_manager_lock_check(&pm, 0, 10);
    mu_check(locked);
    mu_check(pm.previous_pot_value[0] == 0);
    locked = param_manager_lock_check(&pm, 0, 100);
    mu_check(!locked);
    mu_check(pm.previous_pot_value[first] == 100);

    param_manager_next_bank(&pm);
    mu_check(pm.previous_pot_value[0] == 100);
    mu_check(pm.pot_change_locked[0]);

    locked = param_manager_lock_check(&pm, 0, 110);
    mu_check(locked);
    mu_check(pm.previous_pot_value[0] == 100);
    locked = param_manager_lock_check(&pm, 0, 200);
    mu_check(!locked);
    mu_check(pm.previous_pot_value[0] == 200);
}

MU_TEST(param_manager_test_freq) {
    ParamManager pm;
    param_manager_init(&pm);

    bool freq_debounced;

    for (int i = 0; i < (DEBOUNCE_SAMPLES - 1); i++) {
        freq_debounced = param_manager_set_freq(&pm, 500);
        mu_check(!freq_debounced);
    }
    freq_debounced = param_manager_set_freq(&pm, 500);
    mu_check(freq_debounced);
}

MU_TEST_SUITE(param_manager_suite) {
    test_header("Param Manager tests\n");
    MU_RUN_TEST(param_manager_test_bank_change);
    MU_RUN_TEST(param_manager_test_locks);
    MU_RUN_TEST(param_manager_test_freq);
}

MU_TEST(led_flasher_create) {
    Ticker ticker;
    ticker_init(&ticker);
    LEDFlasher flasher;
    flash_init(&flasher, &ticker);

    mu_check(!flasher.running);
    mu_check(!flasher.led_on);
}

MU_TEST(led_flasher_run) {
    Ticker ticker;
    ticker_init(&ticker);
    LEDFlasher flasher;
    flash_init(&flasher, &ticker);

    flash_start(&flasher, 2, 10);
    mu_check(flasher.running);
    mu_check(flasher.led_on);

    ticker.count = 1000;
    flash_update(&flasher);
    ticker.count = 2000;
    flash_update(&flasher);

    ticker.count = 2500;
    flash_update(&flasher);
    mu_check(flasher.running);
    mu_check(flasher.led_on);
    mu_check(flasher.flashes_remaining == 2);

    ticker.count = 2600;
    flash_update(&flasher);
    mu_check(flasher.running);
    mu_check(!flasher.led_on);
    mu_check(flasher.flashes_remaining == 1);

    ticker.count = 5000;
    flash_update(&flasher);
    mu_check(flasher.running);
    mu_check(!flasher.led_on);

    ticker.count = 6000;
    flash_update(&flasher);
    mu_check(flasher.running);
    mu_check(flasher.led_on);
    mu_check(flasher.flashes_remaining == 1);

    ticker.count = 8000;
    flash_update(&flasher);
    mu_check(!flasher.running);
    mu_check(!flasher.led_on);
    mu_check(flasher.flashes_remaining == 0);
}

MU_TEST_SUITE(led_flasher_suite) {
    test_header("LED Flasher tests\n");
    MU_RUN_TEST(led_flasher_create);
    MU_RUN_TEST(led_flasher_run);
}

MU_TEST(button_pressing) {
    Button button;
    Ticker t;
    ticker_init(&t);
    button_init(&button, &t);

    mu_check(button.current_state == BUTTON_UP);
    mu_check(button.previous_state == BUTTON_UP);

    button_update(&button, BUTTON_PRESSED);
    mu_check(button_just_pressed(&button));

    ticker_set(&t, 1000);
    button_update(&button, BUTTON_PRESSED);
    mu_check(!button_just_pressed(&button));

    ticker_set(&t, 7424); // 29 << 8
    button_update(&button, BUTTON_PRESSED);
    mu_assert_int_eq(29, button.time_pressed);
    mu_check(!button_just_pressed(&button));

    ticker_set(&t, 9000);
    button_update(&button, BUTTON_PRESSED);
    mu_assert_int_eq(35, button.time_pressed);
    mu_check(button_is_held(&button));
    mu_check(!button_just_pressed(&button));

    ticker_set(&t, 9100);
    button_update(&button, BUTTON_UP);
    mu_assert_int_eq(0, button.time_pressed);
    mu_check(!button_is_held(&button));
    mu_check(!button_just_pressed(&button));
    mu_check(button_just_let_go(&button));
}

MU_TEST_SUITE(button_suite) {
    test_header("Button tests\n");
    MU_RUN_TEST(button_pressing);
}

MU_TEST(sequencer_run) {
    Sequencer s;
    Ticker t;
    sequencer_init(&s, &t);
    mu_check(!sequencer_is_running(&s));

    sequencer_set_step_length(&s, 200);
    sequencer_set_note_length(&s, 90);
    sequencer_add_step(&s, 100);
    sequencer_add_step(&s, 120);
    sequencer_add_step(&s, 80);
    sequencer_add_step(&s, 60);
    mu_assert_int_eq(4, s.step_count);
    mu_check(s.note_state == SEQUENCER_NOTE_OFF);

    ticker_set(&t, 10);
    sequencer_start(&s);
    mu_check(sequencer_is_running(&s));
    mu_check(s.note_state == SEQUENCER_NOTE_ON);
    mu_check(sequencer_note_started(&s));
    mu_assert_int_eq(0, s.current_step);

    ticker_set(&t, 50);
    sequencer_tick(&s);
    mu_check(sequencer_is_running(&s));
    mu_check(s.note_state == SEQUENCER_NOTE_ON);
    mu_check(!sequencer_note_started(&s));
    mu_assert_int_eq(0, s.current_step);

    ticker_set(&t, 140);
    sequencer_tick(&s);
    mu_check(sequencer_is_running(&s));
    mu_check(s.note_state == SEQUENCER_NOTE_OFF);
    mu_check(sequencer_note_finished(&s));
    mu_assert_int_eq(0, s.current_step);

    for (int i = 1; i < 9; i++) {
        ticker_increment(&t, 99);
        sequencer_tick(&s);
        mu_check(sequencer_is_running(&s));
        mu_check(s.note_state == SEQUENCER_NOTE_ON);
        mu_check(sequencer_note_started(&s));
        mu_assert_int_eq(i % s.step_count, s.current_step);

        ticker_increment(&t, 1);
        sequencer_tick(&s);
        mu_check(!sequencer_note_started(&s));

        ticker_increment(&t, 99);
        sequencer_tick(&s);
        mu_check(sequencer_is_running(&s));
        mu_check(s.note_state == SEQUENCER_NOTE_OFF);
        mu_check(sequencer_note_finished(&s));
        mu_assert_int_eq(i % s.step_count, s.current_step);

        ticker_increment(&t, 1);
        sequencer_tick(&s);
        mu_check(!sequencer_note_finished(&s));
    }
}

MU_TEST(sequencer_add_steps) {
    Sequencer s;
    Ticker t;
    sequencer_init(&s, &t);

    sequencer_set_step_length(&s, 200);
    sequencer_set_note_length(&s, 90);
    sequencer_add_step(&s, 100);
    sequencer_add_step(&s, 120);
    sequencer_add_step(&s, 80);
    sequencer_add_step(&s, 60);

    ticker_set(&t, 10);
    sequencer_start(&s);

    for (int i = 1; i < 10; i++) {
        ticker_increment(&t, 100);
        sequencer_tick(&s);

        ticker_increment(&t, 100);
        sequencer_tick(&s);
        mu_assert_int_eq(i % s.step_count, s.current_step);
    }

    mu_assert_int_eq(9 % s.step_count, s.current_step);

    sequencer_add_step(&s, 180);

    sequencer_reset(&s);
    mu_check(s.note_state == SEQUENCER_NOTE_ON);
    mu_check(sequencer_note_started(&s));

    sequencer_add_step(&s, 30);

    ticker_increment(&t, 10);
    sequencer_tick(&s);
    mu_check(s.note_state == SEQUENCER_NOTE_ON);
    mu_check(!sequencer_note_started(&s));
    mu_assert_int_eq(0, s.current_step);

    ticker_increment(&t, 100);
    sequencer_tick(&s);
    mu_check(s.note_state == SEQUENCER_NOTE_OFF);
    mu_check(sequencer_note_finished(&s));
    mu_assert_int_eq(0, s.current_step);

    for (int i = 1; i < 7; i++) {
        ticker_increment(&t, 99);
        sequencer_tick(&s);
        mu_check(sequencer_is_running(&s));
        mu_check(s.note_state == SEQUENCER_NOTE_ON);
        mu_check(sequencer_note_started(&s));
        mu_assert_int_eq(i % s.step_count, s.current_step);

        ticker_increment(&t, 1);
        sequencer_tick(&s);
        mu_check(!sequencer_note_started(&s));

        ticker_increment(&t, 99);
        sequencer_tick(&s);
        mu_check(sequencer_is_running(&s));
        mu_check(s.note_state == SEQUENCER_NOTE_OFF);
        mu_check(sequencer_note_finished(&s));
        mu_assert_int_eq(i % s.step_count, s.current_step);

        ticker_increment(&t, 1);
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
    MU_RUN_SUITE(led_flasher_suite);
    MU_RUN_SUITE(sequencer_suite);
    MU_RUN_SUITE(button_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
