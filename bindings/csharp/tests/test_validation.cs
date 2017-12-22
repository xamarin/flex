// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

using System;
using Xamarin.Flex;

public partial class Test
{
    void test_validation_aref()
    {
        Item item = new Item();

        foreach (uint val in new uint[] { 0, 1, 100 }) {
            assert_raised(() => { item.ItemAt(val); },
                    typeof(ArgumentOutOfRangeException));

            assert_raised(() => { item.RemoveAt(val); },
                    typeof(ArgumentOutOfRangeException));
        }

        Item item2 = new Item();

        foreach (uint val in new uint[] { 1, 100 }) {
            assert_raised(() => { item.InsertAt(val, item2); },
                    typeof(ArgumentOutOfRangeException));
        }

        item.Add(item2);

        foreach (uint val in new uint[] { 1, 2, 100 }) {
            assert_raised(() => { item.ItemAt(val); },
                    typeof(ArgumentOutOfRangeException));

            assert_raised(() => { item.RemoveAt(val); },
                    typeof(ArgumentOutOfRangeException));
        }

        Item item3 = new Item();

        foreach (uint val in new uint[] { 2, 100 }) {
            assert_raised(() => { item.InsertAt(val, item3); },
                    typeof(ArgumentOutOfRangeException));
        }

        item.RemoveAt(0);

        foreach (uint val in new uint[] { 0, 1, 100 }) {
            assert_raised(() => { item.ItemAt(val); },
                    typeof(ArgumentOutOfRangeException));

            assert_raised(() => { item.RemoveAt(val); },
                    typeof(ArgumentOutOfRangeException));
        }

        foreach (uint val in new uint[] { 1, 100 }) {
            assert_raised(() => { item.InsertAt(val, item2); },
                    typeof(ArgumentOutOfRangeException));
        }

        item3.Dispose();
        item2.Dispose();
        item.Dispose();
    }

    void test_validation_add()
    {
        Item item = new Item();

        assert_raised(() => { item.Add(item); }, typeof(ArgumentException));
        assert_raised(() => { item.InsertAt(0, item); },
                typeof(ArgumentException));

        Item item2 = new Item();
        item.Add(item2);

        Item item3 = new Item();

        assert_raised(() => { item3.Add(item2); }, typeof(ArgumentException));
        assert_raised(() => { item3.InsertAt(0, item2); },
                typeof(ArgumentException));

        item.RemoveAt(0);

        assert_no_raised(() => { item3.Add(item2); });

        item3.RemoveAt(0);

        assert_no_raised(() => { item3.InsertAt(0, item2); });

        item3.Dispose();
        item.Dispose();
    }

    void test_validation_layout()
    {
        Item item = new Item();

        assert_raised(() => { item.Layout(); },
                typeof(InvalidOperationException));

        item.Width = 100;

        assert_raised(() => { item.Layout(); },
                typeof(InvalidOperationException));

        item.Height = 100;

        assert_no_raised(() => { item.Layout(); });

        Item item2 = new Item(100, 100);
        item.Add(item2);

        assert_raised(() => { item2.Layout(); },
                typeof(InvalidOperationException));

        item.RemoveAt(0);

        assert_no_raised(() => { item2.Layout(); });

        item.SelfSizing = delegate(Item _, ref float width, ref float height) {
            // Do nothing.
        };

        assert_raised(() => { item.Layout(); },
                typeof(InvalidOperationException));

        item.Dispose();
    }

    void test_validation_AlignContent()
    {
        Item item = new Item();

        assert_no_raised(() => { item.AlignContent = Align.Stretch; });
        assert_no_raised(() => { item.AlignContent = Align.Center; });
        assert_no_raised(() => { item.AlignContent = Align.Start; });
        assert_no_raised(() => { item.AlignContent = Align.End; });
        assert_no_raised(() => { item.AlignContent = Align.SpaceBetween; });
        assert_no_raised(() => { item.AlignContent = Align.SpaceAround; });
        assert_no_raised(() => { item.AlignContent = Align.SpaceEvenly; });
       
        assert_raised(() => { item.AlignContent = Align.Auto; },
                typeof(ArgumentException));

        item.Dispose();
    }

