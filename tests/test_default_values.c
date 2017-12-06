// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

void
test_default_values1(void)
{
    struct flex_item *item = flex_item_new();

    TEST(isnan(flex_item_get_width(item)));
    TEST(isnan(flex_item_get_height(item)));
    TEST(flex_item_get_self_sizing(item) == NULL);

    TEST(isnan(flex_item_get_left(item)));
    TEST(isnan(flex_item_get_right(item)));
    TEST(isnan(flex_item_get_top(item)));
    TEST(isnan(flex_item_get_bottom(item)));

    TEST_EQUAL(flex_item_get_padding_left(item), 0);
    TEST_EQUAL(flex_item_get_padding_right(item), 0);
    TEST_EQUAL(flex_item_get_padding_top(item), 0);
    TEST_EQUAL(flex_item_get_padding_bottom(item), 0);

    TEST_EQUAL(flex_item_get_margin_left(item), 0);
    TEST_EQUAL(flex_item_get_margin_right(item), 0);
    TEST_EQUAL(flex_item_get_margin_top(item), 0);
    TEST_EQUAL(flex_item_get_margin_bottom(item), 0);

    TEST(flex_item_get_justify_content(item) == FLEX_ALIGN_START);
    TEST(flex_item_get_align_content(item) == FLEX_ALIGN_STRETCH);
    TEST(flex_item_get_align_items(item) == FLEX_ALIGN_STRETCH);
    TEST(flex_item_get_align_self(item) == FLEX_ALIGN_AUTO);

    TEST(flex_item_get_position(item) == FLEX_POSITION_RELATIVE);
    TEST(flex_item_get_direction(item) == FLEX_DIRECTION_COLUMN);
    TEST(flex_item_get_wrap(item) == FLEX_WRAP_NO_WRAP);

    TEST_EQUAL(flex_item_get_grow(item), 0);
    TEST_EQUAL(flex_item_get_shrink(item), 1);
    TEST_EQUAL(flex_item_get_order(item), 0);
    TEST(isnan(flex_item_get_basis(item)));

    flex_item_free(item);
}

void
test_default_values2(void)
{
    // If the width/height property isn't set on a child, it's frame size
    // defaults to 0 for the main axis and the parent's size for the cross
    // axis.
    struct flex_item *root = flex_item_with_size(200, 200);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN);

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

    TEST_EQUAL(flex_item_get_frame_width(child2), 200);
    TEST_EQUAL(flex_item_get_frame_height(child2), 100);

    TEST_EQUAL(flex_item_get_frame_width(child3), 200);
    TEST_EQUAL(flex_item_get_frame_height(child3), 0);

    flex_item_free(root);
}

void
test_default_values3(void)
{
    // If the width/height property isn't set on a child, it's frame size
    // defaults to 0 for the main axis and the parent's size for the cross
    // axis.
    struct flex_item *root = flex_item_with_size(200, 200);
    flex_item_set_direction(root, FLEX_DIRECTION_ROW);

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
    TEST_EQUAL(flex_item_get_frame_height(child1), 200);

    TEST_EQUAL(flex_item_get_frame_width(child2), 0);
    TEST_EQUAL(flex_item_get_frame_height(child2), 100);

    TEST_EQUAL(flex_item_get_frame_width(child3), 0);
    TEST_EQUAL(flex_item_get_frame_height(child3), 200);

    flex_item_free(root);
}
