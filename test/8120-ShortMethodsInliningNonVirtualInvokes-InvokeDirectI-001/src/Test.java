class Test {
    int thingies = 0;

    private int getThingies() {
        return thingies;
    }

    private void setThingies(int newThingies) {
        thingies = newThingies;
    }

    public int gimme() {
       return getThingies();
    }

    public void hereyouare(int newThingies) {
        setThingies(newThingies);
    }
}

