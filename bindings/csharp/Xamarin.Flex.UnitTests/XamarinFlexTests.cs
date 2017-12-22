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
			Assert.IsNotNull(item);
		}
	}
}
