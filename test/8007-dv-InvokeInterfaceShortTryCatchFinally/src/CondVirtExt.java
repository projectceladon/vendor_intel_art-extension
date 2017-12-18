interface CondVirtBase {
   
    public short getThingies();
}

class CondVirtExt implements CondVirtBase {

	short thingies = 3;

    public short getThingies() {
        return thingies;
    }
}

