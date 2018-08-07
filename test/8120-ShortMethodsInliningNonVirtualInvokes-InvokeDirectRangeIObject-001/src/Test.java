class Test {
    Foo thingies = new Foo();

    private Foo getThingies(Foo f1, Foo f2, Foo f3, Foo f4, Foo f5, Foo f6) {
        return thingies;
    }

    private void setThingies(Foo newThingies, Foo f1, Foo f2, Foo f3, Foo f4, Foo f5, Foo f6) {
        thingies = newThingies;
    }

    public Foo gimme(Foo f1, Foo f2, Foo f3, Foo f4, Foo f5, Foo f6) {
       return getThingies(f1, f2, f3, f4, f5, f6);
    }

    public void hereyouare(Foo newThingies, Foo f1, Foo f2, Foo f3, Foo f4, Foo f5, Foo f6) {
        setThingies(newThingies, f1, f2, f3, f4, f5, f6);
    }
}

