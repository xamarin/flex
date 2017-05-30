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
test_default_values1(void)
{
    struct flex_item *item = flex_item_new();

    TEST(isnan(flex_item_get_width(item)));
    TEST(isnan(flex_item_get_height(item)));

    TEST_EQUAL(flex_item_get_left(item), 0);
    TEST_EQUAL(flex_item_get_right(item), 0);
    TEST_EQUAL(flex_item_get_top(item), 0);
    TEST_EQUAL(flex_item_get_bottom(item), 0);

    TEST_EQUAL(flex_item_get_padding_left(item), 0);
    TEST_EQUAL(flex_item_get_padding_right(item), 0);
    TEST_EQUAL(flex_item_get_padding_top(item), 0);
    TEST_EQUAL(flex_item_get_padding_bottom(item), 0);

    TEST_EQUAL(flex_item_get_margin_left(item), 0);
    TEST_EQUAL(flex_item_get_margin_right(item), 0);
    TEST_EQUAL(flex_item_get_margin_top(item), 0);
    TEST_EQUAL(flex_item_get_margin_bottom(item), 0);

    TEST(flex_item_get_justify_content(item) == FLEX_ALIGN_FLEX_START);
    TEST(flex_item_get_align_content(item) == FLEX_ALIGN_FLEX_START);
    TEST(flex_item_get_align_items(item) == FLEX_ALIGN_FLEX_START);
    TEST(flex_item_get_align_self(item) == FLEX_ALIGN_AUTO);

    TEST(flex_item_get_position(item) == FLEX_POSITION_RELATIVE);
    TEST(flex_item_get_direction(item) == FLEX_DIRECTION_COLUMN);
    TEST(flex_item_get_wrap(item) == FLEX_WRAP_NOWRAP);

    TEST_EQUAL(flex_item_get_grow(item), 0);
    TEST_EQUAL(flex_item_get_shrink(item), 1);
    TEST_EQUAL(flex_item_get_order(item), 0);
    TEST_EQUAL(flex_item_get_basis(item), 0);

    flex_item_free(item);
}

static void
test_default_values2(void)
{
    // If the width/height property isn't set, it defaults to 0 in the frame.
    struct flex_item *root = flex_item_with_size(200, 200);

    struct flex_item *child1 = flex_item_new();
    flex_item_set_width(child1, 100);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_new();
    flex_item_set_height(child2, 100);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_new();
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_EQUAL(flex_item_get_frame_width(child1), 100);
    TEST_EQUAL(flex_item_get_frame_height(child1), 0);

    TEST_EQUAL(flex_item_get_frame_width(child2), 0);
    TEST_EQUAL(flex_item_get_frame_height(child2), 100);

    TEST_EQUAL(flex_item_get_frame_width(child3), 0);
    TEST_EQUAL(flex_item_get_frame_height(child3), 0);

    flex_item_free(root);
}

static void
test_children1(void)
{
    struct flex_item *root = flex_item_new();

    TEST_EQUAL(flex_item_count(root), 0);
    TEST(flex_item_parent(root) == NULL);

    struct flex_item *child1 = flex_item_new();
    flex_item_add(root, child1);

    TEST_EQUAL(flex_item_count(root), 1);
    TEST(flex_item_parent(child1) == root);
    TEST(flex_item_child(root, 0) == child1);

    struct flex_item *child2 = flex_item_new();
    flex_item_add(root, child2);

    TEST_EQUAL(flex_item_count(root), 2);
    TEST(flex_item_parent(child2) == root);
    TEST(flex_item_child(root, 1) == child2);

    TEST(flex_item_delete(root, 0) == child1);

    TEST_EQUAL(flex_item_count(root), 1);
    TEST(flex_item_parent(child1) == NULL);
    TEST(flex_item_child(root, 0) == child2);

    flex_item_insert(root, 0, child1);

    TEST_EQUAL(flex_item_count(root), 2);
    TEST(flex_item_parent(child1) == root);
    TEST(flex_item_child(root, 0) == child1);
    TEST(flex_item_child(root, 1) == child2);

    flex_item_free(root);
}

static void
test_children2(void)
{
#define CENTER_ITEM(width, height) \
    ({ \
        struct flex_item *_item = flex_item_with_size(width, height); \
        flex_item_set_align_items(_item, FLEX_ALIGN_CENTER); \
        flex_item_set_justify_content(_item, FLEX_ALIGN_CENTER); \
        _item; \
    })

    struct flex_item *root = CENTER_ITEM(100, 100);

    struct flex_item *child1 = CENTER_ITEM(90, 90);
    flex_item_add(root, child1);

    struct flex_item *child2 = CENTER_ITEM(80, 80);
    flex_item_add(child1, child2);

    struct flex_item *child3 = CENTER_ITEM(70, 70);
    flex_item_add(child2, child3);

    struct flex_item *child4 = CENTER_ITEM(60, 60);
    flex_item_add(child3, child4);

    struct flex_item *child5 = CENTER_ITEM(50, 50);
    flex_item_add(child4, child5);

    flex_layout(root);

    TEST(flex_item_child(root, 0) == child1);
    TEST(flex_item_child(child1, 0) == child2);
    TEST(flex_item_child(child2, 0) == child3);
    TEST(flex_item_child(child3, 0) == child4);
    TEST(flex_item_child(child4, 0) == child5);

    TEST_FRAME_EQUAL(child1, 5, 5, 90, 90);
    TEST_FRAME_EQUAL(child2, 5, 5, 80, 80);
    TEST_FRAME_EQUAL(child3, 5, 5, 70, 70);
    TEST_FRAME_EQUAL(child4, 5, 5, 60, 60);
    TEST_FRAME_EQUAL(child5, 5, 5, 50, 50);

    flex_item_free(root);

#undef CENTER_ITEM
}

