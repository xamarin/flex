// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

void
test_wrap1(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_wrap(root, FLEX_WRAP_NO_WRAP);

    struct flex_item *child1 = flex_item_with_size(100, 150);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(100, 150);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(100, 150);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(100, 150);
    flex_item_add(root, child4);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 100, 75);
    TEST_FRAME_EQUAL(child2, 0, 75, 100, 75);
    TEST_FRAME_EQUAL(child3, 0, 150, 100, 75);
    TEST_FRAME_EQUAL(child4, 0, 225, 100, 75);

    flex_item_free(root);
}

void
test_wrap2(void)
{
    struct flex_item *root = flex_item_with_size(100, 300);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);

    struct flex_item *child1 = flex_item_with_size(50, 150);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 150);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 150);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(50, 150);
    flex_item_add(root, child4);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 150);
    TEST_FRAME_EQUAL(child2, 0, 150, 50, 150);
    TEST_FRAME_EQUAL(child3, 50, 0, 50, 150);
    TEST_FRAME_EQUAL(child4, 50, 150, 50, 150);

    flex_item_free(root);
}

void
test_wrap3(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 50, 50, 50);
    TEST_FRAME_EQUAL(child3, 50, 0, 50, 50);

    flex_item_free(root);
}

void
test_wrap4(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);

    struct flex_item *child1 = flex_item_with_size(25, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(25, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(25, 50);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(25, 50);
    flex_item_add(root, child4);

    struct flex_item *child5 = flex_item_with_size(25, 50);
    flex_item_add(root, child5);

    struct flex_item *child6 = flex_item_with_size(25, 50);
    flex_item_add(root, child6);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 25, 50);
    TEST_FRAME_EQUAL(child2, 0, 50, 25, 50);
    TEST_FRAME_EQUAL(child3, 25, 0, 25, 50);
    TEST_FRAME_EQUAL(child4, 25, 50, 25, 50);
    TEST_FRAME_EQUAL(child5, 50, 0, 25, 50);
    TEST_FRAME_EQUAL(child6, 50, 50, 25, 50);

    flex_item_free(root);
}

void
test_wrap5(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_justify_content(root, FLEX_ALIGN_END);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 20, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 70, 50, 50);
    TEST_FRAME_EQUAL(child3, 50, 70, 50, 50);

    flex_item_free(root);
}

void
test_wrap6(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_justify_content(root, FLEX_ALIGN_CENTER);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 10, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 60, 50, 50);
    TEST_FRAME_EQUAL(child3, 50, 35, 50, 50);

    flex_item_free(root);
}

void
test_wrap7(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_justify_content(root, FLEX_ALIGN_SPACE_AROUND);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 5, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 65, 50, 50);
    TEST_FRAME_EQUAL(child3, 50, 35, 50, 50);

    flex_item_free(root);
}

void
test_wrap8(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_justify_content(root, FLEX_ALIGN_SPACE_BETWEEN);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 70, 50, 50);
    TEST_FRAME_EQUAL(child3, 50, 0, 50, 50);

    flex_item_free(root);
}

void
test_wrap9(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_set_grow(child2, 1);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_set_grow(child3, 1);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(50, 50);
    flex_item_add(root, child4);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 0, 50, 50, 70);
    TEST_FRAME_EQUAL(child3, 50, 0, 50, 70);
    TEST_FRAME_EQUAL(child4, 50, 70, 50, 50);

    flex_item_free(root);
}

void
test_wrap10(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_align_items(root, FLEX_ALIGN_START);

    struct flex_item *child1 = flex_item_with_size(50, 40);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(70, 30);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(60, 40);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(40, 50);
    flex_item_add(root, child4);

    struct flex_item *child5 = flex_item_with_size(50, 60);
    flex_item_add(root, child5);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 0, 0, 50, 40);
    TEST_FRAME_EQUAL(child2, 0, 40, 70, 30);
    TEST_FRAME_EQUAL(child3, 0, 70, 60, 40);
    TEST_FRAME_EQUAL(child4, 70, 0, 40, 50);
    TEST_FRAME_EQUAL(child5, 70, 50, 50, 60);

    flex_item_free(root);
}

