// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

void
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

void
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

void
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

void
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
