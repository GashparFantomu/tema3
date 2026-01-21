
%token id
%left '+'
%left '-'  
%start E   

%%

E : E '+' T    
  | E '-' T
  | T          
  ;

T : T '*' F
  | F
  ;

F : '(' E ')'
  | id
  ;


%%