    void test_validation_AlignItems()
    {
        Item item = new Item();

        assert_no_raised(() => { item.AlignItems = Align.Center; });
        assert_no_raised(() => { item.AlignItems = Align.Start; });
        assert_no_raised(() => { item.AlignItems = Align.End; });
        assert_no_raised(() => { item.AlignItems = Align.SpaceBetween; });
        assert_no_raised(() => { item.AlignItems = Align.SpaceAround; });
        assert_no_raised(() => { item.AlignItems = Align.SpaceEvenly; });
        assert_no_raised(() => { item.AlignItems = Align.Stretch; });
       
        assert_raised(() => { item.AlignItems = Align.Auto; },
                typeof(ArgumentException));

        item.Dispose();
    }

    void test_validation_AlignSelf()
    {
        Item item = new Item();

        assert_no_raised(() => { item.AlignSelf = Align.Center; });
        assert_no_raised(() => { item.AlignSelf = Align.Start; });
        assert_no_raised(() => { item.AlignSelf = Align.End; });
        assert_no_raised(() => { item.AlignSelf = Align.SpaceBetween; });
        assert_no_raised(() => { item.AlignSelf = Align.SpaceAround; });
        assert_no_raised(() => { item.AlignSelf = Align.SpaceEvenly; });
        assert_no_raised(() => { item.AlignSelf = Align.Stretch; });
        assert_no_raised(() => { item.AlignSelf = Align.Auto; });

        item.Dispose();
    }

    void test_validation_JustifyContent()
    {
        Item item = new Item();

        assert_no_raised(() => { item.JustifyContent = Align.Center; });
        assert_no_raised(() => { item.JustifyContent = Align.Start; });
        assert_no_raised(() => { item.JustifyContent = Align.End; });
        assert_no_raised(() => { item.JustifyContent = Align.SpaceBetween; });
        assert_no_raised(() => { item.JustifyContent = Align.SpaceAround; });
        assert_no_raised(() => { item.JustifyContent = Align.SpaceEvenly; });
       
        assert_raised(() => { item.JustifyContent = Align.Auto; },
                typeof(ArgumentException));
        assert_raised(() => { item.JustifyContent = Align.Stretch; },
                typeof(ArgumentException));

        item.Dispose();
    }

    void test_validation_padding()
    {
        Item item = new Item();
        
        assert_no_raised(() => { item.Padding = 0; });
        assert_no_raised(() => { item.Padding = 1; });
        assert_no_raised(() => { item.PaddingLeft = 0; });
        assert_no_raised(() => { item.PaddingLeft = 1; });
        assert_no_raised(() => { item.PaddingRight = 0; });
        assert_no_raised(() => { item.PaddingRight = 1; });
        assert_no_raised(() => { item.PaddingTop = 0; });
        assert_no_raised(() => { item.PaddingTop = 1; });
        assert_no_raised(() => { item.PaddingBottom = 0; });
        assert_no_raised(() => { item.PaddingBottom = 1; });

        assert_raised(() => { item.Padding = -1; }, typeof(ArgumentException));
        assert_raised(() => { item.PaddingLeft = -1; },
                typeof(ArgumentException));
        assert_raised(() => { item.PaddingRight = -1; },
                typeof(ArgumentException));
        assert_raised(() => { item.PaddingTop = -1; },
                typeof(ArgumentException));
        assert_raised(() => { item.PaddingBottom = -1; },
                typeof(ArgumentException));

        item.Dispose();
    }

    void test_validation_grow()
    {
        Item item = new Item();

        assert_no_raised(() => { item.Grow = 0; });
        assert_no_raised(() => { item.Grow = 1; });
        assert_raised(() => { item.Grow = -1; }, typeof(ArgumentException));

        assert_raised(() => { item.Flex(grow:-1); },
                typeof(ArgumentException));

        item.Dispose();
    }

    void test_validation_shrink()
    {
        Item item = new Item();

        assert_no_raised(() => { item.Shrink = 0; });
        assert_no_raised(() => { item.Shrink = 1; });
        assert_raised(() => { item.Shrink = -1; }, typeof(ArgumentException));

        assert_raised(() => { item.Flex(shrink:-1); },
                typeof(ArgumentException));

        item.Dispose();
    }

    void test_validation_basis()
    {
        Item item = new Item();

        assert_no_raised(() => { item.Basis = 0; });
        assert_no_raised(() => { item.Basis = 1; });
        assert_raised(() => { item.Basis = -1; }, typeof(ArgumentException));

        assert_raised(() => { item.Flex(basis:-1); },
                typeof(ArgumentException));

        item.Dispose();
    }
}
