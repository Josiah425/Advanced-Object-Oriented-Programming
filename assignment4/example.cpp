
#include <iostream>
#include <tuple>

/*
 * This implementation does not use a helper class, but does require C++17 due
 * to the class template arg deduction.
 */

template <typename... Ts>
class Pack {
        // Overload output operator.
        friend std::ostream &operator<<(std::ostream &os, const Pack &i) {
            // Initiate the recursion.
            i.apply<0>(i.args);
            return os;
        }
    public:
        // Pack all args into a tuple.
        Pack(const Ts &... as) : args{as...} {}
    private:
        // Tuple to hold all the args.
        std::tuple<const Ts &...> args;

        // This helper function unpacks the tuple one-by-one.
        template <std::size_t I, typename T>
        static void constexpr apply(const T &t) {
            if constexpr (I < std::tuple_size_v<T>) {
                std::cout << "Element " << I << ": " << std::get<I>(t) << std::endl;
                apply<I + 1>(t);
            }
        }
};

int
main() {

    std::cout << Pack(1, 1.3, "foo");
}