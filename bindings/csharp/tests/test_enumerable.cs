// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

using System;
using System.Collections.Generic;
using Xamarin.Flex;

public partial class Test
{
    void test_enumerable1()
    {
        List<Item> items = new List<Item>();

        Item item = new Item();

        items.Clear();
        foreach (var child in item) {
            items.Add(child);
        }

        assert(items.Count == 0);

        Item item2 = new Item();
        Item item3 = new Item();
        Item item4 = new Item();

        item.Add(item2);
        item.Add(item3);
        item.Add(item4);

        items.Clear();
        foreach (var child in item) {
            items.Add(child);
        }

        assert(items.Count == 3);
        assert(items[0] == item2);
        assert(items[1] == item3);
        assert(items[2] == item4);

        item.RemoveAt(1);

        items.Clear();
        foreach (var child in item) {
            items.Add(child);
        }

        assert(items.Count == 2);
        assert(items[0] == item2);
        assert(items[1] == item4);

        item3.Dispose();
        item.Dispose();
    }

    void test_enumerable2()
    {
        Item item = new Item();
        item.Add(new Item());

        assert_raised(() => { 
                foreach (var child in item) {
                    item.RemoveAt(0);
                }}, typeof(InvalidOperationException));
    }

    void test_enumerable3()
    {
        Item item = new Item();
        item.Add(new Item());

        assert_raised(() => { 
                foreach (var child in item) {
                    item.Add(new Item());
                }}, typeof(InvalidOperationException));

        item.Dispose();
    }

    void test_enumerable4()
    {
        Item item = new Item();
        item.Add(new Item());

        assert_raised(() => { 
                foreach (var child in item) {
                    item.InsertAt(0, new Item());
                }}, typeof(InvalidOperationException));

        item.Dispose();
    }
}
