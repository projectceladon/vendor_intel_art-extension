class CondVirtBase {
   
	float thingies = 32;

    public float getThingies() {
        return thingies;
    }
}

class CondVirtExt extends CondVirtBase {

	float thingies = 3;

    public float getThingies() {
        return thingies + 2;
    }
}

