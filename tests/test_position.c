// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

void
test_position1(void)
{
    // Items with an absolute position default to the left/top corner.
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(10, 10);
    flex_item_set_position(child1, FLEX_POSITION_ABSOLUTE);
    flex_item_add(root, child1);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 10, 10);

    flex_item_free(root);
}

void
test_position2(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(10, 10);
    flex_item_set_position(child1, FLEX_POSITION_ABSOLUTE);
    flex_item_set_left(child1, 10);
    flex_item_set_top(child1, 10);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(10, 10);
    flex_item_set_position(child2, FLEX_POSITION_ABSOLUTE);
    flex_item_set_right(child2, 10);
    flex_item_set_top(child2, 10);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(10, 10);
    flex_item_set_position(child3, FLEX_POSITION_ABSOLUTE);
    flex_item_set_right(child3, 10);
    flex_item_set_bottom(child3, 10);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(10, 10);
    flex_item_set_position(child4, FLEX_POSITION_ABSOLUTE);
    flex_item_set_left(child4, 10);
    flex_item_set_bottom(child4, 10);
    flex_item_add(root, child4);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 10, 10, 10, 10);
    TEST_FRAME_EQUAL(child2, 80, 10, 10, 10);
    TEST_FRAME_EQUAL(child3, 80, 80, 10, 10);
    TEST_FRAME_EQUAL(child4, 10, 80, 10, 10);

    flex_item_free(root);
}

void
test_position3(void)
{
    // If both left/right or top/bottom are given, left/top get the priority
    // if the item has the appropriate size dimension set.
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(10, 10);
    flex_item_set_position(child1, FLEX_POSITION_ABSOLUTE);
    flex_item_set_left(child1, 10);
    flex_item_set_right(child1, 10);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(10, 10);
    flex_item_set_position(child2, FLEX_POSITION_ABSOLUTE);
    flex_item_set_top(child2, 10);
    flex_item_set_bottom(child2, 10);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 10, 0, 10, 10);
    TEST_FRAME_EQUAL(child2, 0, 10, 10, 10);

    flex_item_free(root);
}

void
test_position4(void)
{
    // If both left/right or top/bottom are given, the item is properly resized
    // if the appropriate size dimension hasn't been set.
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_new();
    flex_item_set_height(child1, 20);
    flex_item_set_position(child1, FLEX_POSITION_ABSOLUTE);
    flex_item_set_left(child1, 10);
    flex_item_set_right(child1, 10);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_new();
    flex_item_set_width(child2, 20);
    flex_item_set_position(child2, FLEX_POSITION_ABSOLUTE);
    flex_item_set_top(child2, 10);
    flex_item_set_bottom(child2, 10);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 10, 0, 80, 20);
    TEST_FRAME_EQUAL(child2, 0, 10, 20, 80);

    flex_item_free(root);
}

void
test_position5(void)
{
    // The `basis' property is ignored for items with an absolute position.
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(10, 10);
    flex_item_set_basis(child1, 20);
    flex_item_set_position(child1, FLEX_POSITION_ABSOLUTE);
    flex_item_set_left(child1, 10);
    flex_item_set_bottom(child1, 10);
    flex_item_add(root, child1);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 10, 80, 10, 10);

    flex_item_free(root);
}

void
test_position6(void)
{
    // Items with an absolute position are separated from the other items
    // during the layout.
    struct flex_item *root = flex_item_with_size(200, 200);
    flex_item_set_direction(root, FLEX_DIRECTION_ROW);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_set_position(child2, FLEX_POSITION_ABSOLUTE);
    flex_item_set_right(child2, 0);
    flex_item_set_bottom(child2, 0);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 150, 150, 50, 50);
    TEST_FRAME_EQUAL(child3, 50, 0, 50, 50);

    flex_item_free(root);
}

void
test_position7(void)
{
    // Items with an absolute position are separated from the other items
    // during the layout and are not taken into account when calculating
    // spacing.
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_justify_content(root, FLEX_ALIGN_SPACE_AROUND);
    flex_item_set_align_content(root, FLEX_ALIGN_START);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_set_position(child3, FLEX_POSITION_ABSOLUTE);
    flex_item_set_right(child3, 0);
    flex_item_set_top(child3, 0);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(50, 50);
    flex_item_add(root, child4);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 5, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 65, 50, 50);
    TEST_FRAME_EQUAL(child3, 70, 0, 50, 50);
    TEST_FRAME_EQUAL(child4, 50, 35, 50, 50);

    flex_item_free(root);
}

void
test_position8(void)
{
    // Items with an absolute position can be nested.
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_direction(root, FLEX_DIRECTION_ROW);

    struct flex_item *child1 = flex_item_new();
    flex_item_set_position(child1, FLEX_POSITION_ABSOLUTE);
    flex_item_set_left(child1, 10);
    flex_item_set_top(child1, 10);
    flex_item_set_right(child1, 10);
    flex_item_set_bottom(child1, 10);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(60, 60);
    flex_item_set_position(child2, FLEX_POSITION_ABSOLUTE);
    flex_item_set_right(child2, 10);
    flex_item_set_top(child2, 10);
    flex_item_add(child1, child2);

    struct flex_item *child3 = flex_item_with_size(40, 40);
    flex_item_set_position(child3, FLEX_POSITION_ABSOLUTE);
    flex_item_set_left(child3, 10);
    flex_item_set_bottom(child3, 10);
    flex_item_add(child2, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 10, 10, 80, 80);
    TEST_FRAME_EQUAL(child2, 10, 10, 60, 60);
    TEST_FRAME_EQUAL(child3, 10, 10, 40, 40);

    flex_item_free(root);
}
