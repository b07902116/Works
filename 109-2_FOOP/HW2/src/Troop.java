import java.util.ArrayList;
import java.util.List;

public class Troop {
    private int troop_index, unit_num, living_unit_num;
    private List<Unit> units;

    public Troop(int index){
        this.troop_index = index;
        this.unit_num = 0;
        this.units = new ArrayList<>();
    }
    public void AddUnit(String[] tokens){
        Unit unit;
        if ("Hero".equals(tokens[0]))
            unit = new Hero(this.troop_index, this.unit_num, tokens);
        else
            unit = new AIUnit(this.troop_index, this.unit_num, tokens);
        units.add(unit);
        this.unit_num++;
        this.living_unit_num++;
    }
    public Unit getUnit(int unit_index){
        return this.units.get(unit_index);
    }
    public int getUnit_num(){
        return this.unit_num;
    }
    public int getLiving_unit_num(){
        return this.living_unit_num;
    }
    public List<Unit> LivingUnitList(){
        List<Unit> living_units = new ArrayList<>();
        for (int i = 0; i < this.getUnit_num(); i++){
            Unit unit = this.getUnit(i);
            if (unit.getHP() > 0){
                living_units.add(unit);
            }
        }
        return living_units;
    }

    public void UpdateLivingUnitNum(){
        int living_unit_num = 0;
        for (int i = 0; i < this.getUnit_num(); i++){
            if (this.getUnit(i).getHP() > 0){
                living_unit_num++;
            }
        }
        this.living_unit_num = living_unit_num;
    }

}
