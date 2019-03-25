//  char **codes = new char*[4];
#include <Arduino.h>

#define menu_next 1
#define menu_prev 2
#define menu_event 3
#define menu_back 4
#define menu_home 5
#define menu_call 8


class MenuItem {
  public:
  char *name;
  MenuItem **childs; 
  uint8_t _madded=0, current=0;
  uint8_t _nchild=0;
  MenuItem *parent=NULL;
  MenuItem(){
    //default constructor.
  }
  MenuItem(char *nm) {
    name=nm;
  }
  
 void appendChild(MenuItem *mi) {
    mi->parent=this; 
    MenuItem **ch2=new MenuItem*[_nchild+1];
    for(int i=0;i<_nchild;i++) {
      ch2[i]=childs[i];
    }
    ch2[_nchild]=mi;
    childs=ch2;
    _nchild++;
  }
  /*
   * num: number of arguments.
   * ...: list of pointers. 
   */
  void setChild(int num, ...) {   
    va_list argList;
    va_start(argList, num);
    
     for(; num; num--) {
       int valu = va_arg(argList,  int); 
       appendChild(valu);               //NOTE: pointers are integer, here pointers of MenuItems are passed.
    }   
    va_end(argList);
   }
    
  MenuItem *getCurrent(){ 
    return childs[current];
  }
  MenuItem *next(){
    ++current;
    if(current>=_nchild-1) {
      current=_nchild-1;
    }
    return getCurrent();
  }
   MenuItem *prev(){ 
    if(current>0)   {
      --current;
    }
    return getCurrent();
  }

  void print_childs() {
    Serial.println("__");
    for(uint8_t i=0;i<_nchild;i++) {
      char ch=' ';
      if(i==current) ch='*';
      Serial.print("[");
      Serial.print(ch);
      Serial.print("] "); 
      Serial.println(childs[i]->name);
    }
  }



  
};

 



MenuItem *root;

MenuItem rt("Main");
MenuItem mi("SMS"), mi2("Call"), mi3("Family");
MenuItem mi11("Inbox"), mi12("Sent"), mi13("Info");
MenuItem mi21("InCall"), mi22("OutCall");

void setup() {
  Serial.begin(9600);
  Serial.println("Ready");
  
   mi.setChild(3, &mi11, &mi12, &mi13);   
   mi2.setChild(2, &mi21, &mi22);
   rt.setChild(3, &mi, &mi2, &mi3);

  root=&rt;
  Serial.println(root->name);
  root->print_childs();
  Serial.println("\n__OK__");
}

void onMenu(int sp) {
  if(sp==menu_next) {
        root->next();
        root->print_childs();
      }else if(sp==menu_prev) {
        root->prev();
        root->print_childs();
      }
      else if(sp==menu_event) {
//        Serial.println("open");
        MenuItem *mi=root->getCurrent(); 
        root->print_childs();
        if(mi->_nchild>0) {
//          Serial.println("HAS CHILD");
          root=mi;
          root->print_childs();
        }else {
          Serial.println("Event="+ String(mi->name));
        }
        
      }
      else if(sp==menu_back) {
          Serial.println("back"); 
          if(root->parent==NULL) {
            Serial.println("NO_PARENT");
          }else {   
            root=root->parent;
            root->print_childs();
          }
      }
}

void loop() {
   int sp=getPressed_serial();
   if(sp) {
////    Serial.println("emulated p="+String(sp));
      onMenu(sp);
   } 
   
}


/*
 * emulate keypress.
 */
int getPressed_serial() {
  if(Serial.available() ) {
      char ch=Serial.read();
      int d=ch-'0';
      return d;
  }
  return 0;
}
