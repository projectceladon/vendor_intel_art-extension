class Test {
    boolean thingies = false;

    private boolean getThingies(int i1, int i2, int i3, int i4, int i5, int i6) {
        return thingies;
    }

    private void setThingies(boolean newThingies, int i1, int i2, int i3, int i4, int i5, int i6) {
        thingies = newThingies;
    }

    public boolean gimme(int i1, int i2, int i3, int i4, int i5, int i6) {
       return getThingies(i1, i2, i3, i4, i5, i6);
    }

    public void hereyouare(boolean newThingies, int i1, int i2, int i3, int i4, int i5, int i6) {
        setThingies(newThingies, i1, i2, i3, i4, i5, i6);
    }
}

