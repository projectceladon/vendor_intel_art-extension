// The test checks that stack after NullPointerException occurs is correct despite inlining
class Foo {
    public int i;
    public Foo(int k) {
        this.i = k;
    }
}
