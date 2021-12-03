public class Pair extends PatternCard{
    public int[] ValidCheck(Player player, int[] indexes){
        int[] arr = new int[2];
        if (indexes.length == 2){
            int[] rank = new int[2];
            rank[0] = player.GetHandcard(indexes[0]).Get().ordinal() / 4;
            rank[1] = player.GetHandcard(indexes[1]).Get().ordinal() / 4;
            if (rank[0] == rank[1]){
                arr[0] = 3; // Pair
                arr[1] = player.GetHandcard(indexes[1]).Get().ordinal();
            }
            else{
                arr[0] = 0; // Invalid
                arr[1] = -1;
            }
        }
        else{
            arr[0] = 0; // Invalid
            arr[1] = -1;
        }
        return arr;
    }
    public int GetType(){
        return 3;
    }

    public void PrintPlayMsg(Player player, int[] indexes){
        System.out.printf("Player %s plays a pair %s %s.\n", player.GetName(), player.GetHandcard(indexes[0]).GetOutputFormat(), player.GetHandcard(indexes[1]).GetOutputFormat());
    }
}
