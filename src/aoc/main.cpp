#include <frstd/prelude.hpp>

#include <frstd/baseutil.hpp>

void run1(String input) {
    DynArray<String> inputList = split(strip(input), '\n');

    i32 increaseCount = 0;
    i32 prev;
    for(usz i = 0; i < len(inputList); ++i) {
        i32 x = fromString<i32>(strip(inputList[i]));
        if(i != 0 && x > prev) {
            ++increaseCount;
        }
        prev = x;
    }

    writeStdout(toString(increaseCount) + "\n");

    i32 sumIncreaseCount = 0;
    i32 prevSum;
    for(usz i = 0; i < len(inputList) - 2; ++i) {
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

void run2(String input) {
    DynArray<String> lines = split(strip(input), '\n');

    i32 x = 0;
    i32 y = 0;
    for(usz lineIdx = 0; lineIdx < len(lines); ++lineIdx) {
        DynArray<String> line = split(strip(lines[lineIdx]), ' ');
        String cmd = strip(line[0]);
        i32 amount = fromString<i32>(strip(line[1]));
        if(cmd == "forward") x += amount;
        if(cmd == "down") y += amount;
        if(cmd == "up") y -= amount;
    }

    writeStdout(toString(x * y) + "\n");

    x = 0;
    y = 0;
    i32 aim = 0;
    for(usz lineIdx = 0; lineIdx < len(lines); ++lineIdx) {
        DynArray<String> line = split(strip(lines[lineIdx]), ' ');
        String cmd = strip(line[0]);
        i32 amount = fromString<i32>(strip(line[1]));
        if(cmd == "forward") {
            x += amount;
            y += aim * amount;
        }
        if(cmd == "down") aim += amount;
        if(cmd == "up") aim -= amount;
    }

    writeStdout(toString(x * y) + "\n");
}

u32 rec(DynArray<String> lines, usz i, usz k, bool oxy) { 
    if(len(lines) == 0) {
        frstd::baseutil::fail("FAIL");
    }
    if(len(lines) == 1) {
        u32 ret = 0;
        for(usz j = 0; j < k; ++j) {
            ret *= 2;
            if(lines[0][j] == '1') {
                ++ret;
            }
        }
        return ret;
    }
    if(i >= k) {
        frstd::baseutil::fail("FAIL2");
    }
    usz count = 0;
    for(usz lineIdx = 0; lineIdx < len(lines); ++lineIdx) {
        if(lines[lineIdx][i] == '1') {
            ++count;
        }
    }
    u8 keep;
    if((2 * count >= len(lines)) == oxy) {
        keep = '1';
    } else {
        keep = '0';
    }
    DynArray<String> newLines;
    for(usz lineIdx = 0; lineIdx < len(lines); ++lineIdx) {
        if(lines[lineIdx][i] == keep) {
            newLines.push(lines[lineIdx]);
        }
    }
    return rec(newLines, i + 1, k, oxy);
}

void run3(String input) {
    DynArray<String> lines = split(strip(input), '\n'); 
    usz k;
    for(usz lineIdx = 0; lineIdx < len(lines); ++lineIdx) {
        if(lineIdx == 0) {
            k = len(lines[lineIdx]);
        } else {
            if(k != len(lines[lineIdx])) {
                frstd::baseutil::fail("FAIL");
            }
        }
    }
    DynArray<usz> counts(k, 0);
    for(usz lineIdx = 0; lineIdx < len(lines); ++lineIdx) {
        const String& line = lines[lineIdx];
        for(usz i = 0; i < k; ++i) {
            if(line[i] == '1') {
                ++counts[i];
            }
        }
    }

    u32 a = 0;
    u32 b = 0;
    for(usz i = 0; i < k; ++i) {
        a *= 2;
        b *= 2;
        if(counts[i] > len(lines) / 2) {
            ++a;
        } else {
            ++b;
        }
    }
    writeStdout(toString(a * b) + "\n");
    writeStdout(toString(rec(lines, 0, k, true) * rec(lines, 0, k, false)) + "\n");
}

struct Cell {
    u32 num;
    bool marked;
};

void run4(String input) {
    DynArray<String> lines = split(strip(input), '\n');
    DynArray<String> numStrs = split(strip(lines[0]), ',');
    DynArray<u32> nums;
    for(usz i = 0; i < len(numStrs); ++i) {
        nums.push(fromString<u32>(numStrs[i]));
    }

    DynArray<DynArray<DynArray<Cell>>> boards;
    DynArray<bool> boardWin;
    for(usz s = 2; s + 4 < len(lines); s += 6) {
        DynArray<DynArray<Cell>> board;
        for(usz i = 0; i < 5; ++i) {
            board.push(DynArray<Cell>());
            DynArray<String> row = split(strip(lines[s + i]), ' ');
            for(usz j = 0; j < len(row); ++j) {
                String elem = strip(row[j]);
                if(len(elem) > 0) {
                    board[i].push({fromString<u32>(elem), false});
                }
            }
        }
        boards.push(move(board));
        boardWin.push(false);
    }

    bool winOnce = false;
    u32 lastScore;
    for(usz numIdx = 0; numIdx < len(nums); ++numIdx) {
        u32 num = nums[numIdx];
        for(usz boardIdx = 0; boardIdx < len(boards); ++boardIdx) {
            DynArray<DynArray<Cell>>& board = boards[boardIdx];
            for(usz i = 0; i < 5; ++i) {
                for(usz j = 0; j < 5; ++j) {
                    if(board[i][j].num == num) {
                        board[i][j].marked = true;
                    }
                }
            }

            bool win = false;
            for(usz i = 0; i < 5; ++i) {
                bool rowWin = true;
                bool colWin = true;
                for(usz j = 0; j < 5; ++j) {
                    if(!board[i][j].marked) {
                        rowWin = false;
                    }
                    if(!board[j][i].marked) {
                        colWin = false;
                    }
                }
                if(rowWin || colWin) {
                    win = true;
                }
            }

            if(win && !boardWin[boardIdx]) {
                boardWin[boardIdx] = true;
                u32 sum = 0;
                for(usz i = 0; i < 5; ++i) {
                    for(usz j = 0; j < 5; ++j) {
                        if(!board[i][j].marked) {
                            sum += board[i][j].num;
                        }
                    }
                }
                u32 score = sum * num;
                if(!winOnce) {
                    winOnce = true;
                    writeStdout(toString(score) + "\n");
                }
                lastScore = score;
            }
        }
    }

    writeStdout(toString(lastScore) + "\n");
}

void run(const DynArray<String>& args) {
    frstd::LeakCheck leakCheck;

    if(len(args) != 1) {
        writeStderr("Usage: aoc2021 <day#>\n");
        return;
    }
    String day = args[0];
    if(day == "1") {
        String input = readStdin();
        run1(move(input));
    } else if(day == "2") {
        String input = readStdin();
        run2(move(input));
    } else if(day == "3") {
        String input = readStdin();
        run3(move(input));
    } else if(day == "4") {
        String input = readStdin();
        run4(move(input));
    } else {
        writeStderr("Unknown day\n");
    }
}
