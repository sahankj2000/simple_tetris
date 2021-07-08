#include<iostream>
#include<GL/glut.h>
#include<stdlib.h>
using namespace std;

int menuInt=0;
int gameInt=0;
bool paused=false;
const int R=30;  // Till R=80 and C=40 worked well
const int C=15;
const int W=350;
const int H=700;
int tim=250;           // 1 sec = 1000tim
int mat[R][C];
bool fresh=false;

void menu();

void clearLine(int index){
    for(int i=index;i<R-1;i++){
        for(int j=0;j<C;j++){
            mat[i][j]=mat[i+1][j];
        }
    }
}

void lineClean(){
    for(int i=0;i<R;i++){
        bool cleared=true;
        for(int j=0;j<C;j++){
            if(mat[i][j]==0){
                cleared=false;
                break;
            }
        }
        if(cleared){
            clearLine(i);
            glutPostRedisplay();
        }
    }
}

float* color(int x){
    static float c0[3]={0.0,0.0,0.0};
    static float c1[3]={1.0,0.0,0.0};
    static float c2[3]={0.0,1.0,0.0};
    static float c3[3]={0.0,0.0,1.0};
    static float c4[3]={0.0,1.0,1.0};
    static float c5[3]={1.0,0.0,1.0};
    static float c6[3]={1.0,1.0,0.0};
    switch(x){
        case 1: return c1;
                break;
        case 2: return c2;
                break;
        case 3: return c3;
                break;
        case 4: return c4;
                break;
        case 5: return c5;
                break;
        case 6: return c6;
                break;
        default: return c0;
    }
}

class block{
    public:
        bool out[4][4],set,init=false;
        int s,p,c,x,y,sc;

        void project(){
            for(int i=0;i<4;i++){
                for(int j=0;j<4;j++){
                    if(out[i][j]){
                        mat[x+i][y+j]=c;
                    }
                }
            }
        }

