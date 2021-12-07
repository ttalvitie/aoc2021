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

struct Line {
    usz x1;
    usz y1;
    usz x2;
    usz y2;
};

usz min(usz a, usz b) { return a < b ? a : b; }
usz max(usz a, usz b) { return a < b ? b : a; }
void swap(usz& a, usz& b) { usz t = a; a = b; b = t; }
isz abs(isz a) { return a < 0 ? -a : a; }

void run5(String input) {
    DynArray<String> inputLines = split(strip(input), '\n');
    DynArray<Line> lines;
    for(usz i = 0; i < len(inputLines); ++i) {
        DynArray<String> parts = split(strip(inputLines[i]), ' ');
        DynArray<String> p1 = split(strip(parts[0]), ',');
        DynArray<String> p2 = split(strip(parts[2]), ',');
        Line line;
        line.x1 = fromString<usz>(p1[0]);
        line.y1 = fromString<usz>(p1[1]);
        line.x2 = fromString<usz>(p2[0]);
        line.y2 = fromString<usz>(p2[1]);
        lines.push(line);
    }

    usz w = 0;
    usz h = 0;
    for(usz i = 0; i < len(lines); ++i) {
        Line line = lines[i];
        w = max(w, max(line.x1, line.x2) + 1);
        h = max(h, max(line.y1, line.y2) + 1);
    }

    DynArray<DynArray<usz>> counts;
    for(usz y = 0; y < h; ++y) {
        counts.push(DynArray<usz>(w, 0));
    }

    for(usz i = 0; i < len(lines); ++i) {
        Line line = lines[i];
        if(line.x1 == line.x2 || line.y1 == line.y2) {
            for(usz y = min(line.y1, line.y2); y <= max(line.y1, line.y2); ++y) {
                for(usz x = min(line.x1, line.x2); x <= max(line.x1, line.x2); ++x) {
                    ++counts[y][x];
                }
            }
        }
    }

    usz ret = 0;
    for(usz y = 0; y < h; ++y) {
        for(usz x = 0; x < w; ++x) {
            if(counts[y][x] >= 2) ++ret;
        }
    }

    writeStdout(toString(ret) + "\n");

    for(usz i = 0; i < len(lines); ++i) {
        Line line = lines[i];
        isz dx = (isz)line.x2 - (isz)line.x1;
        isz dy = (isz)line.y2 - (isz)line.y1;
        if(abs(dx) == abs(dy)) {
            dx /= abs(dx);
            dy /= abs(dy);
            usz x = line.x1;
            usz y = line.y1;
            while(true) {
                ++counts[y][x];
                if(x == line.x2 && y == line.y2) {
                    break;
                }
                x = (usz)((isz)x + dx);
                y = (usz)((isz)y + dy);
            }
        }
    }

    usz ret2 = 0;
    for(usz y = 0; y < h; ++y) {
        for(usz x = 0; x < w; ++x) {
            if(counts[y][x] >= 2) ++ret2;
        }
    }

    writeStdout(toString(ret2) + "\n");
}

void run6(String input) {
    DynArray<String> init = split(strip(input), ',');
    DynArray<u64> counts(9, 0);
    for(usz i = 0; i < len(init); ++i) {
        ++counts[fromString<usz>(init[i])];
    }
    u64 total1 = 0;
    u64 total2 = 0;
    for(usz day = 0; day < 256; ++day) {
        DynArray<u64> newCounts(9, 0);
        newCounts[6] = newCounts[8] = counts[0];
        for(usz i = 0; i < 8; ++i) {
            newCounts[i] += counts[i + 1];
        }
        counts = move(newCounts);

        u64 total = 0;
        for(usz i = 0; i < 9; ++i) {
            total += counts[i];
        }
        if(day == 79) total1 = total;
        if(day == 255) total2 = total;
    }
    writeStdout(toString(total1) + "\n");
    writeStdout(toString(total2) + "\n");
}

void sort(MutArraySlice<i32> s, DynArray<i32>& tmp) {
    if(len(s) <= 1) {
        return;
    }
    MutArraySlice<i32> a = mutSlice(s, 0, len(s) / 2);
    MutArraySlice<i32> b = mutSlice(s, len(s) / 2);
    sort(a, tmp);
    sort(b, tmp);
    usz i = 0;
    usz j = 0;
    tmp.resize(0, 0);
    while(i < len(a) && j < len(b)) {
        if(a[i] < b[j]) {
            tmp.push(a[i++]);
        } else {
            tmp.push(b[j++]);
        }
    }
    while(i < len(a)) {
        tmp.push(a[i++]);
    }
    while(j < len(b)) {
        tmp.push(b[j++]);
    }
    for(usz k = 0; k < len(tmp); ++k) {
        s[k] = tmp[k];
    }
}

void run7(String input) {
    DynArray<String> elems = split(strip(input), ',');
    DynArray<i32> xs;
    for(usz i = 0; i < len(elems); ++i) {
        xs.push(fromString<i32>(elems[i]));
    }
    DynArray<i32> tmp;
    sort(mutSlice(xs), tmp);
    i32 mid = xs[len(xs) / 2];
    i32 ret = 0;
    for(usz i = 0; i < len(xs); ++i) {
        i32 d = xs[i] - mid;
        ret += (d < 0 ? -d : d);
    }
    writeStdout(toString(ret) + "\n");

    i32 best = -1;
    for(i32 p = xs[0]; p <= xs[len(xs) - 1]; ++p) {
        i32 cost = 0;
        for(usz i = 0; i < len(xs); ++i) {
            i32 d = xs[i] - p;
            d = (d < 0 ? -d : d);
            cost += d * (d + 1) / 2;
        }
        if(best == -1 || cost < best) {
            best = cost;
        }
    }
    writeStdout(toString(best) + "\n");
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
    } else if(day == "5") {
        String input = readStdin();
        run5(move(input));
    } else if(day == "6") {
        String input = readStdin();
        run6(move(input));
    } else if(day == "7") {
        String input = readStdin();
        run7(move(input));
    } else {
        writeStderr("Unknown day\n");
    }
}
