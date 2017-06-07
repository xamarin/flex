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
        CreateHandleForItem(child);
        flex_item_add(item, child.item);
    }

    public void InsertAt(int index, FlexItem child)
    {
        CreateHandleForItem(child);
        flex_item_insert(item, index, child.item);
    }

    public FlexItem RemoveAt(int index)
    {
        IntPtr child_item = flex_item_delete(item, index);
        return ReleaseHandleForItem(child_item, false);
    }

    public int Count()
    {
        return flex_item_count(item);
    }

    public FlexItem ItemAt(int index)
    {
        return ItemFromItem(flex_item_child(item, index));
    }

    public FlexItem Parent()
    {
        return ItemFromItem(flex_item_parent(item));
    }

    public FlexItem Root()
    {
        FlexItem root = ItemFromItem(flex_item_root(item));
        if (root != null) {
            return root;
        }
        return this;
    }

    public void Layout()
    {
        flex_layout(item);
    }

    private static void CreateHandleForItem(FlexItem item)
    {
        if (flex_item_get_managed_ptr(item.item) == IntPtr.Zero) {
            GCHandle handle = GCHandle.Alloc(item, GCHandleType.Pinned);
            flex_item_set_managed_ptr(item.item, GCHandle.ToIntPtr(handle));
        }
    }

    private static Nullable<GCHandle> HandleFromItem(IntPtr item)
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

    private static FlexItem ItemFromItem(IntPtr item)
    {
        if (item == IntPtr.Zero) {
            return null;
        }
        var ret = HandleFromItem(item);
        if (ret.HasValue) {
            return (FlexItem)ret.Value.Target;
        }
        return null;
    }

    private static FlexItem ReleaseHandleForItem(IntPtr item, bool reset)
    {
        var ret = HandleFromItem(item);
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
}
