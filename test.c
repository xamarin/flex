#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <tgmath.h>

#include "flex.h"

static const char *current_unit = NULL;
#define MAX_FAILURES 100
static const char *failures[MAX_FAILURES];
static unsigned int failures_n = 0;

#define UNIT(unit) \
    do { \
        current_unit = #unit; \
        test_##unit(); \
    } \
    while (0)

#define _PASS() printf(".")
#define _FAIL(fmt, ...) \
    do { \
        if (failures_n < MAX_FAILURES) { \
            printf("E"); \
            char _buf[100] = { 0 }; \
            int _p = snprintf(_buf, sizeof(_buf), "%s (%s:%d): ", \
                    current_unit, __FILE__, __LINE__); \
            snprintf(_buf + _p, sizeof(_buf) - _p, fmt, __VA_ARGS__); \
            failures[failures_n++] = strdup(_buf); \
        } \
    } \
    while (0)

#define TEST(expr) \
    do { \
        if (expr) { \
            _PASS(); \
        } \
        else { \
            _FAIL("failed test `%s'", #expr); \
        } \
    } \
    while (0)

#define TEST_EQUAL_T(expr, val, type_fmt) \
    do { \
        __auto_type _res = expr; \
        typeof(_res) _val = val; \
        if (_res == _val) { \
            _PASS(); \
        } \
        else { \
            char _fmt[100] = { 0 }; \
            snprintf(_fmt, sizeof _fmt, "failed test `%%s == %s' (is %s)", \
                    type_fmt, type_fmt); \
            _FAIL(_fmt, #expr, _val, _res); \
        } \
    } \
    while (0)

#define TEST_EQUAL(expr, val) \
    TEST_EQUAL_T(expr, val, \
            (_Generic((expr), \
                      unsigned int: "%u", \
                      int: "%d", \
                      float: "%0.1f")))

#define TEST_FRAME_EQUAL(item, x, y, width, height) \
    do { \
        struct flex_item *_item = item; \
        TEST_EQUAL(flex_item_get_frame_x(_item), x); \
        TEST_EQUAL(flex_item_get_frame_y(_item), y); \
        TEST_EQUAL(flex_item_get_frame_width(_item), width); \
        TEST_EQUAL(flex_item_get_frame_height(_item), height); \
    } \
    while (0)

static struct flex_item *
flex_item_with_size(float width, float height)
{
    struct flex_item *item = flex_item_new();
    flex_item_set_width(item, width);
    flex_item_set_height(item, height);
    return item;
}

static void
test_default_values(void)
{
    struct flex_item *item = flex_item_new();

    TEST(isnan(flex_item_get_width(item)));
    TEST(isnan(flex_item_get_height(item)));

    TEST(isnan(flex_item_get_left(item)));
    TEST(isnan(flex_item_get_right(item)));
    TEST(isnan(flex_item_get_top(item)));
    TEST(isnan(flex_item_get_bottom(item)));

    TEST(isnan(flex_item_get_padding_left(item)));
    TEST(isnan(flex_item_get_padding_right(item)));
    TEST(isnan(flex_item_get_padding_top(item)));
    TEST(isnan(flex_item_get_padding_bottom(item)));

    TEST(isnan(flex_item_get_margin_left(item)));
    TEST(isnan(flex_item_get_margin_right(item)));
    TEST(isnan(flex_item_get_margin_top(item)));
    TEST(isnan(flex_item_get_margin_bottom(item)));

    TEST(flex_item_get_align_content(item) == FLEX_ALIGN_AUTO);
    TEST(flex_item_get_align_items(item) == FLEX_ALIGN_AUTO);
    TEST(flex_item_get_align_self(item) == FLEX_ALIGN_AUTO);

    TEST(flex_item_get_position(item) == FLEX_POSITION_RELATIVE);
    TEST(flex_item_get_direction(item) == FLEX_DIRECTION_COLUMN);
    TEST(flex_item_get_wrap(item) == FLEX_WRAP_NOWRAP);

    TEST_EQUAL(flex_item_get_grow(item), 0);
    TEST_EQUAL(flex_item_get_shrink(item), 1);
    TEST_EQUAL(flex_item_get_order(item), 0);

    flex_item_free(item);
}

static void
test_grow1(void)
{
    struct flex_item *root = flex_item_with_size(60, 240);

    struct flex_item *child1 = flex_item_with_size(60, 30);
    flex_item_set_grow(child1, 0);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(60, 0);
    flex_item_set_grow(child2, 1);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(60, 0);
    flex_item_set_grow(child3, 2);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 60, 30);
    TEST_FRAME_EQUAL(child2, 0, 30, 60, 70);
    TEST_FRAME_EQUAL(child3, 0, 100, 60, 140);

    flex_item_free(root);
}

static void
test_grow2(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(100, 20);
    flex_item_set_grow(child1, 1);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(100, 20);
    flex_item_set_grow(child2, 0);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(100, 20);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 100, 60);
    TEST_FRAME_EQUAL(child2, 0, 60, 100, 20);
    TEST_FRAME_EQUAL(child3, 0, 80, 100, 20);

    flex_item_free(root);
}

