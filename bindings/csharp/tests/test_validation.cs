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

        foreach (int val in new int[] { -100, -1, 0, 1, 100 }) {
            assert_raised(() => { item.ItemAt(val); },
                    typeof(ArgumentOutOfRangeException));

            assert_raised(() => { item.RemoveAt(val); },
                    typeof(ArgumentOutOfRangeException));
        }

        Item item2 = new Item();

        foreach (int val in new int[] { -100, -1, 1, 100 }) {
            assert_raised(() => { item.InsertAt(val, item2); },
                    typeof(ArgumentOutOfRangeException));
        }

        item.Add(item2);

        foreach (int val in new int[] { -100, -1, 1, 2, 100 }) {
            assert_raised(() => { item.ItemAt(val); },
                    typeof(ArgumentOutOfRangeException));

            assert_raised(() => { item.RemoveAt(val); },
                    typeof(ArgumentOutOfRangeException));
        }

        Item item3 = new Item();

        foreach (int val in new int[] { -100, -1, 2, 100 }) {
            assert_raised(() => { item.InsertAt(val, item3); },
                    typeof(ArgumentOutOfRangeException));
        }

        item.RemoveAt(0);

        foreach (int val in new int[] { -100, -1, 0, 1, 100 }) {
            assert_raised(() => { item.ItemAt(val); },
                    typeof(ArgumentOutOfRangeException));

            assert_raised(() => { item.RemoveAt(val); },
                    typeof(ArgumentOutOfRangeException));
        }

        foreach (int val in new int[] { -100, -1, 1, 100 }) {
            assert_raised(() => { item.InsertAt(val, item2); },
                    typeof(ArgumentOutOfRangeException));
        }
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
    }
}
