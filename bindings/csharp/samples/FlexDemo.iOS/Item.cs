using System;
using UIKit;

namespace FlexDemo.iOS
{
    public class Item : Xamarin.Flex.Item
    {
        private UIView view;

        public Item(UIView _view)
        {
            view = _view;
        }

        public void Add(Item item)
        {
            base.Add(item);
            view.AddSubview(item.view);
        }

        private void UpdateChildrenFrames()
        {
			foreach (Item item in this)
			{
				item.view.Frame = new CoreGraphics.CGRect((nfloat)item.FrameX, (nfloat)item.FrameY, (nfloat)item.FrameWidth, (nfloat)item.FrameHeight);
                item.UpdateChildrenFrames();
            }
        }

        public new void Layout()
        {
            base.Layout();
            UpdateChildrenFrames();
        }
    }
}
