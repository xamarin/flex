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
using Xamarin.Flex;

public partial class Test
{
    private string current_unit;
    private Item handle1;

    class Error
    {
        public string unit;
        public string callstack;
    }
    List<Error> errors;

    public Test()
    {
        handle1 = null;
        current_unit = null;
        errors = new List<Error>();

        if (handle1 != null) {} // needed to remove a compiler warning about the instance variable assigned but never used
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
