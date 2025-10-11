// ChatGPT generated

#if defined Uses_cirbuf_packet_sync || !defined __COMPILING

#define QUEUE_CAPACITY 10000
#define PACKET_SIZE MAX_PACKET_SIZE

typedef struct cirbuf_packet_sync // conveys synchronized producer/consumer
{
	char data[ QUEUE_CAPACITY ][ PACKET_SIZE ];
	size_t lengths[ QUEUE_CAPACITY ];
	int head;
	int tail;
	int count;

	pthread_mutex_t lock;
	pthread_cond_t not_empty;
	pthread_cond_t not_full;
} cbuf_packet;

// Initialize the queue
void cbuf_ps_init( cbuf_packet * q );

// Destroy the queue
void cbuf_ps_destroy( cbuf_packet * q );

// Blocking push: waits if full
void cbuf_ps_push( cbuf_packet * q , const buffer buf , size_t len );

// Blocking pop: waits if empty
void cbuf_ps_pop( cbuf_packet * q , buffer out_buf , size_t * out_len );

// Non-blocking: returns 1 if data exists, 0 otherwise
int cbuf_ps_peek_available( cbuf_packet * q );

// Non-blocking pop: returns 0 if successful, -1 if empty
int cbuf_ps_try_pop( cbuf_packet * q , buffer out_buf , size_t * out_len );

#endif

