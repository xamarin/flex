// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

using System;
using Xamarin.Flex;

public partial class Test
{
    void test_set_padding_all()
    {
        Item item = new Item();

        item.PaddingTop = 1;
        item.PaddingBottom = 2;
        item.PaddingLeft = 3;
        item.PaddingRight = 4;

        assert(item.PaddingTop == 1);
        assert(item.PaddingBottom == 2);
        assert(item.PaddingLeft == 3);
        assert(item.PaddingRight == 4);

        item.Padding = 42;

        assert(item.PaddingTop == 42);
        assert(item.PaddingBottom == 42);
        assert(item.PaddingLeft == 42);
        assert(item.PaddingRight == 42);

        item.Padding = 0;

        assert(item.PaddingTop == 0);
        assert(item.PaddingBottom == 0);
        assert(item.PaddingLeft == 0);
        assert(item.PaddingRight == 0);

        item.Dispose();
    }
}
