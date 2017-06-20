﻿using System;
using System.Collections.Generic;
using Android.Widget;
using Android.Views;
using Android.Content;

using XF = Xamarin.Flex;

namespace Xamarin.Flex.Android
{
    abstract public class Base : XF.Item
    {
        public View view = null;

        public new void Add(XF.Item child)
        {
            throw new NotSupportedException("use LayoutItem instead");
        }
		
        public new void InsertAt(int index, XF.Item child)
        {
            throw new NotSupportedException("use LayoutItem instead");
        }

        public new XF.Item RemoveAt(int index)
        {
            throw new NotSupportedException("use LayoutItem instead");
        }

        public new void Layout()
        {
            throw new NotSupportedException("use LayoutItem instead");
        }
    }

    public class LayoutItem : XF.Item
    {
        public FrameLayout view = null;

        public LayoutItem(Context context)
        {
            view = new FrameLayout(context);
            view.LayoutParameters = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MatchParent, ViewGroup.LayoutParams.MatchParent);
        }

        static private View ViewOfItem(XF.Item item)
        {
            if (item is Base) {
                return ((Base)item).view;
            }
            if (item is LayoutItem) {
                return ((LayoutItem)item).view;
            }
            throw new NotSupportedException("use Item or LayoutItem instead");
        }

        public new void Add(XF.Item child)
        {
            base.Add(child);
            view.AddView(ViewOfItem(child));
        }

        public new void InsertAt(int index, XF.Item child)
        {
            base.InsertAt(index, child);
            view.AddView(ViewOfItem(child), index);
        }

        public new XF.Item RemoveAt(int index)
        {
            var child = base.RemoveAt(index);
            if (child != null) {
                view.RemoveView(ViewOfItem(child));
            }
            return child;
        }

        public new void Layout()
        {
            base.Layout();
            UpdateChildrenFrames(this);
        }

        static private void UpdateChildrenFrames(XF.Item parent)
        {
            foreach (XF.Item child in parent)
            {
                var view = ViewOfItem(child);
                var lp = view.LayoutParameters;

                var mlp = lp as ViewGroup.MarginLayoutParams;
                if (mlp != null) {
                    mlp.LeftMargin = (int)child.FrameX;
                    mlp.TopMargin = (int)child.FrameY;
                }
                else {
                    throw new NotSupportedException("child does not have margin layout params");
                }

                lp.Width = (int)child.FrameWidth;
                lp.Height = (int)child.FrameHeight;

                view.LayoutParameters = lp;

                UpdateChildrenFrames(child);
            }
        }
    }

    public class Item <T> : Base where T : View
    {
        public Item(Context context) : base()
        {
            View = (T)Activator.CreateInstance(typeof(T), new object[] { context });
        }

        public T View
        {
            get { return view as T; }
            set { view = value; }
        }
    }
}
