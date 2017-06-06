// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

using System;
using static FlexNativeFunctions;

public class FlexItem
{
    protected IntPtr item;

    public FlexItem()
    {
        item = flex_item_new();
    }

    public void Add(FlexItem child)
    {
        flex_item_add(item, child.item);
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

    ~FlexRoot()
    {
        flex_item_free(item);
    }

    public void Layout()
    {
        flex_layout(item);
    }
}