        bool clash(){
            bool temp[R+2][C+2];
            int tx=x+1;
            int ty=y+1;
            for(int i=0;i<R+2;i++){
                for(int j=0;j<C+2;j++){
                    temp[i][j]=false;
                }
            }
            for(int i=0;i<R+2;i+=(R+1)){
                for(int j=0;j<C+2;j++){
                    temp[i][j]=true;
                }
            }
            for(int i=0;i<R+2;i++){
                for(int j=0;j<C+2;j+=(C+1)){
                    temp[i][j]=true;
                }
            }
            for(int i=1;i<=R;i++){
                for(int j=1;j<=C;j++){
                    if(mat[i-1][j-1]!=0){
                        temp[i][j]=true;
                    }
                }
            }
            for(int i=0;i<4;i++){
                for(int j=0;j<4;j++){
                    if(out[i][j]){
                        if(temp[tx+i][ty+j]){
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        bool blockDown(){
            cleanBlock();
            x-=1;
            if(!clash()){
                project();
            }
            else{
                x+=1;
                project();
                return false;
            }
            return true;
        }

        bool blockLeft(){
            cleanBlock();
            y-=1;
            if(!clash()){
                project();
            }
            else{
                y+=1;
                project();
                return false;
            }
            return true;
        }

        bool blockRight(){
            cleanBlock();
            y+=1;
            if(!clash()){
                project();
            }
            else{
                y-=1;
                project();
                return false;
            }
            return true;
        }

        bool blockUp(){
            int op=p;
            p=(p%4)+1;
            outClean();
            cleanBlock();
            drawOut();
            if(!clash()){
                project();
            }
            else{
                p=op;
                outClean();
                cleanBlock();
                drawOut();
                project();
                return false;
            }
            return true;
        }

        void outClean(){
            for(int i=0;i<4;i++){
                for(int j=0;j<4;j++){
                    out[i][j]=false;
                }
            }
        }

        void drawOut(){
            switch(s){
                case 1:
                    switch(p){
                        case 1:
                        case 3:
                            out[0][1]=true;
                            out[1][0]=true;
                            out[1][1]=true;
                            out[2][0]=true;
                        break;
                        case 2:
                        case 4:
                            out[0][0]=true;
                            out[0][1]=true;
                            out[1][1]=true;
                            out[1][2]=true;
                        break;
                        default: cout<<"Error in position switch"<<endl;
                    }
                break;
                case 2:
                    switch(p){
                        case 1:
                        case 3:
                            out[0][0]=true;
                            out[1][0]=true;
                            out[1][1]=true;
                            out[2][1]=true;
                        break;
                        case 2:
                        case 4:
                            out[0][1]=true;
                            out[0][2]=true;
                            out[1][0]=true;
                            out[1][1]=true;
                        break;
                        default: cout<<"Error in position switch"<<endl;
                    }
                break;
                case 3:
                case 4:
                    switch(p){
                        case 1:
                        case 3:
                            out[0][1]=true;
                            out[1][1]=true;
                            out[2][1]=true;
                            out[3][1]=true;
                        break;
                        case 2:
                        case 4:
                            out[1][0]=true;
                            out[1][1]=true;
                            out[1][2]=true;
                            out[1][3]=true;
                        break;
                        default: cout<<"Error in position switch"<<endl;
                    }
                break;
                case 5:
                case 6:
                    switch(p){
                        case 1:
                        case 2:
                        case 3:
                        case 4:
                            out[0][0]=true;
                            out[0][1]=true;
                            out[1][0]=true;
                            out[1][1]=true;
                        break;
                        default: cout<<"Error in position switch"<<endl;
                    }
                break;
                case 7:
                    switch(p){
                        case 1:
                            out[0][1]=true;
                            out[0][2]=true;
                            out[1][1]=true;
                            out[2][1]=true;
                        break;
                        case 2:
                            out[1][0]=true;
                            out[1][1]=true;
                            out[1][2]=true;
                            out[2][2]=true;
                        break;
                        case 3:
                            out[0][1]=true;
                            out[1][1]=true;
                            out[2][0]=true;
                            out[2][1]=true;
                        break;
                        case 4:
                            out[0][0]=true;
                            out[1][0]=true;
                            out[1][1]=true;
                            out[1][2]=true;
                        break;
                        default: cout<<"Error in position switch"<<endl;
                    }
                break;
                case 8:
                    switch(p){
                        case 1:
                            out[0][0]=true;
                            out[0][1]=true;
                            out[1][1]=true;
                            out[2][1]=true;
                        break;
                        case 2:
                            out[0][2]=true;
                            out[1][0]=true;
                            out[1][1]=true;
                            out[1][2]=true;
                        break;
                        case 3:
                            out[0][1]=true;
                            out[1][1]=true;
                            out[2][1]=true;
                            out[2][2]=true;
                        break;
                        case 4:
                            out[1][0]=true;
                            out[1][1]=true;
                            out[1][2]=true;
                            out[2][0]=true;
                        break;
                        default: cout<<"Error in position switch"<<endl;
                    }
                break;
                case 9:
                case 10:
                    switch(p){
                        case 1:
                            out[0][1]=true;
                            out[1][1]=true;
                            out[1][2]=true;
                            out[2][1]=true;
                        break;
                        case 2:
                            out[1][0]=true;
                            out[1][1]=true;
                            out[1][2]=true;
                            out[2][1]=true;
                        break;
                        case 3:
                            out[0][1]=true;
                            out[1][0]=true;
                            out[1][1]=true;
                            out[2][1]=true;
                        break;
                        case 4:
                            out[0][1]=true;
                            out[1][0]=true;
                            out[1][1]=true;
                            out[1][2]=true;
                        break;
                        default: cout<<"Error in position switch"<<endl;
                    }
                break;
                default: cout<<"Error in struct switch"<<endl;
            }
        }

        bool blockInit(){
            lineClean();
            outClean();
            y=(C/2)-1;
            x=R-4;
            set=false;
            init=true;
            s=(rand()%10)+1;
            p=(rand()%4)+1;
            c=(rand()%6)+1;
            drawOut();
            if(!clash()){
                project();
                return true;
            }
            else{
                return false;
            }
        }

        void cleanBlock(){
            for(int i=0;i<4;i++){
                for(int j=0;j<4;j++){
                    if(out[i][j]){
                        mat[x+i][y+j]=0;
                    }
                }
            }
        }

        bool process(){
            if(true){
                return blockDown();
            }
            else{
                cout<<"ASDASD"<<endl;
                return blockInit();
            }
        }
};

void test(){
    for(int i=0;i<30;i++){
        mat[rand()%25][rand()%15]=(rand()%6)+1;
    }
}

void clearMat(){
    for(int i=0;i<R;i++){
        for(int j=0;j<C;j++){
            mat[i][j]=0;
        }
    }
}

void matInit(){
    for(int i=0;i<R;i++){
        for(int j=0;j<C;j++){
            mat[i][j]=0;
        }
    }
}

void init(){
    glClearColor(0.0,0.0,0.0,1.0);  // BGColor
}

void reshape(int w, int h){
    float x=(float)C,y=(float)R;
    glutReshapeWindow(W,H);
    glViewport(0,0,(GLsizei)w,(GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,x,0.0,y);
    glMatrixMode(GL_MODELVIEW);
}

void border(){
    float x=(float)C,y=(float)R;
    glColor3f(1.0,1.0,1.0);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(0.0,0.0);
    glVertex2f(x,0.0);
    glVertex2f(x,0.0);
    glVertex2f(x,y);
    glVertex2f(x,y);
    glVertex2f(0.0,y);
    glVertex2f(0.0,y);
    glVertex2f(0.0,0.0);
    glEnd();
}

void mapMatrix(){
    for(int y=0;y<R;y++){
        for(int x=0;x<C;x++){
            if(mat[y][x]!=0){
                glBegin(GL_QUADS);
                float* c;
                c=color(mat[y][x]);
                glColor3f(c[0],c[1],c[2]);;
                glVertex2f(x,y);
                glVertex2f(x+1,y);
                glVertex2f(x+1,y+1);
                glVertex2f(x,y+1);
                glEnd();
                glColor3f(0.0,0.0,0.0);
                glLineWidth(2);
                glBegin(GL_LINES);
                glVertex2f(x,y);
                glVertex2f(x+1,y);
                glVertex2f(x+1,y);
                glVertex2f(x+1,y+1);
                glVertex2f(x+1,y+1);
                glVertex2f(x,y+1);
                glVertex2f(x,y+1);
                glVertex2f(x,y);
                glEnd();
            }
        }
    }
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT );
    glLoadIdentity();
    mapMatrix();
    border();
    glutSwapBuffers();
}

void printMat(){
    for(int i=0;i<R;i++){
        for(int j=0;j<C;j++){
            cout<<mat[i][j]<<" ";
        }
        cout<<endl;
    }
}

block b;
int t=0;

void timer(int){
    if(!paused){
        if(t==0){
            b.blockInit();
            t=1;
        }
        if(!b.process()){
            if(b.x<R-4){
                b.outClean();
                b.blockInit();
            }
        }
        glutPostRedisplay();
        glutTimerFunc(tim,timer,0.0);
    }
}

void keyboard(unsigned char c,int x,int y){
    switch(c){
        case '\e':
            clearMat();
            b.outClean();
            b.cleanBlock();
            fresh=true;
        case '\r':
        case ' ':
            paused=true;
            glutDestroyWindow(glutGetWindow());
            //glutSetWindow(menuInt);
            menu();
        break;
    }
}

void specialFunc(int Key, int x, int y){
    switch(Key){
        case GLUT_KEY_UP:
            b.cleanBlock();
            b.blockUp();
            glutPostRedisplay();
        break;
        case GLUT_KEY_DOWN:
            b.cleanBlock();
            b.blockDown();
            glutPostRedisplay();
        break;
        case GLUT_KEY_LEFT:
            b.cleanBlock();
            b.blockLeft();
            glutPostRedisplay();
        break;
        case GLUT_KEY_RIGHT:
            b.cleanBlock();
            b.blockRight();
            glutPostRedisplay();
        break;
        default: cout<<"INPUT FAILURE"<<endl;
    }
}

void game(){
    glutInitWindowSize(W,H);
    glutInitWindowPosition(200,0);
    glutCreateWindow("Tetris");
    gameInt=glutGetWindow();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialFunc);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0,timer,0);
    init();
    glutMainLoop();  
}

void drawString(float x, float y, float z, char *string) {
    glRasterPos3f(x, y, z);
    for (char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);  // Updates the position
    }
}

void drawButton(){
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_QUADS);
    glVertex2f((C/2)-2,(R/2)-1);
    glVertex2f((C/2)+3,(R/2)-1);
    glVertex2f((C/2)+3,(R/2)+2);
    glVertex2f((C/2)-2,(R/2)+2);
    glEnd();
    glColor3f(0.0,0.0,0.0);
    char play[]="PLAY";
    drawString((C/2)-0.8,(R/2)+0.2,0.0,play);
}
    
bool inButton(int x,int y){
    if(x>=118&&x<=232){
        if(y>=304&&y<=370){
            return true;
        }
    }
    return false;
}

void mouseCall(int button,int state,int x,int y){
    if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN){
        if(inButton(x,y)){
            paused=false;
            game();
        }
    }
}

