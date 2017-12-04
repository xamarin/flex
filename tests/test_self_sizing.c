// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

static int dummy_self_sizing_called;
static float dummy_self_sizing_size[2];

static void
dummy_self_sizing(struct flex_item *item, float size[2])
{
    dummy_self_sizing_called++;
    dummy_self_sizing_size[0] = size[0];
    dummy_self_sizing_size[1] = size[1];
}

#define DUMMY_SELF_SIZING_RESET() \
    do { \
        dummy_self_sizing_called = 0; \
        dummy_self_sizing_size[0] = 0; \
        dummy_self_sizing_size[1] = 0; \
    } \
    while (0)

void
test_self_sizing_dummy0(void)
{
    struct flex_item *item = flex_item_new();

    TEST(flex_item_get_self_sizing(item) == NULL);

    flex_item_set_self_sizing(item, dummy_self_sizing);
    TEST(flex_item_get_self_sizing(item) == dummy_self_sizing);

    flex_item_set_self_sizing(item, NULL);
    TEST(flex_item_get_self_sizing(item) == NULL);

    flex_item_free(item);
}

void
test_self_sizing_dummy1(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child = flex_item_new();
    flex_item_set_self_sizing(child, dummy_self_sizing);
    flex_item_add(root, child);

    DUMMY_SELF_SIZING_RESET();

    flex_layout(root);

    TEST_EQUAL(dummy_self_sizing_called, 1);
    TEST_EQUAL(dummy_self_sizing_size[0], 100);
    TEST_EQUAL(dummy_self_sizing_size[1], 0);

    flex_item_free(root);
}

void
test_self_sizing_dummy2(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child = flex_item_new();
    flex_item_set_width(child, 50);
    flex_item_set_self_sizing(child, dummy_self_sizing);
    flex_item_add(root, child);

    DUMMY_SELF_SIZING_RESET();

    flex_layout(root);

    TEST_EQUAL(dummy_self_sizing_called, 1);
    TEST_EQUAL(dummy_self_sizing_size[0], 50);
    TEST_EQUAL(dummy_self_sizing_size[1], 0);

    flex_item_free(root);
}

void
test_self_sizing_dummy3(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child = flex_item_new();
    flex_item_set_height(child, 50);
    flex_item_set_self_sizing(child, dummy_self_sizing);
    flex_item_add(root, child);

    DUMMY_SELF_SIZING_RESET();

    flex_layout(root);

    TEST_EQUAL(dummy_self_sizing_called, 1);
    TEST_EQUAL(dummy_self_sizing_size[0], 100);
    TEST_EQUAL(dummy_self_sizing_size[1], 50);

    flex_item_free(root);
}

void
test_self_sizing_dummy4(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child = flex_item_new();
    flex_item_set_width(child, 50);
    flex_item_set_height(child, 50);
    flex_item_set_self_sizing(child, dummy_self_sizing);
    flex_item_add(root, child);

    DUMMY_SELF_SIZING_RESET();

    flex_layout(root);

    TEST_EQUAL(dummy_self_sizing_called, 1);
    TEST_EQUAL(dummy_self_sizing_size[0], 50);
    TEST_EQUAL(dummy_self_sizing_size[1], 50);

    flex_item_free(root);
}

void
test_self_sizing_dummy5(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child = flex_item_new();
    flex_item_set_grow(child, 1);
    flex_item_set_self_sizing(child, dummy_self_sizing);
    flex_item_add(root, child);

    DUMMY_SELF_SIZING_RESET();

    flex_layout(root);

    TEST_EQUAL(dummy_self_sizing_called, 1);
    TEST_EQUAL(dummy_self_sizing_size[0], 100);
    TEST_EQUAL(dummy_self_sizing_size[1], 0);

    flex_item_free(root);
}

void
test_self_sizing_dummy6(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child = flex_item_new();
    flex_item_set_self_sizing(child, dummy_self_sizing);
    flex_item_add(root, child);

    DUMMY_SELF_SIZING_RESET();

    for (int i = 0; i < 42; i++) {
        flex_layout(root);
        TEST_EQUAL(dummy_self_sizing_called, i + 1);
    }

    TEST_EQUAL(dummy_self_sizing_called, 42);

    flex_item_free(root);
}

static bool simulate_self_sizing_nan_called = false;

static void
simulate_self_sizing_nan(struct flex_item *item, float size[2])
{
    simulate_self_sizing_nan_called = true;

    size[0] = NAN;
    size[1] = NAN;
}

void
test_self_sizing_nan(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child = flex_item_with_size(10, 20);
    flex_item_set_self_sizing(child, simulate_self_sizing_nan);
    flex_item_add(root, child);

    flex_layout(root);

    TEST_EQUAL(simulate_self_sizing_nan_called, true);
    TEST_FRAME_EQUAL(child, 0, 0, 10, 20);

    flex_item_free(root);
}

