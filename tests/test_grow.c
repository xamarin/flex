// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

void
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

void
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

void
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

void
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

void
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

void
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

void
test_grow7(void)
{
    // Sizes of flexible items should be ignored when growing.
    struct flex_item *root = flex_item_with_size(500, 600);

    struct flex_item *child1 = flex_item_with_size(250, 0);
    flex_item_set_grow(child1, 1);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(250, 50);
    flex_item_set_grow(child2, 1);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(250, 0);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(250, 0);
    flex_item_set_grow(child4, 1);
    flex_item_add(root, child4);

    struct flex_item *child5 = flex_item_with_size(250, 0);
    flex_item_add(root, child5);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 250, 200);
    TEST_FRAME_EQUAL(child2, 0, 200, 250, 200);
    TEST_FRAME_EQUAL(child3, 0, 400, 250, 0);
    TEST_FRAME_EQUAL(child4, 0, 400, 250, 200);
    TEST_FRAME_EQUAL(child5, 0, 600, 250, 0);

    flex_item_free(root);
}

void
test_grow8(void)
{
    // Grow can be floating point.
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(100, 10);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(100, 20);
    flex_item_set_grow(child2, 1);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(100, 20);
    flex_item_set_grow(child3, 1.5);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 100, 10);
    TEST_FRAME_EQUAL(child2, 0, 10, 100, 36);
    TEST_FRAME_EQUAL(child3, 0, 46, 100, 54);

    flex_item_free(root);
}
