using Android.App;
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

            var root = new LayoutItem()
            {
                Width = metrics.WidthPixels,
                Height = metrics.HeightPixels,
                PaddingBottom = 80
            };
            root.view = new FrameLayout(this)
            {
                LayoutParameters = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MatchParent, ViewGroup.LayoutParams.MatchParent)
            };

            var label = new Item()
            {
                Margin = spacing
            };
            var label_view = new TextView(this)
            {
                Text = "This is the list of items you have added. You can add new items and clear the list using the buttons at the bottom. This label has extra lines on purpose.",
            };
            label.view = label_view;
            label.Height = new StaticLayout(label_view.Text, label_view.Paint, (int)root.Width, Layout.Alignment.AlignNormal, 1, 0, true).Height;
            root.Add(label);

            var list = new Item()
            {
                Grow = 1,
                MarginLeft = spacing,
                MarginRight = spacing
            };
            var list_view = new ListView(this);
            list_view.Adapter = new ArrayAdapter(this, Resource.Layout.TextViewItem);
            list.view = list_view;
            root.Add(list);

            var input = new Item()
            {
                Margin = spacing
            };
            var input_view = new EditText(this);
            input_view.Hint = "Enter list item";
            input.Height = 80;
            input.view = input_view;
            root.Add(input);

            var buttons_row = new LayoutItem()
            {
                Direction = Xamarin.Flex.Direction.Row,
                Height = 80,
                MarginLeft = spacing,
                MarginRight = spacing,
                MarginBottom = spacing
            };
            buttons_row.view = new FrameLayout(this)
            {
                LayoutParameters = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MatchParent, ViewGroup.LayoutParams.MatchParent)
            };
            root.Add(buttons_row);

            var add_button = new Item()
            {
                Grow = 1,
                Height = 80
            };
            var add_button_view = new Button(this);
            add_button_view.Text = "Add";
            add_button_view.Click += delegate
            {
                var adapter = (ArrayAdapter)list_view.Adapter;
                if (input_view.Text != "") {
                    adapter.Add(input_view.Text);
                    input_view.Text = "";
                }
            };
            add_button.view = add_button_view;
            buttons_row.Add(add_button);

            var clear_button = new Item()
            {
                Grow = 1,
                Height = 80
            };
            var clear_button_view = new Button(this);
            clear_button_view.Text = "Clear";
            clear_button_view.Click += delegate
            {
                var adapter = (ArrayAdapter)list_view.Adapter;
                adapter.Clear();
            };
            clear_button.view = clear_button_view;
            buttons_row.Add(clear_button);

            root.Layout();

            SetContentView(root.view);
        }
    }
}
