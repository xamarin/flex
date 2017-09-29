﻿using Android.App;
using Android.Widget;
using Android.OS;
using Android.Util;
using Android.Views;
using Android.Text;
using Xamarin.Flex.Android;

namespace FlexDemo.Android
{
    [Activity(Label = "FlexDemo", MainLauncher = true)]
    public class MainActivity : Activity
    {
        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);

            var metrics = new DisplayMetrics();
            WindowManager.DefaultDisplay.GetMetrics(metrics);

            float spacing = 40;

            var root = new LayoutItem(this)
            {
                Width = metrics.WidthPixels,
                Height = metrics.HeightPixels,
                PaddingBottom = 80
            };

            var label = new Item<TextView>(this)
            {
                Margin = spacing
            };
            label.View.Text = "This is the list of items you have added. You can add new items and clear the list using the buttons at the bottom. This label has extra lines on purpose.";
            label.SelfSizing = delegate (Xamarin.Flex.Item item, ref float width, ref float height)
            {
                height = new StaticLayout(label.View.Text, label.View.Paint, (int)width, Layout.Alignment.AlignNormal, 1, 0, true).Height;
            };
            root.Add(label);

            var list = new Item<ListView>(this)
            {
                Grow = 1,
                MarginLeft = spacing,
                MarginRight = spacing
            };
            list.View.Adapter = new ArrayAdapter(this, Resource.Layout.TextViewItem);
            root.Add(list);

            var input = new Item<EditText>(this)
            {
                Margin = spacing
            };
            input.View.Hint = "Enter list item";
            input.Height = 80;
            root.Add(input);

            var buttons_row = new LayoutItem(this)
            {
                Direction = Xamarin.Flex.Direction.Row,
                Height = 80,
                MarginLeft = spacing,
                MarginRight = spacing,
                MarginBottom = spacing
            };
            root.Add(buttons_row);

            var add_button = new Item<Button>(this)
            {
                Grow = 1,
                Height = 80
            };
            add_button.View.Text = "Add";
            add_button.View.Click += delegate
            {
                var adapter = (ArrayAdapter)list.View.Adapter;
                if (input.View.Text != "") {
                    adapter.Add(input.View.Text);
                    input.View.Text = "";
                }
            };
            buttons_row.Add(add_button);

            var clear_button = new Item<Button>(this)
            {
                Grow = 1,
                Height = 80
            };
            clear_button.View.Text = "Clear";
            clear_button.View.Click += delegate
            {
                var adapter = (ArrayAdapter)list.View.Adapter;
                adapter.Clear();
            };
            buttons_row.Add(clear_button);

            root.Layout();

            SetContentView(root.view);
        }
    }
}
