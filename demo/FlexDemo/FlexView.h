//
//  FlexView.h
//  FlexDemo
//
//  Created by Laurent Sansonetti on 09/05/17.
//  Copyright © 2017 Microsoft. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "flex.h"

struct flex_item;

@interface FlexView : NSView
{
    struct flex_item *_item;
    bool _is_root;
    NSColor *_background_color;
}

- (void)addChild:(FlexView *)item;
- (void)removeChild:(FlexView *)item;

- (void)setRoot:(bool)root;
- (bool)isRoot;

- (struct flex_item *)item;

- (void)updateLayout;

@end
