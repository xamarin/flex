#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define DEBUG_PRINT_FRAMES      0

#if DEBUG_PRINT_FRAMES
# include <stdio.h>
#endif

#include "flex.h"

struct flex_item {
#undef FLEX_ATTRIBUTE
#define FLEX_ATTRIBUTE(name, type, def) type name;
#include "flex.h"

    float frame[4];

    struct flex_item *parent;
    struct {
	unsigned int cap;
	unsigned int count;
	struct flex_item **ary;
    } children;
};

#undef FLEX_ATTRIBUTE
#define FLEX_ATTRIBUTE(name, type, def) \
    type flex_item_get_##name(struct flex_item *item) { \
        return item->name; \
    } \
    void flex_item_set_##name(struct flex_item *item, type value) { \
        item->name = value; \
    }
#include "flex.h"

struct flex_item *
flex_item_new(void)
{
    struct flex_item *item =
        (struct flex_item *)malloc(sizeof(struct flex_item));
    assert(item != NULL);

#undef FLEX_ATTRIBUTE
#define FLEX_ATTRIBUTE(name, type, def) item->name = def;
#include "flex.h"

    memset(item->frame, 0, sizeof item->frame);

    item->parent = NULL;
    item->children.cap = 0;
    item->children.count = 0;
    item->children.ary = NULL;

    return item;
}

