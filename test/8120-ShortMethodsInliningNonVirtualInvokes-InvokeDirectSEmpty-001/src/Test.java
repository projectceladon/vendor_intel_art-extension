class Test {
    static int thingies = 0;

    private int getThingies() {
        return thingies;
    }

    private void setThingies(int newThingies) {
// empty method
    }

    public int gimme() {
       return getThingies();
    }

    public void hereyouare(int newThingies) {
        setThingies(newThingies);
    }
}

