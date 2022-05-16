#include "nkea.h"
string moon[] = {
    R"(    _,--._    )",
    R"(  ,`   ,"``   )",
    R"( /    /       )",
    R"( |    '       )",
    R"( \     `.___,/)",
    R"(  `._     _,* )",
    R"(     `'"""`   )"
};
string sky[] = {R"(
                                       .                         _                            .                           
                                                                  -__ -_           
         .                                                 .                                                   +
                     *                                                                                                 |
                         .                                                                        '  
                                                                                         .      
                                                                                     
                                               .                                                          '
                                                                                      
                                                                                            .
)",
R"(
                                                                 _                            .                          
                                                                  -__ -_             
         .                                                 .                                                   *
                     +                                                                                                 |
                         .                                                                        '  
                                                                                         .      
                                                                                     
                                               .                                                          '
                                                                                      
                                                                                            .

)",
R"(
                                       .                                                      .                           
                                                                                     
         .                                                 .                                                   +
                     *                                                                                                 |
                         .                                                                        '  
                                                                                         .      
                                                                                     
                                               .                                                          '
                                                                                      
                                                                                            .
)",
R"(
                                       .                                                      .                           
                                                                                     
         .                                                 .                                                   *
                     +                                                                                                 |
                         .                                                                        '  
                                                                                         .      
                                                                                     
                                               .                                                          '
                                                                                      
                                                                                            .

)"
};
string art[] = {
R"(
                                       .                                                      .                           
                                                                                     
         .                                                 .                                                   +
                     .                                                                                                 |
                         .                                                                        '  
                                                                                         .      
             *                                                                       
                                               .                                                          '
                                                                                      
                                                                                            .
)",      
R"(
                                                                       
'--`\---` , - . ___    __                                                                   
                    ` \   ; - . , . . _ _                                                                  
                                          ! : ; , . .  ___ _ _)",
R"(
|   |     |   |   | 
|   |   ` | , |   |   |   |   | |   | ||||
|   |   \':'  |   | _ | . | _ ' ' `` `````  '''           ` ` `   
| _ | ,  ':, -----                                 _,. .' '              
         `''                          .. .. _, : ""'
                            , _ . , ; : 
                     ,  _ ; :
               __, -   |' 
      __ , - |  |  '
'  |`    |  
|
)"
};
string rightrock[]{
    R"(                                   _ ,--/)",
    R"(                          _  __.-.` / |  )",
    R"(                __  _..,-: /' / -:  ,-   )",
    R"( _ __....':;:!;/  ; '_,-  _,' ,'_,-|     )",
    R"(```..::;/;'/;' /_ ' /_;'_/ ;,-./  /   _  )",
    R"(        '"":;,-; ,     / `:/ ,-  ' . '   )",
    R"()"

};
string river[] = {
"   __    _          ",
"     -   _      __",
"  -__   __    -     ",
"     -     __-      ",
"        _ _     __"};
string graph[] = {
    R"(        /\        )",
    R"( _      \/       _)",
    R"((_____/\/\/\______))",
    R"( ______      ______)",
   R"((_     \/\/\/     _))",
    R"(         /\         )",
    R"(         \/         )"
};
void ThreadAnimation(){
    while (1) {
    //try on loop sky
        for (int i = 0; i < 5; i++) {
            setTextColor(15);
            GotoXY(0, 0);
            cout << (sky[i]);
            //draw moon
            setTextColor(14);
            for (int i = 0; i < 7; i++) {
                GotoXY(63, 4 + i);
                cout << moon[i];

            }
            Sleep(500);

        }
        
    }
}
void draw_gameover() {
    int x_over = 15, y_over = 15;
    setTextColor(15);
    cout << sky[0]; 
   setTextColor(7);
   GotoXY(0, 10);
   cout<<art[1];
   GotoXY(0, 12);
   setTextColor(8);
   cout << art[2];
   setTextColor(8);
   GotoXY(63, 14);
   cout << "__             __";
   setTextColor(7);
   for (int i = 0; i < 4; i++) {
       GotoXY(80, 11 + i);
       cout << rightrock[i];
   }
   GotoXY(80, 15 );
   for (int i = 0; i < rightrock[4].length(); i++) {
       if (rightrock[4][i] == ';' || rightrock[4][i] == ':')
           setTextColor(8);
       else setTextColor(7);
       cout << rightrock[4][i];
   }
   setTextColor(14);
   int river_num = sizeof(river) / sizeof(river[0]);
   for (int i = 0; i < river_num; i++) {
       GotoXY(63, 15 + i);
       cout << river[i];
   }
   setTextColor(15);
   for (int i = 0; i < 3; i++) {
       GotoXY(x_over, y_over + i);
       cout << graph[i];
   }
   for (int i = 3; i < 7; i++) {
       GotoXY(x_over -1, y_over + i+8);
       cout << graph[i];
   }
   //draw moon
   setTextColor(14);
   for (int i = 0; i < 7; i++) {
       GotoXY(63, 4 + i);
       cout << moon[i];

   }
}