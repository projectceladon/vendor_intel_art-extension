// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class Test {
    Foo[] thingiesArray;

    public Test (int iter) {
        this.thingiesArray = new Foo[iter];
        for(int i = 0; i < iter; i++) {
            this.thingiesArray[i] = new Foo(i);
        }
    }

    private Foo getThingies(Foo[] arr, int i) {
        return arr[i];
    }

    private void setThingies(Foo[] arr, Foo newThingy, int i) {
        arr[i] = newThingy;
    }

    public Foo gimme(Foo[] arr, int i) {
       return getThingies(arr, i);
    }

    public void hereyouare(Foo[] arr, Foo newThingy, int i) {
        setThingies(arr, newThingy, i);
    }
}

