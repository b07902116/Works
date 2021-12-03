import java.util.Arrays;

public class Player {
    private String name;
    private int handcard_num;
    private Card[] handcard;

    public Player(String inName){
        handcard = new Card[13];
        this.handcard_num = 0;
        this.name = inName;
    }

    public int GetHandcardNum(){
        return this.handcard_num;
    }
    public String GetName(){
        return this.name;
    }
    public Card GetHandcard(int index){
        if (index >= handcard_num){
            return null;
        }
        return handcard[index];
    }
    public void DealCard(Card inCard){
        handcard[handcard_num] = new Card(inCard.Get());
        handcard_num++;
    }
    public void SortCard(){
        // sort cards according cards' ordinal value
        Arrays.sort(handcard, (o1, o2) -> {
            if (o1.Get().ordinal() > o2.Get().ordinal())
                return 1;
            else
                return -1;
        });
    }

    public void ValidReleaseCard(int[] indexes){
        int out_num = indexes.length;
        for (int i = 0; i < out_num; i++){
            this.handcard[indexes[i]].Set(Card.SuitAndRank.NONE);
        }
        this.handcard_num -= out_num;
    }
    public boolean IsWin(){
        return handcard_num == 0;
    }


}
