#ifndef _SHAREDPTR_HPP_
#define _SHAREDPTR_HPP_

#include <cstddef>
#include <iostream>
#include <atomic>
#include <pthread.h>

namespace cs540{
	//Need this to prevent the can not convert from derived to base1 in the future
	//Essentially this allows me to not have to declare the templated type of ReferenceCount
	//In the future, because it's not a templated struct, only it's constructor is templated
	struct Destructor{
		virtual void destroy() = 0;
		virtual ~Destructor() = default;
	};

	template <typename T>
	struct TypedDestructor : public Destructor{
		T * data;
		TypedDestructor() : data(nullptr) {}
		TypedDestructor(T * p) : data(p) {}
		void destroy() override{
			delete data;
		}
	};

	struct ReferenceCount{
		std::atomic_size_t reference_count;
		pthread_mutex_t lock;
		Destructor * dest;
		ReferenceCount() : reference_count(0), lock(PTHREAD_MUTEX_INITIALIZER), dest(nullptr) {}
		template <typename T>
		ReferenceCount(T * p) : reference_count(1), lock(PTHREAD_MUTEX_INITIALIZER), dest(new TypedDestructor<T>(p)) {}
		~ReferenceCount(){
			if(dest != nullptr){
				dest->destroy();
				delete dest;
			}
			pthread_mutex_unlock(&lock);
		}
	};

	template <typename T>
	class SharedPtr{
		public:
			SharedPtr(){
				data = nullptr;
				references = nullptr;
			}
			
			template <typename U>
			explicit SharedPtr(U * p){
				data = p;
				references = new ReferenceCount(p);
			}

			SharedPtr(const SharedPtr &p){
				if(p.data != nullptr){
					data = p.data;
					references = p.references;
					pthread_mutex_lock(&(references->lock));
					references->reference_count++;
					pthread_mutex_unlock(&(references->lock));
				}
				else{
					data = nullptr;
					references = nullptr;
				}
			}

			template <typename U>
			SharedPtr(const SharedPtr<U> &p){
				if(p.data != nullptr){
					data = p.data;
					references = p.references;
					pthread_mutex_lock(&(references->lock));
					references->reference_count++;
					pthread_mutex_unlock(&(references->lock));
				}
				else{
					data = nullptr;
					references = nullptr;
				}
			}

			SharedPtr(SharedPtr &&p){
				data = std::move(p.data);
				references = std::move(p.references);
				p.data = nullptr;
				p.references = nullptr;
			}

			template <typename U>
			SharedPtr(SharedPtr<U> && p){
				data = std::move(p.data);
				references = std::move(p.references);
				p.data = nullptr;
				p.references = nullptr;
			}

			//Required for the dynamic and static casting
			template <typename U>
			SharedPtr(const SharedPtr<U> &p, T * other){
				data = other;
				references = p.references;
				if(data != nullptr){
					pthread_mutex_lock(&(references->lock));
					references->reference_count++;
					pthread_mutex_unlock(&(references->lock));
				}
			}

			SharedPtr &operator=(const SharedPtr & p){
				if(&p == this || data == p.data) return *this;
				if(references != nullptr){
					pthread_mutex_lock(&(references->lock));
					bool countIsZero = false;
					references->reference_count--;
					if(references->reference_count == 0){
						delete references;
						countIsZero = true;
					}
					if(!countIsZero) pthread_mutex_unlock(&(references->lock));
				}
				data = p.data;
				references = p.references;
				if(data != nullptr){
					pthread_mutex_lock(&(references->lock));
					references->reference_count++;
					pthread_mutex_unlock(&(references->lock));
				}
				return *this;
			}

			template <typename U>
			SharedPtr<T> &operator=(const SharedPtr<U> & p){
				if(data == p.data) return *this;
				if(references != nullptr){
					pthread_mutex_lock(&(references->lock));
					bool countIsZero = false;
					references->reference_count--;
					if(references->reference_count == 0){
						delete references;
						countIsZero = true;
					}
					if(!countIsZero) pthread_mutex_unlock(&(references->lock));
				}
				data = p.data;
				references = p.references;
				if(data != nullptr){
					pthread_mutex_lock(&(references->lock));
					references->reference_count++;
					pthread_mutex_unlock(&(references->lock));
				}
				return *this;
			}

