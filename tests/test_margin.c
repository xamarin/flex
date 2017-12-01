// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

void
test_margin1(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_align_items(root, FLEX_ALIGN_START);
    flex_item_set_justify_content(root, FLEX_ALIGN_START);

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

void
test_margin2(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_align_items(root, FLEX_ALIGN_END);
    flex_item_set_justify_content(root, FLEX_ALIGN_START);

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

void
test_margin3(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_align_items(root, FLEX_ALIGN_START);
    flex_item_set_justify_content(root, FLEX_ALIGN_END);

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

void
test_margin4(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_align_items(root, FLEX_ALIGN_END);
    flex_item_set_justify_content(root, FLEX_ALIGN_END);

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

void
test_margin5(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_align_items(root, FLEX_ALIGN_CENTER);
    flex_item_set_justify_content(root, FLEX_ALIGN_START);

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

void
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

void
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

void
test_margin8(void)
{
    // When defaulting to the parent's cross axis size, margins must be taken
    // into account.
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN);

    struct flex_item *child1 = flex_item_new();
    flex_item_set_height(child1, 10);
    flex_item_set_margin_left(child1, 10);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_new();
    flex_item_set_height(child2, 10);
    flex_item_set_margin_right(child2, 10);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_new();
    flex_item_set_height(child3, 10);
    flex_item_set_margin_left(child3, 10);
    flex_item_set_margin_right(child3, 20);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 10, 0, 90, 10);
    TEST_FRAME_EQUAL(child2, 0, 10, 90, 10);
    TEST_FRAME_EQUAL(child3, 10, 20, 70, 10);

    flex_item_free(root);
}

void
test_margin9(void)
{
    // When defaulting to the parent's cross axis size, margins must be taken
    // into account.
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_direction(root, FLEX_DIRECTION_ROW);

    struct flex_item *child1 = flex_item_new();
    flex_item_set_width(child1, 10);
    flex_item_set_margin_top(child1, 10);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_new();
    flex_item_set_width(child2, 10);
    flex_item_set_margin_bottom(child2, 10);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_new();
    flex_item_set_width(child3, 10);
    flex_item_set_margin_top(child3, 10);
    flex_item_set_margin_bottom(child3, 20);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 10, 10, 90);
    TEST_FRAME_EQUAL(child2, 10, 0, 10, 90);
    TEST_FRAME_EQUAL(child3, 20, 10, 10, 70);

    flex_item_free(root);
}

void
test_margin10(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_direction(root, FLEX_DIRECTION_ROW);

    struct flex_item *child1 = flex_item_new();
    flex_item_set_grow(child1, 1);
    flex_item_set_margin_left(child1, 10);
    flex_item_set_margin_right(child1, 10);
    flex_item_add(root, child1);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 10, 0, 80, 100);

    flex_item_free(root);
}
