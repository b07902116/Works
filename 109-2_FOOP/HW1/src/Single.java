public class Single extends PatternCard{
    public int[] ValidCheck(Player player, int[] indexes){
        int[] arr = new int[2];
        if (indexes.length == 1){
            if (indexes[0] == -1){
                arr[0] = 1; // Pass
                arr[1] = -1;
            }
            else{
                arr[0] = 2; // Single
                arr[1] = player.GetHandcard(indexes[0]).Get().ordinal();
            }
        }
        else{
            arr[0] = 0; // Invalid
            arr[1] = -1;
        }
        return arr;
    }
    public int GetType(){
        return 2;
    }
    public void PrintPlayMsg(Player player, int[] indexes){
        System.out.printf("Player %s plays a single %s.\n", player.GetName(), player.GetHandcard(indexes[0]).GetOutputFormat());
    }
}
