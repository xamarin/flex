// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

void
test_justify_content1(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_justify_content(root, FLEX_ALIGN_CENTER);

    struct flex_item *child1 = flex_item_with_size(50, 100);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 100);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 50, 50, 100);
    TEST_FRAME_EQUAL(child2, 0, 150, 50, 100);

    flex_item_free(root);
}

void
test_justify_content2(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_justify_content(root, FLEX_ALIGN_START);

    struct flex_item *child1 = flex_item_with_size(50, 100);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 100);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 100);
    TEST_FRAME_EQUAL(child2, 0, 100, 50, 100);

    flex_item_free(root);
}

void
test_justify_content3(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_justify_content(root, FLEX_ALIGN_END);

    struct flex_item *child1 = flex_item_with_size(50, 100);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 100);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 100, 50, 100);
    TEST_FRAME_EQUAL(child2, 0, 200, 50, 100);

    flex_item_free(root);
}

void
test_justify_content4(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_justify_content(root, FLEX_ALIGN_SPACE_BETWEEN);

    struct flex_item *child1 = flex_item_with_size(50, 100);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 100);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 100);
    TEST_FRAME_EQUAL(child2, 0, 200, 50, 100);

    flex_item_free(root);
}

void
test_justify_content5(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_justify_content(root, FLEX_ALIGN_SPACE_BETWEEN);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 125, 50, 50);
    TEST_FRAME_EQUAL(child3, 0, 250, 50, 50);

    flex_item_free(root);
}

void
test_justify_content6(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_justify_content(root, FLEX_ALIGN_SPACE_AROUND);

    struct flex_item *child1 = flex_item_with_size(50, 100);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 100);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 25, 50, 100);
    TEST_FRAME_EQUAL(child2, 0, 175, 50, 100);

    flex_item_free(root);
}

void
test_justify_content7(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_justify_content(root, FLEX_ALIGN_SPACE_AROUND);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 25, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 125, 50, 50);
    TEST_FRAME_EQUAL(child3, 0, 225, 50, 50);

    flex_item_free(root);
}

void
test_justify_content8(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_justify_content(root, FLEX_ALIGN_SPACE_EVENLY);

    struct flex_item *child1 = flex_item_with_size(50, 105);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 105);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 30, 50, 105);
    TEST_FRAME_EQUAL(child2, 0, 165, 50, 105);

    flex_item_free(root);
}

void
test_justify_content9(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_justify_content(root, FLEX_ALIGN_SPACE_EVENLY);

    struct flex_item *child1 = flex_item_with_size(50, 40);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 40);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 40);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 45, 50, 40);
    TEST_FRAME_EQUAL(child2, 0, 130, 50, 40);
    TEST_FRAME_EQUAL(child3, 0, 215, 50, 40);

    flex_item_free(root);
}

void
test_justify_content10(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN_REVERSE);
    flex_item_set_justify_content(root, FLEX_ALIGN_CENTER);

    struct flex_item *child1 = flex_item_with_size(50, 100);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 100);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 150, 50, 100);
    TEST_FRAME_EQUAL(child2, 0, 50, 50, 100);

    flex_item_free(root);
}

void
test_justify_content11(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN_REVERSE);
    flex_item_set_justify_content(root, FLEX_ALIGN_START);

    struct flex_item *child1 = flex_item_with_size(50, 100);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 100);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 200, 50, 100);
    TEST_FRAME_EQUAL(child2, 0, 100, 50, 100);

    flex_item_free(root);
}

void
test_justify_content12(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN_REVERSE);
    flex_item_set_justify_content(root, FLEX_ALIGN_END);

    struct flex_item *child1 = flex_item_with_size(50, 100);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 100);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 100, 50, 100);
    TEST_FRAME_EQUAL(child2, 0, 0, 50, 100);

    flex_item_free(root);
}

void
test_justify_content13(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN_REVERSE);
    flex_item_set_justify_content(root, FLEX_ALIGN_SPACE_BETWEEN);

    struct flex_item *child1 = flex_item_with_size(50, 100);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 100);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 200, 50, 100);
    TEST_FRAME_EQUAL(child2, 0, 0, 50, 100);

    flex_item_free(root);
}

void
test_justify_content14(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN_REVERSE);
    flex_item_set_justify_content(root, FLEX_ALIGN_SPACE_AROUND);

    struct flex_item *child1 = flex_item_with_size(50, 100);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 100);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 175, 50, 100);
    TEST_FRAME_EQUAL(child2, 0, 25, 50, 100);

    flex_item_free(root);
}

