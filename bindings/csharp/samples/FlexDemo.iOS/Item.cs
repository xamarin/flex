﻿﻿using System;
using System.Collections.Generic;
using UIKit;

using XF = Xamarin.Flex;

namespace Xamarin.Flex.iOS
{
    abstract public class Base : XF.Item
    {
        protected UIView view = null;

        public new void Add(XF.Item child)
        {
            base.Add(child);
            view.AddSubview((child as Base).view);
        }

        public new void InsertAt(int index, XF.Item child)
        {
            base.InsertAt(index, child);
            view.InsertSubview((child as Base).view, index);
        }

        public new XF.Item RemoveAt(int index)
        {
            var child = base.RemoveAt(index);
            if (child != null) {
                (child as Base).view.RemoveFromSuperview();
            }
            return child;
        }

        public new void Layout()
        {
            base.Layout();
            UpdateChildrenFrames();
        }

        private void UpdateChildrenFrames()
        {
            foreach (Base child in this) {
                child.view.Frame = new CoreGraphics.CGRect((nfloat)child.FrameX, (nfloat)child.FrameY, (nfloat)child.FrameWidth, (nfloat)child.FrameHeight);
                child.UpdateChildrenFrames();
            }
        }
    }

    public class Item <T> : Base where T : UIView
    {
        public Item() : base()
        {
            View = (T)Activator.CreateInstance(typeof(T), null);
        }

        public Item(T view) : base()
        {
            View = view;
            var frame_size = view.Frame.Size;
            Width = (float)frame_size.Width;
            Height = (float)frame_size.Height;
        }

        public T View
        {
            get { return view as T; }
            set { view = value; }
        }
    }
}
