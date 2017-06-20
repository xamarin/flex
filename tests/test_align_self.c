// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

void
test_align_self1(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child1, FLEX_ALIGN_START);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child2, FLEX_ALIGN_START);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child3, FLEX_ALIGN_START);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 25);
    TEST_FRAME_EQUAL(child2, 0, 25, 50, 25);
    TEST_FRAME_EQUAL(child3, 0, 50, 50, 25);

    flex_item_free(root);
}

void
test_align_self2(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child1, FLEX_ALIGN_END);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child2, FLEX_ALIGN_END);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child3, FLEX_ALIGN_END);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 50, 0, 50, 25);
    TEST_FRAME_EQUAL(child2, 50, 25, 50, 25);
    TEST_FRAME_EQUAL(child3, 50, 50, 50, 25);

    flex_item_free(root);
}

void
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

void
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

void
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

void
test_align_self6(void)
{
    // Mixed config.
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child1, FLEX_ALIGN_START);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child2, FLEX_ALIGN_CENTER);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(0, 25);
    flex_item_set_align_self(child3, FLEX_ALIGN_STRETCH);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child4, FLEX_ALIGN_END);
    flex_item_add(root, child4);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 25);
    TEST_FRAME_EQUAL(child2, 25, 25, 50, 25);
    TEST_FRAME_EQUAL(child3, 0, 50, 100, 25);
    TEST_FRAME_EQUAL(child4, 50, 75, 50, 25);

    flex_item_free(root);
}
