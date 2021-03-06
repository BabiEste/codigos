-- Calcula a sequencia de Fibonacci
fibonacci x =
    if x == 0 then 0
    else if x == 1 then 1
    else fibonacci (x-1) + fibonacci (x-2)

-- Multiplicacao de a e b por somatória
multi a b =
    if b == 1 then a
    else a + multi a (b-1)

-- Potenciação de a e b através de multiplicações
pow a b =
    if b == 1 then a
    else a * pow a (b-1)

-- Multiplicação russa
multiRussa a b = multiRussaAux a b [(a, b)]

-- Soma os valores de b da tupla (a, b) em que a é ímpar
multiRussaSoma a b s = sum [b | (a, b) <- s, mod a 2 /= 0]

multiRussaAux a b s =
    if a == 1
    then multiRussaSoma a b s
    else multiRussaAux a' b' ((a', b') : s)
    where
        a' = a `div` 2
        b' = b * 2