// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

void
test_padding1(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN);
    flex_item_set_justify_content(root, FLEX_ALIGN_START);
    flex_item_set_align_items(root, FLEX_ALIGN_START);

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

void
test_padding2(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN);
    flex_item_set_justify_content(root, FLEX_ALIGN_END);
    flex_item_set_align_items(root, FLEX_ALIGN_START);

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

void
test_padding3(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN);
    flex_item_set_justify_content(root, FLEX_ALIGN_END);
    flex_item_set_align_items(root, FLEX_ALIGN_END);

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

void
test_padding4(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN);
    flex_item_set_justify_content(root, FLEX_ALIGN_START);
    flex_item_set_align_items(root, FLEX_ALIGN_END);

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

void
test_padding5(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN);
    flex_item_set_justify_content(root, FLEX_ALIGN_START);
    flex_item_set_align_items(root, FLEX_ALIGN_START);

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
