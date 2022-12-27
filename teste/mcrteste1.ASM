section text
swap: macro &a &b &c
copy &a &c
copy &b &a
copy &c &b
endmacro

mul_n: macro
mul n
store n
endmacro

input n
load n
swap n a b
swap a n b
swap b a n
mul_n
swap n a b
mul_n

section data
n: space
a: space
b: space