static void
test_basis1(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_new();
    flex_item_set_width(child1, 100);
    flex_item_set_basis(child1, 60);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(100, 40);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 100, 60);
    TEST_FRAME_EQUAL(child2, 0, 60, 100, 40);

    flex_item_free(root);
}

static void
test_basis2(void)
{
    // The basis attribute has priority over width/height.
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(100, 40);
    flex_item_set_basis(child1, 60);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(100, 40);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 100, 60);
    TEST_FRAME_EQUAL(child2, 0, 60, 100, 40);

    flex_item_free(root);
}

static void
test_basis3(void)
{
    // The basis attribute is ignored if negative.
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_new();
    flex_item_set_width(child1, 100);
    flex_item_set_basis(child1, -60);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(100, 40);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 100, 0);
    TEST_FRAME_EQUAL(child2, 0, 0, 100, 40);

    flex_item_free(root);
}

static void
test_basis4(void)
{
    // The basis attribute is ignored if negative.
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(100, 40);
    flex_item_set_basis(child1, -60);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(100, 40);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 100, 40);
    TEST_FRAME_EQUAL(child2, 0, 40, 100, 40);

    flex_item_free(root);
}

static void
test_basis5(void)
{
    // The basis attribute is ignored if 0.
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(100, 40);
    flex_item_set_basis(child1, 0);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(100, 40);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 100, 40);
    TEST_FRAME_EQUAL(child2, 0, 40, 100, 40);

    flex_item_free(root);
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

static void
test_order1(void)
{
    struct flex_item *root = flex_item_with_size(200, 200);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_set_order(child1, 1);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_set_order(child2, 3);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_set_order(child3, 2);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 100, 50, 50);
    TEST_FRAME_EQUAL(child3, 0, 50, 50, 50);

    flex_item_free(root);
}

static void
test_order2(void)
{
    struct flex_item *root = flex_item_with_size(200, 200);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN_REVERSE);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_set_order(child1, 2);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_set_order(child2, 3);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_set_order(child3, 1);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 100, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 50, 50, 50);
    TEST_FRAME_EQUAL(child3, 0, 150, 50, 50);

    flex_item_free(root);
}

static void
test_order3(void)
{
    struct flex_item *root = flex_item_with_size(200, 200);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_set_order(child2, -1);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 50, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 0, 50, 50);
    TEST_FRAME_EQUAL(child3, 0, 100, 50, 50);

    flex_item_free(root);
}

static void
test_order4(void)
{
    // This test ensures that the insertion order of the children is preserved
    // when they get re-ordered during layout.
    struct flex_item *root = flex_item_with_size(200, 200);

    const int max = 100;
    for (int i = 0; i < max; i++) {
        flex_item_add(root, flex_item_with_size(1, 1));
    }

    flex_item_set_order(flex_item_child(root, 0), 1);
    flex_item_set_order(flex_item_child(root, max - 1), -1);

    flex_layout(root);

    TEST_FRAME_EQUAL(flex_item_child(root, max - 1), 0, 0, 1, 1);

    for (int i = 1; i < max - 1; i++) {
        TEST_FRAME_EQUAL(flex_item_child(root, i), 0, i, 1, 1);
    }

    TEST_FRAME_EQUAL(flex_item_child(root, 0), 0, max - 1, 1, 1);
}

static void
test_align_self1(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child1, FLEX_ALIGN_FLEX_START);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child2, FLEX_ALIGN_FLEX_START);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child3, FLEX_ALIGN_FLEX_START);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 25);
    TEST_FRAME_EQUAL(child2, 0, 25, 50, 25);
    TEST_FRAME_EQUAL(child3, 0, 50, 50, 25);

    flex_item_free(root);
}

static void
test_align_self2(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child1, FLEX_ALIGN_FLEX_END);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child2, FLEX_ALIGN_FLEX_END);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child3, FLEX_ALIGN_FLEX_END);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 50, 0, 50, 25);
    TEST_FRAME_EQUAL(child2, 50, 25, 50, 25);
    TEST_FRAME_EQUAL(child3, 50, 50, 50, 25);

    flex_item_free(root);
}

static void
test_align_self3(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child1, FLEX_ALIGN_CENTER);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child2, FLEX_ALIGN_CENTER);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child3, FLEX_ALIGN_CENTER);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 25, 0, 50, 25);
    TEST_FRAME_EQUAL(child2, 25, 25, 50, 25);
    TEST_FRAME_EQUAL(child3, 25, 50, 50, 25);

    flex_item_free(root);
}

static void
test_align_self4(void)
{
    // Stretch works if the align dimension is not set or is 0.
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_new();
    flex_item_set_height(child1, 25);
    flex_item_set_align_self(child1, FLEX_ALIGN_STRETCH);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(0, 25);
    flex_item_set_align_self(child2, FLEX_ALIGN_STRETCH);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_new();
    flex_item_set_height(child3, 25);
    flex_item_set_align_self(child3, FLEX_ALIGN_STRETCH);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 100, 25);
    TEST_FRAME_EQUAL(child2, 0, 25, 100, 25);
    TEST_FRAME_EQUAL(child3, 0, 50, 100, 25);

    flex_item_free(root);
}

