// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

void
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

void
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

void
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

void
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
