// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "flex.h"

struct flex_item {
#define FLEX_ATTRIBUTE(name, type, def) type name;
#include "flex.h"

    float frame[4];
    struct flex_item *parent;
    struct {
	unsigned int cap;
	unsigned int count;
	struct flex_item **ary;
    } children;
    bool should_order_children;
};

typedef enum {
#define FLEX_ATTRIBUTE(name, type, def) FLEX_PROPERTY_##name,
#include "flex.h"
} flex_property;

static void
update_should_order_children(struct flex_item *item)
{
    if (item->order != 0 && item->parent != NULL) {
        item->parent->should_order_children = true;
    }
}

static void
item_property_changed(struct flex_item *item, flex_property property)
{
    if (property == FLEX_PROPERTY_order) {
        update_should_order_children(item);
    }
}

#define FLEX_ATTRIBUTE(name, type, def) \
    DLLEXPORT type flex_item_get_##name(struct flex_item *item) { \
        return item->name; \
    } \
    DLLEXPORT void flex_item_set_##name(struct flex_item *item, \
			type value) { \
        item->name = value; \
        item_property_changed(item, FLEX_PROPERTY_##name); \
    }
#include "flex.h"

DLLEXPORT
struct flex_item *
flex_item_new(void)
{
    struct flex_item *item =
        (struct flex_item *)malloc(sizeof(struct flex_item));
    assert(item != NULL);

#define FLEX_ATTRIBUTE(name, type, def) item->name = def;
#include "flex.h"

    memset(item->frame, 0, sizeof item->frame);

    item->parent = NULL;
    item->children.cap = 0;
    item->children.count = 0;
    item->children.ary = NULL;
    item->should_order_children = false;

    return item;
}

DLLEXPORT
void
flex_item_free(struct flex_item *item)
{
    assert(item->parent == NULL);

    if (item->children.cap > 0) {
        assert(item->children.ary != NULL);

        for (unsigned int i = 0; i < item->children.count; i++) {
            struct flex_item *child = item->children.ary[i];
            child->parent = NULL;
            flex_item_free(child);
        }
        free(item->children.ary);
    }
    free(item);
}

static void
grow_if_needed(struct flex_item *item)
{
    if (item->children.count == item->children.cap) {
        item->children.cap++;
        assert(item->children.count < item->children.cap);

        item->children.ary = (struct flex_item **)realloc(item->children.ary,
                sizeof(struct flex_item *) * item->children.cap);
        assert(item->children.ary != NULL);
    }
}

static void
child_set(struct flex_item *item, struct flex_item *child, unsigned int index)
{
    assert(child->parent == NULL && "child already has a parent");
    item->children.ary[index] = child;
    item->children.count++;
    child->parent = item;
    update_should_order_children(child);
}

DLLEXPORT
void
flex_item_add(struct flex_item *item, struct flex_item *child)
{
    grow_if_needed(item);
    child_set(item, child, item->children.count);
}

DLLEXPORT
void
flex_item_insert(struct flex_item *item, unsigned int index,
    struct flex_item *child)
{
    assert(index <= item->children.count);

    grow_if_needed(item);
    for (unsigned int i = item->children.count; i > index; i--) {
        item->children.ary[i] = item->children.ary[i - 1];
    }
    child_set(item, child, index);
}

DLLEXPORT
struct flex_item *
flex_item_delete(struct flex_item *item, unsigned int index)
{
    assert(index < item->children.count);
    assert(item->children.count > 0);

    struct flex_item *child = item->children.ary[index];

    for (unsigned int i = index; i < item->children.count - 1; i++) {
        item->children.ary[i] = item->children.ary[i + 1];
    }
    item->children.count--;

    child->parent = NULL;
    return child;
}

DLLEXPORT
unsigned int
flex_item_count(struct flex_item *item)
{
    return item->children.count;
}

DLLEXPORT
struct flex_item *
flex_item_child(struct flex_item *item, unsigned int index)
{
    assert(index < item->children.count);

    return item->children.ary[index];
}

DLLEXPORT
struct flex_item *
flex_item_parent(struct flex_item *item)
{
    return item->parent;
}

DLLEXPORT
struct flex_item *
flex_item_root(struct flex_item *item)
{
    while (item->parent != NULL) {
        item = item->parent;
    }
    return item;
}

#define FRAME_GETTER(name, index) \
    DLLEXPORT float flex_item_get_frame_##name(struct flex_item *item) { \
        return item->frame[index]; \
    }

