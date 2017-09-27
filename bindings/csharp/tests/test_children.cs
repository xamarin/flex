// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

using System;
using Xamarin.Flex;

public partial class Test
{
    void test_children1()
    {
        Item item1 = new Item();

        assert(item1.Root == item1);
        assert(item1.Parent == null);
        assert(item1.Count == 0);

        Item item2 = new Item();
        item1.Add(item2);

        assert(item1.Count == 1);
        assert(item1.ItemAt(0) == item2);
        assert(item2.Root == item1);
        assert(item2.Parent == item1);

        Item item3 = new Item();
        item1.Add(item3);

        assert(item1.Count == 2);
        assert(item1.ItemAt(0) == item2);
        assert(item1.ItemAt(1) == item3);
        assert(item3.Root == item1);
        assert(item3.Parent == item1);

        assert(item1.RemoveAt(0) == item2);

        assert(item1.Count == 1);
        assert(item1.ItemAt(0) == item3);
        assert(item2.Root == item2);
        assert(item2.Parent == null);

        item3.Add(item2);

        assert(item3.Count == 1);
        assert(item3.ItemAt(0) == item2);
        assert(item2.Root == item1);
        assert(item2.Parent == item3);

        item1.Dispose();
    }

    void test_children2()
    {
        Item item1 = new Item();

        Item item2 = new Item();
        item1.InsertAt(0, item2);

        assert(item1.Count == 1);
        assert(item1.ItemAt(0) == item2);

        Item item3 = new Item();
        item1.InsertAt(0, item3);

        assert(item1.Count == 2);
        assert(item1.ItemAt(0) == item3);
        assert(item1.ItemAt(1) == item2);

        Item item4 = new Item();
        item1.InsertAt(1, item4);

        assert(item1.Count == 3);
        assert(item1.ItemAt(0) == item3);
        assert(item1.ItemAt(1) == item4);
        assert(item1.ItemAt(2) == item2);

        assert(item1.RemoveAt(1) == item4);

        assert(item1.Count == 2);
        assert(item1.ItemAt(0) == item3);
        assert(item1.ItemAt(1) == item2);

        assert(item1.RemoveAt(1) == item2);

        assert(item1.Count == 1);
        assert(item1.ItemAt(0) == item3);

        assert(item1.RemoveAt(0) == item3);

        assert(item1.Count == 0);

        item4.Dispose();
        item3.Dispose();
        item2.Dispose();
        item1.Dispose();
    }
}
