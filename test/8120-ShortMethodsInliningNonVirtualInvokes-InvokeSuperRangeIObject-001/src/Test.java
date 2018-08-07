class Test extends SuperTest {

    public Foo getThingies(Foo f1, Foo f2, Foo f3, Foo f4, Foo f5, Foo f6) {
        return super.getThingies(f1, f2, f3, f4, f5, f6);
    }

    public void setThingies(Foo newThingies, Foo f1, Foo f2, Foo f3, Foo f4, Foo f5, Foo f6) {
        super.setThingies(newThingies, f1, f2, f3, f4, f5, f6);
    } 
}

