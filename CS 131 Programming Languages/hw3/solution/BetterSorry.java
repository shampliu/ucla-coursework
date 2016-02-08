import java.util.concurrent.atomic.AtomicInteger;

class BetterSorry implements State {
    private byte maxval;
    private AtomicInteger[] value;

    BetterSorry(byte[] v) { 
	value = new AtomicInteger[v.length];
	for (int i = 0; i < v.length; i++) {
	    value[i] = new AtomicInteger(v[i]);
	} 
	maxval = 127; 
    }

    BetterSorry(byte[] v, byte m) { 
	value = new AtomicInteger[v.length]; 
	for (int i = 0; i < v.length; i++) {
	    value[i] = new AtomicInteger(v[i]);
	}
	maxval = m; 
    }

    public int size() { return value.length; }

    public byte[] current() { 
	byte[] result = new byte[value.length];
	for (int i = 0; i < value.length; i++) {
	    result[i] = (byte) value[i].intValue();
	}
	return result;
    }

    public boolean swap(int i, int j) {
	if (value[i].get() <= 0 || value[j].get() >= maxval) {
	    return false;
	}

	value[i].getAndDecrement(); 
	value[j].getAndIncrement();

	return true;
    }
}
