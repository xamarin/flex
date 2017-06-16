using System;
using System.Collections.Generic;
using UIKit;

namespace FlexDemo.iOS
{
    abstract public class Base : Xamarin.Flex.Item
    {
        protected UIView view = null;

        public void Add(Base item)
        {
            base.Add(item);
            view.AddSubview(item.view);
        }

        private void UpdateChildrenFrames()
        {
            foreach (Base child in this) {
                child.view.Frame = new CoreGraphics.CGRect((nfloat)child.FrameX, (nfloat)child.FrameY, (nfloat)child.FrameWidth, (nfloat)child.FrameHeight);
                child.UpdateChildrenFrames();
            }
        }

        public new void Layout()
        {
            base.Layout();
            UpdateChildrenFrames();
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
