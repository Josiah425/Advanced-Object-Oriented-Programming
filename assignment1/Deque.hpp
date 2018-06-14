#ifndef _DEQUE_H_
#define _DEQUE_H_
#define Deque_DEFINE(t)											\
	struct Deque_##t##_Iterator;									\
	typedef struct Deque_##t{									\
		t* arr;											\
		size_t length;										\
		size_t max_length;									\
		char type_name[6 + sizeof(#t)] = "Deque_" #t;						\
		int head_index;										\
		int tail_index;										\
		Deque_##t##_Iterator * iter_head;							\
		Deque_##t##_Iterator * iter_tail;							\
		size_t (*size)(Deque_##t *);								\
		bool (*empty)(Deque_##t *);								\
		void (*push_back)(Deque_##t *, t);							\
		void (*push_front)(Deque_##t *, t);							\
		void (*pop_back)(Deque_##t *);								\
		void (*pop_front)(Deque_##t *);								\
		void (*reallocate)(Deque_##t *);							\
		void (*clear)(Deque_##t *);								\
		t& (*front)(Deque_##t *);								\
		t& (*back)(Deque_##t *);								\
		Deque_##t##_Iterator& (*begin)(Deque_##t *);						\
		Deque_##t##_Iterator& (*end)(Deque_##t *);						\
		t& (*at)(Deque_##t *, int i);								\
		bool (*cmp)(const t &, const t &);							\
		void (*dtor)(Deque_##t *);								\
		void (*sort)(Deque_##t *, Deque_##t##_Iterator, Deque_##t##_Iterator);			\
	} Deque_##t;											\
	typedef struct Deque_##t##_Iterator{								\
		t* curr;										\
		t* queue_begin;										\
		t* queue_end;										\
		void (*inc)(Deque_##t##_Iterator * self);						\
		void (*dec)(Deque_##t##_Iterator * self);						\
		t& (*deref)(Deque_##t##_Iterator *self);						\
		bool (*Deque_##t##_Iterator_equal)(Deque_##t##_Iterator self, Deque_##t##_Iterator other);\
	} Deque_##t##_Iterator;										\
	size_t size(Deque_##t * self){									\
		return self->length;									\
	}												\
	bool empty(Deque_##t * self){									\
		return size(self) == 0;									\
	}												\
	bool Deque_##t##_Iterator_equal(Deque_##t##_Iterator it, Deque_##t##_Iterator otherIt);		\
	t& deref(Deque_##t##_Iterator *);								\
	void inc(Deque_##t##_Iterator *);								\
	void dec(Deque_##t##_Iterator *);								\
	Deque_##t##_Iterator& begin(Deque_##t *);							\
	Deque_##t##_Iterator& end(Deque_##t *);								\
	void reallocate(Deque_##t *self) {								\
		self->max_length *= 2;									\
		t * double_arr = (t*)malloc(sizeof(t) * self->max_length);				\
		struct Deque_##t##_Iterator * double_begin = 						\
			(Deque_##t##_Iterator *) malloc(sizeof(Deque_##t##_Iterator));			\
		struct Deque_##t##_Iterator * double_end =						\
			(Deque_##t##_Iterator *) malloc(sizeof(Deque_##t##_Iterator));			\
		double_begin->curr = double_arr;							\
		double_end->curr = double_arr + self->length;						\
		double_begin->queue_begin = double_end->queue_begin = double_arr;			\
		double_begin->queue_end = double_end->queue_end = double_arr + self->max_length-1;	\
		int temp = 0;										\
		for (Deque_##t##_Iterator it = self->begin(self);					\
         		!Deque_##t##_Iterator_equal(it, self->end(self)); it.inc(&it)) {		\
            			double_arr[temp] = *it.curr;						\
				temp++;									\
	        	}										\
		self->dtor(self);									\
		self->arr = double_arr;									\
		self->iter_head = double_begin;								\
		self->iter_tail = double_end;								\
		self->iter_head->queue_begin = double_arr;						\
		self->iter_tail->queue_begin = double_arr;						\
		self->iter_head->queue_end = double_arr + self->max_length-1;				\
		self->iter_tail->queue_end = double_arr + self->max_length-1;				\
		self->head_index = 0;									\
		self->tail_index = self->length;							\
		self->iter_head->curr = double_arr;							\
		self->iter_tail->curr = double_arr + self->length;					\
		self->iter_head->inc = self->iter_tail->inc = &inc;					\
		self->iter_head->dec = self->iter_tail->dec = &dec;					\
		self->iter_head->deref = self->iter_tail->deref = &deref;				\
		self->iter_head->Deque_##t##_Iterator_equal = self->iter_tail->Deque_##t##_Iterator_equal = \
			&Deque_##t##_Iterator_equal;							\
	} 												\
	void push_front(Deque_##t * self, t val){							\
		if(self->length == self->max_length-1){							\
			reallocate(self);								\
		}											\
		self->length++;										\
		int index = self->head_index-1 % self->max_length;					\
		if(index < 0) index += self->max_length;						\
		self->head_index = index;								\
		self->arr[self->head_index] = val;							\
		self->iter_head->curr = self->arr + index;						\
	}												\
	void push_back(Deque_##t * self, t val){							\
		if(self->length == self->max_length-1){							\
			reallocate(self);								\
		}											\
		self->length++;										\
		self->arr[self->tail_index] = val;							\
		self->iter_tail->curr = self->arr + ((self->tail_index+1) % self->max_length);		\
		self->tail_index = (self->tail_index+1) % self->max_length;				\
	}												\
	void pop_front(Deque_##t * self){								\
		if(self->length == 0) return;								\
		self->length--;										\
		self->head_index = (self->head_index+1) % self->max_length;				\
		self->iter_head->curr = self->arr + (self->head_index % self->max_length);		\
	}												\
	void pop_back(Deque_##t * self){								\
		if(self->length == 0) return;								\
		self->length--;										\
		int index = self->tail_index-1 % self->max_length;					\
		if(index < 0) index += self->max_length;						\
		self->iter_tail->curr = self->arr + index;						\
		self->tail_index = index % self->max_length;						\
	}												\
	t& back(Deque_##t * self){									\
		int index = self->tail_index-1 % self->max_length;					\
		if(index < 0) index += self->max_length;						\
		return self->arr[index];								\
	}												\
	t& front(Deque_##t * self){									\
		return self->arr[(self->head_index)%self->max_length];					\
	}												\
	Deque_##t##_Iterator& begin(Deque_##t * self){							\
		return *self->iter_head;								\
	}												\
	Deque_##t##_Iterator& end(Deque_##t * self){							\
		return *self->iter_tail;								\
	}												\
	bool Deque_##t##_Iterator_equal(Deque_##t##_Iterator it, Deque_##t##_Iterator otherIt){		\
		return it.curr == otherIt.curr;								\
	}												\
	void inc(Deque_##t##_Iterator * it){								\
		if(it->curr == it->queue_end) it->curr = it->queue_begin;				\
		else it->curr++;									\
	}												\
	void dec(Deque_##t##_Iterator * it){								\
		if(it->curr == it->queue_begin) it->curr = it->queue_end;				\
		else it->curr--;									\
	}												\
	t& deref(Deque_##t##_Iterator * it){								\
		return *it->curr;									\
	}												\
	t& at(Deque_##t * self, int i){									\
		if(&self->front(self) + i > self->end(self).queue_end){					\
			i -= self->iter_tail->queue_end - &self->front(self)+1;				\
			return *(self->arr + i);							\
		}											\
		return *(&self->front(self) + i);							\
	}												\
	void dtor(Deque_##t * self){									\
		free(self->arr);									\
		free(self->iter_head);									\
		free(self->iter_tail);									\
	}												\
	void clear(Deque_##t * self);									\
	int Deque_##t##_cmp_func(const void * a, const void * b, void * self) {				\
		Deque_##t * tmp = (Deque_##t *)self;							\
		if(tmp->cmp(*((t*) a), *((t*)b))) return -1;						\
		else if(tmp->cmp(*((t*) b), *((t*)a))) return 1;					\
		return 0;										\
	}												\
	void sort(Deque_##t * self, Deque_##t##_Iterator head, Deque_##t##_Iterator tail){		\
		size_t distance = 0;									\
		for (Deque_##t##_Iterator it = head;							\
         		!Deque_##t##_Iterator_equal(it, tail); it.inc(&it)) {				\
            			distance++;								\
	        	}										\
		t * temp_arr = (t*)malloc(sizeof(t) * distance);					\
		size_t temp = 0;									\
		for (Deque_##t##_Iterator it = head;							\
         		!Deque_##t##_Iterator_equal(it, tail); it.inc(&it)) {				\
            			temp_arr[temp] = *it.curr;						\
				temp++;									\
	        	}										\
		qsort_r((void*)temp_arr, distance, sizeof(t), Deque_##t##_cmp_func, self);		\
		temp = 0;										\
		for(Deque_##t##_Iterator it = head; !Deque_##t##_Iterator_equal(it, tail); it.inc(&it), temp++){\
			*it.curr = temp_arr[temp];							\
		}											\
		free(temp_arr);										\
	}												\
	bool Deque_##t##_equal(Deque_##t deq1, Deque_##t deq2){						\
		if(deq1.size(&deq1) != deq2.size(&deq2)) return false;					\
		for(size_t i = 0; i < deq1.size(&deq1); i++){						\
			if(deq1.cmp(deq1.at(&deq1, i), deq2.at(&deq2, i)) ||				\
			 deq2.cmp(deq2.at(&deq2, i), deq1.at(&deq1, i))) return false;			\
		}											\
		return true;										\
	}												\
	void Deque_##t##_ctor(Deque_##t * self, bool (*less)(const t &, const t &)){			\
		self->length = 0;									\
		self->max_length = 20;									\
		self->arr = (t*) malloc(sizeof(t) * self->max_length);					\
		self->head_index = 0;									\
		self->tail_index = 0;									\
		self->iter_head = (Deque_##t##_Iterator *) malloc(sizeof(Deque_##t##_Iterator));	\
		self->iter_tail = (Deque_##t##_Iterator *) malloc(sizeof(Deque_##t##_Iterator));	\
		self->iter_head->curr = self->arr;							\
		self->iter_tail->curr = self->arr;							\
		self->iter_head->inc = self->iter_tail->inc = &inc;					\
		self->iter_head->dec = self->iter_tail->dec = &dec;					\
		self->iter_head->deref = self->iter_tail->deref = &deref;				\
		self->iter_head->Deque_##t##_Iterator_equal = self->iter_tail->Deque_##t##_Iterator_equal = &Deque_##t##_Iterator_equal;\
		self->iter_head->queue_begin = self->arr;						\
		self->iter_head->queue_end = self->arr + self->max_length-1;				\
		self->iter_tail->queue_begin = self->arr;						\
		self->iter_tail->queue_end = self->arr + self->max_length-1;				\
		self->cmp = less;									\
		self->size = &size;									\
		self->empty = &empty;									\
		self->push_back = &push_back;								\
		self->push_front = &push_front;								\
		self->pop_back = &pop_back;								\
		self->pop_front = &pop_front;								\
		self->front = &front;									\
		self->back = &back;									\
		self->begin = &begin;									\
		self->end = &end;									\
		self->at = &at;										\
		self->reallocate = &reallocate;								\
		self->clear = &clear;									\
		self->dtor = &dtor;									\
		self->sort = &sort;									\
	}												\
	void clear(Deque_##t * self){									\
		self->dtor(self);									\
		Deque_##t##_ctor(self, self->cmp);								\
													\
	}																
#endif
