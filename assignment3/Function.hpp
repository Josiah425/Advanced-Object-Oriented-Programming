#ifndef _FUNCTION_HPP_
#define _FUNCTION_HPP_

#include <iostream>
#include <cstddef>
#include <exception>
#include <memory>

namespace cs540{
	class BadFunctionCall : std::exception{
		public:
			virtual std::string Error() const throw() {
				return "Error, can't call function";
			}		
	};

	template <typename ResultType, typename ... ArgumentTypes>
	class FunctorCallable{
		public:
			virtual ~FunctorCallable() = default;
			virtual ResultType invoke(ArgumentTypes...) = 0;
			virtual FunctorCallable<ResultType, ArgumentTypes...>* clone() = 0;
	};


	template <typename>
	class Function;

	template <typename ResultType, typename ... ArgumentTypes>
	class Function<ResultType(ArgumentTypes...)>{
		public:
			template <typename T>
			class Functor : public FunctorCallable<ResultType, ArgumentTypes...>{
				public:
					Functor(const T& t) : target(t) {}

					virtual ~Functor() = default;

					ResultType invoke(ArgumentTypes... args) override{
						target(args...);
					}

					virtual FunctorCallable<ResultType, ArgumentTypes...>* clone(){
						return new Functor<T>(target);
					}

				private:
					T target;
			};

			Function() : callable(false), functor(nullptr) {}

			template <typename FunctionType>
			Function(FunctionType f) : callable(true) {
				functor = new Functor<FunctionType>(f);
			}

			Function(const Function &f) : callable(f.callable){
				if(f.functor != nullptr) functor = f.functor->clone();
				else functor = nullptr;
			}

			Function &operator=(const Function &f) {
				if(this != &f){
					if(functor != nullptr){
						delete functor;
					}
					callable = f.callable;
					if(f.functor != nullptr) functor = f.functor->clone();
					else functor = nullptr;
				}
				return *this;
			}

			~Function(){
				if(functor != nullptr){
					delete functor;
					functor = nullptr;
				}
			}

			ResultType operator()(ArgumentTypes... args){
				if(!callable){
					throw BadFunctionCall();
				}
				else return (*functor).invoke(args...);
			}

			explicit operator bool() const{
				return functor != nullptr;
			}

		private:
			bool callable;
			FunctorCallable<ResultType, ArgumentTypes...> * functor;
	};

	template <typename ResultType, typename... ArgumentTypes>
	bool operator==(const Function<ResultType(ArgumentTypes...)> &f, std::nullptr_t){
		return !f;
	}
	
	template <typename ResultType, typename... ArgumentTypes>
	bool operator==(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> &f){
		return !f;
	}

	template <typename ResultType, typename... ArgumentTypes>
	bool operator!=(const Function<ResultType(ArgumentTypes...)> &f, std::nullptr_t){
		return bool(f);
	}

	template <typename ResultType, typename... ArgumentTypes>
	bool operator!=(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> &f){
		return bool(f);
	}
}




#endif