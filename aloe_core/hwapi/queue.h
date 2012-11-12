#ifndef QUEUE_H_
#define QUEUE_H_

#define MAX_QUEUE_SZ	64

typedef struct {
	int count;
	int front;
	int rear;
	void *items[MAX_QUEUE_SZ];
}queue_t;


void queue_init(queue_t *q);
int queue_is_empty(queue_t *q);
int queue_is_full(queue_t *q);
int queue_put(queue_t *q, void *value);
void *queue_get(queue_t *q);
#endif
