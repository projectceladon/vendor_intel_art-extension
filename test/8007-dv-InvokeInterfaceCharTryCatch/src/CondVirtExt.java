interface CondVirtBase {
   
    public char getThingies();
}

class CondVirtExt implements CondVirtBase {

	char thingies = 'a';

    public char getThingies() {
        return thingies;
    }
}

