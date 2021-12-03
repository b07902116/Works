# reference: ChiaVDF (https://github.com/Chia-Network/chiavdf)
# You don't need to understand the implementation of the class group operations.
# You can directly jump to main function to see the usage

import math

def linear_congruence(a, b, c): # ak = b (mod c) => k = pn + q
    g = math.gcd(a, c)
    assert b%g == 0
    A = a//g
    B = b//g
    C = c//g
    p = C
    q = (pow(A, -1, C) * B) % c
    return p, q

class ClassGroup:
    def __init__(self, d):
        if d % 8 != 1:
            raise BaseException("Invalid discriminent.")
        self.d = d
    
    def identity(self):
        return Form.from_abd(1, 1, self.d)

    def generator(self):
        return Form.from_abd(2, 1, self.d)

    def getForm(self, a, b):
        return Form.from_abd(a, b, self.d)
    
class Form():
    def __init__(self, a, b, c):
        if a < 0:
            raise BaseException("Invalid form. Positive a.")
        d = b*b - 4*a*c
        if d % 8 != 1:
            raise BaseException("Invalid discriminent.")
        self.a = a
        self.b = b
        self.c = c
        self.normalize()

    @staticmethod
    def from_abd(a, b, d):
        if a < 0:
            raise BaseException("Invalid form. Positive a.")
        if (b*b-d)%(4*a) != 0:
            raise BaseException("Invalid form. Can't find c.")
        return Form(a, b, (b*b-d)//(4*a))

    def __str__(self):
        return f"{self.a} {self.b} {self.c}"

    def __eq__(self, other):
        return self.a == other.a and self.b == other.b and self.c == other.c

    def __mul__(self, other):
        if self == other:
            return self.square()
        else:
            return self.multiple(other)

    def normalize(self):
        r = (self.a-self.b)//(2*self.a)
        A = self.a
        B = self.b + (2*self.a*r)
        C = self.a*r*r + self.b*r + self.c
        self.a = A
        self.b = B
        self.c = C
    
    def reduce_impl(self):
        s = (self.b+self.c)//(2*self.c)
        A = self.c
        B = 2*self.c*s - self.b
        C = self.c*s*s - self.b*s + self.a
        self.a = A
        self.b = B
        self.c = C

    def reduce(self):
        self.normalize()
        while self.a>self.c or (self.a==self.c and self.b<0):
            self.reduce_impl()
        self.normalize()

    def square(self):
        k = (pow(self.b, -1, self.a) * self.c) % self.a
        A = self.a * self.a
        B = self.b - 2*self.a*k
        C = k*k - (self.b*k-self.c)//self.a
        D = Form(A, B, C)
        D.reduce()
        return D

    def multiple(self, other):
        g = (other.b + self.b) // 2
        h = (other.b - self.b) // 2
        w = math.gcd(math.gcd(self.a, other.a), g)
        s = self.a // w
        t = other.a // w
        u = g // w
        i, j = linear_congruence(t*u, h*u+s*self.c, s*t)
        _, n = linear_congruence(t*i, h-t*j, s)
        k = j + i * n
        l = (k*t-h) // s
        m = (t*u*k - h*u - self.c*s) // (s*t)
        A = s*t
        B = w*u - k*t - l*s
        C = k*l - w*m
        D = Form(A, B, C)
        D.reduce()
        return D

