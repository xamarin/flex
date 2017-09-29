// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

using System;
using Xamarin.Flex;

public partial class Test
{
    void test_self_sizing()
    {
        using (Item root = new Item(500, 500)) {
            Item item = new Item(10, 20);
            item.SelfSizing = delegate(Item _, ref float width,
                    ref float height) {
                width += 100;
                height += 100;
            };
            root.Add(item);

            root.Layout();

            assert(item.FrameWidth == 110);
            assert(item.FrameHeight == 120);

            item.SelfSizing = null;

            root.Layout();

            assert(item.FrameWidth == 10);
            assert(item.FrameHeight == 20);
        }
    }

    void test_self_sizing2()
    {
        Item root = new Item(100, 100);

        new Action(() => {
            Item item = new Item(10, 10);
            item.SelfSizing = delegate(Item _, ref float width,
                    ref float height) {
                width = 12;
                height = 34;
            };
            root.Add(item); 
        })();

        run_gc();

        root.Layout();

        assert(root[0].FrameWidth == 12);
        assert(root[0].FrameHeight == 34);

        root.Dispose();
    }
}
