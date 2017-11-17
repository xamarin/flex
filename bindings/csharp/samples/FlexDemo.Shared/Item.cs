using System;
using System.Collections.Generic;
using XF = Xamarin.Flex;

#if __IOS__
using NativeView = UIKit.UIView;
namespace Xamarin.Flex.iOS
#elif WINDOWS_UWP
using NativeView = Windows.UI.Xaml.Controls.UserControl;
namespace Xamarin.Flex.UWP
#else
using NativeView = AppKit.NSView;
namespace Xamarin.Flex.macOS
#endif
{
	abstract public class Base : XF.Item
	{
		protected NativeView view = null;

		public new void Add(XF.Item child)
		{
			base.Add(child);
#if WINDOWS_UWP
#else
			view.AddSubview((child as Base).view);
#endif
		}

		public new void InsertAt(int index, XF.Item child)
		{
			base.InsertAt(index, child);
#if __IOS__
			view.InsertSubview((child as Base).view, index);
#elif WINDOWS_UWP

#else
			if(index == 0)
				view.AddSubview((child as Base).view);
			var previousIndex = index - 1;
			var previousView = view.Subviews[previousIndex];
			view.AddSubview((child as Base).view, AppKit.NSWindowOrderingMode.Above, previousView);
#endif

		}

		public new XF.Item RemoveAt(int index)
		{
			var child = base.RemoveAt(index);
#if WINDOWS_UWP
#else
            if (child != null) {
                (child as Base).view.RemoveFromSuperview();
            }
#endif
			return child;
		}

		public new void Layout()
		{
			base.Layout();
			UpdateChildrenFrames();
		}

		private void UpdateChildrenFrames()
		{
			foreach (Base child in this)
			{
#if WINDOWS_UWP
#else
                child.view.Frame = new CoreGraphics.CGRect((nfloat)child.FrameX, (nfloat)child.FrameY, (nfloat)child.FrameWidth, (nfloat)child.FrameHeight);
#endif
				child.UpdateChildrenFrames();
			}
		}
	}

	public class Item<T> : Base where T : NativeView
	{
		public Item() : base()
		{
			View = (T)Activator.CreateInstance(typeof(T), null);
		}

		public Item(T view) : base()
		{
			View = view;
#if WINDOWS_UWP
#else
			var frame_size = view.Frame.Size;
			Width = (float)frame_size.Width;
			Height = (float)frame_size.Height;
#endif
		}

		public T View
		{
			get { return view as T; }
			set { view = value; }
		}
	}
}
