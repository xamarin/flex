//
//  AppDelegate.h
//  FlexDemo
//
//  Created by Laurent Sansonetti on 09/05/17.
//  Copyright © 2017 Microsoft. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class FlexView;

@interface AppDelegate : NSObject <NSApplicationDelegate>
{
    IBOutlet NSWindow *window;
    IBOutlet FlexView *root;
    IBOutlet NSPopUpButton *items;
    IBOutlet NSButton *remove;
    IBOutlet NSPopUpButton *direction;
    IBOutlet NSPopUpButton *alignItems;
    IBOutlet NSPopUpButton *justifyContent;
    IBOutlet NSPopUpButton *wrap;
    IBOutlet NSPopUpButton *alignContent;
    IBOutlet NSPopUpButton *alignSelf;
    IBOutlet NSTextField *width;
    IBOutlet NSTextField *height;
    IBOutlet NSTextField *basis;
    IBOutlet NSTextField *grow;
    IBOutlet NSTextField *shrink;
    IBOutlet NSTextField *order;
    IBOutlet NSTextField *marginLeft;
    IBOutlet NSTextField *marginRight;
    IBOutlet NSTextField *marginTop;
    IBOutlet NSTextField *marginBottom;
    IBOutlet NSTextField *paddingLeft;
    IBOutlet NSTextField *paddingRight;
    IBOutlet NSTextField *paddingTop;
    IBOutlet NSTextField *paddingBottom;
    IBOutlet NSPopUpButton *position;
    IBOutlet NSTextField *positionLeft;
    IBOutlet NSTextField *positionRight;
    IBOutlet NSTextField *positionTop;
    IBOutlet NSTextField *positionBottom;
    IBOutlet NSColorWell *color;
    
    NSInteger _selected_item;
}

- (IBAction)addItem:(id)sender;
- (IBAction)removeItem:(id)sender;
- (IBAction)itemSelected:(id)sender;
- (IBAction)directionSelected:(id)sender;
- (IBAction)alignItemsSelected:(id)sender;
- (IBAction)justifyContentSelected:(id)sender;
- (IBAction)wrapSelected:(id)sender;
- (IBAction)alignContentSelected:(id)sender;
- (IBAction)alignSelfSelected:(id)sender;
- (IBAction)widthSelected:(id)sender;
- (IBAction)heightSelected:(id)sender;
- (IBAction)basisSelected:(id)sender;
- (IBAction)growSelected:(id)sender;
- (IBAction)shrinkSelected:(id)sender;
- (IBAction)orderSelected:(id)sender;
- (IBAction)marginLeftSelected:(id)sender;
- (IBAction)marginRightSelected:(id)sender;
- (IBAction)marginTopSelected:(id)sender;
- (IBAction)marginBottomSelected:(id)sender;
- (IBAction)paddingLeftSelected:(id)sender;
- (IBAction)paddingRightSelected:(id)sender;
- (IBAction)paddingTopSelected:(id)sender;
- (IBAction)paddingBottomSelected:(id)sender;
- (IBAction)positionSelected:(id)sender;
- (IBAction)positionLeftSelected:(id)sender;
- (IBAction)positionRightSelected:(id)sender;
- (IBAction)positionTopSelected:(id)sender;
- (IBAction)positionBottomSelected:(id)sender;
- (IBAction)colorSelected:(id)sender;

@end
