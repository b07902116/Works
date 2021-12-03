public class Fullhouse extends PatternCard{
    public int[] ValidCheck(Player player, int[] indexes){
        int[] arr = new int[5];
        if (indexes.length == 5){
            int[] rank = new int[5];
            for (int i = 0; i < 5; i++){
                rank[i] = player.GetHandcard(indexes[i]).Get().ordinal() / 4;
            }
            if ((rank[0] == rank[1] && rank[1] == rank[2] && rank[3] == rank[4]) ||
                (rank[0] == rank[1] && rank[2] == rank[3] && rank[3] == rank[4])){
                arr[0] = 4; // Fullhouse
                arr[1] = player.GetHandcard(indexes[3]).Get().ordinal();
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
        return 4;
    }

    public void PrintPlayMsg(Player player, int[] indexes){
        StringBuilder cards = new StringBuilder();
        for (int i = 0; i < 5; i++){
            String card = player.GetHandcard(indexes[i]).GetOutputFormat();
            cards.append(card).append(" ");
        }
        System.out.printf("Player %s plays a full house %s.\n", player.GetName(), cards.toString().stripTrailing());
    }
}
