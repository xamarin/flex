// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

using System;
using Xamarin.Flex;

public partial class Test
{
    void test_set_margin_all()
    {
        Item item = new Item();

        item.MarginTop = 1;
        item.MarginBottom = 2;
        item.MarginLeft = 3;
        item.MarginRight = 4;

        assert(item.MarginTop == 1);
        assert(item.MarginBottom == 2);
        assert(item.MarginLeft == 3);
        assert(item.MarginRight == 4);

        item.Margin = 42;

        assert(item.MarginTop == 42);
        assert(item.MarginBottom == 42);
        assert(item.MarginLeft == 42);
        assert(item.MarginRight == 42);

        item.Margin = 0;

        assert(item.MarginTop == 0);
        assert(item.MarginBottom == 0);
        assert(item.MarginLeft == 0);
        assert(item.MarginRight == 0);

        item.Dispose();
    }
}
