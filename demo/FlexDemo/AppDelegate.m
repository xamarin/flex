//
//  AppDelegate.m
//  FlexDemo
//
//  Created by Laurent Sansonetti on 09/05/17.
//  Copyright © 2017 Microsoft. All rights reserved.
//

#import "AppDelegate.h"
#import "FlexView.h"

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    [root setRoot:true];
    [self _reloadItems];
    [self itemSelected:nil];
}

- (FlexView *)_selectedItem
{
    NSInteger index = [items indexOfSelectedItem];
    return index == 0 ? root : [[root subviews] objectAtIndex:index - 1];
}

- (IBAction)addItem:(id)sender
{
    FlexView *item = [[FlexView alloc] init];
    flex_item_set_width([item item], 100);
    flex_item_set_height([item item], 100);
    [root addChild:item];
    [item autorelease];
    
    [self _reloadItems];
}

- (IBAction)removeItem:(id)sender
{
    FlexView *item = [self _selectedItem];
    assert(![item isRoot]);
    [root removeChild:item];

    [self _reloadItems];
    [items selectItemAtIndex:0];
    [self itemSelected:nil];
}

- (IBAction)itemSelected:(id)sender
{
    FlexView *view = [self _selectedItem];
    bool is_root = [view isRoot];

    [width setEnabled:!is_root];
    [height setEnabled:!is_root];
    [basis setEnabled:!is_root];
    [marginLeft setEnabled:!is_root];
    [marginRight setEnabled:!is_root];
    [marginTop setEnabled:!is_root];
    [marginBottom setEnabled:!is_root];
    [remove setEnabled:!is_root];

    struct flex_item *item = [view item];

#define FLEX_ENUM_GET(name, popup) \
    [popup selectItemWithTag:flex_item_get_##name(item)]

#define FLEX_FLOAT_GET(name, field) \
    do { \
	float _val = flex_item_get_##name(item); \
    	[field setStringValue:[NSString stringWithFormat:@"%0.1f", \
            isnan(_val) ? 0 : _val]]; \
    } \
    while (0)

#define FLEX_INT_GET(name, field) \
    [field setStringValue:[NSString stringWithFormat:@"%d", \
        flex_item_get_##name(item)]]
    
    FLEX_ENUM_GET(direction, direction);
    FLEX_ENUM_GET(align_items, alignItems);
    FLEX_ENUM_GET(justify_content, justifyContent);
    FLEX_ENUM_GET(align_self, alignSelf);

    [direction selectItemWithTag:flex_item_get_direction(item)];
    [alignItems selectItemWithTag:flex_item_get_align_items(item)];
    [justifyContent selectItemWithTag:flex_item_get_justify_content(item)];

    if (!is_root) {
        FLEX_FLOAT_GET(width, width);
        FLEX_FLOAT_GET(height, height);
        FLEX_FLOAT_GET(basis, basis);
        FLEX_FLOAT_GET(margin_left, marginLeft);
        FLEX_FLOAT_GET(margin_right, marginRight);
        FLEX_FLOAT_GET(margin_top, marginTop);
        FLEX_FLOAT_GET(margin_bottom, marginBottom);

    }
    else {
        [width setStringValue:@""];
        [height setStringValue:@""];
        [basis setStringValue:@""];
        [marginLeft setStringValue:@""];
        [marginRight setStringValue:@""];
        [marginTop setStringValue:@""];
        [marginBottom setStringValue:@""];
    }

    FLEX_INT_GET(grow, grow);
    FLEX_INT_GET(shrink, shrink);
    FLEX_INT_GET(order, order);

#undef FLEX_ENUM_GET
#undef FLEX_FLOAT_GET
#undef FLEX_INT_GET
}

#define FLEX_ENUM_ACTION(name, popup) \
    - (IBAction)popup##Selected:(id)sender \
    { \
	flex_item_set_##name([[self _selectedItem] item], \
                (int)[popup selectedTag]); \
	[root updateLayout]; \
    }

#define FLEX_FLOAT_ACTION(name, field) \
    - (IBAction)field##Selected:(id)sender \
    { \
	NSString *str = [field stringValue]; \
	if ([str length] > 0) { \
            flex_item_set_##name([[self _selectedItem] item], \
            	[[field stringValue] floatValue]); \
            [root updateLayout]; \
    	} \
    }

#define FLEX_INT_ACTION(name, field) \
    - (IBAction)field##Selected:(id)sender \
    { \
	NSString *str = [field stringValue]; \
	if ([str length] > 0) { \
            flex_item_set_##name([[self _selectedItem] item], \
                [[field stringValue] intValue]); \
            [root updateLayout]; \
    	} \
    }

FLEX_ENUM_ACTION(direction, direction);
FLEX_ENUM_ACTION(align_items, alignItems);
FLEX_ENUM_ACTION(justify_content, justifyContent);
FLEX_ENUM_ACTION(align_self, alignSelf);

FLEX_FLOAT_ACTION(width, width);
FLEX_FLOAT_ACTION(height, height);
FLEX_FLOAT_ACTION(basis, basis);

FLEX_FLOAT_ACTION(margin_left, marginLeft);
FLEX_FLOAT_ACTION(margin_right, marginRight);
FLEX_FLOAT_ACTION(margin_top, marginTop);
FLEX_FLOAT_ACTION(margin_bottom, marginBottom);

FLEX_INT_ACTION(grow, grow);
FLEX_INT_ACTION(shrink, shrink);
FLEX_INT_ACTION(order, order);

#undef FLEX_INT_ACTION
#undef FLEX_FLOAT_ACTION
#undef FLEX_ENUM_ACTION

- (void)_reloadItems
{
    [items removeAllItems];
    [items addItemWithTitle:@"Root"];

    for (NSInteger i = 0, count = [[root subviews] count]; i < count; i++) {
        [items addItemWithTitle:[NSString stringWithFormat:@"Item #%ld", i]];
    }
    
    [root updateLayout];
}

@end
