package DataModel;

import java.util.Map.*;
import java.util.AbstractMap.SimpleEntry;
import java.util.*;

public class Point2D extends SimpleEntry<Integer, Integer> {

    public Point2D(int _x, int _y) {
        super(_x, _y);
    }

    public List<Entry<Integer, Integer>> pathList;

    public Entry<Integer, Integer> extractPosPair() {
        return new SimpleEntry<>(getKey(), getValue());
    }

    // @Override
    // public boolean equals(Object object) {
    // if (this == object)
    // return true;
    // if (object == null || getClass() != object.getClass())
    // return false;
    // Point2D p = (Point2D) object;
    // return p.getKey().equals(getKey()) && p.getValue().equals(getValue());
    // }

    // @Override
    // public int hashCode() {
    // return Objects.hash(getKey(), getValue());
    // }

    public static boolean isValid(int rows, int cols, Point2D coordinate) {
        return coordinate.getKey() >= 0 && coordinate.getKey() < rows && coordinate.getValue() >= 0
                && coordinate.getValue() < cols;
    }

    public static Point2D getNextPosition(Point2D curPos, int index) {
        return new Point2D(curPos.getKey() + dx[index], curPos.getValue() + dy[index]);
    }

    // up down left right lu ld ru rd
    public static int[] dy = new int[] { 0, 0, -1, 1, -1, -1, 1, 1 };
    public static int[] dx = new int[] { -1, 1, 0, 0, -1, 1, -1, 1 };

}
