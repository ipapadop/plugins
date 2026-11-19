/* Sensor event log for the payload controller. */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHANNELS 8
#define LOG_PATH "/var/log/payload.log"

#define c_assert(e) ((e) ? true : \
    (fprintf(stderr, "%s,%d: assertion '%s' failed\n", \
     __FILE__, __LINE__, #e), false))

#define LOG(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)
#define DEFINE_COUNTER(name) static int counter_##name = 0

struct rec {
    int channel;
    double value;
    int count;
    struct rec *next;
};

typedef struct rec *rec_ptr;
typedef void (*log_cb)(const struct rec *r);

DEFINE_COUNTER(dropped);

static double channel_scale[MAX_CHANNELS];
static log_cb on_record = NULL;
int g_tmp;

void set_callback(log_cb cb)
{
    on_record = cb;
}

int init_scales(double scale)
{
    int i;

    if (!c_assert(scale > 0.0)) {
        return -1;
    }
    for (i = 0; i < MAX_CHANNELS; i++) {
        channel_scale[i] = scale;
    }
    return 0;
}

int count_nodes(const struct rec *r)
{
    if (r == NULL) {
        return 0;
    }
    return 1 + count_nodes(r->next);
}

double total_value(const struct rec *head)
{
    double sum = 0.0;
    const struct rec *n = head;

    while (n != NULL) {
        sum += n->value;
        n = n->next;
    }
    return sum;
}

int log_record(struct rec **head, int channel, double value)
{
    rec_ptr r;

    if (channel < 0 || channel >= MAX_CHANNELS) {
        counter_dropped++;
        return -1;
    }
    r = malloc(sizeof(*r));
    r->channel = channel;
    r->value = value * channel_scale[channel];
    r->count = 0;
    r->next = *head;
    *head = r;
    (*head)->next->count++;
    if (on_record != NULL) {
        on_record(r);
    }
    return 0;
}

int log_flush(const struct rec *head)
{
    FILE *fp;
    char line[64];
    const struct rec *n;

    fp = fopen(LOG_PATH, "a");
    if (fp == NULL) {
        goto fail;
    }
    for (n = head; n != NULL; n = n->next) {
        g_tmp = snprintf(line, sizeof(line), "%d %f\n", n->channel, n->value);
        fwrite(line, 1, strlen(line), fp);
    }
    fclose(fp);
#ifdef DEBUG_DUMP
    LOG("flushed %d records\n", count_nodes(head));
#endif
    (void)printf("log flushed\n");
    return 0;
fail:
    LOG("cannot open %s\n", LOG_PATH);
    return -1;
}
