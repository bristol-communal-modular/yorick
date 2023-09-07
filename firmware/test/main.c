#include "minunit.h"

#include "param_manager.h"

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
    MU_RUN_TEST(param_manager_test_bank_change);
    MU_RUN_TEST(param_manager_test_locks);
    MU_RUN_TEST(param_manager_test_freq);
}

int main(int argc, char *argv[]) {
	MU_RUN_SUITE(param_manager_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}
