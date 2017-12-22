// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

using System;
using Xamarin.Flex;

public partial class Test
{
    void test_aref()
    {
        using (Item item = new Item()) {
            Item item2 = new Item();
            Item item3 = new Item();
            Item item4 = new Item();

            item.Add(item2);
            item.Add(item3);
            item.Add(item4);

            assert(item[0] == item2);
            assert(item[1] == item3);
            assert(item[2] == item4);

            assert(item.ItemAt(0) == item[0]);
            assert(item.ItemAt(1) == item[1]);
            assert(item.ItemAt(2) == item[2]);

            assert_raised(() => { var x = item[3]; },
                    typeof(ArgumentOutOfRangeException));
        }
    }
}
