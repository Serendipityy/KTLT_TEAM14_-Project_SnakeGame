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
string river[] = {
       "   _     _   ",
       "    -    _   ", 
       "_   -  _-   -",
       "  -_    _-   _",
       "   _    - _  _",
       "    _   --   _",
       " - _    -_   -",
       "       _      ",
       "_      -  _   ",
       "    -   _     ",
       "      -       ",
       "    _     - _" ,
       "     _       " , 
       " _   - _     " , 
       "   _ -      -" ,  
      "            - "};
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
    R"(        '"":;,-; ,     / `:/ ,-__' . '  /)",
    R"(              `";:,` - . ,-: ,`  `.    | )",
    R"(                   ``  ,-\  '_   _ :    \)",
    R"(                       ` ',-: `_/    _   )",
    R"(                             '/  ` ,-:_' )",
    R"(                               /`. ,-' -.)"
};
string congrats[] = {
R"(                                                                   ,---.)",
R"( ,-----.                                              ,--.         |   | )",
R"('  .--./ ,---. ,--,--,  ,---.        ,--.--. ,--,--.,-'  '-. ,---. |  .' )",
R"(|  |    | .-. ||      \| .-. |       |  .--'' ,-.  |'-.  .-'(  .-' |  |  )",
R"('  '--'\' '-' '|  ||  |' '-' '       |  |   \ '-'  |  |  |  .-'  `)`--'  )",
R"( `-----' `---' `--''--'.`-  /        `--'    `--`--'  `--'  `----' .--.  )",
R"(                       `---'                                       '--'  )"};
string graph[] = {
    R"(        /\        )",
    R"( _      \/       _)",
    R"((_____/\/\/\______))",
    R"( ______      ______)",
   R"((_     \/\/\/     _))",
    R"(         /\         )",
    R"(         \/         )"
};

//Hiệu ứng bầu trời
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

//Vẽ màn hình chờ game over
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
   int right_rock_num = sizeof(rightrock) /sizeof(rightrock[0]);
   for (int j = 4; j < right_rock_num; j++) {
       GotoXY(80, 11+j);
       for (int i = 0; i < rightrock[j].length(); i++) {
           if (rightrock[j][i] == ';' || rightrock[j][i] == ':' || rightrock[j][i]=='`'||rightrock[j][i]=='\\'||rightrock[j][i]=='`'||rightrock[j][i]=='"')
               setTextColor(8);
           else setTextColor(7);
           cout << rightrock[j][i];
       }
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

//Vẽ kết thúc game
void Draw_endgame() {
     string moon[] = {
    R"(    _,--._    )",
    R"(  ,`   ,"``   )",
    R"( /    /  -_ _ )",
    R"( |    '  _- _ )",
    R"( \     `.___,/)",
    R"(  `._     _,* )",
    R"(     `'"""`   )"
    };
    int x_over = 15, y_over = 15;
    setTextColor(15);
    cout << sky[0];
    setTextColor(7);
    GotoXY(0, 10);
    cout << art[1];
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
    int right_rock_num = sizeof(rightrock) / sizeof(rightrock[0]);
    for (int j = 4; j < right_rock_num; j++) {
        GotoXY(80, 11 + j);
        for (int i = 0; i < rightrock[j].length(); i++) {
            if (rightrock[j][i] == ';' || rightrock[j][i] == ':' || rightrock[j][i] == '`' || rightrock[j][i] == '\\' || rightrock[j][i] == '`' || rightrock[j][i] == '"')
                setTextColor(8);
            else setTextColor(7);
            cout << rightrock[j][i];
        }
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
        GotoXY(x_over - 1, y_over + i + 8);
        cout << graph[i];
    }
    //draw moon
    setTextColor(14);
    int num = sizeof(congrats) / sizeof(congrats[0]);
    for (int i = 0; i < num; i++)
    {
        GotoXY(40, 4 + i);
        cout << congrats[i];
    }
    for (int i = 0; i < 7; i++) {
        GotoXY(63, 4 + i);
        cout << moon[i];

    }
}
