class CondVirtBase {
   
	int thingies = 32;

    public int getThingies() {
        return thingies;
    }
}

class CondVirtExt extends CondVirtBase {

	int thingies = 3;

    public int getThingies() {
        return thingies + 2;
    }
}

