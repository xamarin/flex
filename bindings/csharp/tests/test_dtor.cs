// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

using System;
using Xamarin.Flex;

public partial class Test
{
    void test_dtor1()
    {
        WeakReference ref1 = null;
        WeakReference ref2 = null;
        WeakReference ref3 = null;

        new Action(() => {
            Item item1 = new Item();
            Item item2 = new Item();
            Item item3 = new Item();

            ref1 = new WeakReference(item1);
            ref2 = new WeakReference(item2);
            ref3 = new WeakReference(item3);
        })();

        run_gc();

        assert(!ref1.IsAlive);
        assert(!ref2.IsAlive);
        assert(!ref3.IsAlive);
    }

    void test_dtor2()
    {
        WeakReference ref1 = null;
        WeakReference ref2 = null;
        WeakReference ref3 = null;

        handle1 = null;

        new Action(() => {
            Item item1 = new Item();
            Item item2 = new Item();
            Item item3 = new Item();
    
            item1.Add(item2);
            item2.Add(item3);
            handle1 = item1;
    
            ref1 = new WeakReference(item1);
            ref2 = new WeakReference(item2);
            ref3 = new WeakReference(item3);
        })();

        run_gc();

        assert(ref1.IsAlive);
        assert(ref2.IsAlive);
        assert(ref3.IsAlive);

        handle1 = null;
    }

    void test_dtor3()
    {
        WeakReference ref1 = null;
        WeakReference ref2 = null;
        WeakReference ref3 = null;

        new Action(() => {
            Item item1 = new Item();
            Item item2 = new Item();
            Item item3 = new Item();
    
            item1.Add(item2);
            item2.Add(item3);
    
            ref1 = new WeakReference(item1);
            ref2 = new WeakReference(item2);
            ref3 = new WeakReference(item3);

            item1.Dispose();
        })();

        run_gc();

        assert(!ref1.IsAlive);
        assert(!ref2.IsAlive);
        assert(!ref3.IsAlive);
    }

    void test_dtor4()
    {
        WeakReference ref1 = null;
        WeakReference ref2 = null;
        WeakReference ref3 = null;

        handle1 = null;

        new Action(() => {
            Item item1 = new Item();
            Item item2 = new Item();
            Item item3 = new Item();
    
            item1.Add(item2);
            item2.Add(item3);
            handle1 = item1;
   
            item1.RemoveAt(0);
 
            ref1 = new WeakReference(item1);
            ref2 = new WeakReference(item2);
            ref3 = new WeakReference(item3);

            item2.Dispose();
        })();

        run_gc();

        assert(ref1.IsAlive);
        assert(!ref2.IsAlive);
        assert(!ref3.IsAlive);

        handle1 = null;
    }

    void test_dtor5()
    {
        WeakReference ref1 = null;
        WeakReference ref2 = null;
        WeakReference ref3 = null;

        handle1 = null;

        new Action(() => {
            Item item1 = new Item();
            Item item2 = new Item();
            Item item3 = new Item();

            handle1 = item1;
            item1.InsertAt(0, item2);
            item1.InsertAt(0, item3);

            ref1 = new WeakReference(item1);
            ref2 = new WeakReference(item2);
            ref3 = new WeakReference(item3);
        })();

        run_gc();

        assert(ref1.IsAlive);
        assert(ref2.IsAlive);
        assert(ref3.IsAlive);

        handle1 = null;
    }
}
