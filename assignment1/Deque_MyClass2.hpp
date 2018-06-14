#ifndef _DEQUE_H_
#define _DEQUE_H_
#include <algorithm>
#define Deque_DEFINE(t)											\
	struct Deque_##t##_Iterator;									\
	struct Deque_##t{										\
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
		t& (*front)(Deque_##t *);								\
		t& (*back)(Deque_##t *);								\
		Deque_##t##_Iterator& (*begin)(Deque_##t *);						\
		Deque_##t##_Iterator& (*end)(Deque_##t *);						\
		t& (*at)(Deque_##t *, int i);								\
	};												\
	struct Deque_##t##_Iterator{									\
		t* curr;										\
		t* queue_begin;										\
		t* queue_end;										\
		void (*inc)(Deque_##t##_Iterator * self);						\
		void (*dec)(Deque_##t##_Iterator * self);						\
		t& (*deref)(Deque_##t##_Iterator *self);						\
		bool (*Deque_##t##_Iterator_equal)(Deque_##t##_Iterator self, Deque_##t##_Iterator other);\
	};												\
	size_t size(Deque_##t * self){									\
		return self->length;									\
	}												\
	bool empty(Deque_##t * self){									\
		return size(self) == 0;									\
	}												\
	void reallocate(Deque_##t *self) {								\
	} 												\
	void push_front(Deque_##t * self, t val){							\
		if(self->length == self->max_length){							\
			reallocate(self);								\
		}											\
		self->length++;										\
		self->iter_head->curr = self->arr + ((self->head_index-1) % self->max_length);		\
		self->head_index = (self->head_index-1) % self->max_length;				\
		self->arr[self->head_index] = val;							\
	}												\
	void push_back(Deque_##t * self, t val){							\
		if(self->length == self->max_length){							\
			reallocate(self);								\
		}											\
		self->length++;										\
		self->arr[self->tail_index] = val;							\
		self->iter_tail->curr = self->arr + ((self->tail_index) % self->max_length);		\
		self->tail_index = (self->tail_index+1) % self->max_length;				\
	}												\
	void pop_front(Deque_##t * self){								\
		if(self->length == 0) return;								\
		self->length--;										\
		self->head_index = ((self->head_index)+1) % self->max_length;				\
		self->iter_head->curr = self->arr + ((self->head_index) % self->max_length);		\
	}												\
	void pop_back(Deque_##t * self){								\
		if(self->length == 0) return;								\
		self->length--;										\
		self->iter_tail->curr = self->arr + ((self->tail_index) % self->max_length);		\
		self->tail_index = ((self->tail_index)-1) % self->max_length;				\
	}												\
	t& back(Deque_##t * self){									\
		return self->arr[(self->tail_index-1)%self->max_length];				\
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
		if(it->curr == it->queue_end-1) it->curr = it->queue_begin;				\
		else it->curr++;									\
	}												\
	void dec(Deque_##t##_Iterator * it){								\
		if(it->curr == it->queue_begin) it->curr = it->queue_end-1;				\
		else it->curr--;									\
	}												\
	t& deref(Deque_##t##_Iterator * it){								\
		return *it->curr;									\
	}												\
	t& at(Deque_##t * self, int i){									\
		while(i > 0){										\
			inc(self->iter_head);								\
			i--;										\
		}											\
		return *self->iter_head->curr;								\
	}												\
	void Deque_##t##_ctor(Deque_##t * self, bool (*less)(const t &, const t &)){			\
		self->length = 0;									\
		self->max_length = 8;									\
		self->arr = (t*) malloc(sizeof(t) * self->max_length);					\
		self->head_index = -1;									\
		self->tail_index = 0;									\
		self->iter_head = (Deque_##t##_Iterator *) malloc(sizeof(Deque_##t##_Iterator));	\
		self->iter_tail = (Deque_##t##_Iterator *) malloc(sizeof(Deque_##t##_Iterator));	\
		self->iter_head->curr = self->arr;							\
		self->iter_tail->curr = self->arr + 1;							\
		self->iter_head->inc = self->iter_tail->inc = &inc;					\
		self->iter_head->dec = self->iter_tail->dec = &dec;					\
		self->iter_head->deref = self->iter_tail->deref = &deref;				\
		self->iter_head->Deque_##t##_Iterator_equal = self->iter_tail->Deque_##t##_Iterator_equal = &Deque_##t##_Iterator_equal;\
		self->iter_head->queue_begin = self->arr + 1;						\
		self->iter_head->queue_end = self->arr + self->max_length-1;				\
		self->iter_tail->queue_begin = self->arr + 1;						\
		self->iter_tail->queue_end = self->arr + self->max_length-1;				\
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
	}																
#endif