static void
test_align_self5(void)
{
    // Stretch does not work if the align dimension is set.
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child1, FLEX_ALIGN_STRETCH);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_set_align_self(child2, FLEX_ALIGN_STRETCH);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child3, FLEX_ALIGN_STRETCH);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 25);
    TEST_FRAME_EQUAL(child2, 0, 25, 50, 50);
    TEST_FRAME_EQUAL(child3, 0, 75, 50, 25);

    flex_item_free(root);
}

static void
test_align_self6(void)
{
    // Mixed config.
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child1, FLEX_ALIGN_FLEX_START);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child2, FLEX_ALIGN_CENTER);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(0, 25);
    flex_item_set_align_self(child3, FLEX_ALIGN_STRETCH);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child4, FLEX_ALIGN_FLEX_END);
    flex_item_add(root, child4);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 25);
    TEST_FRAME_EQUAL(child2, 25, 25, 50, 25);
    TEST_FRAME_EQUAL(child3, 0, 50, 100, 25);
    TEST_FRAME_EQUAL(child4, 50, 75, 50, 25);

    flex_item_free(root);
}

static void
test_align_items1(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_align_items(root, FLEX_ALIGN_FLEX_START);

    struct flex_item *child1 = flex_item_with_size(50, 25);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 25);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 25);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 25);
    TEST_FRAME_EQUAL(child2, 0, 25, 50, 25);
    TEST_FRAME_EQUAL(child3, 0, 50, 50, 25);

    flex_item_free(root);
}

static void
test_align_items2(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_align_items(root, FLEX_ALIGN_FLEX_END);

    struct flex_item *child1 = flex_item_with_size(50, 25);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 25);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 25);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 50, 0, 50, 25);
    TEST_FRAME_EQUAL(child2, 50, 25, 50, 25);
    TEST_FRAME_EQUAL(child3, 50, 50, 50, 25);

    flex_item_free(root);
}

static void
test_align_items3(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_align_items(root, FLEX_ALIGN_CENTER);

    struct flex_item *child1 = flex_item_with_size(50, 25);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 25);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 25);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 25, 0, 50, 25);
    TEST_FRAME_EQUAL(child2, 25, 25, 50, 25);
    TEST_FRAME_EQUAL(child3, 25, 50, 50, 25);

    flex_item_free(root);
}

static void
test_align_items4(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_align_items(root, FLEX_ALIGN_STRETCH);

    struct flex_item *child1 = flex_item_with_size(50, 25);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(0, 25);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_new();
    flex_item_set_height(child3, 25);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 25);
    TEST_FRAME_EQUAL(child2, 0, 25, 100, 25);
    TEST_FRAME_EQUAL(child3, 0, 50, 100, 25);

    flex_item_free(root);
}

static void
test_align_items5(void)
{
    // The align_items property is ignored for children with a align_self
    // property set to a value that's not _AUTO (the default). 
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_align_items(root, FLEX_ALIGN_CENTER);

    struct flex_item *child1 = flex_item_with_size(50, 25);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child2, FLEX_ALIGN_FLEX_START);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child3, FLEX_ALIGN_AUTO);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child4, FLEX_ALIGN_FLEX_END);
    flex_item_add(root, child4);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 25, 0, 50, 25);
    TEST_FRAME_EQUAL(child2, 0, 25, 50, 25);
    TEST_FRAME_EQUAL(child3, 25, 50, 50, 25);
    TEST_FRAME_EQUAL(child4, 50, 75, 50, 25);

    flex_item_free(root);
}

static void
test_wrap1(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_wrap(root, FLEX_WRAP_NOWRAP);

    struct flex_item *child1 = flex_item_with_size(100, 150);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(100, 150);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(100, 150);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(100, 150);
    flex_item_add(root, child4);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 100, 75);
    TEST_FRAME_EQUAL(child2, 0, 75, 100, 75);
    TEST_FRAME_EQUAL(child3, 0, 150, 100, 75);
    TEST_FRAME_EQUAL(child4, 0, 225, 100, 75);

    flex_item_free(root);
}

static void
test_wrap2(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);

    struct flex_item *child1 = flex_item_with_size(50, 150);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 150);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 150);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(50, 150);
    flex_item_add(root, child4);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 150);
    TEST_FRAME_EQUAL(child2, 0, 150, 50, 150);
    TEST_FRAME_EQUAL(child3, 50, 0, 50, 150);
    TEST_FRAME_EQUAL(child4, 50, 150, 50, 150);

    flex_item_free(root);
}

static void
test_wrap3(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 50, 50, 50);
    TEST_FRAME_EQUAL(child3, 50, 0, 50, 50);

    flex_item_free(root);
}

