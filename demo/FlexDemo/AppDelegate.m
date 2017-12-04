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

- (void)awakeFromNib
{
    _selected_item = -1;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    [root setRoot:true];
    [root setDelegate:self];
    [self _reloadItems];
    [self _selectItem:0];
}

- (IBAction)addItem:(id)sender
{
    FlexView *parent = [self _selectedItem];

    FlexView *item = [[FlexView alloc] init];
    [item setDelegate:self];

    float item_width = 100, item_height = 100;
    if (![parent isRoot]) {
        item_width = flex_item_get_frame_width([parent item]) / 5.0;
        item_height = flex_item_get_frame_height([parent item]) / 5.0;
    }

    flex_item_set_width([item item], item_width);
    flex_item_set_height([item item], item_height);

    [parent addChild:item];

    [item autorelease];
    
    [self _reloadItems];
}

- (IBAction)removeItem:(id)sender
{
    FlexView *item = [self _selectedItem];
    assert(![item isRoot]);
    [(FlexView *)[item superview] removeChild:item];

    [self _reloadItems];
    [self _selectItem:0];
}

- (FlexView *)_selectedItem
{
    return [[items selectedItem] representedObject];
}

- (FlexView *)_itemAtIndex:(NSInteger)index
{
    return [[items itemAtIndex:index] representedObject];
}

- (void)_selectItem:(NSInteger)index
{
    if (_selected_item != index) {
        if (_selected_item != -1) {
            [[self _itemAtIndex:_selected_item] setSelected:false];
        }
        
        _selected_item = index;
        [items selectItemAtIndex:index];

        FlexView *item = [self _itemAtIndex:index];
        [self _updateWithItem:item];
        [item setSelected:true];
    }
}

