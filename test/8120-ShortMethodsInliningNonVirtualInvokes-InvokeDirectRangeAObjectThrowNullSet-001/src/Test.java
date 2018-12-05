// The test checks that stack after NullPointerException occurs is correct despite inlining
class Test {
    Foo[] thingiesArray;
    int iterr;

    public Test (int iter) {
        this.thingiesArray = new Foo[iter];
        this.iterr = iter;
        for(int i = 0; i < iter; i++) {
            this.thingiesArray[i] = new Foo();
        }
    }

    private Foo getThingies(Foo[] arr, int i, Foo f1, Foo f2, Foo f3, Foo f4, Foo f5, Foo f6) {
        return arr[i];
    }

    private void setThingies(Foo[] arr, Foo newThingy, int i, Foo f1, Foo f2, Foo f3, Foo f4, Foo f5, Foo f6) {
        arr[i] = newThingy;
    }

    public Foo gimme(Foo[] arr, int i, Foo f1, Foo f2, Foo f3, Foo f4, Foo f5, Foo f6) {
        return getThingies(arr, i, f1, f2, f3, f4, f5, f6);
    }

    public void hereyouare(Foo[] arr, Foo newThingy, int i, Foo f1, Foo f2, Foo f3, Foo f4, Foo f5, Foo f6) {
        if (i == iterr - 1)
           arr = null;       
        setThingies(arr, newThingy, i, f1, f2, f3, f4, f5, f6);
    }
}

