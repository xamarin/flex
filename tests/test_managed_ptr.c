// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

void
test_managed_ptr1(void)
{
    struct flex_item *item = flex_item_new();

    TEST(flex_item_get_managed_ptr(item) == NULL);

    flex_item_set_managed_ptr(item, (void *)0x42);

    TEST(flex_item_get_managed_ptr(item) == (void *)0x42);

    flex_item_set_managed_ptr(item, NULL);

    TEST(flex_item_get_managed_ptr(item) == NULL);
}
