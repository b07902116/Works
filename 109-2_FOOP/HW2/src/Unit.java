import java.util.*;

public abstract class Unit {
    private int HP, MP, STR, troop_index, unit_index;
    private int dot, extra_dmg, duration;
    private boolean can_move;
    private String name, state;
    private List<String> action_list;
    private Set<Integer> curse_set;
    public Unit(int troop_index, int unit_index, String[] tokens){
        this.troop_index = troop_index;
        this.unit_index = unit_index;
        this.name = tokens[0];
        this.HP = Integer.parseInt(tokens[1]);
        this.MP = Integer.parseInt(tokens[2]);
        this.STR = Integer.parseInt(tokens[3]);
        this.action_list = new ArrayList<>();
        this.action_list.add("Basic Attack");
        if (tokens.length > 4){
            for (int i = 4; i < tokens.length; i++){
                this.action_list.add(tokens[i]);
            }
        }
        this.state = "Normal";
        this.duration = 0;
        this.can_move = true;
        this.dot = 0;
        this.extra_dmg = 0;
        this.curse_set = new HashSet<>();
    }

    public int getHP(){
        return HP;
    }
    public int getMP(){
        return MP;
    }
    public int getSTR(){
        return STR;
    }
    public boolean getCan_move(){
        return can_move;
    }
    public int getDot(){
        return dot;
    }
    public int getExtra_dmg(){
        return extra_dmg;
    }
    public int getTroop_index(){
        return troop_index;
    }
    public int getUnit_index(){
        return unit_index;
    }
    public String getName(){
        return String.format("[%d]%s", troop_index, name);
    }
    public String getState(){
        return state;
    }
    public List<String> getAction_list(){
        return action_list;
    }
    public Set<Integer> getCurse_list(){
        return this.curse_set;
    }
    public void setHP(int HP){
        this.HP = HP;
    }
    public void setMP(int MP){
        this.MP = MP;
    }
    public void setSTR(int STR){
        this.STR = STR;
    }
    public void setCan_move(boolean can_move){
        this.can_move = can_move;
    }
    public void setDot(int dot){
        this.dot = dot;
    }
    public void setExtra_dmg(int extra_dmg){
        this.extra_dmg = extra_dmg;
    }
    public void setState(String state){
        this.state = state;
    }
    public void setDuration(int duration){
        this.duration = duration;
    }

    public void OneRoundPass(){
        if (this.duration > 0){
            this.duration--;
            if (this.duration == 0){
                this.state = "Normal";
                this.can_move = true;
                this.dot = 0;
                this.extra_dmg = 0;
            }
        }
    }
    public void DeathTask(Troop[] troops){
        System.out.printf("%s dies.\n", this.getName());
        List<Integer> curse_list = new ArrayList<>();
        curse_list.addAll(this.curse_set);
        Troop enemy_troop = troops[this.troop_index % 2];
        for (int k = 0; k < curse_list.size(); k++){
            Unit unit = enemy_troop.getUnit(curse_list.get(k));
            if (unit.getHP() > 0){
                unit.setHP(unit.getHP() + this.MP);
            }
        }
    }
    public void ShowUnitInfo(){
        System.out.printf("[%d]%s's turn (HP: %d, MP: %d, STR: %d, State: %s).\n", troop_index, name, HP, MP, STR, state);
    }
    public void ShowActionList(){
        System.out.printf("Select an action:");
        for (int i = 0; i < getAction_list().size(); i++){
            System.out.printf(" (%d) %s", i, action_list.get(i));
        }
        System.out.println();
    }
    public abstract Action selectAction(Map<String, Action> actionmap);
    public abstract List<Integer> selectTargets(Action action, Troop[] troops);

}