FRAME_GETTER(x, 0)
FRAME_GETTER(y, 1)
FRAME_GETTER(width, 2)
FRAME_GETTER(height, 3)

#undef FRAME_GETTER

struct flex_layout {
    // Set during init.
    bool wrap;
    bool reverse;               // whether main axis is reversed
    bool reverse2;              // whether cross axis is reversed (wrap only)
    bool vertical;
    float size_dim;             // main axis parent size
    float align_dim;            // cross axis parent size
    unsigned int frame_pos_i;   // main axis position
    unsigned int frame_pos2_i;  // cross axis position
    unsigned int frame_size_i;  // main axis size
    unsigned int frame_size2_i; // cross axis size
    unsigned int *ordered_indices;

    // Set for each line layout.
    float line_dim;             // the cross axis size
    float flex_dim;             // the flexible part of the main axis size
    float extra_flex_dim;       // sizes of flexible items
    float flex_grows;
    float flex_shrinks;
    float pos2;                 // cross axis position

    // Calculated layout lines - only tracked when needed:
    //   - if the root's align_content property isn't set to FLEX_ALIGN_START
    //   - or if any child item doesn't have a cross-axis size set
    bool need_lines;
    struct flex_layout_line {
        unsigned int child_begin;
        unsigned int child_end;
        float size;
    } *lines;
    unsigned int lines_count;
    float lines_sizes;
};

static void
layout_init(struct flex_item *item, float width, float height,
        struct flex_layout *layout)
{
    assert(item->padding_left >= 0);
    assert(item->padding_right >= 0);
    assert(item->padding_top >= 0);
    assert(item->padding_bottom >= 0);
    width -= item->padding_left + item->padding_right;
    height -= item->padding_top + item->padding_bottom;
    assert(width >= 0);
    assert(height >= 0);

    layout->reverse = false;
    layout->vertical = true;
    switch (item->direction) {
        case FLEX_DIRECTION_ROW_REVERSE:
            layout->reverse = true;
        case FLEX_DIRECTION_ROW:
            layout->vertical = false;
            layout->size_dim = width;
            layout->align_dim = height;
            layout->frame_pos_i = 0;
            layout->frame_pos2_i = 1;
            layout->frame_size_i = 2;
            layout->frame_size2_i = 3;
            break;

        case FLEX_DIRECTION_COLUMN_REVERSE:
            layout->reverse = true;
        case FLEX_DIRECTION_COLUMN:
            layout->size_dim = height;
            layout->align_dim = width;
            layout->frame_pos_i = 1;
            layout->frame_pos2_i = 0;
            layout->frame_size_i = 3;
            layout->frame_size2_i = 2;
            break;

        default:
            assert(false && "incorrect direction");
    }

    layout->ordered_indices = NULL;
    if (item->should_order_children && item->children.count > 0) {
        unsigned int *indices = (unsigned int *)malloc(sizeof(unsigned int)
                * item->children.count);
        assert(indices != NULL);

        // Creating a list of item indices sorted using the children's `order'
        // attribute values. We are using a simple insertion sort as we need
        // stability (insertion order must be preserved) and cross-platform
        // support. We should eventually switch to merge sort (or something
        // else) if the number of items becomes significant enough.
        for (unsigned int i = 0; i < item->children.count; i++) {
            indices[i] = i;
            for (unsigned int j = i; j > 0; j--) {
                unsigned int prev = indices[j - 1];
                unsigned int curr = indices[j];
                if (item->children.ary[prev]->order
                        <= item->children.ary[curr]->order) {
                    break;
                }
                indices[j - 1] = curr;
                indices[j] = prev;
            }
        }
        layout->ordered_indices = indices;
    }

