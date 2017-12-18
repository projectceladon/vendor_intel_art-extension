interface CondVirtBase {
   
    public byte getThingies();
}

class CondVirtExt implements CondVirtBase {

	byte thingies = 3;

    public byte getThingies() {
        return thingies;
    }
}

