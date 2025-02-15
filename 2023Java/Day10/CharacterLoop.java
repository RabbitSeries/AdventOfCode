package Day10;

import java.io.*;
import java.util.*;
import java.util.AbstractMap.SimpleEntry;
import java.util.Map.Entry;
import java.util.stream.*;
import Day10.PipeModel.*;
import DataModel.Point2D;

public class CharacterLoop {
    List<List<Character>> PipeMap;
    int rows, cols;
    Point2D StartPos;

    void readFile() throws IOException {
        PipeMap = new ArrayList<>();
        BufferedReader input = new BufferedReader(new FileReader("input.txt"));
        String buf;
        while ((buf = input.readLine()) != null) {
            if (buf.indexOf("S") != -1) {
                StartPos = new Point2D(PipeMap.size(), buf.indexOf("S"));
            }
            PipeMap.add(buf.chars().mapToObj(l -> (char) l).toList());
        }
        rows = PipeMap.size();
        cols = PipeMap.get(0).size();
        input.close();
    }

    void PrintMap(List<List<Character>> CharacterMap) {
        CharacterMap.forEach(line -> {
            line.forEach(c -> {
                System.out.printf("%c", !c.equals('*') ? ' ' : c);
            });
            System.out.println();
        });
    }

    void PrintPath(List<Point2D> pathList, List<List<Character>> CharacterMap) {
        // new ArrayList<>(CharacterMap) means copy CharacterMap's value (second level
        // reference) but the underlying value is not copied to a new memory space;
        // List<List<Character>> printMap = new ArrayList<>(CharacterMap);
        List<List<Character>> printMap = new ArrayList<>();
        for (List<Character> row : CharacterMap) {
            // Deep copy instead of shallow copy.
            printMap.add(new ArrayList<>(row));
        }
        for (Point2D p : pathList) {
            printMap.get(p.getKey()).set(p.getValue(), '*');
        }
        printMap.get(StartPos.getKey()).set(StartPos.getValue(), 'S');
        printMap.forEach(line -> {
            line.forEach(c -> {
                System.out.printf("%c", c.equals('S') ? 'S' : c.equals('*') ? '*' : ' ');
            });
            System.out.println();
        });
    }

    Character getPipe(Point2D curPos) {
        return PipeMap.get(curPos.getKey()).get(curPos.getValue());
    }

    boolean isValid(Point2D curPos) {
        return Point2D.isValid(rows, cols, curPos) && PipeMap.get(curPos.getKey()).get(curPos.getValue()) != '.';
    }

    List<Point2D> Solution1() throws IOException, InterruptedException {
        readFile();
        Queue<Entry<Point2D, Integer>> q = new LinkedList<>();
        HashMap<Entry<Point2D, Integer>, Boolean> visited = new HashMap<>();

        for (int i = 0; i < 4; i++) {
            Point2D nextPos = Point2D.getNextPosition(StartPos, i);

            if (isValid(nextPos) && PipeModel.NextPipeDirection.get(getPipe(nextPos)).getOrDefault(i, -1) != -1) {
                // Start of path, ClockOrder is null;
                q.add(new SimpleEntry<>(nextPos, i));
                visited.put(new SimpleEntry<>(nextPos, i), true);
            }
        }

        int loopLen = 1;
        // BFS
        while (!q.isEmpty()) {
            var curLevelList = q.stream().toList();
            q.clear();
            for (var front : curLevelList) {
                // Level info
                Point2D curPos = front.getKey();
                int curFace = front.getValue();

                // Path process
                if (curPos.pathList == null) {
                    curPos.pathList = new ArrayList<>();
                }
                curPos.pathList.add(curPos.extractPosPoint2D());

                // Destination process
                if (curPos.equals(StartPos)) {
                    System.out.println("Solution 1: " + loopLen / 2);
                    return new ArrayList<>(
                            curPos.pathList.stream().map(entry -> new Point2D(entry.getKey(), entry.getValue()))
                                    .toList());
                }

                // Construct nextPos info
                int nextFace = PipeModel.NextPipeDirection.get(getPipe(curPos)).getOrDefault(curFace, -1);
                if (nextFace != -1) {
                    Point2D nextPos = Point2D.getNextPosition(curPos, nextFace);

                    // Enqueue
                    if (isValid(nextPos) && !visited.getOrDefault(new SimpleEntry<>(nextPos, nextFace), false)) {
                        nextPos.pathList = new ArrayList<>(curPos.pathList);
                        q.add(new SimpleEntry<>(nextPos, nextFace));
                        visited.put(new SimpleEntry<>(nextPos, nextFace), true);
                    }
                }
            }
            loopLen++;
        }
        return null;
    }

    static ClockOrder PathClockOrder = null;
    static boolean innerBlock = true;

    int innerFlood(Point2D s, HashMap<Point2D, Boolean> visited) {
        Queue<Point2D> q = new LinkedList<>();
        q.add(s);
        int cellCnt = 0;
        while (!q.isEmpty()) {
            Point2D curPos = q.poll();
            cellCnt++;
            for (int i = 0; i < 4; i++) {
                Point2D nextPos = Point2D.getNextPosition(curPos, i);
                if (Point2D.isValid(rows, cols, nextPos)) {
                    if (!visited.containsKey(nextPos)) {
                        visited.put(nextPos, true);
                        q.add(nextPos);
                    }
                } else {
                    innerBlock = false;
                }
            }
        }
        if (innerBlock) {
            return cellCnt;
        }
        return 0;
    }

    void Solution2(List<Point2D> pathList) {
        for (int i = 0; i < 2; i++) {
            var QueryModel = i == 0 ? PipeModel.ClockwiseQuery : PipeModel.CounterclockwiseQuery;
            HashMap<Point2D, Boolean> visited = new HashMap<>();
            for (Point2D curPos : pathList) {
                visited.putIfAbsent(curPos, true);
            }
            int res = 0;
            Point2D prePos = null;
            for (Point2D curPos : pathList) {
                if (curPos.equals(StartPos)) {
                    System.out.println("Solution 2: " + res);
                    return;
                }
                if (prePos != null) {
                    for (var adjPos : getInnerAdjacents(curPos, prePos, QueryModel, visited)) {
                        res += innerFlood(adjPos, visited);
                        if (!innerBlock) {
                            break;
                        }
                    }
                }
                prePos = curPos;
            }
            innerBlock = true;
        }
    }

    List<Point2D> getInnerAdjacents(Point2D curPos, Point2D prePos,
            HashMap<Character, HashMap<Integer, List<Integer>>> QueryModel, HashMap<Point2D, Boolean> visited) {
        int inDirection = -1;
        int dx = curPos.getKey() - prePos.getKey(), dy = curPos.getValue() - prePos.getValue();
        if (dx == 0) {
            inDirection = (dy == 1 ? 2 : 3);
        } else {
            inDirection = (dx == 1 ? 0 : 1);
        }
        List<Point2D> PointList = new ArrayList<>();
        for (int v : QueryModel.get(getPipe(curPos)).get(inDirection)) {
            Point2D nextPos = Point2D.getNextPosition(curPos, v);
            if (Point2D.isValid(rows, cols, nextPos) && !visited.containsKey(nextPos)) {
                visited.put(nextPos, true);
                PointList.add(nextPos);
            }
        }
        return PointList;
    }

    public static void main(String[] args) throws IOException, InterruptedException {
        CharacterLoop Day10 = new CharacterLoop();
        List<Point2D> pathList = Day10.Solution1();
        Day10.Solution2(pathList);
    }
}