    layout->flex_dim = 0;
    layout->flex_grows = 0;
    layout->flex_shrinks = 0;

    layout->reverse2 = false;
    layout->wrap = item->wrap != FLEX_WRAP_NO_WRAP;
    if (layout->wrap) {
        if (item->wrap == FLEX_WRAP_WRAP_REVERSE) {
            layout->reverse2 = true;
            layout->pos2 = layout->align_dim;
        }
    }
    else {
        layout->pos2 = layout->vertical
            ? item->padding_left : item->padding_top;
    }

    layout->need_lines = layout->wrap
        && item->align_content != FLEX_ALIGN_START;
    layout->lines = NULL;
    layout->lines_count = 0;
    layout->lines_sizes = 0;
}

static void
layout_cleanup(struct flex_layout *layout)
{
    if (layout->ordered_indices != NULL) {
        free(layout->ordered_indices);
        layout->ordered_indices = NULL;
    }
    if (layout->lines != NULL) {
        free(layout->lines);
        layout->lines = NULL;
    }
    layout->lines_count = 0;
}

#define LAYOUT_RESET() \
    do { \
        layout->line_dim = layout->wrap ? 0 : layout->align_dim; \
        layout->flex_dim = layout->size_dim; \
        layout->extra_flex_dim = 0; \
        layout->flex_grows = 0; \
        layout->flex_shrinks = 0; \
    } \
    while (0)

#define LAYOUT_CHILD_AT(item, i) \
    (item->children.ary[(layout->ordered_indices != NULL \
                         ? layout->ordered_indices[i] : i)]) \

#define _LAYOUT_FRAME(child, name) child->frame[layout->frame_##name##_i]

#define CHILD_POS(child) _LAYOUT_FRAME(child, pos)
#define CHILD_POS2(child) _LAYOUT_FRAME(child, pos2)
#define CHILD_SIZE(child) _LAYOUT_FRAME(child, size)
#define CHILD_SIZE2(child) _LAYOUT_FRAME(child, size2)

