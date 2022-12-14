int factorial(int n) {
    if (n == 2) {
        return 2;
    }
    return n * factorial(n - 1);
}

int main(int argc, char const *argv[]) {
    int a = factorial(argc);

    int b = 0;
    if (a > argc) {
        b = 1;
    }
    return b;
}