static void
test_wrap4(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);

    struct flex_item *child1 = flex_item_with_size(25, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(25, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(25, 50);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(25, 50);
    flex_item_add(root, child4);

    struct flex_item *child5 = flex_item_with_size(25, 50);
    flex_item_add(root, child5);

    struct flex_item *child6 = flex_item_with_size(25, 50);
    flex_item_add(root, child6);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 25, 50);
    TEST_FRAME_EQUAL(child2, 0, 50, 25, 50);
    TEST_FRAME_EQUAL(child3, 25, 0, 25, 50);
    TEST_FRAME_EQUAL(child4, 25, 50, 25, 50);
    TEST_FRAME_EQUAL(child5, 50, 0, 25, 50);
    TEST_FRAME_EQUAL(child6, 50, 50, 25, 50);

    flex_item_free(root);
}

static void
test_wrap5(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_justify_content(root, FLEX_ALIGN_FLEX_END);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 20, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 70, 50, 50);
    TEST_FRAME_EQUAL(child3, 50, 70, 50, 50);

    flex_item_free(root);
}

static void
test_wrap6(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_justify_content(root, FLEX_ALIGN_CENTER);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 10, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 60, 50, 50);
    TEST_FRAME_EQUAL(child3, 50, 35, 50, 50);

    flex_item_free(root);
}

static void
test_wrap7(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_justify_content(root, FLEX_ALIGN_SPACE_AROUND);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 5, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 65, 50, 50);
    TEST_FRAME_EQUAL(child3, 50, 35, 50, 50);

    flex_item_free(root);
}

static void
test_wrap8(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_justify_content(root, FLEX_ALIGN_SPACE_BETWEEN);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 70, 50, 50);
    TEST_FRAME_EQUAL(child3, 50, 0, 50, 50);

    flex_item_free(root);
}

static void
test_wrap9(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_set_grow(child2, 1);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_set_grow(child3, 1);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(50, 50);
    flex_item_add(root, child4);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 50, 50, 70);
    TEST_FRAME_EQUAL(child3, 50, 0, 50, 70);
    TEST_FRAME_EQUAL(child4, 50, 70, 50, 50);

    flex_item_free(root);
}

static void
test_wrap10(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_align_items(root, FLEX_ALIGN_FLEX_START);

    struct flex_item *child1 = flex_item_with_size(50, 40);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(70, 30);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(60, 40);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(40, 50);
    flex_item_add(root, child4);

    struct flex_item *child5 = flex_item_with_size(50, 60);
    flex_item_add(root, child5);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 40);
    TEST_FRAME_EQUAL(child2, 0, 40, 70, 30);
    TEST_FRAME_EQUAL(child3, 0, 70, 60, 40);
    TEST_FRAME_EQUAL(child4, 70, 0, 40, 50);
    TEST_FRAME_EQUAL(child5, 70, 50, 50, 60);

    flex_item_free(root);
}

static void
test_wrap11(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_align_items(root, FLEX_ALIGN_CENTER);

    struct flex_item *child1 = flex_item_with_size(50, 40);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(70, 30);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(60, 40);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(40, 50);
    flex_item_add(root, child4);

    struct flex_item *child5 = flex_item_with_size(50, 60);
    flex_item_add(root, child5);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 10, 0, 50, 40);
    TEST_FRAME_EQUAL(child2, 0, 40, 70, 30);
    TEST_FRAME_EQUAL(child3, 5, 70, 60, 40);
    TEST_FRAME_EQUAL(child4, 75, 0, 40, 50);
    TEST_FRAME_EQUAL(child5, 70, 50, 50, 60);

    flex_item_free(root);
}

static void
test_wrap12(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_align_items(root, FLEX_ALIGN_FLEX_END);

    struct flex_item *child1 = flex_item_with_size(50, 40);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(70, 30);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(60, 40);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(40, 50);
    flex_item_add(root, child4);

    struct flex_item *child5 = flex_item_with_size(50, 60);
    flex_item_add(root, child5);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 20, 0, 50, 40);
    TEST_FRAME_EQUAL(child2, 0, 40, 70, 30);
    TEST_FRAME_EQUAL(child3, 10, 70, 60, 40);
    TEST_FRAME_EQUAL(child4, 80, 0, 40, 50);
    TEST_FRAME_EQUAL(child5, 70, 50, 50, 60);

    flex_item_free(root);
}

static void
test_wrap13(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);

    struct flex_item *child1 = flex_item_with_size(50, 40);
    flex_item_set_align_self(child1, FLEX_ALIGN_FLEX_END);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(70, 30);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(60, 40);
    flex_item_set_align_self(child3, FLEX_ALIGN_CENTER);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(40, 50);
    flex_item_set_align_self(child4, FLEX_ALIGN_FLEX_START);
    flex_item_add(root, child4);

    struct flex_item *child5 = flex_item_with_size(50, 60);
    flex_item_add(root, child5);

    struct flex_item *child6 = flex_item_with_size(10, 10);
    flex_item_set_align_self(child6, FLEX_ALIGN_FLEX_END);
    flex_item_add(root, child6);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 20, 0, 50, 40);
    TEST_FRAME_EQUAL(child2, 0, 40, 70, 30);
    TEST_FRAME_EQUAL(child3, 5, 70, 60, 40);
    TEST_FRAME_EQUAL(child4, 70, 0, 40, 50);
    TEST_FRAME_EQUAL(child5, 70, 50, 50, 60);
    TEST_FRAME_EQUAL(child6, 110, 110, 10, 10);

    flex_item_free(root);
}

static void
test_wrap14(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP_REVERSE);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 70, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 70, 50, 50, 50);
    TEST_FRAME_EQUAL(child3, 20, 0, 50, 50);

    flex_item_free(root);
}

