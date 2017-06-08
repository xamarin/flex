// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

using System;
using System.Collections;
using System.Runtime.InteropServices;
using static FlexNativeFunctions;

public class FlexItem : FlexBase, IEnumerable
{
    public FlexItem()
    {
        item = flex_item_new();
        CreateHandle(false);
    }

    public FlexItem(float width, float height) : this()
    {
        this.Width = width;
        this.Height = height;
    }

    ~FlexItem()
    {
        if (item != IntPtr.Zero) {
            ReleaseHandlesWithinItem(item, false);
            flex_item_free(item);
            item = IntPtr.Zero;
        }
    }

    public void Add(FlexItem child)
    {
        ValidateNewChild(child);
        child.CreateHandle(true);
        flex_item_add(item, child.item);
        notify_changed = true;
    }

    public void InsertAt(int index, FlexItem child)
    {
        ValidateNewChild(child);
        ValidateIndex(index, true);
        child.CreateHandle(true);
        flex_item_insert(item, index, child.item);
        notify_changed = true;
    }

    public FlexItem RemoveAt(int index)
    {
        ValidateIndex(index, false);
        IntPtr child_item = flex_item_delete(item, index);
        FlexItem child = ReleaseHandleForItem(child_item, false);
        child.CreateHandle(false);
        notify_changed = true;
        return child;
    }

    public int Count
    {
        get { return flex_item_count(item); }
    }

    public FlexItem ItemAt(int index)
    {
        ValidateIndex(index, false);
        return FlexItemFromItem(flex_item_child(item, index));
    }

    public FlexItem Parent
    {
        get { return FlexItemFromItem(flex_item_parent(item)); }
    }

    public FlexItem Root
    {
        get { return FlexItemFromItem(flex_item_root(item)); }
    }

    public void Layout()
    {
        if (Parent != null) {
            throw new InvalidOperationException("Layout() must be called on a root item (that hasn't been added to another item)");
        }
        if (Double.IsNaN(Width) || Double.IsNaN(Height)) {
            throw new InvalidOperationException("Layout() must be called on an item that has proper values for the Width and Height properties");
        }
        flex_layout(item);
    }

    public class FlexItemEnumerator : IEnumerator
    {
        private FlexItem item;
        private int index;

        public FlexItemEnumerator(FlexItem _item)
        {
            item = _item;
            index = -1;
        }

        object IEnumerator.Current
        {
            get { return Current; }
        }

        public FlexItem Current
        {
            get { return item.ItemAt(index); }
        }

        public void Reset()
        {
            index = -1;
        }

        public bool MoveNext()
        {
            if (item.notify_changed) {
                throw new InvalidOperationException("the item's children list was modified; enumeration cannot proceed");
            }
            index++;
            return index < item.Count;
        } 
    }

    IEnumerator IEnumerable.GetEnumerator()
    {
        return GetEnumerator();
    }

    private bool notify_changed = false;
    public FlexItemEnumerator GetEnumerator()
    {
        notify_changed = false;
        return new FlexItemEnumerator(this);
    }

    private static Nullable<GCHandle> HandleOfItem(IntPtr item)
    {
        if (item == IntPtr.Zero) {
            return null;
        }
        IntPtr ptr = flex_item_get_managed_ptr(item);
        if (ptr == IntPtr.Zero) {
            return null;
        }
        return GCHandle.FromIntPtr(ptr);
    }

    private static FlexItem FlexItemFromItem(IntPtr item)
    {
        if (item == IntPtr.Zero) {
            return null;
        }
        var ret = HandleOfItem(item);
        if (ret.HasValue) {
            return (FlexItem)ret.Value.Target;
        }
        return null;
    }

    private static FlexItem ReleaseHandleForItem(IntPtr item, bool reset)
    {
        var ret = HandleOfItem(item);
        if (ret.HasValue) {
            GCHandle handle = ret.Value;
            FlexItem child = (FlexItem)handle.Target;
            if (reset) {
                child.item = IntPtr.Zero;
            }
            handle.Free();
            flex_item_set_managed_ptr(item, IntPtr.Zero);
            return child;
        }
        return null;
    }

    private static void ReleaseHandlesWithinItem(IntPtr item, bool reset)
    {
        for (int i = 0, count = flex_item_count(item); i < count; i++) {
            ReleaseHandlesWithinItem(flex_item_child(item, i), true);
        }
        ReleaseHandleForItem(item, reset);
    }

    private void CreateHandle(bool strong)
    {
        ReleaseHandleForItem(item, false);
        GCHandle handle = GCHandle.Alloc(this,
                strong ? GCHandleType.Pinned : GCHandleType.Weak);
        flex_item_set_managed_ptr(item, GCHandle.ToIntPtr(handle));
    }

    private void ValidateNewChild(FlexItem child)
    {
        if (this == child) {
            throw new ArgumentException("cannot add item into self");
        }
        if (child.Parent != null) {
            throw new ArgumentException("child already has a parent");
        } 
    }

    private void ValidateIndex(int index, bool inc)
    {
        int max = Count;
        if (inc) {
            max++;
        }
        if (index < 0 || index >= max) {
            throw new ArgumentOutOfRangeException();
        }
    }
}
