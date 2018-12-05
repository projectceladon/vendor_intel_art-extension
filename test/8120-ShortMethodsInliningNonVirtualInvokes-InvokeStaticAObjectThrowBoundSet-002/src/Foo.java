// The test checks that stack after ArrayIndexOutOffBouns Exception occurs is correct despite inlining
class Foo {
    public int i;
    public Foo(int k) {
        this.i = k;
    }
}