static void
simulate_wrapping_text(struct flex_item *item, float size[2])
{
    if (size[0] >= 68) {
        size[0] = 68;
        size[1] = 16;
    }
    else if (size[0] >= 50) {
        size[0] = 50;
        size[1] = 32;
    }
    else {
        // Do nothing.
    }
}

void
test_self_sizing1(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_align_items(root, FLEX_ALIGN_START);

    struct flex_item *child = flex_item_new();
    flex_item_set_self_sizing(child, simulate_wrapping_text);
    flex_item_add(root, child);

    flex_layout(root);

    TEST_FRAME_EQUAL(child, 0, 0, 68, 16);

    flex_item_free(root);
}

void
test_self_sizing2(void)
{
    struct flex_item *root = flex_item_with_size(55, 100);
    flex_item_set_align_items(root, FLEX_ALIGN_START);

    struct flex_item *child = flex_item_new();
    flex_item_set_self_sizing(child, simulate_wrapping_text);
    flex_item_add(root, child);

    flex_layout(root);

    TEST_FRAME_EQUAL(child, 0, 0, 50, 32);

    flex_item_free(root);
}

void
test_self_sizing3(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child = flex_item_new();
    flex_item_set_width(child, 10);
    flex_item_set_height(child, 10);
    flex_item_set_self_sizing(child, simulate_wrapping_text);
    flex_item_add(root, child);

    flex_layout(root);

    TEST_FRAME_EQUAL(child, 0, 0, 10, 10);

    flex_item_free(root);
}

void
test_self_sizing4(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child = flex_item_new();
    flex_item_set_width(child, 10);
    flex_item_set_height(child, 10);
    flex_item_set_grow(child, 1);
    flex_item_set_self_sizing(child, simulate_wrapping_text);
    flex_item_add(root, child);

    flex_layout(root);

    TEST_FRAME_EQUAL(child, 0, 0, 10, 100);

    flex_item_free(root);
}

void
test_self_sizing5(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_direction(root, FLEX_DIRECTION_ROW);

    struct flex_item *child = flex_item_new();
    flex_item_set_self_sizing(child, simulate_wrapping_text);
    flex_item_add(root, child);

    flex_layout(root);

    TEST_FRAME_EQUAL(child, 0, 0, 0, 100);

    flex_item_free(root);
}

// Items with a self_sizing callback returning a positive value for the
// cross-axis size will be ignored if said item is configured to stretch.

static int self_sizing_align_measure_size = 0;
static float self_sizing_align_measure_value = 0;

static void
self_sizing_align_measure(struct flex_item *item, float size[2])
{
    size[self_sizing_align_measure_size] = self_sizing_align_measure_value;
}

void
test_self_sizing6(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN);

    struct flex_item *child = flex_item_new();
    flex_item_set_height(child, 100);
    flex_item_set_self_sizing(child, self_sizing_align_measure);
    flex_item_set_align_self(child, FLEX_ALIGN_STRETCH);
    flex_item_add(root, child);

    self_sizing_align_measure_size = 0;
    self_sizing_align_measure_value = 10;

    flex_layout(root);

    TEST_FRAME_EQUAL(child, 0, 0, 100, 100);

    self_sizing_align_measure_size = 0;
    self_sizing_align_measure_value = 0;

    flex_layout(root);

    TEST_FRAME_EQUAL(child, 0, 0, 100, 100);

    self_sizing_align_measure_size = 0;
    self_sizing_align_measure_value = NAN;

    flex_layout(root);

    TEST_FRAME_EQUAL(child, 0, 0, 100, 100);

    flex_item_set_width(child, 10);

    flex_layout(root);

    TEST_FRAME_EQUAL(child, 0, 0, 10, 100);

    flex_item_free(root);
}

void
test_self_sizing7(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_direction(root, FLEX_DIRECTION_ROW);

    struct flex_item *child = flex_item_new();
    flex_item_set_width(child, 100);
    flex_item_set_self_sizing(child, self_sizing_align_measure);
    flex_item_set_align_self(child, FLEX_ALIGN_STRETCH);
    flex_item_add(root, child);

    flex_layout(root);

    self_sizing_align_measure_size = 1;
    self_sizing_align_measure_value = 10;

    flex_layout(root);

    TEST_FRAME_EQUAL(child, 0, 0, 100, 100);

    self_sizing_align_measure_size = 1;
    self_sizing_align_measure_value = 0;

    flex_layout(root);

    TEST_FRAME_EQUAL(child, 0, 0, 100, 100);

    self_sizing_align_measure_size = 1;
    self_sizing_align_measure_value = NAN;

    flex_layout(root);

    TEST_FRAME_EQUAL(child, 0, 0, 100, 100);

    flex_item_set_height(child, 10);

    flex_layout(root);

    TEST_FRAME_EQUAL(child, 0, 0, 100, 10);

    flex_item_free(root);
}
