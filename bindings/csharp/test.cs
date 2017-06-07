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
        public string message;
        public string callstack;
    }
    List<Error> errors;

    public Test()
    {
        current_unit = null;
        errors = new List<Error>();
    }

    void test_gc1()
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

        assert(!ref1.IsAlive, "ref1 is alive");
        assert(!ref2.IsAlive, "ref2 is alive");
        assert(!ref3.IsAlive, "ref3 is alive");
    }

    void test_gc2()
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

        assert(ref1.IsAlive, "ref1 is not alive");
        assert(ref2.IsAlive, "ref2 is not alive");
        assert(ref3.IsAlive, "ref3 is not alive");

        handle1 = null;
    }

    void test_gc3()
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

        assert(!ref1.IsAlive, "ref1 is alive");
        assert(!ref2.IsAlive, "ref2 is alive");
        assert(!ref3.IsAlive, "ref3 is alive");
    }

    void test_gc4()
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

        assert(ref1.IsAlive, "ref1 is not alive");
        assert(!ref2.IsAlive, "ref2 is alive");
        assert(!ref3.IsAlive, "ref3 is alive");

        handle1 = null;
    }

    void test_gc5()
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

        assert(ref1.IsAlive, "ref1 is not alive");
        assert(ref2.IsAlive, "ref2 is not alive");
        assert(ref3.IsAlive, "ref3 is not alive");

        handle1 = null;
    }

    void run_gc()
    {
        for (int i = 0; i < 10; i++) {
            GC.Collect();
            GC.WaitForPendingFinalizers();
        }
    }

    void assert(bool condition, string error)
    {
        if (condition) {
            Console.Write('.');
        }
        else {
            Console.Write('E');

            var err = new Error();
            err.unit = current_unit;
            err.message = error;
            err.callstack = Environment.StackTrace;
            errors.Add(err);
        }
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
                Console.WriteLine("\nfailed test `{0}': {1}\n#{2}", error.unit,
                        error.message, error.callstack);
            }
        }
    }

    static public void Main() 
    {
        Test test = new Test();
        test.run();
    }
}
