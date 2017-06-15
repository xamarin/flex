using System;
using System.Collections;
using UIKit;

namespace FlexDemo.iOS
{
	public class TableSource : UITableViewSource
	{
		public ArrayList tableItems;
		protected string cellIdentifier = "TableCell";
        public TableSource()
		{
			tableItems = new ArrayList();
		}
		public override nint RowsInSection(UITableView tableview, nint section)
		{
			return tableItems.Count;
		}
		public override UITableViewCell GetCell(UITableView tableView, Foundation.NSIndexPath indexPath)
		{
			// request a recycled cell to save memory
			UITableViewCell cell = tableView.DequeueReusableCell(cellIdentifier);
			// if there are no cells to reuse, create a new one
            if (cell == null) {
				cell = new UITableViewCell(UITableViewCellStyle.Default, cellIdentifier);
            }
            cell.TextLabel.Text = (string)tableItems[indexPath.Row];
			return cell;
		}
	}

	public partial class FirstViewController : UIViewController
    {
        protected FirstViewController(IntPtr handle) : base(handle)
        {
            // Note: this .ctor should not contain any initialization logic.
        }

        public override void ViewDidLoad()
        {
            base.ViewDidLoad();
            // Perform any additional setup after loading the view, typically from a nib.

            var root = new Item(this.View)
            {
                Width = (float)this.View.Frame.Size.Width,
                Height = (float)(this.View.Frame.Size.Height - TabBarController.TabBar.Frame.Size.Height),
                PaddingTop = 20,
            };

            float spacing = 20;

            var label = new UILabel();
            label.Text = "This is the list of items you have added. You can add new items and clear the list using the buttons at the bottom. This label has extra lines on purpose.";
            label.LineBreakMode = UILineBreakMode.WordWrap;
            label.Lines = 0;
			label.TextAlignment = UITextAlignment.Center;
			var label_item = new Item(label);
			label_item.Height = (float)label.AttributedText.GetBoundingRect(new CoreGraphics.CGSize(root.Width - 20, Double.MaxValue), Foundation.NSStringDrawingOptions.UsesLineFragmentOrigin, null).Size.Height;
            label_item.MarginTop = spacing;
            label_item.MarginLeft = label_item.MarginRight = 10;
            root.Add(label_item);

            var list = new UITableView();
            list.Source = new TableSource();
			var list_item = new Item(list);
			list_item.Grow = 1;
            list_item.MarginTop = spacing;
            root.Add(list_item);

            var input = new UITextField();
            input.Placeholder = "Enter list item";
            input.TextAlignment = UITextAlignment.Center;
            var input_item = new Item(input);
            input_item.Height = 25;
            input_item.MarginTop = spacing;
            root.Add(input_item);

            var buttons_row = new Item(new UIView());
            buttons_row.Direction = Xamarin.Flex.Direction.Row;
			buttons_row.Height = 25;
            buttons_row.MarginTop = buttons_row.MarginBottom = spacing;
            root.Add(buttons_row);

            var add = new UIButton();
            add.SetTitle("Add", UIControlState.Normal);
            add.SetTitleColor(UIColor.Black, UIControlState.Normal);
            add.TouchUpInside += delegate
            {
                var s = (TableSource)list.Source;
                var t = input.Text;
                if (t != "") {
                    s.tableItems.Add(input.Text);
                    list.ReloadData();
                    input.Text = "";
                }
            };
            var add_item = new Item(add);
            add_item.Grow = 1;
			add_item.Height = 25;
            buttons_row.Add(add_item);

			var clear = new UIButton();
			clear.SetTitle("Clear", UIControlState.Normal);
			clear.SetTitleColor(UIColor.Black, UIControlState.Normal);
			clear.TouchUpInside += delegate
			{
				var s = (TableSource)list.Source;
                s.tableItems.Clear();
                list.ReloadData();
			};
			var clear_item = new Item(clear);
            clear_item.Grow = 1;
			clear_item.Height = 25;
			buttons_row.Add(clear_item);

			root.Layout();
        }

        public override void DidReceiveMemoryWarning()
        {
            base.DidReceiveMemoryWarning();
            // Release any cached data, images, etc that aren't in use.
        }
    }
}
