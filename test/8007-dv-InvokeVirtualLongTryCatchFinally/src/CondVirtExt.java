class CondVirtBase {
   
	long thingies = 32;

    public long getThingies() {
        return thingies;
    }
}

class CondVirtExt extends CondVirtBase {

	long thingies = 3;

    public long getThingies() {
        return thingies + 2;
    }
}

