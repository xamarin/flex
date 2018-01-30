using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace Xamarin.Flex.UnitTests
{
	[TestClass]
	public class XamarinFlexTests
	{
		[TestMethod]
		public void TestFlex()
		{
			var item = new Item(22, 22);
			var item1 = new Item
			{
				SelfSizing = (Item it, ref float w, ref float h) =>
				{
					w = 10.0f;
					h = 10.0f;
				}
			};
			item.Add(item1);
			item.Layout();
			Assert.IsNotNull(item);
		}
	}
}
