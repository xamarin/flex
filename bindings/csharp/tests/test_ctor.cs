// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

using System;
using Xamarin.Flex;

public partial class Test
{
    void _test_default_values_except_size(Item item)
    {
        assert(Double.IsNaN(item.Left));
        assert(Double.IsNaN(item.Right));
        assert(Double.IsNaN(item.Top));
        assert(Double.IsNaN(item.Bottom));

        assert(item.PaddingLeft == 0);
        assert(item.PaddingRight == 0);
        assert(item.PaddingTop == 0);
        assert(item.PaddingBottom == 0);

        assert(item.MarginLeft == 0);
        assert(item.MarginRight == 0);
        assert(item.MarginTop == 0);
        assert(item.MarginBottom == 0);

        assert(item.JustifyContent == Align.Start);
        assert(item.AlignContent == Align.Stretch);
        assert(item.AlignItems == Align.Stretch);
        assert(item.AlignSelf == Align.Auto);

        assert(item.Position == Position.Relative);
        assert(item.Direction == Direction.Column);
        assert(item.Wrap == Wrap.NoWrap);

        assert(item.Grow == 0);
        assert(item.Shrink == 1);
        assert(item.Order == 0);
        assert(Double.IsNaN(item.Basis));
    }

    void test_ctor1()
    {
        Item item = new Item();

        assert(Double.IsNaN(item.Width));
        assert(Double.IsNaN(item.Height));

        _test_default_values_except_size(item);

        item.Dispose();
    }

    void test_ctor2()
    {
        Item item = new Item(100, 200);

        assert(item.Width == 100);
        assert(item.Height == 200);

        _test_default_values_except_size(item);

        item.Dispose();
    }
}
