/*
 *          File: queue2.c
 *        Author: Robert I. Pitts <rip@cs.bu.edu>
 * Last Modified: March 9, 2000
 *         Topic: Queue - Array Implementation
 * ----------------------------------------------------------------
 *
 * Implementation with a array that is grown when the queue is
 * full and another element is added.  A pointer and dynamic
 * allocation is used to implement such a dynamic array.
 */

#include <stdio.h>
#include <stdlib.h>                  
#include <po_hi_queue.h>       

/*
 * Constants
 * ---------
 * INIT_QUEUE_SIZE = Initial number of items queue can hold.
 */

#define INIT_QUEUE_SIZE  5

/*
 * struct queueCDT gives the implementation of a queue.
 * It holds the information that we need for each queue.
 */
typedef struct queueCDT {
  queueElementT *contents;
  int curSize;
  int front;
  int count;
} queueCDT;

queueADT QueueCreate(void)
{
  queueADT queue;

  queue = (queueADT)malloc(sizeof(queueCDT));

  if (queue == NULL) {
    fprintf(stderr, "Insufficient Memory for new Queue.\n");
    exit(ERROR_MEMORY);  /* Exit program, returning error code. */
  }

  queue->contents = malloc(sizeof(queueElementT) * INIT_QUEUE_SIZE);

  if (queue->contents == NULL) {
    fprintf(stderr, "Insufficient Memory for new Queue.\n");
    exit(ERROR_MEMORY);  /* Exit program, returning error code. */
  }

  queue->curSize = INIT_QUEUE_SIZE;
  queue->front = 0;
  queue->count = 0;

  return queue;
}

void QueueDestroy(queueADT queue)
{
  free(queue->contents);
  free(queue);
}

void QueueEnter(queueADT queue, queueElementT element)
{
  int newElementIndex;

  if (queue->count >= queue->curSize) {
    queueElementT *newContents;
    int i;

    /* Make an array twice as big. */
    newContents = malloc(sizeof(queueElementT) * queue->curSize * 2);

    if (newContents == NULL) {
      fprintf(stderr, "Insufficient Memory for Queue to grow.\n");
      exit(ERROR_MEMORY);  /* Exit program, returning error code. */
    }

    /* Copy the elements to new array. */

    for (i = 0; i < queue->count; i++) {
      int oldIndex;
      oldIndex = (queue->front + i) % queue->curSize;
      newContents[i] = queue->contents[oldIndex];
    }

    /* printf("Was size= %d, front= %d\n", queue->curSize, queue->front); */

    free(queue->contents);
    queue->contents = newContents;
    queue->front = 0;
    queue->curSize *= 2;

    /* printf("Now size= %d, front= %d\n", queue->curSize, queue->front); */
  }

  /*
   * Calculate index at which to put
   * next element.
   */
  newElementIndex = (queue->front + queue->count)
                    % queue->curSize;
  queue->contents[newElementIndex] = element;

  queue->count++;
}

queueElementT QueueDelete(queueADT queue)
{
  queueElementT oldElement;

  if (queue->count <= 0) {
    fprintf(stderr, "QueueDelete on Empty Queue.\n");
    exit(ERROR_QUEUE);  /* Exit program, returning error code. */
  }

  /* Save the element so we can return it. */
  oldElement = queue->contents[queue->front];

  /*
   * Advance the index of the front,
   * making sure it wraps around the
   * array properly.
   */
  queue->front++;
  queue->front %= queue->curSize;

  queue->count--;

  return oldElement;
}

int QueueIsEmpty(queueADT queue)
{
  return queue->count <= 0;
}

int QueueIsFull(queueADT queue)
{
  return 0;
}