- (void)_updateWithItem:(FlexView *)view
{
    bool is_root = [view isRoot];

    [width setEnabled:!is_root];
    [height setEnabled:!is_root];
    [basis setEnabled:!is_root];
    [marginLeft setEnabled:!is_root];
    [marginRight setEnabled:!is_root];
    [marginTop setEnabled:!is_root];
    [marginBottom setEnabled:!is_root];
    [position setEnabled:!is_root];
    [remove setEnabled:!is_root];

    [color setColor:[view color]];

    struct flex_item *item = [view item];

#define FLEX_ENUM_GET(name, popup) \
    [popup selectItemWithTag:flex_item_get_##name(item)]

#define FLEX_FLOAT_GET(name, field) \
    do { \
        float _val = flex_item_get_##name(item); \
    	[field setStringValue:isnan(_val) ? @"NAN" : [NSString stringWithFormat:@"%0.1f", \
            _val]]; \
    } \
    while (0)

#define FLEX_INT_GET(name, field) \
    [field setStringValue:[NSString stringWithFormat:@"%d", \
        flex_item_get_##name(item)]]
    
    FLEX_ENUM_GET(direction, direction);
    FLEX_ENUM_GET(align_items, alignItems);
    FLEX_ENUM_GET(justify_content, justifyContent);
    FLEX_ENUM_GET(wrap, wrap);
    FLEX_ENUM_GET(align_content, alignContent);
    FLEX_ENUM_GET(align_self, alignSelf);

    if (!is_root) {
        FLEX_FLOAT_GET(width, width);
        FLEX_FLOAT_GET(height, height);
        FLEX_FLOAT_GET(basis, basis);
        FLEX_FLOAT_GET(margin_left, marginLeft);
        FLEX_FLOAT_GET(margin_right, marginRight);
        FLEX_FLOAT_GET(margin_top, marginTop);
        FLEX_FLOAT_GET(margin_bottom, marginBottom);
        FLEX_ENUM_GET(position, position);
        FLEX_FLOAT_GET(left, positionLeft);
        FLEX_FLOAT_GET(right, positionRight);
        FLEX_FLOAT_GET(top, positionTop);
        FLEX_FLOAT_GET(bottom, positionBottom);
    }
    else {
        [width setStringValue:@""];
        [height setStringValue:@""];
        [basis setStringValue:@""];
        [marginLeft setStringValue:@""];
        [marginRight setStringValue:@""];
        [marginTop setStringValue:@""];
        [marginBottom setStringValue:@""];
        [positionLeft setStringValue:@""];
        [positionRight setStringValue:@""];
        [positionTop setStringValue:@""];
        [positionBottom setStringValue:@""];
    }

    FLEX_FLOAT_GET(padding_left, paddingLeft);
    FLEX_FLOAT_GET(padding_right, paddingRight);
    FLEX_FLOAT_GET(padding_top, paddingTop);
    FLEX_FLOAT_GET(padding_bottom, paddingBottom);
    
    FLEX_FLOAT_GET(grow, grow);
    FLEX_FLOAT_GET(shrink, shrink);
    FLEX_INT_GET(order, order);

#undef FLEX_ENUM_GET
#undef FLEX_FLOAT_GET
#undef FLEX_INT_GET
    
    [self _propertyChanged];
}

- (void)_propertyChanged
{
    [alignContent setEnabled:[wrap selectedTag] != FLEX_WRAP_NO_WRAP];

    bool position_enabled = [position selectedTag] == FLEX_POSITION_ABSOLUTE;
    [positionLeft setEnabled:position_enabled];
    [positionRight setEnabled:position_enabled];
    [positionTop setEnabled:position_enabled];
    [positionBottom setEnabled:position_enabled];
}

#define _GET_ITEM \
    struct flex_item *item = [[self _selectedItem] item]; \
    if (item == NULL) { \
        return; \
    }

#define FLEX_ENUM_ACTION(name, popup) \
    - (IBAction)popup##Selected:(id)sender \
    { \
        _GET_ITEM \
        flex_item_set_##name(item, (int)[popup selectedTag]); \
        [root updateLayout]; \
        [self _propertyChanged]; \
    }

#define FLEX_FLOAT_ACTION(name, field) \
    - (IBAction)field##Selected:(id)sender \
    { \
        _GET_ITEM \
        NSString *str = [field stringValue]; \
        float val = ([str isEqualToString:@""] || [str isEqualToString:@"NAN"]) \
            ? NAN : [str floatValue]; \
        flex_item_set_##name(item, val); \
        [root updateLayout]; \
        [self _propertyChanged]; \
    }

#define FLEX_INT_ACTION(name, field) \
    - (IBAction)field##Selected:(id)sender \
    { \
        _GET_ITEM \
        flex_item_set_##name(item, [[field stringValue] intValue]); \
        [root updateLayout]; \
        [self _propertyChanged]; \
    }

FLEX_ENUM_ACTION(direction, direction);
FLEX_ENUM_ACTION(align_items, alignItems);
FLEX_ENUM_ACTION(justify_content, justifyContent);
FLEX_ENUM_ACTION(wrap, wrap);
FLEX_ENUM_ACTION(align_content, alignContent);
FLEX_ENUM_ACTION(align_self, alignSelf);
FLEX_ENUM_ACTION(position, position);

FLEX_FLOAT_ACTION(width, width);
FLEX_FLOAT_ACTION(height, height);
FLEX_FLOAT_ACTION(basis, basis);

FLEX_FLOAT_ACTION(margin_left, marginLeft);
FLEX_FLOAT_ACTION(margin_right, marginRight);
FLEX_FLOAT_ACTION(margin_top, marginTop);
FLEX_FLOAT_ACTION(margin_bottom, marginBottom);

FLEX_FLOAT_ACTION(padding_left, paddingLeft);
FLEX_FLOAT_ACTION(padding_right, paddingRight);
FLEX_FLOAT_ACTION(padding_top, paddingTop);
FLEX_FLOAT_ACTION(padding_bottom, paddingBottom);

FLEX_FLOAT_ACTION(left, positionLeft);
FLEX_FLOAT_ACTION(right, positionRight);
FLEX_FLOAT_ACTION(top, positionTop);
FLEX_FLOAT_ACTION(bottom, positionBottom);

FLEX_FLOAT_ACTION(grow, grow);
FLEX_FLOAT_ACTION(shrink, shrink);
FLEX_INT_ACTION(order, order);

#undef _GET_ITEM
#undef FLEX_INT_ACTION
#undef FLEX_FLOAT_ACTION
#undef FLEX_ENUM_ACTION

- (IBAction)colorSelected:(id)sender
{
    [[self _selectedItem] setColor:[color color]];
}

- (IBAction)itemSelected:(id)sender
{
    [self _selectItem:[items indexOfSelectedItem]];
}

- (void)_addItem:(FlexView *)view indentation:(int)indentation
{
    NSInteger index = [items numberOfItems];
    [view setIndex:index];

    NSString *title = [view isRoot] ? @"Root" : [NSString stringWithFormat:@"Item #%ld", index];

    NSMenuItem *item = [[NSMenuItem alloc] initWithTitle:title action:nil keyEquivalent:@""];
    [item setRepresentedObject:view];
    [item setIndentationLevel:indentation];
    [[items menu] addItem:item];
    [item release];

    for (FlexView *child in [view subviews]) {
        [self _addItem:child indentation:indentation + 1];
    }
}

- (void)_reloadItems
{
    [items removeAllItems];
    _selected_item = -1;

    [self _addItem:root indentation:0];
    
    [root updateLayout];
}

- (void)flexViewClicked:(FlexView *)view
{
    [self _selectItem:[view index]];
}

@end
