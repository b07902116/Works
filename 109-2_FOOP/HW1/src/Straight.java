public class Straight extends PatternCard{
    public int[] ValidCheck(Player player, int[] indexes){
        int[] arr = new int[5];
        if (indexes.length == 5){
            int[] rank = new int[5];
            boolean isstraight = false;
            for (int i = 0; i < 5; i++){
                rank[i] = player.GetHandcard(indexes[i]).Get().ordinal() / 4;
            }
            for (int i = 0; i < 5; i++){
                isstraight = true;
                for (int j = 1; j < 5; j++){
                    if (((rank[i] + j) % 13) != rank[(i + j) % 5]){
                        isstraight = false;
                    }
                }
                if (isstraight) {
                    arr[0] = 5; // Straight
                    arr[1] = player.GetHandcard(indexes[4]).Get().ordinal();
                    break;
                }
            }
            if (!isstraight){
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
        return 5;
    }

    public void PrintPlayMsg(Player player, int[] indexes){
        StringBuilder cards = new StringBuilder();
        for (int i = 0; i < 5; i++){
            String card = player.GetHandcard(indexes[i]).GetOutputFormat();
            cards.append(card).append(" ");
        }
        System.out.printf("Player %s plays a straight %s.\n", player.GetName(), cards.toString().stripTrailing());
    }
}