static void
test_wrap15(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP_REVERSE);

    struct flex_item *child1 = flex_item_with_size(25, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(25, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(25, 50);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(25, 50);
    flex_item_add(root, child4);

    struct flex_item *child5 = flex_item_with_size(25, 50);
    flex_item_add(root, child5);

    struct flex_item *child6 = flex_item_with_size(25, 50);
    flex_item_add(root, child6);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 95, 0, 25, 50);
    TEST_FRAME_EQUAL(child2, 95, 50, 25, 50);
    TEST_FRAME_EQUAL(child3, 70, 0, 25, 50);
    TEST_FRAME_EQUAL(child4, 70, 50, 25, 50);
    TEST_FRAME_EQUAL(child5, 45, 0, 25, 50);
    TEST_FRAME_EQUAL(child6, 45, 50, 25, 50);

    flex_item_free(root);
}

static void
test_justify_content1(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_justify_content(root, FLEX_ALIGN_CENTER);

    struct flex_item *child1 = flex_item_with_size(50, 100);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 100);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 50, 50, 100);
    TEST_FRAME_EQUAL(child2, 0, 150, 50, 100);

    flex_item_free(root);
}

static void
test_justify_content2(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_justify_content(root, FLEX_ALIGN_FLEX_START);

    struct flex_item *child1 = flex_item_with_size(50, 100);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 100);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 100);
    TEST_FRAME_EQUAL(child2, 0, 100, 50, 100);

    flex_item_free(root);
}

static void
test_justify_content3(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_justify_content(root, FLEX_ALIGN_FLEX_END);

    struct flex_item *child1 = flex_item_with_size(50, 100);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 100);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 100, 50, 100);
    TEST_FRAME_EQUAL(child2, 0, 200, 50, 100);

    flex_item_free(root);
}

static void
test_justify_content4(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_justify_content(root, FLEX_ALIGN_SPACE_BETWEEN);

    struct flex_item *child1 = flex_item_with_size(50, 100);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 100);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 100);
    TEST_FRAME_EQUAL(child2, 0, 200, 50, 100);

    flex_item_free(root);
}

static void
test_justify_content5(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_justify_content(root, FLEX_ALIGN_SPACE_BETWEEN);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 125, 50, 50);
    TEST_FRAME_EQUAL(child3, 0, 250, 50, 50);

    flex_item_free(root);
}

static void
test_justify_content6(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_justify_content(root, FLEX_ALIGN_SPACE_AROUND);

    struct flex_item *child1 = flex_item_with_size(50, 100);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 100);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 25, 50, 100);
    TEST_FRAME_EQUAL(child2, 0, 175, 50, 100);

    flex_item_free(root);
}

static void
test_justify_content7(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_justify_content(root, FLEX_ALIGN_SPACE_AROUND);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 25, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 125, 50, 50);
    TEST_FRAME_EQUAL(child3, 0, 225, 50, 50);

    flex_item_free(root);
}

static void
test_justify_content8(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_justify_content(root, FLEX_ALIGN_SPACE_EVENLY);

    struct flex_item *child1 = flex_item_with_size(50, 105);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 105);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 30, 50, 105);
    TEST_FRAME_EQUAL(child2, 0, 165, 50, 105);

    flex_item_free(root);
}

static void
test_justify_content9(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_justify_content(root, FLEX_ALIGN_SPACE_EVENLY);

    struct flex_item *child1 = flex_item_with_size(50, 40);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 40);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 40);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 45, 50, 40);
    TEST_FRAME_EQUAL(child2, 0, 130, 50, 40);
    TEST_FRAME_EQUAL(child3, 0, 215, 50, 40);

    flex_item_free(root);
}

static void
test_justify_content10(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN_REVERSE);
    flex_item_set_justify_content(root, FLEX_ALIGN_CENTER);

    struct flex_item *child1 = flex_item_with_size(50, 100);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 100);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 150, 50, 100);
    TEST_FRAME_EQUAL(child2, 0, 50, 50, 100);

    flex_item_free(root);
}

static void
test_justify_content11(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN_REVERSE);
    flex_item_set_justify_content(root, FLEX_ALIGN_FLEX_START);

    struct flex_item *child1 = flex_item_with_size(50, 100);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 100);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 200, 50, 100);
    TEST_FRAME_EQUAL(child2, 0, 100, 50, 100);

    flex_item_free(root);
}

static void
test_justify_content12(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN_REVERSE);
    flex_item_set_justify_content(root, FLEX_ALIGN_FLEX_END);

    struct flex_item *child1 = flex_item_with_size(50, 100);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 100);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 100, 50, 100);
    TEST_FRAME_EQUAL(child2, 0, 0, 50, 100);

    flex_item_free(root);
}

static void
test_justify_content13(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN_REVERSE);
    flex_item_set_justify_content(root, FLEX_ALIGN_SPACE_BETWEEN);

    struct flex_item *child1 = flex_item_with_size(50, 100);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 100);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 200, 50, 100);
    TEST_FRAME_EQUAL(child2, 0, 0, 50, 100);

    flex_item_free(root);
}

static void
test_justify_content14(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN_REVERSE);
    flex_item_set_justify_content(root, FLEX_ALIGN_SPACE_AROUND);

    struct flex_item *child1 = flex_item_with_size(50, 100);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 100);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 175, 50, 100);
    TEST_FRAME_EQUAL(child2, 0, 25, 50, 100);

    flex_item_free(root);
}

