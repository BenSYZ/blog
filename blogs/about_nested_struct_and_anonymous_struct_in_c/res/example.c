#include <stdio.h>
struct s {
    int abc;

    struct {
        int a;
    };

    struct {
        int b;
    } intern;

    struct i {
        int c;
    };

    struct i2 {
        int d;
    } intern2;

    struct i3 {
        int e;
    };
    struct i3 intern3;
};

struct i3 AA;

int main(int argc, char const *argv[])
{
    struct s mystruct;

    mystruct.abc = 0;
    mystruct.a = 1;
    mystruct.intern.b = 2;
    mystruct.c = 3; // <-- does not compile
    mystruct.intern2.d = 4;
    mystruct.intern3.e = 5;
    AA.e=0;

    return 0;
}
