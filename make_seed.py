#!/usr/bin/python

import random;
import string;

tipoFondo = ['A', 'B', 'C', 'D', 'E'];
tipoActivo = ['RV', 'RF'];

type1 = [
    'ACC',
    'ACE',
    'AEE',
    'CFID',
    'CFMV',
    'CFMD',
    'CMED',
    'CMEV',
    'ETFA',
    'ETFB',
    'ETFC'
];
type2 = [
    'DPF',
    'ECO'
]

def addPadding(s, n = 10, c = ' '):
    if len(s) < n:
        return c * (n - len(s)) + s;
    return s;

def makeString(n = 10):
    return ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(n))

for i in xrange(0,500000):
    L = [
        addPadding(random.choice(tipoFondo), 1),
        addPadding(makeString(20), 50),
        addPadding(random.choice(type1 + type2), 10),
        str(random.random()*1000000),
        str(int(random.random()*100) / 100.0),
        str(random.random()*5000000),
        str(int(random.random()*120))
    ];
    print ';'.join(L) + '\n',;
