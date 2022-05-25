#include "theft.h"
#include "list.h"
#include <string.h>

#include <stdio.h>

typedef struct ValInfo {
    int *vals;
    int numOfVals;
} ValInfo;

void destroyValInfo(ValInfo *vi) {
    free(vi->vals);
    free(vi);
}

static enum theft_trial_res
prop_list_push_size_match(struct theft *t, void *arg) {
    ValInfo *vi = (ValInfo*)arg;
    int limit = vi->numOfVals;

    List *l = createList();

    for (int i = 0; i < limit; ++i) {
        listPush(l, &vi->vals[i]);
        if (listSize(l) != i+1)
            return THEFT_TRIAL_FAIL;
    }

    return THEFT_TRIAL_PASS;
}

static enum theft_trial_res
prop_list_push_and_pop_equal_in_order(struct theft *t, void *arg) {
    ValInfo *vi = (ValInfo*)arg;
    int limit = vi->numOfVals;

    List *l = createList();

    for (int i = 0; i < limit; ++i) {
        listPush(l, &vi->vals[i]);
    }

    for (int i = 0; i < limit; ++i) {
        int *v = (int *)listPop(l);

        if (*v != vi->vals[i])
            return THEFT_TRIAL_FAIL;
    }

    return THEFT_TRIAL_PASS;
}

enum theft_alloc_res
random_data_alloc_cb(struct theft *t, void *env, void **instance) {
    uint64_t limit = theft_random_choice(t, 1000);

    ValInfo *vi = (ValInfo*)malloc(sizeof(ValInfo));
    vi->vals = (int*)malloc(limit*sizeof(int));
    vi->numOfVals = limit;

    int *pos = vi->vals;
    for (int i = 0; i < limit; ++i) {
        *pos = theft_random_choice(t, 1024);
        ++pos;
    }

    *instance = vi;

    return THEFT_ALLOC_OK;
}

static struct theft_type_info random_data_info = {
    .alloc = random_data_alloc_cb,
};

bool test_list_push_pop(void) {
    theft_seed seed = theft_seed_of_time();

    struct theft_run_config config = {
        .name = __func__,
        .prop1 = prop_list_push_size_match,
        .type_info = { &random_data_info },
        .seed = seed
    };
    enum theft_run_res res = theft_run(&config);

    return res == THEFT_RUN_PASS;
}

bool test_list_push_pop_equal_and_in_order(void) {
    theft_seed seed = theft_seed_of_time();

    struct theft_run_config config = {
        .name = __func__,
        .prop1 = prop_list_push_and_pop_equal_in_order,
        .type_info = { &random_data_info },
        .seed = seed
    };

    enum theft_run_res res = theft_run(&config);
    return res == THEFT_RUN_PASS;

}

int main(void) {
    test_list_push_pop();
    test_list_push_pop_equal_and_in_order();
}