static void
test_grow3(void)
{
    // The grow attributes aren't taken into account when there is no flexible
    // space available.
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(100, 50);
    flex_item_set_grow(child1, 2);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(100, 50);
    flex_item_set_grow(child2, 3);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 100, 50);
    TEST_FRAME_EQUAL(child2, 0, 50, 100, 50);

    flex_item_free(root);
}

static void
test_grow4(void)
{
    // The grow attribute is not inherited from children.
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_grow(root, 2);

    struct flex_item *child1 = flex_item_with_size(100, 25);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(100, 25);
    flex_item_add(root, child2);

    TEST_EQUAL(flex_item_get_grow(child1), 0);
    TEST_EQUAL(flex_item_get_grow(child2), 0);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 100, 25);
    TEST_FRAME_EQUAL(child2, 0, 25, 100, 25);

    flex_item_free(root);
}

static void
test_grow5(void)
{
    // All the container space is used when there is only one item with a
    // positive value for the grow attribute.
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(100, 25);
    flex_item_set_grow(child1, 1);
    flex_item_add(root, child1);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 100, 100);

    flex_item_free(root);
}

static void
test_grow6(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(100, 45);
    flex_item_set_grow(child1, 1);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(100, 45);
    flex_item_set_grow(child2, 1);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 100, 50);
    TEST_FRAME_EQUAL(child2, 0, 50, 100, 50);

    flex_item_free(root);
}

static void
test_shrink1(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(100, 100);
    flex_item_set_shrink(child1, 2);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(100, 100);
    flex_item_set_shrink(child2, 3);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 100, 60);
    TEST_FRAME_EQUAL(child2, 0, 60, 100, 40);

    flex_item_free(root);
}

static void
test_shrink2(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(100, 100);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(100, 100);
    flex_item_set_shrink(child2, 4);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 100, 80);
    TEST_FRAME_EQUAL(child2, 0, 80, 100, 20);

    flex_item_free(root);
}

static void
test_shrink3(void)
{
    // The shrink attributes are not taken into account when there is enough
    // flexible space available.
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(100, 40);
    flex_item_set_shrink(child1, 2);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(100, 40);
    flex_item_set_shrink(child2, 3);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 100, 40);
    TEST_FRAME_EQUAL(child2, 0, 40, 100, 40);

    flex_item_free(root);
}

static void
test_shrink4(void)
{
    // The shrink attribute is not inherited from children.
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_shrink(root, 2);

    struct flex_item *child1 = flex_item_with_size(100, 25);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(100, 25);
    flex_item_add(root, child2);

    TEST_EQUAL(flex_item_get_shrink(child1), 1);
    TEST_EQUAL(flex_item_get_shrink(child2), 1);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 100, 25);
    TEST_FRAME_EQUAL(child2, 0, 25, 100, 25);

    flex_item_free(root);
}

static void
test_shrink5(void)
{
    // All the container space is used when there is only one item with a
    // positive value for the shrink attribute.
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(100, 550);
    flex_item_set_shrink(child1, 1);
    flex_item_add(root, child1);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 100, 100);

    flex_item_free(root);
}

static void
test_shrink6(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(100, 75);
    flex_item_set_shrink(child1, 1);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(100, 75);
    flex_item_set_shrink(child2, 1);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 100, 50);
    TEST_FRAME_EQUAL(child2, 0, 50, 100, 50);

    flex_item_free(root);
}

static void
test_direction1(void)
{
    struct flex_item *root = flex_item_with_size(200, 200);
    flex_item_set_direction(root, FLEX_DIRECTION_ROW);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 50, 0, 50, 50);
    TEST_FRAME_EQUAL(child3, 100, 0, 50, 50);

    flex_item_free(root);
}

static void
test_direction2(void)
{
    struct flex_item *root = flex_item_with_size(200, 200);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 50, 50, 50);
    TEST_FRAME_EQUAL(child3, 0, 100, 50, 50);

    flex_item_free(root);
}

static void
test_direction3(void)
{
    struct flex_item *root = flex_item_with_size(200, 200);
    flex_item_set_direction(root, FLEX_DIRECTION_ROW_REVERSE);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 150, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 100, 0, 50, 50);
    TEST_FRAME_EQUAL(child3, 50, 0, 50, 50);

    flex_item_free(root);
}

static void
test_direction4(void)
{
    struct flex_item *root = flex_item_with_size(200, 200);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN_REVERSE);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 150, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 100, 50, 50);
    TEST_FRAME_EQUAL(child3, 0, 50, 50, 50);

    flex_item_free(root);
}

int
main(void)
{
    UNIT(default_values);

    UNIT(grow1);
    UNIT(grow2);
    UNIT(grow3);
    UNIT(grow4);
    UNIT(grow5);
    UNIT(grow6);

    UNIT(shrink1);
    UNIT(shrink2);
    UNIT(shrink3);
    UNIT(shrink4);
    UNIT(shrink5);
    UNIT(shrink6);

    UNIT(direction1);
    UNIT(direction2);
    UNIT(direction3);
    UNIT(direction4);

    if (failures_n > 0) {
        printf("\n");
        for (int i = 0; i < failures_n; i++) {
            printf("%s\n", failures[i]);
        }
        return 1;
    }
    printf(" OK\n");
    return 0;
}
