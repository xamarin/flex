// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

#ifndef __FLEX_H_
# define __FLEX_H_

struct flex_item;

// Create a new flex item.
struct flex_item *flex_item_new(void);

// Free memory associated with a flex item and its children.
// This function can only be called on a root item.
void flex_item_free(struct flex_item *item);

// Manage items.
void flex_item_add(struct flex_item *item, struct flex_item *child);
void flex_item_insert(struct flex_item *item, unsigned int index,
        struct flex_item *child);
struct flex_item *flex_item_delete(struct flex_item *item, unsigned int index);
unsigned int flex_item_count(struct flex_item *item);
struct flex_item *flex_item_child(struct flex_item *item, unsigned int index);
struct flex_item *flex_item_parent(struct flex_item *item);

// Layout the items associated with this item, as well as their children.
// This function can only be called on a root item whose `width' and `height'
// properties have been set.
void flex_layout(struct flex_item *item);

// Retrieve the layout frame associated with an item. These functions should
// be called *after* the layout is done.
float flex_item_get_frame_x(struct flex_item *item);
float flex_item_get_frame_y(struct flex_item *item);
float flex_item_get_frame_width(struct flex_item *item);
float flex_item_get_frame_height(struct flex_item *item);

typedef enum {
    FLEX_ALIGN_AUTO = 0,
    FLEX_ALIGN_STRETCH,
    FLEX_ALIGN_CENTER,
    FLEX_ALIGN_FLEX_START,
    FLEX_ALIGN_FLEX_END,
    FLEX_ALIGN_SPACE_BETWEEN,
    FLEX_ALIGN_SPACE_AROUND,
    FLEX_ALIGN_SPACE_EVENLY
} flex_align;

typedef enum {
    FLEX_POSITION_RELATIVE = 0,
    FLEX_POSITION_ABSOLUTE
} flex_position;

typedef enum {
    FLEX_DIRECTION_ROW = 0,
    FLEX_DIRECTION_ROW_REVERSE,
    FLEX_DIRECTION_COLUMN,
    FLEX_DIRECTION_COLUMN_REVERSE
} flex_direction;

typedef enum {
    FLEX_WRAP_NOWRAP = 0,
    FLEX_WRAP_WRAP,
    FLEX_WRAP_WRAP_REVERSE
} flex_wrap;

# ifndef FLEX_ATTRIBUTE
#  define FLEX_ATTRIBUTE(name, type, def) \
    type flex_item_get_##name(struct flex_item *item); \
    void flex_item_set_##name(struct flex_item *item, type value);
# endif

#else // !__FLEX_H_

# ifndef FLEX_ATTRIBUTE
#  define FLEX_ATTRIBUTE(name, type, def)
# endif

#endif

// Following are the list of properties associated with an item.
//
// Each property is exposed with getter and setter functions, for instance
// the `width' property can be get and set using the respective
// `flex_item_get_width()' and `flex_item_set_width()' functions.
//
// You can also see the type and default value for each property.

FLEX_ATTRIBUTE(width, float, NAN)
FLEX_ATTRIBUTE(height, float, NAN)

FLEX_ATTRIBUTE(left, float, 0)
FLEX_ATTRIBUTE(right, float, 0)
FLEX_ATTRIBUTE(top, float, 0)
FLEX_ATTRIBUTE(bottom, float, 0)

FLEX_ATTRIBUTE(padding_left, float, 0)
FLEX_ATTRIBUTE(padding_right, float, 0)
FLEX_ATTRIBUTE(padding_top, float, 0)
FLEX_ATTRIBUTE(padding_bottom, float, 0)

FLEX_ATTRIBUTE(margin_left, float, 0)
FLEX_ATTRIBUTE(margin_right, float, 0)
FLEX_ATTRIBUTE(margin_top, float, 0)
FLEX_ATTRIBUTE(margin_bottom, float, 0)

FLEX_ATTRIBUTE(justify_content, flex_align, FLEX_ALIGN_FLEX_START)
FLEX_ATTRIBUTE(align_content, flex_align, FLEX_ALIGN_AUTO)
FLEX_ATTRIBUTE(align_items, flex_align, FLEX_ALIGN_FLEX_START)
FLEX_ATTRIBUTE(align_self, flex_align, FLEX_ALIGN_AUTO)

FLEX_ATTRIBUTE(position, flex_position, FLEX_POSITION_RELATIVE)
FLEX_ATTRIBUTE(direction, flex_direction, FLEX_DIRECTION_COLUMN)
FLEX_ATTRIBUTE(wrap, flex_wrap, FLEX_WRAP_NOWRAP)

FLEX_ATTRIBUTE(grow, unsigned int, 0)
FLEX_ATTRIBUTE(shrink, unsigned int, 1)
FLEX_ATTRIBUTE(order, int, 0)
FLEX_ATTRIBUTE(basis, float, 0)
