int fastpow(int a, int n) {
	if (n == 0) {
		return 1;
	}
	else if (n % 2 == 0) {
		int b = (a, n / 2);
		return b * b;
	}
	else {
		return (a, n - 1) * a;
	}
}

int fib_matrix(int n)
{
    int a = 1, ta,
        b = 1, tb,
        c = 1, rc = 0, tc,
        d = 0, rd = 1;

    while (n)
    {
        //power is odd
        if (n & 1)
        {
            tc = rc;
            rc = rc * a + rd * c;
            rd = tc * b + rd * d;
        }

        ta = a; tb = b; tc = c;
        a = a * a + b * c;
        b = ta * b + b * d;
        c = c * ta + d * c;
        d = tc * tb + d * d;

        n >>= 1;
    }
    //return vector value 
    return rc;
}

void reverse() {
    node* prev = nullptr, curr = head, next = nullptr;
    while (curr != nullptr) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
}