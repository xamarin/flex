// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

using System;
using System.Runtime.InteropServices;
using static FlexNativeFunctions;

public class FlexItem
{
    protected IntPtr item = IntPtr.Zero;

    public FlexItem()
    {
        item = flex_item_new();
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

    public float Width
    {
        get { return flex_item_get_width(item); }
        set { flex_item_set_width(item, value); }
    }

    public float Height
    {
        get { return flex_item_get_height(item); }
        set { flex_item_set_height(item, value); }
    }

    public float Left
    {
        get { return flex_item_get_left(item); }
        set { flex_item_set_left(item, value); }
    }

    public float Right
    {
        get { return flex_item_get_right(item); }
        set { flex_item_set_right(item, value); }
    }

    public float Top
    {
        get { return flex_item_get_top(item); }
        set { flex_item_set_top(item, value); }
    }

    public float Bottom
    {
        get { return flex_item_get_bottom(item); }
        set { flex_item_set_bottom(item, value); }
    }

    public float PaddingLeft
    {
        get { return flex_item_get_padding_left(item); }
        set { flex_item_set_padding_left(item, value); }
    }

    public float PaddingRight
    {
        get { return flex_item_get_padding_right(item); }
        set { flex_item_set_padding_right(item, value); }
    }

    public float PaddingTop
    {
        get { return flex_item_get_padding_top(item); }
        set { flex_item_set_padding_top(item, value); }
    }

    public float PaddingBottom
    {
        get { return flex_item_get_padding_bottom(item); }
        set { flex_item_set_padding_bottom(item, value); }
    }

    public float MarginLeft
    {
        get { return flex_item_get_margin_left(item); }
        set { flex_item_set_margin_left(item, value); }
    }

    public float MarginRight
    {
        get { return flex_item_get_margin_right(item); }
        set { flex_item_set_margin_right(item, value); }
    }

    public float MarginTop
    {
        get { return flex_item_get_margin_top(item); }
        set { flex_item_set_margin_top(item, value); }
    }

    public float MarginBottom
    {
        get { return flex_item_get_margin_bottom(item); }
        set { flex_item_set_margin_bottom(item, value); }
    }

    public FlexAlign JustifyContent
    {
        get { return (FlexAlign)flex_item_get_justify_content(item); }
        set { flex_item_set_justify_content(item, (int)value); }
    }

    public FlexAlign AlignContent
    {
        get { return (FlexAlign)flex_item_get_align_content(item); }
        set { flex_item_set_align_content(item, (int)value); }
    }

    public FlexAlign AlignItems
    {
        get { return (FlexAlign)flex_item_get_align_items(item); }
        set { flex_item_set_align_items(item, (int)value); }
    }

    public FlexAlign AlignSelf
    {
        get { return (FlexAlign)flex_item_get_align_self(item); }
        set { flex_item_set_align_self(item, (int)value); }
    }

    public FlexPosition Position
    {
        get { return (FlexPosition)flex_item_get_position(item); }
        set { flex_item_set_position(item, (int)value); }
    }

    public FlexDirection Direction
    {
        get { return (FlexDirection)flex_item_get_direction(item); }
        set { flex_item_set_direction(item, (int)value); }
    }

    public FlexWrap Wrap
    {
        get { return (FlexWrap)flex_item_get_wrap(item); }
        set { flex_item_set_wrap(item, (int)value); }
    }

    public int Grow
    {
        get { return flex_item_get_grow(item); }
        set { flex_item_set_grow(item, value); }
    }

    public int Shrink
    {
        get { return flex_item_get_shrink(item); }
        set { flex_item_set_shrink(item, value); }
    }

    public int Order
    {
        get { return flex_item_get_order(item); }
        set { flex_item_set_order(item, value); }
    }

    public float Basis
    {
        get { return flex_item_get_basis(item); }
        set { flex_item_set_basis(item, value); }
    }
}

public class FlexRoot : FlexItem
{
    public FlexRoot(float width, float height) : base()
    {
        this.Width = width;
        this.Height = height;
    }

    public void Layout()
    {
        flex_layout(item);
    }
}
