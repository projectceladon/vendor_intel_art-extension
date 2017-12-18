class CondVirtBase {
   
	double thingies = 32;

    public double getThingies() {
        return thingies;
    }
}

class CondVirtExt extends CondVirtBase {

	double thingies = 3;

    public double getThingies() {
        return thingies + 2;
    }
}

