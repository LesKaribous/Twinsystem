#include "condition.h"
#include "os.h"

String symbols[9]{
    "&&",
    "||",
    "==",
    "!=",
    "!",
    "<=",
    ">=",
    "<",
    ">"
};

String solveOrder[9]{
    "<",
    ">",
    "<=",
    ">=",
    "!",
    "==",
    "!=",
    "&&",
    "||"
};


int findClosestOperator(const String& str, int index, String& buf){
    int min = str.length();
    //THROW(str)
    for(String test : symbols){
        int r = str.indexOf(test, index);
        if(r < min && r != -1){
            min = r;
            buf = test;
        }
    }

    if(min == str.length()) return -1;
    else return min;
}

void parseCondition(String& raw, std::vector<Command>& args, std::vector<String>& operators){
    // Count the operators in condition
    int i = 0;
    int argc = 1;

    String operatorBuf;
    while(true){
        i = findClosestOperator(raw, i, operatorBuf);
        //THROW(i)
        if(i != -1){
            //THROW(operatorBuf)
            operators.push_back(operatorBuf);
            if(operatorBuf != "!") argc++;
            i++;
        }else break;
        
    }; 

    String subC = raw;
    for(String test : symbols){
        if(test == "!") subC.replace(test, "");
        else subC.replace(test, ",");
    }
    subC.replace(" ", "");

    int start = 0;
    int end = subC.indexOf(',', start);
    THROW(subC)
    // Find the start and end position of the argument at the specified index
    for (int i = 0; i < argc; i++) {
        if (end != -1){
            args.emplace_back(subC.substring(start, end).trim());
            start = end + 1;
            end = subC.indexOf(',', start);
        }

    }
    args.emplace_back(subC.substring(start, end).trim());
}


bool evaluate(String&  a, String& b, String op){
    if(op == "=="){
        return a.equalsIgnoreCase(b);
    }else if(op == "!="){
        return !a.equalsIgnoreCase(b);
    }else if(op == "<"){
        return a.toFloat() < b.toFloat(); 
    }else if(op == ">"){
        return a.toFloat() > b.toFloat(); 
    }else if(op == "<="){
        return a.toFloat() <= b.toFloat(); 
    }else if(op == ">="){
        return a.toFloat() >= b.toFloat(); 
    }else if(op == "&&"){
        return bool(a.toInt()) && bool(b.toInt()); 
    }else if(op == "||"){
        return bool(a.toInt()) || bool(b.toInt()); 
    }
    return "0";
}


void Condition::solve(){

    if(outputs.size() != commands.size()){
        THROW("ERROR : Condition not evaluated yet. Cannot solve")
        os.console.error("Condition") << "Condition not evaluated yet. Cannot solve" << os.console.endl;
        return;
    }
    

    std::vector<String> fix;

    for(String& cur : operators){
        if(cur != "!")fix.push_back(cur);
    }


    for(String& cur : solveOrder){
        THROW("Solving : " + cur);
        for(int i = 0; i < fix.size(); i++){
            if( cur != "!"){
                String test = fix[i];
                if(test == cur){
                    int ia = i;
                    int ib = i+1;
                    THROW("ia : " + String(ia));
                    THROW("ib : " + String(ib));
                    outputs[ia] = outputs[ib] = String(evaluate( outputs[ia], outputs[ib], cur));
                }
            }else{
                outputs[i] =  String(!bool(outputs[i].toInt())); // TODO : The problem is here
                i++;
            }
        }
    }

    os.console.print("Result : {");
    for(String& d:  outputs) os.console.print(d + ",");
    os.console.println("}");

}


Condition::Condition(String raw){
    parseCondition(raw, commands, operators);
}

Condition::~Condition(){
}