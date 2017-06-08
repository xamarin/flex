// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

// This file contains a series of tests for the C# bindings. They only cover
// functionality that was written as part of the bindings themselves. They do
// not cover the layout engine as it's tested elsewhere (in C).

// To add a test simply add a function with the `test_' prefix to it and it will
// be picked up at runtime.

using System;
using System.Reflection;
using System.Collections.Generic;

public class Test
{
    private string current_unit;
    private FlexItem handle1;

    class Error
    {
        public string unit;
        public string callstack;
    }
    List<Error> errors;

    public Test()
    {
        current_unit = null;
        errors = new List<Error>();
    }

    void _test_default_values_except_size(FlexItem item)
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

        assert(item.JustifyContent == FlexAlign.Start);
        assert(item.AlignContent == FlexAlign.Start);
        assert(item.AlignItems == FlexAlign.Start);
        assert(item.AlignSelf == FlexAlign.Auto);

        assert(item.Position == FlexPosition.Relative);
        assert(item.Direction == FlexDirection.Column);
        assert(item.Wrap == FlexWrap.Nowrap);

        assert(item.Grow == 0);
        assert(item.Shrink == 1);
        assert(item.Order == 0);
        assert(item.Basis == 0);
    }

    void test_ctor1()
    {
        FlexItem item = new FlexItem();

        assert(Double.IsNaN(item.Width));
        assert(Double.IsNaN(item.Height));

        _test_default_values_except_size(item);
    }

    void test_ctor2()
    {
        FlexItem item = new FlexItem(100, 200);

        assert(item.Width == 100);
        assert(item.Height == 200);

        _test_default_values_except_size(item);
    }

    void test_dtor1()
    {
        WeakReference ref1 = null;
        WeakReference ref2 = null;
        WeakReference ref3 = null;

        new Action(() => {
            FlexItem item1 = new FlexItem();
            FlexItem item2 = new FlexItem();
            FlexItem item3 = new FlexItem();

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
            FlexItem item1 = new FlexItem();
            FlexItem item2 = new FlexItem();
            FlexItem item3 = new FlexItem();
    
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
            FlexItem item1 = new FlexItem();
            FlexItem item2 = new FlexItem();
            FlexItem item3 = new FlexItem();
    
            item1.Add(item2);
            item2.Add(item3);
    
            ref1 = new WeakReference(item1);
            ref2 = new WeakReference(item2);
            ref3 = new WeakReference(item3);
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
            FlexItem item1 = new FlexItem();
            FlexItem item2 = new FlexItem();
            FlexItem item3 = new FlexItem();
    
            item1.Add(item2);
            item2.Add(item3);
            handle1 = item1;
   
            item1.RemoveAt(0);
 
            ref1 = new WeakReference(item1);
            ref2 = new WeakReference(item2);
            ref3 = new WeakReference(item3);
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
            FlexItem item1 = new FlexItem();
            FlexItem item2 = new FlexItem();
            FlexItem item3 = new FlexItem();

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

    void test_children1()
    {
        FlexItem item1 = new FlexItem();

        assert(item1.Root == item1);
        assert(item1.Parent == null);
        assert(item1.Count == 0);

        FlexItem item2 = new FlexItem();
        item1.Add(item2);

        assert(item1.Count == 1);
        assert(item1.ItemAt(0) == item2);
        assert(item2.Root == item1);
        assert(item2.Parent == item1);

        FlexItem item3 = new FlexItem();
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
    }

    void test_children2()
    {
        FlexItem item1 = new FlexItem();

        FlexItem item2 = new FlexItem();
        item1.InsertAt(0, item2);

        assert(item1.Count == 1);
        assert(item1.ItemAt(0) == item2);

        FlexItem item3 = new FlexItem();
        item1.InsertAt(0, item3);

        assert(item1.Count == 2);
        assert(item1.ItemAt(0) == item3);
        assert(item1.ItemAt(1) == item2);

        FlexItem item4 = new FlexItem();
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
    }

    void test_children_aref_validation()
    {
        FlexItem item = new FlexItem();

        foreach (int val in new int[] { -100, -1, 0, 1, 100 }) {
            assert_raised(() => { item.ItemAt(val); },
                    typeof(ArgumentOutOfRangeException));

            assert_raised(() => { item.RemoveAt(val); },
                    typeof(ArgumentOutOfRangeException));
        }

        FlexItem item2 = new FlexItem();

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

        FlexItem item3 = new FlexItem();

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

    void test_children_add_validation()
    {
        FlexItem item = new FlexItem();

        assert_raised(() => { item.Add(item); }, typeof(ArgumentException));
        assert_raised(() => { item.InsertAt(0, item); },
                typeof(ArgumentException));

        FlexItem item2 = new FlexItem();
        item.Add(item2);

        FlexItem item3 = new FlexItem();

        assert_raised(() => { item3.Add(item2); }, typeof(ArgumentException));
        assert_raised(() => { item3.InsertAt(0, item2); },
                typeof(ArgumentException));

        item.RemoveAt(0);

        assert_no_raised(() => { item3.Add(item2); });

        item3.RemoveAt(0);

        assert_no_raised(() => { item3.InsertAt(0, item2); });
    }

    void test_layout_validation()
    {
        FlexItem item = new FlexItem();

        assert_raised(() => { item.Layout(); },
                typeof(InvalidOperationException));

        item.Width = 100;

        assert_raised(() => { item.Layout(); },
                typeof(InvalidOperationException));

        item.Height = 100;

        assert_no_raised(() => { item.Layout(); });

        FlexItem item2 = new FlexItem(100, 100);
        item.Add(item2);

        assert_raised(() => { item2.Layout(); },
                typeof(InvalidOperationException));

        item.RemoveAt(0);

        assert_no_raised(() => { item2.Layout(); });
    }

    void run_gc()
    {
        for (int i = 0; i < 10; i++) {
            GC.Collect();
            GC.WaitForPendingFinalizers();
        }
    }

    void assert(bool condition)
    {
        if (condition) {
            Console.Write('.');
        }
        else {
            Console.Write('E');

            var err = new Error();
            err.unit = current_unit;
            err.callstack = Environment.StackTrace;
            errors.Add(err);
        }
    }

    void assert_raised(Action code, Type klass)
    {
        bool state = false;
        try {
            code();
        }
        catch (Exception e) {
            if (e.GetType() == klass) {
                state = true;
            }
        }
        assert(state);
    }

    void assert_no_raised(Action code)
    {
        bool state = true;
        try {
            code();
        }
        catch (Exception) {
            state = false;
        }
        assert(state);
    }

    void run()
    {
        var methods = typeof(Test).GetMethods(BindingFlags.NonPublic
                | BindingFlags.Instance);
        int n = 0;
        foreach (var method in methods) {
            string name = method.Name;
            if (name.StartsWith("test_")) {
                current_unit = name.Substring(5);
                method.Invoke(this, null);
                n++;
            }
        }

        if (n == 0) {
            Console.WriteLine("no tests?");
        }
        else if (errors.Count == 0) {
            Console.WriteLine(" OK");
        }
        else {
            foreach (var error in errors) {
                Console.WriteLine("\nfailed test `{0}':\n#{1}", error.unit,
                        error.callstack);
            }
            Console.WriteLine("\n{0} tests failed.", errors.Count);
        }
    }

    static public void Main() 
    {
        Test test = new Test();
        test.run();
    }
}
