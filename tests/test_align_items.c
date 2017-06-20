// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

void
test_align_items1(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_align_items(root, FLEX_ALIGN_START);

    struct flex_item *child1 = flex_item_with_size(50, 25);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 25);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 25);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 25);
    TEST_FRAME_EQUAL(child2, 0, 25, 50, 25);
    TEST_FRAME_EQUAL(child3, 0, 50, 50, 25);

    flex_item_free(root);
}

void
test_align_items2(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_align_items(root, FLEX_ALIGN_END);

    struct flex_item *child1 = flex_item_with_size(50, 25);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 25);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 25);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 50, 0, 50, 25);
    TEST_FRAME_EQUAL(child2, 50, 25, 50, 25);
    TEST_FRAME_EQUAL(child3, 50, 50, 50, 25);

    flex_item_free(root);
}

void
test_align_items3(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_align_items(root, FLEX_ALIGN_CENTER);

    struct flex_item *child1 = flex_item_with_size(50, 25);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 25);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 25);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 25, 0, 50, 25);
    TEST_FRAME_EQUAL(child2, 25, 25, 50, 25);
    TEST_FRAME_EQUAL(child3, 25, 50, 50, 25);

    flex_item_free(root);
}

void
test_align_items4(void)
{
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_align_items(root, FLEX_ALIGN_STRETCH);

    struct flex_item *child1 = flex_item_with_size(50, 25);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(0, 25);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_new();
    flex_item_set_height(child3, 25);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 25);
    TEST_FRAME_EQUAL(child2, 0, 25, 100, 25);
    TEST_FRAME_EQUAL(child3, 0, 50, 100, 25);

    flex_item_free(root);
}

void
test_align_items5(void)
{
    // The align_items property is ignored for children with a align_self
    // property set to a value that's not _AUTO (the default). 
    struct flex_item *root = flex_item_with_size(100, 100);
    flex_item_set_align_items(root, FLEX_ALIGN_CENTER);

    struct flex_item *child1 = flex_item_with_size(50, 25);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child2, FLEX_ALIGN_START);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child3, FLEX_ALIGN_AUTO);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(50, 25);
    flex_item_set_align_self(child4, FLEX_ALIGN_END);
    flex_item_add(root, child4);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 25, 0, 50, 25);
    TEST_FRAME_EQUAL(child2, 0, 25, 50, 25);
    TEST_FRAME_EQUAL(child3, 25, 50, 50, 25);
    TEST_FRAME_EQUAL(child4, 50, 75, 50, 25);

    flex_item_free(root);
}
