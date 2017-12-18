interface CondVirtBase {

    public int getThingies();
}

class CondVirtExt implements CondVirtBase {

	int thingies = 3;

    public int getThingies() {
        return thingies + 2;
    }
}

