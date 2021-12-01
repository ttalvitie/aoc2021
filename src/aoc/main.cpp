#include <frstd/prelude.hpp>

void run1(String input) {
    DynArray<String> inputList = split(strip(input), '\n');

    i32 increaseCount = 0;
    i32 prev;
    for(usz i = 0; i < inputList.size(); ++i) {
        i32 x = fromString<i32>(strip(inputList[i]));
        if(i != 0 && x > prev) {
            ++increaseCount;
        }
        prev = x;
    }

    writeStdout(toString(increaseCount) + "\n");

    i32 sumIncreaseCount = 0;
    i32 prevSum;
    for(usz i = 0; i < inputList.size() - 2; ++i) {
        i32 sum = 0;
        for(usz j = 0; j < 3; ++j) {
            sum += fromString<i32>(strip(inputList[i + j]));
        }
        if(i != 0 && sum > prevSum) {
            ++sumIncreaseCount;
        }
        prevSum = sum;
    }

    writeStdout(toString(sumIncreaseCount) + "\n");
}

void run(DynArray<String> args) {
    if(args.size() != 1) {
        writeStderr("Usage: aoc2021 <day#>\n");
        return;
    }
    String day = args[0];
    if(day == "1") {
        String input = readStdin();
        run1(move(input));
    } else {
        writeStderr("Unknown day\n");
    }
}
