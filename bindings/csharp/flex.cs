// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

using System;
using System.Runtime.InteropServices;
using static FlexNativeFunctions;

public class FlexItem : FlexBase
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
        child.CreateHandle(true);
        flex_item_add(item, child.item);
    }

    public void InsertAt(int index, FlexItem child)
    {
        child.CreateHandle(true);
        flex_item_insert(item, index, child.item);
    }

    public FlexItem RemoveAt(int index)
    {
        IntPtr child_item = flex_item_delete(item, index);
        FlexItem child = ReleaseHandleForItem(child_item, false);
        child.CreateHandle(false);
        return child;
    }

    public int Count
    {
        get { return flex_item_count(item); }
    }

    public FlexItem ItemAt(int index)
    {
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
        flex_layout(item);
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
}
