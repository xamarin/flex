// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

void
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

void
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

void
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

void
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

void
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

void
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

void
test_shrink7(void)
{
    // Shrink can be floating point.
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(100, 75);
    flex_item_set_shrink(child1, 1);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(100, 75);
    flex_item_set_shrink(child2, 1.5);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 100, 55);
    TEST_FRAME_EQUAL(child2, 0, 55, 100, 45);

    flex_item_free(root);
}