static void
test_justify_content15(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN_REVERSE);
    flex_item_set_justify_content(root, FLEX_ALIGN_SPACE_EVENLY);

    struct flex_item *child1 = flex_item_with_size(50, 105);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 105);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 165, 50, 105);
    TEST_FRAME_EQUAL(child2, 0, 30, 50, 105);

    flex_item_free(root);
}

static void
test_justify_content16(void)
{
    // The `justify_content' property is ignored when the children fill up
    // all the space.
    int modes[] = {
        FLEX_ALIGN_CENTER,
        FLEX_ALIGN_FLEX_START,
        FLEX_ALIGN_FLEX_END,
        FLEX_ALIGN_SPACE_BETWEEN,
        FLEX_ALIGN_SPACE_AROUND,
        FLEX_ALIGN_SPACE_EVENLY
    };
    for (int i = 0; i < sizeof(modes) / sizeof(int); i++) {
        struct flex_item *root = flex_item_with_size(100, 100);
        flex_item_set_justify_content(root, modes[i]);

        struct flex_item *child1 = flex_item_with_size(50, 50);
        flex_item_add(root, child1);

        struct flex_item *child2 = flex_item_with_size(50, 50);
        flex_item_add(root, child2);

        flex_layout(root);

        TEST_FRAME_EQUAL(child1, 0, 0, 50, 50);
        TEST_FRAME_EQUAL(child2, 0, 50, 50, 50);

        flex_item_free(root);
    }
}

static void
test_justify_content17(void)
{
    // The `justify_content' property is ignored when the children fill up
    // more than the space.
    int modes[] = {
        FLEX_ALIGN_CENTER,
        FLEX_ALIGN_FLEX_START,
        FLEX_ALIGN_FLEX_END,
        FLEX_ALIGN_SPACE_BETWEEN,
        FLEX_ALIGN_SPACE_AROUND,
        FLEX_ALIGN_SPACE_EVENLY
    };
    for (int i = 0; i < sizeof(modes) / sizeof(int); i++) {
        struct flex_item *root = flex_item_with_size(100, 100);
        flex_item_set_justify_content(root, modes[i]);

        struct flex_item *child1 = flex_item_with_size(50, 100);
        flex_item_add(root, child1);

        struct flex_item *child2 = flex_item_with_size(50, 100);
        flex_item_add(root, child2);

        struct flex_item *child3 = flex_item_with_size(50, 100);
        flex_item_add(root, child3);

        struct flex_item *child4 = flex_item_with_size(50, 100);
        flex_item_add(root, child4);

        flex_layout(root);

        TEST_FRAME_EQUAL(child1, 0, 0, 50, 25);
        TEST_FRAME_EQUAL(child2, 0, 25, 50, 25);
        TEST_FRAME_EQUAL(child3, 0, 50, 50, 25);
        TEST_FRAME_EQUAL(child4, 0, 75, 50, 25);

        flex_item_free(root);
    }
}

static void
test_justify_content18(void)
{
    // The `justify_content' property is ignored when there are flexible
    // children.
    int modes[] = {
        FLEX_ALIGN_CENTER,
        FLEX_ALIGN_FLEX_START,
        FLEX_ALIGN_FLEX_END,
        FLEX_ALIGN_SPACE_BETWEEN,
        FLEX_ALIGN_SPACE_AROUND,
        FLEX_ALIGN_SPACE_EVENLY
    };
    for (int i = 0; i < sizeof(modes) / sizeof(int); i++) {
        struct flex_item *root = flex_item_with_size(100, 100);
        flex_item_set_justify_content(root, modes[i]);

        struct flex_item *child1 = flex_item_with_size(50, 20);
        flex_item_add(root, child1);

        struct flex_item *child2 = flex_item_with_size(50, 20);
        flex_item_set_grow(child2, 1);
        flex_item_add(root, child2);

        struct flex_item *child3 = flex_item_with_size(50, 20);
        flex_item_add(root, child3);

        flex_layout(root);

        TEST_FRAME_EQUAL(child1, 0, 0, 50, 20);
        TEST_FRAME_EQUAL(child2, 0, 20, 50, 60);
        TEST_FRAME_EQUAL(child3, 0, 80, 50, 20);

        flex_item_free(root);
    }
}

