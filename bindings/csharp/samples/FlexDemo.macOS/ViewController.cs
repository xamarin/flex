using System;

using AppKit;
using Foundation;
using Xamarin.Flex.macOS;

namespace FlexDemo.macOS
{
	public partial class ViewController : NSViewController
	{
		public ViewController(IntPtr handle) : base(handle)
		{
		}

		public override void ViewDidLoad()
		{
			base.ViewDidLoad();
			var root = new Item<NSView>(this.View)
            {
                PaddingTop = 10,
                PaddingBottom = 10
            };

			// Do any additional setup after loading the view.
		}

		public override NSObject RepresentedObject
		{
			get
			{
				return base.RepresentedObject;
			}
			set
			{
				base.RepresentedObject = value;
				// Update the view, if already loaded.
			}
		}
	}
}