void writeCredits(){
    glColor3f(1.0,1.0,1.0);
    char oglt[]="OpenGL_Mini_Project";
    char title[]="TETRIS";
    char by[]="Developed By :";
    char one[]="Sahan KJ : 4SN18CS076";
    char two[]="Shibin Joseph : 4SN18083";
    drawString(3.0,R-2.0,0.0,oglt);
    drawString(5.9,R-12.0,0.0,title);
    drawString(4.5,5.5,0.0,by);
    drawString(2.3,4.0,0.0,one);
    drawString(2.0,2.5,0.0,two);
}

void displayMenu(void){
    glClear(GL_COLOR_BUFFER_BIT );
    glLoadIdentity();
    border();
    drawButton();
    writeCredits();
    glutSwapBuffers();
}

void menuSpecialFunc(int key,int x,int y){
    switch(key){
        case GLUT_KEY_UP:
            paused=false;
            game();
        break;
    }
}

void keyboardMenu(unsigned char c,int x,int y){
    switch(c){
        case '\r':
        case ' ':
            paused=false;
            if(!fresh){
                glutDestroyWindow(glutGetWindow());
                game();
            }
            else{
                fresh=false;
                glutDestroyWindow(glutGetWindow());
                b.blockInit();
                game();
            }
        break;
        case '\e':
            glutDestroyWindow(glutGetWindow());
    }
}


void menu(){
    glutInitWindowSize(W,H);
    glutInitWindowPosition(200,0);
    glutCreateWindow("Tetris");
    menuInt=glutGetWindow();
    glutDisplayFunc(displayMenu);
    glutReshapeFunc(reshape);
    glutSpecialFunc(menuSpecialFunc);
    glutKeyboardFunc(keyboardMenu);
    glutMouseFunc(mouseCall);
    init();
    glutMainLoop();  
}

int main(int argc,char** argv){
    std::ios_base::sync_with_stdio(false);
    matInit();
    //printMat();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE );
    menu();
    return 0; 
}