#define CHILD_MARGIN(child, if_vertical, if_horizontal) \
    (layout->vertical \
     ? child->margin_##if_vertical \
     : child->margin_##if_horizontal)

static void layout_item(struct flex_item *item, float width, float height);

static bool
layout_align(flex_align align, float flex_dim, unsigned int children_count,
        float *pos_p, float *spacing_p, bool stretch_allowed)
{
    assert(flex_dim > 0);

    float pos = 0;
    float spacing = 0;
    switch (align) {
        case FLEX_ALIGN_START:
            break;

        case FLEX_ALIGN_END:
            pos = flex_dim;
            break;

        case FLEX_ALIGN_CENTER:
            pos = flex_dim / 2;
            break;

        case FLEX_ALIGN_SPACE_BETWEEN:
            if (children_count > 0) {
                spacing = flex_dim / (children_count - 1);
            }
            break;

        case FLEX_ALIGN_SPACE_AROUND:
            if (children_count > 0) {
                spacing = flex_dim / children_count;
                pos = spacing / 2;
            }
            break;

        case FLEX_ALIGN_SPACE_EVENLY:
            if (children_count > 0) {
                spacing = flex_dim / (children_count + 1);
                pos = spacing;
            }
            break;

        case FLEX_ALIGN_STRETCH:
            if (stretch_allowed) {
                spacing = flex_dim / children_count;
                break;
            }
            // fall through

        default:
            return false;
    }

    *pos_p = pos;
    *spacing_p = spacing;
    return true;
}

static flex_align
child_align(struct flex_item *child, struct flex_item *parent)
{
    flex_align align = child->align_self;
    if (align == FLEX_ALIGN_AUTO) {
        align = parent->align_items;
    }
    return align;
}

static void
layout_items(struct flex_item *item, unsigned int child_begin,
        unsigned int child_end, unsigned int children_count,
        struct flex_layout *layout)
{
    assert(children_count <= (child_end - child_begin));
    if (children_count <= 0) {
        return;
    }

    if (layout->flex_dim > 0 && layout->extra_flex_dim > 0) {
        // If the container has a positive flexible space, let's add to it
        // the sizes of all flexible children.
        layout->flex_dim += layout->extra_flex_dim;
    }

    // Determine the main axis initial position and optional spacing.
    float pos = 0;
    float spacing = 0;
    if (layout->flex_grows == 0 && layout->flex_dim > 0) {
        assert(layout_align(item->justify_content, layout->flex_dim,
                    children_count, &pos, &spacing, false)
                && "incorrect justify_content");
        if (layout->reverse) {
            pos = layout->size_dim - pos;
        }
    }

    if (layout->reverse) {
        pos -= layout->vertical ? item->padding_bottom : item->padding_right;
    }
    else {
        pos += layout->vertical ? item->padding_top : item->padding_left;
    }
    if (layout->wrap && layout->reverse2) {
        layout->pos2 -= layout->line_dim;
    }

    for (unsigned int i = child_begin; i < child_end; i++) {
        struct flex_item *child = LAYOUT_CHILD_AT(item, i);
        if (child->position == FLEX_POSITION_ABSOLUTE) {
            // Already positioned.
            continue;
        }

        // Grow or shrink the main axis item size if needed.
        float flex_size = 0;
        if (layout->flex_dim > 0) {
            if (child->grow != 0) {
                CHILD_SIZE(child) = 0; // Ignore previous size when growing.
                flex_size = (layout->flex_dim / layout->flex_grows)
                    * child->grow;
            }
        }
        else if (layout->flex_dim < 0) {
            if (child->shrink != 0) {
                flex_size = (layout->flex_dim / layout->flex_shrinks)
                    * child->shrink;
            }
        }
        CHILD_SIZE(child) += flex_size;

        // Set the cross axis position (and stretch the cross axis size if
        // needed).
        float align_size = CHILD_SIZE2(child);
        float align_pos = layout->pos2 + 0;
        switch (child_align(child, item)) {
            case FLEX_ALIGN_END:
                align_pos += layout->line_dim - align_size
                    - CHILD_MARGIN(child, right, bottom);
                break;

            case FLEX_ALIGN_CENTER:
                align_pos += (layout->line_dim / 2) - (align_size / 2)
                    + (CHILD_MARGIN(child, left, top)
                            - CHILD_MARGIN(child, right, bottom));
                break;

            case FLEX_ALIGN_STRETCH:
                if (align_size == 0) {
                    CHILD_SIZE2(child) = layout->line_dim
                        - (CHILD_MARGIN(child, left, top)
                                + CHILD_MARGIN(child, right, bottom));
                }
                // fall through

            case FLEX_ALIGN_START:
                align_pos += CHILD_MARGIN(child, left, top);
                break;

            default:
                assert(false && "incorrect align_self");
        }
        CHILD_POS2(child) = align_pos;

        // Set the main axis position.
        if (layout->reverse) {
            pos -= CHILD_MARGIN(child, bottom, right);
            pos -= CHILD_SIZE(child);
            CHILD_POS(child) = pos;
            pos -= spacing;
            pos -= CHILD_MARGIN(child, top, left);
        }
        else {
            pos += CHILD_MARGIN(child, top, left);
            CHILD_POS(child) = pos;
            pos += CHILD_SIZE(child);
            pos += spacing;
            pos += CHILD_MARGIN(child, bottom, right);
        }

        // Now that the item has a frame, we can layout its children.
        layout_item(child, child->frame[2], child->frame[3]);
    }

    if (layout->wrap && !layout->reverse2) {
        layout->pos2 += layout->line_dim;
    }

    if (layout->need_lines) {
        layout->lines = (struct flex_layout_line *)realloc(layout->lines,
                sizeof(struct flex_layout_line) * (layout->lines_count + 1));
        assert(layout->lines != NULL);

        struct flex_layout_line *line = &layout->lines[layout->lines_count];
        line->child_begin = child_begin;
        line->child_end = child_end;
        line->size = layout->line_dim;

        layout->lines_count++;
        layout->lines_sizes += line->size;
    }
}

static void
layout_item(struct flex_item *item, float width, float height)
{
    if (item->children.count == 0) {
        return;
    }

    struct flex_layout layout_s = { 0 }, *layout = &layout_s;
    layout_init(item, width, height, &layout_s);

    LAYOUT_RESET();

    unsigned int last_layout_child = 0;
    unsigned int relative_children_count = 0;
    for (unsigned int i = 0; i < item->children.count; i++) {
        struct flex_item *child = LAYOUT_CHILD_AT(item, i);

        // Items with an absolute position have their frames determined
        // directly and are skipped during layout.
        if (child->position == FLEX_POSITION_ABSOLUTE) {
#define ABSOLUTE_SIZE(val, pos1, pos2, dim) \
            (!isnan(val) \
             ? val \
             : (!isnan(pos1) && !isnan(pos2) \
                 ? dim - pos2 - pos1 \
                 : 0))

#define ABSOLUTE_POS(pos1, pos2, size, dim) \
            (!isnan(pos1) \
             ? pos1 \
             : (!isnan(pos2) \
                 ? dim - size - pos2 \
                 : 0))

            float child_width = ABSOLUTE_SIZE(child->width, child->left,
                    child->right, width);

            float child_height = ABSOLUTE_SIZE(child->height, child->top,
                    child->bottom, height);

            float child_x = ABSOLUTE_POS(child->left, child->right,
                    child_width, width);

            float child_y = ABSOLUTE_POS(child->top, child->bottom,
                    child_height, height);

            child->frame[0] = child_x;
            child->frame[1] = child_y;
            child->frame[2] = child_width;
            child->frame[3] = child_height;

            // Now that the item has a frame, we can layout its children.
            layout_item(child, child->frame[2], child->frame[3]);

#undef ABSOLUTE_POS
#undef ABSOLUTE_SIZE
            continue;
        }

        // Initialize frame.
        child->frame[0] = 0;
        child->frame[1] = 0;
        child->frame[2] = child->width;
        child->frame[3] = child->height;

        // Main axis size defaults to 0.
        if (isnan(CHILD_SIZE(child))) {
            CHILD_SIZE(child) = 0;
        }

        // Cross axis size defaults to the parent's size (or line size in wrap
        // mode, which is calculated later on).
        if (isnan(CHILD_SIZE2(child))) {
            if (layout->wrap) {
                layout->need_lines = true;
            }
            else {
                CHILD_SIZE2(child) = (layout->vertical ? width : height)
                    - CHILD_MARGIN(child, left, top)
                    - CHILD_MARGIN(child, right, bottom);
            }
        }

        // Call the self_sizing callback if provided. Only non-NAN values
        // are taken into account. If the item's cross-axis align property
        // is set to stretch, ignore the value returned by the callback.
        if (child->self_sizing != NULL) {
            float size[2] = { child->frame[2], child->frame[3] };

            child->self_sizing(child, size);

            for (unsigned int j = 0; j < 2; j++) {
                unsigned int size_off = j + 2;
                if (size_off == layout->frame_size2_i
                        && child_align(child, item) == FLEX_ALIGN_STRETCH) {
                    continue;
                }
                float val = size[j];
                if (!isnan(val)) {
                    child->frame[size_off] = val;
                }
            }
        }

        // Honor the `basis' property which overrides the main-axis size.
        if (!isnan(child->basis)) {
            assert(child->basis >= 0);
            CHILD_SIZE(child) = child->basis;
        }

        float child_size = CHILD_SIZE(child);
        if (layout->wrap) {
            if (layout->flex_dim < child_size) {
                // Not enough space for this child on this line, layout the
                // remaining items and move it to a new line.
                layout_items(item, last_layout_child, i,
                        relative_children_count, layout);

                LAYOUT_RESET();
                last_layout_child = i;
                relative_children_count = 0;
            }

            float child_size2 = CHILD_SIZE2(child);
            if (!isnan(child_size2) && child_size2 > layout->line_dim) {
                layout->line_dim = child_size2;
            }
        }

        assert(child->grow >= 0);
        assert(child->shrink >= 0);

        layout->flex_grows += child->grow;
        layout->flex_shrinks += child->shrink;

        layout->flex_dim -= child_size
            + (CHILD_MARGIN(child, top, left)
                    + CHILD_MARGIN(child, bottom, right));

        relative_children_count++;

        if (child_size > 0 && child->grow > 0) {
            layout->extra_flex_dim += child_size;
        }
    }

    // Layout remaining items in wrap mode, or everything otherwise.
    layout_items(item, last_layout_child, item->children.count,
            relative_children_count, layout);

    // In wrap mode we may need to tweak the position of each line according to
    // the align_content property as well as the cross-axis size of items that
    // haven't been set yet.
    if (layout->need_lines && layout->lines_count > 0) {
        float pos = 0;
        float spacing = 0;
        float flex_dim = layout->align_dim - layout->lines_sizes;
        if (flex_dim > 0) {
            assert(layout_align(item->align_content, flex_dim,
                        layout->lines_count, &pos, &spacing, true)
                    && "incorrect align_content");
        }

        float old_pos = 0;
        if (layout->reverse2) {
            pos = layout->align_dim - pos;
            old_pos = layout->align_dim;
        }

        for (unsigned int i = 0; i < layout->lines_count; i++) {
            struct flex_layout_line *line = &layout->lines[i];

            if (layout->reverse2) {
                pos -= line->size;
                pos -= spacing;
                old_pos -= line->size;
            }

            // Re-position the children of this line, honoring any child
            // alignment previously set within the line.
            for (unsigned int j = line->child_begin; j < line->child_end;
                    j++) {
                struct flex_item *child = LAYOUT_CHILD_AT(item, j);
                if (child->position == FLEX_POSITION_ABSOLUTE) {
                    // Should not be re-positioned.
                    continue;
                }
                if (isnan(CHILD_SIZE2(child))) {
                    // If the child's cross axis size hasn't been set it, it
                    // defaults to the line size.
                    CHILD_SIZE2(child) = line->size
                        + (item->align_content == FLEX_ALIGN_STRETCH
                                ? spacing : 0);
                }
                CHILD_POS2(child) = pos + (CHILD_POS2(child) - old_pos);
            }

            if (!layout->reverse2) {
                pos += line->size;
                pos += spacing;
                old_pos += line->size;
            }
        }
    }

    layout_cleanup(layout);
}

#undef CHILD_MARGIN
#undef CHILD_POS
#undef CHILD_POS2
#undef CHILD_SIZE
#undef CHILD_SIZE2
#undef _LAYOUT_FRAME
#undef LAYOUT_CHILD_AT
#undef LAYOUT_RESET

DLLEXPORT
void
flex_layout(struct flex_item *item)
{
    assert(item->parent == NULL);
    assert(!isnan(item->width));
    assert(!isnan(item->height));
    assert(item->self_sizing == NULL);

    layout_item(item, item->width, item->height);
}
