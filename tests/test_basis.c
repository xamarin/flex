// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

void
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

void
test_basis2(void)
{
    // The basis attribute has priority over the main-axis size.
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

void
test_basis3(void)
{
    // The basis attribute is honored if 0.
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(100, 40);
    flex_item_set_basis(child1, 0);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(100, 40);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 100, 0);
    TEST_FRAME_EQUAL(child2, 0, 0, 100, 40);

    flex_item_free(root);
}
