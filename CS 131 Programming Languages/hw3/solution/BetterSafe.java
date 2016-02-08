import java.util.concurrent.locks.ReentrantLock;

class BetterSafe implements State {
    private byte[] value;
    private byte maxval;
    private final ReentrantLock lock = new ReentrantLock(); 

    BetterSafe(byte[] v) { value = v; maxval = 127; }

    BetterSafe(byte[] v, byte m) { value = v; maxval = m; }

    public int size() { return value.length; }

    public byte[] current() { return value; }

    public boolean swap(int i, int j) {
	lock.lock();
	try {
	
	    if (value[i] <= 0 || value[j] >= maxval) {
	    	return false;
	    }
	    value[i]--;
	    value[j]++;
	    return true;
	}
	finally {
	    lock.unlock();
	}
    }
}
