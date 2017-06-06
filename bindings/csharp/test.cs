using System;
using System.Diagnostics;

public class Test
{
    static FlexItem handle1 = null;

    static private void test1()
    {
        System.Console.WriteLine("test1");

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

        gc();

        Debug.Assert(!ref1.IsAlive, "ref1 is alive");
        Debug.Assert(!ref2.IsAlive, "ref2 is alive");
        Debug.Assert(!ref3.IsAlive, "ref3 is alive");
    }

    static private void test2()
    {
        System.Console.WriteLine("test2");

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

        gc();

        Debug.Assert(ref1.IsAlive, "ref1 is not alive");
        Debug.Assert(ref2.IsAlive, "ref2 is not alive");
        Debug.Assert(ref3.IsAlive, "ref3 is not alive");

        handle1 = null;
    }

    static private void test3()
    {
        System.Console.WriteLine("test3");

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

        gc();

        Debug.Assert(!ref1.IsAlive, "ref1 is alive");
        Debug.Assert(!ref2.IsAlive, "ref2 is alive");
        Debug.Assert(!ref3.IsAlive, "ref3 is alive");
    }

    static void test4()
    {
        System.Console.WriteLine("test4");

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

        gc();

        Debug.Assert(ref1.IsAlive, "ref1 is not alive");
        Debug.Assert(!ref2.IsAlive, "ref2 is alive");
        Debug.Assert(!ref3.IsAlive, "ref3 is alive");

        handle1 = null;
    }

    static private void test5()
    {
        System.Console.WriteLine("test5");

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

        gc();

        Debug.Assert(ref1.IsAlive, "ref1 is not alive");
        Debug.Assert(ref2.IsAlive, "ref2 is not alive");
        Debug.Assert(ref3.IsAlive, "ref3 is not alive");

        handle1 = null;
    }

    static private void gc()
    {
        for (int i = 0; i < 10; i++) {
            GC.Collect();
            GC.WaitForPendingFinalizers();
        }
    }

    static public void Main() 
    {
        test1();
        test2();
        test3();
        test4();
        test5();
    }
}
