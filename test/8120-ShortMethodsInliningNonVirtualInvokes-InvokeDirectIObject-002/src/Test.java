class Test {
    Foo thingies;

    private Foo getThingies() {
        return thingies;
    }

    private void setThingies(Foo newThingies) {
        thingies = newThingies;
    }

    public Foo gimme() {
       return getThingies();
    }

    public void hereyouare(Foo newThingies) {
        setThingies(newThingies);
    }
}

