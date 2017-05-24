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

static int
child_order_cmp(void *ctx, const void *e1, const void *e2)
{
    struct flex_item *item = (struct flex_item *)ctx;
    int order1 = item->children.ary[*(int *)e1]->order;
    int order2 = item->children.ary[*(int *)e2]->order;
    return order1 > order2 ? 1 : (order1 < order2 ? -1 : 0);
}

static void
layout_item(struct flex_item *item, float width, float height)
{
    if (item->children.count == 0) {
        return;
    }

    width -= item->padding_left + item->padding_right;
    height -= item->padding_top + item->padding_bottom;
    assert(width >= 0);
    assert(height >= 0);

    bool reverse = false;
    bool vertical = true;
    float size_dim = 0;
    float align_dim = 0;
    unsigned int frame_pos_i = 0;
    unsigned int frame_pos2_i = 0;
    unsigned int frame_size_i = 0;
    unsigned int frame_size2_i = 0;
    switch (item->direction) {
        case FLEX_DIRECTION_ROW_REVERSE:
            reverse = true;
        case FLEX_DIRECTION_ROW:
            vertical = false;
            size_dim = width;
            align_dim = height;
            frame_pos_i = 0;
            frame_pos2_i = 1;
            frame_size_i = 2;
            frame_size2_i = 3;
            break;

        case FLEX_DIRECTION_COLUMN_REVERSE:
            reverse = true;
        case FLEX_DIRECTION_COLUMN:
            size_dim = height;
            align_dim = width;
            frame_pos_i = 1;
            frame_pos2_i = 0;
            frame_size_i = 3;
            frame_size2_i = 2;
            break;

        default:
            assert(false && "incorrect direction");
    }

    float flex_dim = size_dim;
    int flex_grows = 0;
    int flex_shrinks = 0;
    int *ordered_indices = NULL;
    bool wrap = item->wrap != FLEX_WRAP_NOWRAP;
    for (int i = 0; i < item->children.count; i++) {
        struct flex_item *child = item->children.ary[i];

        child->frame[0] = 0;
        child->frame[1] = 0;
        child->frame[2] = isnan(child->width) ? 0 : child->width;
        child->frame[3] = isnan(child->height) ? 0 : child->height;

        if (child->basis > 0) {
            child->frame[frame_size_i] = child->basis;
        }

        if (!wrap) {
            flex_grows += child->grow;
            flex_shrinks += child->shrink;

            flex_dim -= child->frame[frame_size_i]
                + (vertical
                        ? child->margin_top + child->margin_bottom
                        : child->margin_left + child->margin_right);
        }

        if (child->order != 0) {
            if (ordered_indices == NULL) {
                ordered_indices =
                    (int *)malloc(sizeof(int) * item->children.count);
                assert(ordered_indices != NULL);
            }
        }
    }

    if (ordered_indices != NULL) {
        for (int i = 0; i < item->children.count; i++) {
            ordered_indices[i] = i;
        }
        qsort_r(ordered_indices, item->children.count, sizeof(int), item,
                child_order_cmp);
    }

#define INIT_POS(pos, spacing, dim, children_count) \
    do { \
        float _dim = dim; \
        int _children_count = children_count; \
        pos = 0; \
        spacing = 0; \
        switch (item->justify_content) { \
            case FLEX_ALIGN_FLEX_START: \
                break; \
            \
            case FLEX_ALIGN_FLEX_END: \
                pos = _dim; \
                break; \
            \
            case FLEX_ALIGN_CENTER: \
                pos = _dim / 2; \
                break; \
            \
            case FLEX_ALIGN_SPACE_BETWEEN: \
                if (_children_count > 0) { \
                    spacing = _dim / (_children_count - 1); \
                } \
                break; \
            \
            case FLEX_ALIGN_SPACE_AROUND: \
                if (_children_count > 0) { \
                    spacing = _dim / _children_count; \
                    pos = spacing / 2; \
                } \
                break; \
            \
            case FLEX_ALIGN_SPACE_EVENLY: \
                if (_children_count > 0) { \
                    spacing = _dim / (_children_count + 1); \
                    pos = spacing; \
                } \
                break; \
            \
            default: \
                assert(false && "incorrect justify_content"); \
        } \
        if (reverse) { \
            pos = size_dim - pos; \
        } \
    } \
    while (0)

#define CHILD_AT(idx) \
    item->children.ary[ordered_indices != NULL ? ordered_indices[idx] : idx]

#define UPDATE_POS(child, pos, spacing, pos2) \
    do { \
        struct flex_item *_child = child; \
        float _child_size = _child->frame[frame_size_i]; \
        if (reverse) { \
            pos -= vertical ? _child->margin_bottom : _child->margin_right; \
            pos -= _child_size; \
            _child->frame[frame_pos_i] = pos; \
            pos -= spacing; \
            pos -= vertical ? _child->margin_top : _child->margin_left; \
        } \
        else { \
            pos += vertical ? _child->margin_top : _child->margin_left; \
            _child->frame[frame_pos_i] = pos; \
            pos += _child_size; \
            pos += spacing; \
            pos += vertical ? _child->margin_bottom : _child->margin_right; \
        } \
        _child->frame[frame_pos2_i] = pos2; \
    } \
    while (0)

    float pos2 = vertical ? item->padding_left : item->padding_top;
    if (wrap) {
        float wrap_dim = size_dim;
        int last_wrap_child = 0;

        for (int i = 0; i < item->children.count; i++) {
            struct flex_item *child = CHILD_AT(i);

            float child_size = child->frame[frame_size_i];
            if (wrap_dim < child_size) {
again:;
                float pos = 0;
                float spacing = 0;
                INIT_POS(pos, spacing, wrap_dim, i - last_wrap_child);

                for (int j = last_wrap_child; j < i; j++) {
                    UPDATE_POS(CHILD_AT(j), pos, spacing, pos2);
                }

                pos2 += child->frame[frame_size2_i];
                wrap_dim = size_dim;
                last_wrap_child = i;
            }
            wrap_dim -= child_size;

            layout_item(child, child->frame[2], child->frame[3]);

            if (i == item->children.count - 1) {
                i++;
                goto again;
            }
        }
    }
    else {
        float pos = 0;
        float spacing = 0;
        if (flex_grows == 0) {
            INIT_POS(pos, spacing, flex_dim, item->children.count);
        }

        if (reverse) {
            pos -= vertical ? item->padding_bottom : item->padding_right;
        }
        else {
            pos += vertical ? item->padding_top : item->padding_left;
        }

        for (int i = 0; i < item->children.count; i++) {
            struct flex_item *child = CHILD_AT(i);

            float flex_size = 0;
            if (flex_dim > 0) {
                if (child->grow != 0) {
                    flex_size = (flex_dim / flex_grows) * child->grow;
                }
            }
            else if (flex_dim < 0) {
                if (child->shrink != 0) {
                    flex_size = (flex_dim / flex_shrinks) * child->shrink;
                }
            }
            child->frame[frame_size_i] += flex_size;

            flex_align align = child->align_self;
            if (align == FLEX_ALIGN_AUTO) {
                align = item->align_items;
            }
            float align_pos = pos2 + 0;
            switch (align) {
                case FLEX_ALIGN_FLEX_END:
                    align_pos = align_dim - child->frame[frame_size2_i]
                        - (vertical
                                ? child->margin_right : child->margin_bottom)
                        + (vertical
                                ? item->padding_left : item->padding_top);
                    break;

                case FLEX_ALIGN_CENTER:
                    align_pos = (align_dim / 2.0)
                        - (child->frame[frame_size2_i] / 2.0)
                        + (vertical
                                ? child->margin_left : child->margin_top)
                        - (vertical
                                ? child->margin_right : child->margin_bottom);
                    break;

                case FLEX_ALIGN_STRETCH:
                    if (child->frame[frame_size2_i] == 0) {
                        child->frame[frame_size2_i] = align_dim
                            - (vertical
                                    ? child->margin_left + child->margin_right
                                    : child->margin_top + child->margin_bottom);
                    }
                    // fall through

                case FLEX_ALIGN_FLEX_START:
                    align_pos += vertical
                        ? child->margin_left : child->margin_top;
                    break;

                default:
                    assert(false && "incorrect align_self");
            }

            UPDATE_POS(child, pos, spacing, align_pos);

            layout_item(child, child->frame[2], child->frame[3]);
        }
    }

#undef INIT_POS
#undef UPDATE_POS
#undef CHILD_AT

    if (ordered_indices != NULL) {
        free(ordered_indices);
    }
}

void
flex_layout(struct flex_item *item)
{
    assert(item->parent == NULL);
    assert(!isnan(item->width));
    assert(!isnan(item->height));

    layout_item(item, item->width, item->height);
}
