class Foo {
    char a;
    Foo() {}
    Foo(char b) {
        this.a = b;
    }
    static Foo gfoo = new Foo('G');
    public static Foo toFoo() {
        return gfoo;
    }
}
