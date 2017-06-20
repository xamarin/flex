// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

void
test_align_content1(void)
{
    struct flex_item *root = flex_item_with_size(200, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_align_content(root, FLEX_ALIGN_START);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(60, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(40, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 50, 60, 50);
    TEST_FRAME_EQUAL(child3, 60, 0, 40, 50);

    flex_item_free(root);
}

void
test_align_content2(void)
{
    struct flex_item *root = flex_item_with_size(200, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_align_content(root, FLEX_ALIGN_CENTER);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(60, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(40, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 50, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 50, 50, 60, 50);
    TEST_FRAME_EQUAL(child3, 110, 0, 40, 50);

    flex_item_free(root);
}

void
test_align_content3(void)
{
    struct flex_item *root = flex_item_with_size(200, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_align_content(root, FLEX_ALIGN_END);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(60, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(40, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 100, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 100, 50, 60, 50);
    TEST_FRAME_EQUAL(child3, 160, 0, 40, 50);

    flex_item_free(root);
}

void
test_align_content4(void)
{
    struct flex_item *root = flex_item_with_size(200, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_align_content(root, FLEX_ALIGN_SPACE_BETWEEN);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(60, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(40, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 50, 60, 50);
    TEST_FRAME_EQUAL(child3, 160, 0, 40, 50);

    flex_item_free(root);
}

void
test_align_content5(void)
{
    struct flex_item *root = flex_item_with_size(200, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_align_content(root, FLEX_ALIGN_SPACE_AROUND);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(60, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(40, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 25, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 25, 50, 60, 50);
    TEST_FRAME_EQUAL(child3, 135, 0, 40, 50);

    flex_item_free(root);
}

void
test_align_content6(void)
{
    struct flex_item *root = flex_item_with_size(250, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_align_content(root, FLEX_ALIGN_SPACE_EVENLY);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(60, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(40, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 50, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 50, 50, 60, 50);
    TEST_FRAME_EQUAL(child3, 160, 0, 40, 50);

    flex_item_free(root);
}
