//
//  FlexView.m
//  FlexDemo
//
//  Created by Laurent Sansonetti on 09/05/17.
//  Copyright © 2017 Microsoft. All rights reserved.
//

#import "FlexView.h"

@implementation FlexView

- (void)awakeFromNib
{
    [self _init];
}

- (id)init
{
    self = [super init];
    if (self != nil) {
        [self _init];
    }
    return self;
}

- (NSColor *)_pickColor
{
    static int num = 0;
    switch (num++) {
        default:
        case 0: return [NSColor lightGrayColor];
        case 1: return [NSColor redColor];
        case 2: return [NSColor blueColor];
        case 3: num = 1; return [NSColor greenColor];
    }
}

- (void)_init
{
    _item = flex_item_new();
    _is_root = false;
    _background_color = [self _pickColor];
    _delegate = nil;
}

- (void)dealloc
{
    if (_is_root) {
        flex_item_free(_item);
    }
    [super dealloc];
}

- (BOOL)isFlipped
{
    return true;
}

- (void)addChild:(FlexView *)item
{
    flex_item_add(_item, item->_item);
    [self addSubview:item];
}

- (void)removeChild:(FlexView *)item
{
    NSInteger index = [[self subviews] indexOfObject:item];
    assert(index != NSNotFound);
    flex_item_delete(_item, (unsigned int)index);
    [item removeFromSuperview];
}

- (void)setRoot:(bool)root
{
    _is_root = root;
    [self _resize];
    [self setNeedsDisplay:YES];
}

- (bool)isRoot
{
    return _is_root;
}

- (struct flex_item *)item
{
    return _item;
}

- (void)updateLayout
{
    flex_layout(_item);

    for (FlexView *child_view in [self subviews]) {
        struct flex_item *child = child_view->_item;
        NSRect frame = NSMakeRect(flex_item_get_frame_x(child),
                flex_item_get_frame_y(child), flex_item_get_frame_width(child), 
                flex_item_get_frame_height(child));
        
        if (NSEqualRects([child_view frame], NSZeroRect)) {
            // This is just to make the animation smoother...
            [child_view setFrame:NSMakeRect(NSMidX(frame), NSMidY(frame),
                    0, 0)];
        }

        [[child_view animator] setFrame:frame];
    }
}

- (void)drawRect:(NSRect)rect
{
    [_background_color set];
    NSRectFill([self bounds]);
}

- (void)_resize
{
    if (_is_root) {
    	flex_item_set_width(_item, [self bounds].size.width);
    	flex_item_set_height(_item, [self bounds].size.height);
        [self updateLayout];
    }
}

- (void)setFrame:(NSRect)frame
{
    [super setFrame:frame];
    [self _resize];
}

- (void)setFrameSize:(NSSize)frame
{
    [super setFrameSize:frame];
    [self _resize];
}

- (id)delegate
{
    return _delegate;
}

- (void)setDelegate:(id)delegate
{
    _delegate = delegate; // weak
}

- (void)mouseDown:(NSEvent *)event
{
    if (_delegate != nil && [_delegate respondsToSelector:@selector(flexViewClicked:)]) {
        [_delegate flexViewClicked:self];
    }
}

@end