void
test_justify_content15(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_direction(root, FLEX_DIRECTION_COLUMN_REVERSE);
    flex_item_set_justify_content(root, FLEX_ALIGN_SPACE_EVENLY);

    struct flex_item *child1 = flex_item_with_size(50, 105);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 105);
    flex_item_add(root, child2);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 165, 50, 105);
    TEST_FRAME_EQUAL(child2, 0, 30, 50, 105);

    flex_item_free(root);
}

void
test_justify_content16(void)
{
    // The `justify_content' property is ignored when the children fill up
    // all the space.
    int modes[] = {
        FLEX_ALIGN_CENTER,
        FLEX_ALIGN_START,
        FLEX_ALIGN_END,
        FLEX_ALIGN_SPACE_BETWEEN,
        FLEX_ALIGN_SPACE_AROUND,
        FLEX_ALIGN_SPACE_EVENLY
    };
    for (int i = 0; i < sizeof(modes) / sizeof(int); i++) {
        struct flex_item *root = flex_item_with_size(100, 100);
        flex_item_set_justify_content(root, modes[i]);

        struct flex_item *child1 = flex_item_with_size(50, 50);
        flex_item_add(root, child1);

        struct flex_item *child2 = flex_item_with_size(50, 50);
        flex_item_add(root, child2);

        flex_layout(root);

        TEST_FRAME_EQUAL(child1, 0, 0, 50, 50);
        TEST_FRAME_EQUAL(child2, 0, 50, 50, 50);

        flex_item_free(root);
    }
}

void
test_justify_content17(void)
{
    // The `justify_content' property is ignored when the children fill up
    // more than the space.
    int modes[] = {
        FLEX_ALIGN_CENTER,
        FLEX_ALIGN_START,
        FLEX_ALIGN_END,
        FLEX_ALIGN_SPACE_BETWEEN,
        FLEX_ALIGN_SPACE_AROUND,
        FLEX_ALIGN_SPACE_EVENLY
    };
    for (int i = 0; i < sizeof(modes) / sizeof(int); i++) {
        struct flex_item *root = flex_item_with_size(100, 100);
        flex_item_set_justify_content(root, modes[i]);

        struct flex_item *child1 = flex_item_with_size(50, 100);
        flex_item_add(root, child1);

        struct flex_item *child2 = flex_item_with_size(50, 100);
        flex_item_add(root, child2);

        struct flex_item *child3 = flex_item_with_size(50, 100);
        flex_item_add(root, child3);

        struct flex_item *child4 = flex_item_with_size(50, 100);
        flex_item_add(root, child4);

        flex_layout(root);

        TEST_FRAME_EQUAL(child1, 0, 0, 50, 25);
        TEST_FRAME_EQUAL(child2, 0, 25, 50, 25);
        TEST_FRAME_EQUAL(child3, 0, 50, 50, 25);
        TEST_FRAME_EQUAL(child4, 0, 75, 50, 25);

        flex_item_free(root);
    }
}

void
test_justify_content18(void)
{
    // The `justify_content' property is ignored when there are flexible
    // children.
    int modes[] = {
        FLEX_ALIGN_CENTER,
        FLEX_ALIGN_START,
        FLEX_ALIGN_END,
        FLEX_ALIGN_SPACE_BETWEEN,
        FLEX_ALIGN_SPACE_AROUND,
        FLEX_ALIGN_SPACE_EVENLY
    };
    for (int i = 0; i < sizeof(modes) / sizeof(int); i++) {
        struct flex_item *root = flex_item_with_size(100, 100);
        flex_item_set_justify_content(root, modes[i]);

        struct flex_item *child1 = flex_item_with_size(50, 20);
        flex_item_add(root, child1);

        struct flex_item *child2 = flex_item_with_size(50, 20);
        flex_item_set_grow(child2, 1);
        flex_item_add(root, child2);

        struct flex_item *child3 = flex_item_with_size(50, 20);
        flex_item_add(root, child3);

        flex_layout(root);

        TEST_FRAME_EQUAL(child1, 0, 0, 50, 20);
        TEST_FRAME_EQUAL(child2, 0, 20, 50, 60);
        TEST_FRAME_EQUAL(child3, 0, 80, 50, 20);

        flex_item_free(root);
    }
}
