interface CondVirtBase {
   
    public long getThingies();
}

class CondVirtExt implements CondVirtBase {

	long thingies = 3;

    public long getThingies() {
        return thingies + 2;
    }
}