static void
test_align_content1(void)
{
    struct flex_item *root = flex_item_with_size(200, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_align_content(root, FLEX_ALIGN_FLEX_START);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(60, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(40, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 50, 60, 50);
    TEST_FRAME_EQUAL(child3, 60, 0, 40, 50);

    flex_item_free(root);
}

static void
test_align_content2(void)
{
    struct flex_item *root = flex_item_with_size(200, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_align_content(root, FLEX_ALIGN_CENTER);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(60, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(40, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 50, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 50, 50, 60, 50);
    TEST_FRAME_EQUAL(child3, 110, 0, 40, 50);

    flex_item_free(root);
}

static void
test_align_content3(void)
{
    struct flex_item *root = flex_item_with_size(200, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_align_content(root, FLEX_ALIGN_FLEX_END);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(60, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(40, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 100, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 100, 50, 60, 50);
    TEST_FRAME_EQUAL(child3, 160, 0, 40, 50);

    flex_item_free(root);
}

static void
test_align_content4(void)
{
    struct flex_item *root = flex_item_with_size(200, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_align_content(root, FLEX_ALIGN_SPACE_BETWEEN);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(60, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(40, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 50, 60, 50);
    TEST_FRAME_EQUAL(child3, 160, 0, 40, 50);

    flex_item_free(root);
}

static void
test_align_content5(void)
{
    struct flex_item *root = flex_item_with_size(200, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_align_content(root, FLEX_ALIGN_SPACE_AROUND);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(60, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(40, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 25, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 25, 50, 60, 50);
    TEST_FRAME_EQUAL(child3, 135, 0, 40, 50);

    flex_item_free(root);
}

static void
test_align_content6(void)
{
    struct flex_item *root = flex_item_with_size(250, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_align_content(root, FLEX_ALIGN_SPACE_EVENLY);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(60, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(40, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 50, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 50, 50, 60, 50);
    TEST_FRAME_EQUAL(child3, 160, 0, 40, 50);

    flex_item_free(root);
}

static void
test_margin1(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_align_items(root, FLEX_ALIGN_FLEX_START);
    flex_item_set_justify_content(root, FLEX_ALIGN_FLEX_START);

    struct flex_item *child1 = flex_item_with_size(25, 25);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(25, 25);
    flex_item_set_margin_top(child2, 10);
    flex_item_set_margin_bottom(child2, 10);
    flex_item_set_margin_left(child2, 15);
    flex_item_set_margin_right(child2, 15);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(25, 25);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 25, 25);
    TEST_FRAME_EQUAL(child2, 15, 35, 25, 25);
    TEST_FRAME_EQUAL(child3, 0, 70, 25, 25);

    flex_item_free(root);
}

static void
test_margin2(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_align_items(root, FLEX_ALIGN_FLEX_END);
    flex_item_set_justify_content(root, FLEX_ALIGN_FLEX_START);

    struct flex_item *child1 = flex_item_with_size(25, 25);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(25, 25);
    flex_item_set_margin_top(child2, 10);
    flex_item_set_margin_bottom(child2, 10);
    flex_item_set_margin_left(child2, 15);
    flex_item_set_margin_right(child2, 15);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(25, 25);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 75, 0, 25, 25);
    TEST_FRAME_EQUAL(child2, 60, 35, 25, 25);
    TEST_FRAME_EQUAL(child3, 75, 70, 25, 25);

    flex_item_free(root);
}

static void
test_margin3(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_align_items(root, FLEX_ALIGN_FLEX_START);
    flex_item_set_justify_content(root, FLEX_ALIGN_FLEX_END);

    struct flex_item *child1 = flex_item_with_size(25, 25);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(25, 25);
    flex_item_set_margin_top(child2, 10);
    flex_item_set_margin_bottom(child2, 10);
    flex_item_set_margin_left(child2, 15);
    flex_item_set_margin_right(child2, 15);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(25, 25);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 5, 25, 25);
    TEST_FRAME_EQUAL(child2, 15, 40, 25, 25);
    TEST_FRAME_EQUAL(child3, 0, 75, 25, 25);

    flex_item_free(root);
}

static void
test_margin4(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_align_items(root, FLEX_ALIGN_FLEX_END);
    flex_item_set_justify_content(root, FLEX_ALIGN_FLEX_END);

    struct flex_item *child1 = flex_item_with_size(25, 25);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(25, 25);
    flex_item_set_margin_top(child2, 10);
    flex_item_set_margin_bottom(child2, 10);
    flex_item_set_margin_left(child2, 15);
    flex_item_set_margin_right(child2, 15);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(25, 25);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 75, 5, 25, 25);
    TEST_FRAME_EQUAL(child2, 60, 40, 25, 25);
    TEST_FRAME_EQUAL(child3, 75, 75, 25, 25);

    flex_item_free(root);
}

static void
test_margin5(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_align_items(root, FLEX_ALIGN_CENTER);
    flex_item_set_justify_content(root, FLEX_ALIGN_FLEX_START);

    struct flex_item *child1 = flex_item_with_size(10, 10);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(10, 10);
    flex_item_set_margin_left(child2, 15);
    flex_item_set_margin_right(child2, 10);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(10, 10);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 45, 0, 10, 10);
    TEST_FRAME_EQUAL(child2, 50, 10, 10, 10);
    TEST_FRAME_EQUAL(child3, 45, 20, 10, 10);

    flex_item_free(root);
}

static void
test_margin6(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(10, 10);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(0, 10);
    flex_item_set_align_self(child2, FLEX_ALIGN_STRETCH);
    flex_item_set_margin_left(child2, 15);
    flex_item_set_margin_left(child2, 15);
    flex_item_set_margin_right(child2, 10);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(10, 10);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 10, 10);
    TEST_FRAME_EQUAL(child2, 15, 10, 75, 10);
    TEST_FRAME_EQUAL(child3, 0, 20, 10, 10);

    flex_item_free(root);
}

static void
test_margin7(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(10, 10);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(10, 10);
    flex_item_set_align_self(child2, FLEX_ALIGN_STRETCH);
    flex_item_set_margin_left(child2, 15);
    flex_item_set_margin_left(child2, 15);
    flex_item_set_margin_right(child2, 10);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(10, 10);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 10, 10);
    TEST_FRAME_EQUAL(child2, 15, 10, 10, 10);
    TEST_FRAME_EQUAL(child3, 0, 20, 10, 10);

    flex_item_free(root);
}

static void
test_padding1(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN);
    flex_item_set_justify_content(root, FLEX_ALIGN_FLEX_START);
    flex_item_set_align_items(root, FLEX_ALIGN_FLEX_START);

    flex_item_set_padding_top(root, 15);
    flex_item_set_padding_left(root, 10);
    flex_item_set_padding_right(root, 15);
    flex_item_set_padding_bottom(root, 10);

    struct flex_item *child1 = flex_item_with_size(25, 25);
    flex_item_add(root, child1);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 10, 15, 25, 25);

    flex_item_free(root);
}

static void
test_padding2(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN);
    flex_item_set_justify_content(root, FLEX_ALIGN_FLEX_END);
    flex_item_set_align_items(root, FLEX_ALIGN_FLEX_START);

    flex_item_set_padding_top(root, 15);
    flex_item_set_padding_left(root, 10);
    flex_item_set_padding_right(root, 15);
    flex_item_set_padding_bottom(root, 10);

    struct flex_item *child1 = flex_item_with_size(25, 25);
    flex_item_add(root, child1);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 10, 65, 25, 25);

    flex_item_free(root);
}

static void
test_padding3(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN);
    flex_item_set_justify_content(root, FLEX_ALIGN_FLEX_END);
    flex_item_set_align_items(root, FLEX_ALIGN_FLEX_END);

    flex_item_set_padding_top(root, 15);
    flex_item_set_padding_left(root, 10);
    flex_item_set_padding_right(root, 15);
    flex_item_set_padding_bottom(root, 10);

    struct flex_item *child1 = flex_item_with_size(25, 25);
    flex_item_add(root, child1);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 60, 65, 25, 25);

    flex_item_free(root);
}

