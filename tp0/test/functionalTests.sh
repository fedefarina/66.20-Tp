#!/usr/bin/env bash


FAILED_TESTS_COUNT=0

#1. Generamos una imagen de 1 punto de lado, centrada en el orı́gen del plano complejo

OUTPUT="$(../build/tp0 -c 0.01+0i -r 1x1 -o -)"
TEST1_OUTPUT=$'P2\n1 1\n255\n19'

if [ "$OUTPUT" == "$TEST1_OUTPUT" ]
then
    echo "Test 1 OK"
else
    FAILED_TESTS_COUNT=$[$FAILED_TESTS_COUNT+1]
    echo "Test 1 FAIL: Expected '$OUTPUT' to be equal to '$TEST1_OUTPUT'"
fi

#2. Repetimos el experimento, pero nos centramos ahora en un punto que seguro no pertenece al conjunto

OUTPUT="$(../build/tp0 -c 10+0i -r 1x1 -o -)"
TEST2_OUTPUT=$'P2\n1 1\n255\n0'

if [ "$OUTPUT" == "$TEST2_OUTPUT" ]
then
    echo "Test 2 OK"
else
    FAILED_TESTS_COUNT=$[$FAILED_TESTS_COUNT+1]
    echo "Test 2 FAIL: Expected '$OUTPUT' to be equal to '$TEST2_OUTPUT'"
fi

#3. Imagen imposible
OUTPUT="$(../build/tp0 -c 0+0i -r 0x1 -o - 2>&1)"
TEST3_OUTPUT=$'fatal: invalid resolution specification.'

if [ "$OUTPUT" == "$TEST3_OUTPUT" ]
then
    echo "Test 3 OK"
else
    FAILED_TESTS_COUNT=$[$FAILED_TESTS_COUNT+1]
    echo "Test 3 FAIL: Expected '$OUTPUT' to be equal to '$TEST3_OUTPUT'"

fi


#4. Archivo de salida imposible
OUTPUT="$(../build/tp0 -o /tmp 2>&1)"
TEST4_OUTPUT=$'fatal: cannot open output file.'

if [ "$OUTPUT" == "$TEST4_OUTPUT" ]
then
    echo "Test 4 OK"
else
    FAILED_TESTS_COUNT=$[$FAILED_TESTS_COUNT+1]
    echo "Test 4 FAIL: Expected '$OUTPUT' to be equal to '$TEST4_OUTPUT'"
fi

#5. Coordenadas complejas imposibles
OUTPUT="$(../build/tp0 -c 1+3 -o - 2>&1)"
TEST5_OUTPUT=$'fatal: invalid center specification.'

if [ "$OUTPUT" == "$TEST5_OUTPUT" ]
then
    echo "Test 5 OK"
else
    FAILED_TESTS_COUNT=$[$FAILED_TESTS_COUNT+1]
    echo "Test 5 FAIL: Expected '$OUTPUT' to be equal to '$TEST5_OUTPUT'"
fi

#6. Argumentos de lı́nea de comando vacı́os.
OUTPUT="$(../build/tp0 -c "" -o - 2>&1)"
TEST6_OUTPUT=$'fatal: invalid center specification.'

if [ "$OUTPUT" == "$TEST6_OUTPUT" ]
then
    echo "Test 6 OK"
else
    FAILED_TESTS_COUNT=$[$FAILED_TESTS_COUNT+1]
    echo "Test 6 FAIL: Expected '$OUTPUT' to be equal to '$TEST6_OUTPUT'"
fi


#Summary
if [ "$FAILED_TESTS_COUNT" == 0 ]
then
 echo "All tests passed!."
else
echo "$FAILED_TESTS_COUNT test failed."
fi

