// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

using System;
using System.Collections;
using System.Runtime.InteropServices;
using static Xamarin.Flex.NativeFunctions;

namespace Xamarin.Flex
{
    public partial class Item : IEnumerable
    {
        public Item()
        {
            item = flex_item_new();
            CreateHandle(false);
        }
    
        public Item(float width, float height) : this()
        {
            this.Width = width;
            this.Height = height;
        }
    
        ~Item()
        {
            if (item != IntPtr.Zero) {
                ReleaseHandlesWithinItem(item, false);
                flex_item_free(item);
                item = IntPtr.Zero;
            }
        }
    
        public void Add(Item child)
        {
            ValidateNewChild(child);
            child.CreateHandle(true);
            flex_item_add(item, child.item);
            notify_changed = true;
        }
    
        public void InsertAt(int index, Item child)
        {
            ValidateNewChild(child);
            ValidateIndex(index, true);
            child.CreateHandle(true);
            flex_item_insert(item, index, child.item);
            notify_changed = true;
        }
    
        public Item RemoveAt(int index)
        {
            ValidateIndex(index, false);
            IntPtr child_item = flex_item_delete(item, index);
            Item child = ReleaseHandleForItem(child_item, false);
            child.CreateHandle(false);
            notify_changed = true;
            return child;
        }
    
        public int Count
        {
            get { return flex_item_count(item); }
        }
    
        public Item ItemAt(int index)
        {
            ValidateIndex(index, false);
            return FlexItemFromItem(flex_item_child(item, index));
        }
 
        // The IndexerName() attribute is required because otherwise we clash
        // with another symbol named `Item' in the builtin library.
        [System.Runtime.CompilerServices.IndexerName("ItemIndexer")]
        public Item this[int index]
        {
            get { return ItemAt(index); }
        }
  
        public Item Parent
        {
            get { return FlexItemFromItem(flex_item_parent(item)); }
        }
    
        public Item Root
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

        public float Padding
        {
            set {
                this.PaddingTop = this.PaddingLeft = this.PaddingRight =
                    this.PaddingBottom = value;
            }
        }

        public float Margin
        {
            set {
                this.MarginTop = this.MarginLeft = this.MarginRight =
                    this.MarginBottom = value;
            }
        }

        public class ItemEnumerator : IEnumerator
        {
            private Item item;
            private int index;
    
            public ItemEnumerator(Item _item)
            {
                item = _item;
                index = -1;
            }
    
            object IEnumerator.Current
            {
                get { return Current; }
            }
    
            public Item Current
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
        public ItemEnumerator GetEnumerator()
        {
            notify_changed = false;
            return new ItemEnumerator(this);
        }
    
        private static GCHandle? HandleOfItem(IntPtr item)
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
    
        private static Item FlexItemFromItem(IntPtr item)
        {
            if (item != IntPtr.Zero) {
                var ret = HandleOfItem(item);
                if (ret.HasValue) {
                    return (Item)ret.Value.Target;
                }
            }
            return null;
        }
    
        private static Item ReleaseHandleForItem(IntPtr item, bool reset)
        {
            var ret = HandleOfItem(item);
            if (ret.HasValue) {
                GCHandle handle = ret.Value;
                Item child = (Item)handle.Target;
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
    
        private void ValidateNewChild(Item child)
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

        partial void ValidatePropertyValue(Properties property, int value)
        {
            bool invalid_align = false;
            if (property == Properties.AlignContent || property == Properties.JustifyContent) {
                if (value == (int)Align.Auto || value == (int)Align.Stretch) {
                    invalid_align = true;
                }
            }
            else if (property == Properties.AlignItems) {
                if (value == (int)Align.Auto) {
                    invalid_align = true;
                }
            }
            if (invalid_align) {
                throw new ArgumentException("invalid Align value");
            }
        }
    }
}