static void
test_padding4(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN);
    flex_item_set_justify_content(root, FLEX_ALIGN_FLEX_START);
    flex_item_set_align_items(root, FLEX_ALIGN_FLEX_END);

    flex_item_set_padding_top(root, 15);
    flex_item_set_padding_left(root, 10);
    flex_item_set_padding_right(root, 15);
    flex_item_set_padding_bottom(root, 10);

    struct flex_item *child1 = flex_item_with_size(25, 25);
    flex_item_add(root, child1);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 60, 15, 25, 25);

    flex_item_free(root);
}

static void
test_padding5(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN);
    flex_item_set_justify_content(root, FLEX_ALIGN_FLEX_START);
    flex_item_set_align_items(root, FLEX_ALIGN_FLEX_START);

    flex_item_set_padding_top(root, 15);
    flex_item_set_padding_left(root, 10);
    flex_item_set_padding_right(root, 15);
    flex_item_set_padding_bottom(root, 10);

    struct flex_item *child1 = flex_item_with_size(0, 25);
    flex_item_set_align_self(child1, FLEX_ALIGN_STRETCH);
    flex_item_add(root, child1);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 10, 15, 75, 25);

    flex_item_free(root);
}

int
main(void)
{
    UNIT(default_values1);
    UNIT(default_values2);

    UNIT(children1);
    UNIT(children2);

    UNIT(basis1);
    UNIT(basis2);
    UNIT(basis3);
    UNIT(basis4);
    UNIT(basis5);

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

    UNIT(order1);
    UNIT(order2);
    UNIT(order3);
    UNIT(order4);

    UNIT(align_self1);
    UNIT(align_self2);
    UNIT(align_self3);
    UNIT(align_self4);
    UNIT(align_self5);
    UNIT(align_self6);

    UNIT(align_items1);
    UNIT(align_items2);
    UNIT(align_items3);
    UNIT(align_items4);
    UNIT(align_items5);

    UNIT(wrap1);
    UNIT(wrap2);
    UNIT(wrap3);
    UNIT(wrap4);
    UNIT(wrap5);
    UNIT(wrap6);
    UNIT(wrap7);
    UNIT(wrap8);
    UNIT(wrap9);
    UNIT(wrap10);
    UNIT(wrap11);
    UNIT(wrap12);
    UNIT(wrap13);
    UNIT(wrap14);
    UNIT(wrap15);

    UNIT(justify_content1);
    UNIT(justify_content2);
    UNIT(justify_content3);
    UNIT(justify_content4);
    UNIT(justify_content5);
    UNIT(justify_content6);
    UNIT(justify_content7);
    UNIT(justify_content8);
    UNIT(justify_content9);
    UNIT(justify_content10);
    UNIT(justify_content11);
    UNIT(justify_content12);
    UNIT(justify_content13);
    UNIT(justify_content14);
    UNIT(justify_content15);
    UNIT(justify_content16);
    UNIT(justify_content17);
    UNIT(justify_content18);

    UNIT(align_content1);
    UNIT(align_content2);
    UNIT(align_content3);
    UNIT(align_content4);
    UNIT(align_content5);
    UNIT(align_content6);

    UNIT(margin1);
    UNIT(margin2);
    UNIT(margin3);
    UNIT(margin4);
    UNIT(margin5);
    UNIT(margin6);
    UNIT(margin7);

    UNIT(padding1);
    UNIT(padding2);
    UNIT(padding3);
    UNIT(padding4);
    UNIT(padding5);

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
