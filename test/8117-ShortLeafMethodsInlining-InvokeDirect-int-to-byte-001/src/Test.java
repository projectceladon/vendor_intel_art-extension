class Test {
    public byte shim(int jj){
        return simple_method(jj);
    }

    private byte simple_method(int jj) {
        byte ii;
        ii = (byte)jj;
        return ii;
    }
}