			SharedPtr &operator=(SharedPtr &&p){
				if(data == p.data) return *this;
				if(references != nullptr){
					pthread_mutex_lock(&(references->lock));
					bool countIsZero = false;
					references->reference_count--;
					if(references->reference_count == 0){
						delete references;
						countIsZero = true;
					}
					if(!countIsZero) pthread_mutex_unlock(&(references->lock));
				}
				data = std::move(p.data);
				references = std::move(p.references);
				p.data = nullptr;
				p.references = nullptr;
			}

			template <typename U>
			SharedPtr &operator=(SharedPtr<U> &&p){
				if(data == p.data) return *this;
				if(references  != nullptr){
					pthread_mutex_lock(&(references->lock));
					bool countIsZero = false;
					references->reference_count--;
					if(references->reference_count == 0){
						delete references;
						countIsZero = true;
					}
					if(!countIsZero) pthread_mutex_unlock(&(references->lock));
				}
				data = std::move(p.data);
				references = std::move(p.references);
				p.data = nullptr;
				p.references = nullptr;
			}

			~SharedPtr(){
				if(references != nullptr){
					pthread_mutex_lock(&(references->lock));
					bool countIsZero = false;
					references->reference_count--;
					if(references->reference_count == 0){
						delete references;
						countIsZero = true;
					}
					if(!countIsZero) pthread_mutex_unlock(&(references->lock));
				}
			}

			void reset(){
				if(data != nullptr){
					pthread_mutex_lock(&(references->lock));
					bool countIsZero = false;
					references->reference_count--;
					if(references->reference_count == 0){
						delete references;
						countIsZero = true;
					}
					if(!countIsZero) pthread_mutex_unlock(&(references->lock));
				}
				data = nullptr;
				references = nullptr;
			}

			template <typename U> void reset(U *p){
				if(data != nullptr){
					pthread_mutex_lock(&(references->lock));
					bool countIsZero = false;
					references->reference_count--;
					if(references->reference_count == 0){
						delete references;
						countIsZero = true;
					}
					if(!countIsZero) pthread_mutex_unlock(&(references->lock));
				}
				data = p;
				references = new ReferenceCount(p);
			}

			T * get() const{
				return data;
			}

			T &operator*() const{
				return *data;
			}

			T *operator->() const{
				return data;
			}

			explicit operator bool() const{
				if(data != nullptr) return true;
				return false;
			}

		public:
			T * data;
			ReferenceCount * references;
	};

	template <typename T1, typename T2>
	bool operator==(const SharedPtr<T1> &lhs, const SharedPtr<T2> &rhs){
		return lhs.get() == rhs.get();
	}

	template <typename T>
	bool operator==(const SharedPtr<T> &lhs, std::nullptr_t rhs){
		return !lhs;
	}

	template <typename T>
	bool operator==(std::nullptr_t lhs, const SharedPtr<T> &rhs){
		return !rhs;
	}

	template <typename T1, typename T2>
	bool operator!=(const SharedPtr<T1> &lhs, const SharedPtr<T2> &rhs){
		return !(lhs == rhs);
	}

	template <typename T>
	bool operator!=(const SharedPtr<T> &lhs, std::nullptr_t rhs){
		return (bool) lhs;
	}

	template <typename T>
	bool operator!=(std::nullptr_t lhs, const SharedPtr<T> &rhs){
		return (bool) rhs;
	}

	template <typename T, typename U>
	SharedPtr<T> static_pointer_cast(const SharedPtr<U> &sp){
		return SharedPtr<T>(sp, static_cast<T*>(sp.get()));
	}

	template <typename T, typename U>
	SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U> &sp){
		if (auto p = dynamic_cast<T*>(sp.get())){
			return SharedPtr<T>(sp, p);
		}else {
			return SharedPtr<T>();
		}
	}
}

#endif