void
test_wrap11(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_align_items(root, FLEX_ALIGN_CENTER);

    struct flex_item *child1 = flex_item_with_size(50, 40);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(70, 30);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(60, 40);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(40, 50);
    flex_item_add(root, child4);

    struct flex_item *child5 = flex_item_with_size(50, 60);
    flex_item_add(root, child5);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 10, 0, 50, 40);
    TEST_FRAME_EQUAL(child2, 0, 40, 70, 30);
    TEST_FRAME_EQUAL(child3, 5, 70, 60, 40);
    TEST_FRAME_EQUAL(child4, 75, 0, 40, 50);
    TEST_FRAME_EQUAL(child5, 70, 50, 50, 60);

    flex_item_free(root);
}

void
test_wrap12(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);
    flex_item_set_align_items(root, FLEX_ALIGN_END);

    struct flex_item *child1 = flex_item_with_size(50, 40);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(70, 30);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(60, 40);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(40, 50);
    flex_item_add(root, child4);

    struct flex_item *child5 = flex_item_with_size(50, 60);
    flex_item_add(root, child5);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 20, 0, 50, 40);
    TEST_FRAME_EQUAL(child2, 0, 40, 70, 30);
    TEST_FRAME_EQUAL(child3, 10, 70, 60, 40);
    TEST_FRAME_EQUAL(child4, 80, 0, 40, 50);
    TEST_FRAME_EQUAL(child5, 70, 50, 50, 60);

    flex_item_free(root);
}

void
test_wrap13(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP);

    struct flex_item *child1 = flex_item_with_size(50, 40);
    flex_item_set_align_self(child1, FLEX_ALIGN_END);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(70, 30);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(60, 40);
    flex_item_set_align_self(child3, FLEX_ALIGN_CENTER);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(40, 50);
    flex_item_set_align_self(child4, FLEX_ALIGN_START);
    flex_item_add(root, child4);

    struct flex_item *child5 = flex_item_with_size(50, 60);
    flex_item_add(root, child5);

    struct flex_item *child6 = flex_item_with_size(10, 10);
    flex_item_set_align_self(child6, FLEX_ALIGN_END);
    flex_item_add(root, child6);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 20, 0, 50, 40);
    TEST_FRAME_EQUAL(child2, 0, 40, 70, 30);
    TEST_FRAME_EQUAL(child3, 5, 70, 60, 40);
    TEST_FRAME_EQUAL(child4, 70, 0, 40, 50);
    TEST_FRAME_EQUAL(child5, 70, 50, 50, 60);
    TEST_FRAME_EQUAL(child6, 110, 110, 10, 10);

    flex_item_free(root);
}

void
test_wrap14(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP_REVERSE);

    struct flex_item *child1 = flex_item_with_size(50, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(50, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(50, 50);
    flex_item_add(root, child3);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 70, 0, 50, 50);
    TEST_FRAME_EQUAL(child2, 70, 50, 50, 50);
    TEST_FRAME_EQUAL(child3, 20, 0, 50, 50);

    flex_item_free(root);
}

void
test_wrap15(void)
{
    struct flex_item *root = flex_item_with_size(120, 120);
    flex_item_set_wrap(root, FLEX_WRAP_WRAP_REVERSE);

    struct flex_item *child1 = flex_item_with_size(25, 50);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(25, 50);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(25, 50);
    flex_item_add(root, child3);

    struct flex_item *child4 = flex_item_with_size(25, 50);
    flex_item_add(root, child4);

    struct flex_item *child5 = flex_item_with_size(25, 50);
    flex_item_add(root, child5);

    struct flex_item *child6 = flex_item_with_size(25, 50);
    flex_item_add(root, child6);

    flex_layout(root);

    TEST_FRAME_EQUAL(child1, 95, 0, 25, 50);
    TEST_FRAME_EQUAL(child2, 95, 50, 25, 50);
    TEST_FRAME_EQUAL(child3, 70, 0, 25, 50);
    TEST_FRAME_EQUAL(child4, 70, 50, 25, 50);
    TEST_FRAME_EQUAL(child5, 45, 0, 25, 50);
    TEST_FRAME_EQUAL(child6, 45, 50, 25, 50);

    flex_item_free(root);
}
