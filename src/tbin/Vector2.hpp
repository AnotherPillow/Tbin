#ifndef VECTOR2HPP
#define VECTOR2HPP

namespace tbin {
    class Vector2 {
        public:
        int x;
        int y;

        Vector2(int x, int y) : x(x), y(y) {}
        Vector2() : x(0), y(0) {}
        private:
    };
}

#endif // VECTOR2HPP
