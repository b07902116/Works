/*
Invalid : 0
Pass : 1
Single : 2
Pair : 3
Full house : 4
Straight : 5
Other new pattern can be appended
 */

abstract class PatternCard {
    abstract int[] ValidCheck(Player player, int[] indexes);
    abstract int GetType();
    abstract void PrintPlayMsg(Player player, int[] indexes);

    static public boolean IsInvalid(int type){
        return type == 0;
    }
    static public boolean IsPass(int type){
        return type == 1;
    }
    static public boolean CanBeat(int[] now_play, int[] top_play){
        if (now_play[0] == top_play[0]){
            if (now_play[1] > top_play[1]){
                return true;
            }
        }
        return false;
    }

}
