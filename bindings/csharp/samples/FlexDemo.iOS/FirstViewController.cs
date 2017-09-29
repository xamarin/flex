﻿﻿using System;
using System.Collections;
using UIKit;
using Xamarin.Flex.iOS;

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
        }

        public override void ViewDidLoad()
        {
            base.ViewDidLoad();

            var root = new Item<UIView>(this.View)
            {
                PaddingTop = (float)UIApplication.SharedApplication.StatusBarFrame.Size.Height,
                PaddingBottom = (float)TabBarController.TabBar.Frame.Size.Height
            };

            float spacing = 20;

            var label = new Item<UILabel>()
            {
                MarginTop = spacing,
                MarginLeft = 10,
                MarginRight = 10
            };
            label.View.Text = "This is the list of items you have added. You can add new items and clear the list using the buttons at the bottom. This label has extra lines on purpose.";
            label.View.TextAlignment = UITextAlignment.Center;
            label.View.LineBreakMode = UILineBreakMode.WordWrap;
            label.View.Lines = 0;
            label.SelfSizing = delegate (Xamarin.Flex.Item item, ref float width, ref float height)
            {
                height = (float)label.View.AttributedText.GetBoundingRect(new CoreGraphics.CGSize(width - 20, Double.MaxValue), Foundation.NSStringDrawingOptions.UsesLineFragmentOrigin, null).Size.Height;
            };
            root.Add(label);

            var list = new Item<UITableView>()
            {
                Grow = 1,
                MarginTop = spacing
            };
            list.View.Source = new TableSource();
            root.Add(list);

            var input = new Item<UITextField>()
            {
                Height = 25,
                MarginTop = spacing
            };
            input.View.Placeholder = "Enter list item";
            input.View.TextAlignment = UITextAlignment.Center;
            root.Add(input);

            var buttons_row = new Item<UIView>()
            {
                Direction = Xamarin.Flex.Direction.Row,
                Height = 25,
                MarginTop = spacing,
                MarginBottom = spacing
            };
            root.Add(buttons_row);

            var add_button = new Item<UIButton>()
            {
                Grow = 1,
                Height = 25
            };
            add_button.View.SetTitle("Add", UIControlState.Normal);
            add_button.View.SetTitleColor(UIColor.Black, UIControlState.Normal);
            add_button.View.TouchUpInside += delegate
            {
                var s = list.View.Source as TableSource;
                var t = input.View.Text;
                if (t != "") {
                    s.tableItems.Add(t);
                    list.View.ReloadData();
                    input.View.Text = "";
                }
            };
            buttons_row.Add(add_button);

            var clear_button = new Item<UIButton>()
            {
                Grow = 1,
                Height = 25
            };
            clear_button.View.SetTitle("Clear", UIControlState.Normal);
            clear_button.View.SetTitleColor(UIColor.Black, UIControlState.Normal);
            clear_button.View.TouchUpInside += delegate
            {
                var s = list.View.Source as TableSource;
                s.tableItems.Clear();
                list.View.ReloadData();
            };
            buttons_row.Add(clear_button);

            root.Layout();
        }
    }
}
