class Test {
    boolean thingies = false;

    private boolean getThingies() {
        return thingies;
    }

    private void setThingies(boolean newThingies) {
        thingies = newThingies;
    }

    public boolean gimme() {
       return getThingies();
    }

    public void hereyouare(boolean newThingies) {
        setThingies(newThingies);
    }
}

