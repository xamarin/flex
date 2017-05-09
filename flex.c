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
#define FLEX_ATTRIBUTE(name, type) type name;
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
#define FLEX_ATTRIBUTE(name, type) \
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

    item->width = NAN;
    item->height = NAN;

    item->left = NAN;
    item->right = NAN;
    item->top = NAN;
    item->bottom = NAN;

    item->padding_left = NAN;
    item->padding_right = NAN;
    item->padding_top = NAN;
    item->padding_bottom = NAN;

    item->margin_left = NAN;
    item->margin_right = NAN;
    item->margin_top = NAN;
    item->margin_bottom = NAN;

    item->justify_content = FLEX_ALIGN_FLEX_START;
    item->align_content = FLEX_ALIGN_AUTO;
    item->align_items = FLEX_ALIGN_FLEX_START;
    item->align_self = FLEX_ALIGN_AUTO;

    item->position = FLEX_POSITION_RELATIVE;
    item->direction = FLEX_DIRECTION_COLUMN;
    item->wrap = FLEX_WRAP_NOWRAP;

    item->grow = 0;
    item->shrink = 1;
    item->order = 0;
    item->basis = 0;

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

void
flex_layout(struct flex_item *item)
{
    assert(item->parent == NULL);
    assert(!isnan(item->width));
    assert(!isnan(item->height));

    if (item->children.count == 0) {
        return;
    }

    bool reverse = false;
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
            size_dim = item->width;
            align_dim = item->height;
            frame_pos_i = 0;
            frame_pos2_i = 1;
            frame_size_i = 2;
            frame_size2_i = 3;
            break;

        case FLEX_DIRECTION_COLUMN_REVERSE:
            reverse = true;
        case FLEX_DIRECTION_COLUMN:
            size_dim = item->height;
            align_dim = item->width;
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
    float pos_orig = reverse ? size_dim : 0;
    float pos = pos_orig;
    float pos2 = 0;
    int *ordered_indices = NULL;
    bool wrap = item->wrap != FLEX_WRAP_NOWRAP;
    for (int i = 0; i < item->children.count; i++) {
        struct flex_item *child = item->children.ary[i];

        child->frame[0] = 0;
        child->frame[1] = 0;
        child->frame[2] = isnan(child->width) ? 0 : child->width;
        child->frame[3] = isnan(child->height) ? 0 : child->height;

        flex_grows += child->grow;
        flex_shrinks += child->shrink;

        if (child->basis > 0) {
            child->frame[frame_size_i] = child->basis;
        }

        flex_dim -= child->frame[frame_size_i];

        if (child->order != 0) {
            if (ordered_indices == NULL) {
                ordered_indices =
                    (int *)malloc(sizeof(int) * item->children.count);
                assert(ordered_indices != NULL);
            }
        }
    }

    float spacing = 0;
    if (flex_grows == 0) {
        float new_pos = 0;
        switch (item->justify_content) {
            case FLEX_ALIGN_FLEX_START:
                break;

            case FLEX_ALIGN_FLEX_END:
                new_pos = flex_dim;
                break;

            case FLEX_ALIGN_CENTER:
                new_pos = flex_dim / 2;
                break;

            case FLEX_ALIGN_SPACE_BETWEEN:
                if (item->children.count > 0) {
                    spacing = flex_dim / (item->children.count - 1);
                }
                break;

            case FLEX_ALIGN_SPACE_AROUND:
                if (item->children.count > 0) {
                    spacing = flex_dim / item->children.count;
                    new_pos = spacing / 2;
                }
                break;

            default:
                assert(false && "incorrect justify_content");
        }
        pos = pos_orig = reverse ? size_dim - new_pos : new_pos;
    }

    if (ordered_indices != NULL) {
        for (int i = 0; i < item->children.count; i++) {
            ordered_indices[i] = i;
        }
        qsort_r(ordered_indices, item->children.count, sizeof(int), item,
                child_order_cmp);
    }

    float wrap_dim = size_dim;
    for (int i = 0; i < item->children.count; i++) {
        int order_i = ordered_indices != NULL ? ordered_indices[i] : i;
        struct flex_item *child = item->children.ary[order_i];

        if (!wrap) {
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
        }
        else {
            float child_size = child->frame[frame_size_i];
            if (wrap_dim >= child_size) {
                wrap_dim -= child_size;
            }
            else {
                wrap_dim = size_dim;
                pos = pos_orig;
                pos2 += child->frame[frame_size2_i];
            }
        }

        float new_pos = child->frame[frame_size_i] + spacing;
        if (reverse) {
            pos -= new_pos;
            child->frame[frame_pos_i] = pos;
        }
        else {
            child->frame[frame_pos_i] = pos;
            pos += new_pos;
        }

        flex_align align = child->align_self;
        if (align == FLEX_ALIGN_AUTO) {
            align = item->align_items;
        }
        float align_pos = pos2 + 0;
        switch (align) {
            case FLEX_ALIGN_FLEX_START:
                break;

            case FLEX_ALIGN_FLEX_END:
                align_pos = align_dim - child->frame[frame_size2_i];
                break;

            case FLEX_ALIGN_CENTER:
                align_pos = ((align_dim / 2.0)
                        - (child->frame[frame_size2_i] / 2.0));
                break;

            case FLEX_ALIGN_STRETCH:
                if (child->frame[frame_size2_i] == 0) {
                    align_pos = 0;
                    child->frame[frame_size2_i] = align_dim;
                }
                break;

            default:
                assert(false && "incorrect align_self");
        }
        child->frame[frame_pos2_i] = align_pos;

#if DEBUG_PRINT_FRAMES
        printf("child %d: %f %f %f %f\n", i, child->frame[0], child->frame[1],
                child->frame[2], child->frame[3]);
#endif
    }

    if (ordered_indices != NULL) {
        free(ordered_indices);
    }
}
