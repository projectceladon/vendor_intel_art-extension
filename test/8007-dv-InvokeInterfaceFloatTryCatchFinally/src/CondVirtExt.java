interface CondVirtBase {
   
    public float getThingies();
}

class CondVirtExt implements CondVirtBase {

	float thingies = 3;

    public float getThingies() {
        return thingies + 2;
    }
}