void
flex_item_free(struct flex_item *item)
{
    assert(item->parent == NULL);

    if (item->children.cap > 0) {
        assert(item->children.ary != NULL);

        for (int i = 0; i < item->children.count; i++) {
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
child_set(struct flex_item *item, struct flex_item *child, int index)
{
    item->children.ary[index] = child;
    item->children.count++;
    child->parent = item;
}

void
flex_item_add(struct flex_item *item, struct flex_item *child)
{
    grow_if_needed(item);
    child_set(item, child, item->children.count);
}

void
flex_item_insert(struct flex_item *item, unsigned int index,
    struct flex_item *child)
{
    assert(index < item->children.count);

    grow_if_needed(item);
    for (int i = index; i < item->children.count; i++) {
        item->children.ary[i + 1] = item->children.ary[i];
    }
    child_set(item, child, index);
}

struct flex_item *
flex_item_delete(struct flex_item *item, unsigned int index)
{
    assert(index < item->children.count);
    assert(item->children.count > 0);

    struct flex_item *child = item->children.ary[index];

    for (int i = index; i < item->children.count - 1; i++) {
        item->children.ary[i] = item->children.ary[i + 1];
    }
    item->children.count--;

    child->parent = NULL;
    return child;
}

unsigned int
flex_item_count(struct flex_item *item)
{
    return item->children.count;
}

struct flex_item *
flex_item_child(struct flex_item *item, unsigned int index)
{
    assert(index < item->children.count);

    return item->children.ary[index];
}

struct flex_item *
flex_item_parent(struct flex_item *item)
{
    return item->parent;
}

#define FRAME_GETTER(name, index) \
    float flex_item_get_frame_##name(struct flex_item *item) { \
        return item->frame[index]; \
    }

FRAME_GETTER(x, 0)
FRAME_GETTER(y, 1)
FRAME_GETTER(width, 2)
FRAME_GETTER(height, 3)

#undef FRAME_GETTER

struct flex_layout {
    // Set during init.
    bool reverse;
    bool vertical;
    float size_dim;
    float align_dim;
    unsigned int frame_pos_i;
    unsigned int frame_pos2_i;
    unsigned int frame_size_i;
    unsigned int frame_size2_i;
    int *ordered_indices;

    // Set for each line layout.
    float flex_dim;
    int flex_grows;
    int flex_shrinks;
    float pos2;
};

static int
child_order_cmp(void *ctx, const void *e1, const void *e2)
{
    struct flex_item *item = (struct flex_item *)ctx;
    int order1 = item->children.ary[*(int *)e1]->order;
    int order2 = item->children.ary[*(int *)e2]->order;
    return order1 > order2 ? 1 : (order1 < order2 ? -1 : 0);
}

static void
layout_init(struct flex_item *item, float width, float height,
        struct flex_layout *layout)
{
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

    // TODO: order indices only when necessary
    layout->ordered_indices =
        (int *)malloc(sizeof(int) * item->children.count);
    assert(layout->ordered_indices != NULL);
    for (int i = 0; i < item->children.count; i++) {
        layout->ordered_indices[i] = i;
    }
    qsort_r(layout->ordered_indices, item->children.count, sizeof(int), item,
            child_order_cmp);

    layout->flex_dim = 0;
    layout->flex_grows = 0;
    layout->flex_shrinks = 0;

    layout->pos2 = layout->vertical ? item->padding_left : item->padding_top;
}

static void
layout_cleanup(struct flex_layout *layout)
{
    if (layout->ordered_indices != NULL) {
        free(layout->ordered_indices);
        layout->ordered_indices = NULL;
    }
}

#define LAYOUT_RESET() \
    do { \
        layout->flex_dim = layout->size_dim; \
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

static void
layout_items(struct flex_item *item, unsigned int child_begin,
        unsigned int child_end, struct flex_layout *layout)
{
    int children_count = child_end - child_begin;
    if (children_count <= 0) {
        return;
    }

    float pos = 0;
    float spacing = 0;
    if (layout->flex_grows == 0) {
        switch (item->justify_content) {
            case FLEX_ALIGN_FLEX_START:
                break;

            case FLEX_ALIGN_FLEX_END:
                pos = layout->flex_dim;
                break;

            case FLEX_ALIGN_CENTER:
                pos = layout->flex_dim / 2;
                break;

            case FLEX_ALIGN_SPACE_BETWEEN:
                if (children_count > 0) {
                    spacing = layout->flex_dim / (children_count - 1);
                }
                break;

            case FLEX_ALIGN_SPACE_AROUND:
                if (children_count > 0) {
                    spacing = layout->flex_dim / children_count;
                    pos = spacing / 2;
                }
                break;

            case FLEX_ALIGN_SPACE_EVENLY:
                if (children_count > 0) {
                    spacing = layout->flex_dim / (children_count + 1);
                    pos = spacing;
                }
                break;

            default:
                assert(false && "incorrect justify_content");
        }
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

    for (int i = child_begin; i < child_end; i++) {
        struct flex_item *child = LAYOUT_CHILD_AT(item, i);

        float flex_size = 0;
        if (layout->flex_dim > 0) {
            if (child->grow != 0) {
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

        flex_align align = child->align_self;
        if (align == FLEX_ALIGN_AUTO) {
            align = item->align_items;
        }
        float align_size = CHILD_SIZE2(child);
        float align_pos = layout->pos2 + 0;
        switch (align) {
            case FLEX_ALIGN_FLEX_END:
                align_pos += layout->align_dim - align_size
                    - CHILD_MARGIN(child, right, bottom);
                break;

            case FLEX_ALIGN_CENTER:
                align_pos += (layout->align_dim / 2.0) - (align_size / 2.0)
                    + (CHILD_MARGIN(child, left, top)
                            - CHILD_MARGIN(child, right, bottom));
                break;

            case FLEX_ALIGN_STRETCH:
                if (align_size == 0) {
                    CHILD_SIZE2(child) = layout->align_dim
                        - (CHILD_MARGIN(child, left, top)
                                + CHILD_MARGIN(child, right, bottom));
                }
                // fall through

            case FLEX_ALIGN_FLEX_START:
                align_pos += CHILD_MARGIN(child, left, top);
                break;

            default:
                assert(false && "incorrect align_self");
        }

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
        CHILD_POS2(child) = align_pos;

        layout_item(child, child->frame[2], child->frame[3]);
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

    bool wrap = item->wrap != FLEX_WRAP_NOWRAP;
    int last_layout_child = 0;
    if (wrap) {
        layout->align_dim = 0;
    }
    for (int i = 0; i < item->children.count; i++) {
        struct flex_item *child = LAYOUT_CHILD_AT(item, i);

        child->frame[0] = 0;
        child->frame[1] = 0;
        child->frame[2] = isnan(child->width) ? 0 : child->width;
        child->frame[3] = isnan(child->height) ? 0 : child->height;

        if (child->basis > 0) {
            CHILD_SIZE(child) = child->basis;
        }

        float child_size = CHILD_SIZE(child);
        if (wrap) {
            if (layout->flex_dim < child_size) {
                layout_items(item, last_layout_child, i, layout);

                LAYOUT_RESET();
                layout->pos2 += layout->align_dim;
                last_layout_child = i;
                layout->align_dim = 0;
            }

            float child_size2 = CHILD_SIZE2(child);
            if (child_size2 > layout->align_dim) {
                layout->align_dim = child_size2;
            }
        }

        layout->flex_grows += child->grow;
        layout->flex_shrinks += child->shrink;

        layout->flex_dim -= child_size
            + (CHILD_MARGIN(child, top, left)
                    + CHILD_MARGIN(child, bottom, right));
    }

    // Layout remaining children in wrap mode, or everything otherwise.
    layout_items(item, last_layout_child, item->children.count, layout);

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

void
flex_layout(struct flex_item *item)
{
    assert(item->parent == NULL);
    assert(!isnan(item->width));
    assert(!isnan(item->height));

    layout_item(item, item->width, item->height);
}
