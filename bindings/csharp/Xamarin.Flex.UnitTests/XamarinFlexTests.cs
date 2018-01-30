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
			var item = new Xamarin.Flex.Item(22, 22);
			var item1 = new Xamarin.Flex.Item(10, 10);
			item.Add(item1);
			item.Layout();
			Assert.IsNotNull(item);
		}
	}
}
