interface CondVirtBase {
   
    public double getThingies();
}

class CondVirtExt implements CondVirtBase {

	double thingies = 3;

    public double getThingies() {
        return thingies + 2;
    }
}

