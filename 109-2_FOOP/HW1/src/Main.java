/**
 * The big-2 game will be started from the Main class.
 */
public class Main {
    public static void main(String[] args) {
        GameManager GM = new GameManager();
        PatternCard single = new Single();
        PatternCard pair = new Pair();
        PatternCard fullhouse = new Fullhouse();
        PatternCard straight = new Straight();
        GM.GameStart(single, pair, fullhouse, straight);
    }
}
