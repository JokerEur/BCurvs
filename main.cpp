#include<GLUT/glut.h>
#include<math.h> 

struct point { float x, y; } P[350];
int n = 0, i, j;
bool tP = false;
 
void Draw( );
void Mouse(int button, int state, int x, int y);
void MenuChek(int v);
void CurveBezier( );
void Line( );

unsigned long factorial(int n)
{
    if (n<=1)
        return(1);
    else
        n=n*factorial(n-1);
    return n;
}

float binomial_coff(float n,float k)
{
    float ans;
    ans = factorial(n) / (factorial(k)*factorial(n-k));
    return ans;
}
 
int main(int argc, char** argv) {
//Инициализируем режим отображения окна OpenGL
//GLUT_DOUBLE - окно с двойной буферизацией
//GLUT_RGB - режим RGBA
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowSize(350,350); //устанавливаем размер окна
	glutCreateWindow("Кривая Безье"); //создаем окно с заголовком
	glutDisplayFunc(Draw); //устанавливаем функцию отрисовки
	glutMouseFunc(Mouse); //устанавливаем функцию обработки нажатий мыши
 
//Установить цвет и значение альфа, используемые при очистке буферов цвета 
	glClearColor(1,1,1,1); //цвет фона (RGBA)
	glMatrixMode(GL_PROJECTION); //Определяем стек матриц (матрица проекций)
	glLoadIdentity(); //установить текущую матрицу равной еденичной
 
	glOrtho(0,350,0,350,0,1); //Установить границы объема отсечения
	glColor3f(0,0,0); //Установить текущий цвет (R,G,B)
 
	glutCreateMenu(MenuChek); //Меню вызываемое нажатием ПКМ
	glutAddMenuEntry("Удалить последнюю точку", 0); //Пункт меню
	glutAddMenuEntry("Очистить", 1); //Пункт меню
	glutAttachMenu(GLUT_RIGHT_BUTTON);
 
	glutMainLoop(); //Запуск основного цикла обработки GLUT

	return 0;
}
 
void Draw( ) {
	glClear(GL_COLOR_BUFFER_BIT); //Очистка буфера цвета
	Line( );
	if (n > 3)
		CurveBezier();
	glutSwapBuffers(); //Переключить буферы в режиме двойной буферизации
}
 
void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
			if (tP && (x > 0 && x < 350)&& (350 > y && y < 700) ) { 
				P[j].x = x; 
				P[j].y = 350-y; 
			} else if (state == GLUT_DOWN && n != 0 && !tP)
				for (i = 0; i <= n; i++)
					if ((x<P[i].x+30 && x>P[i].x-30) && (320-y<P[i].y && 380-y>P[i].y)) {
						tP = true; 
						j = i; 
						break;
					}	
			if (state == GLUT_UP) {
				if (!tP) {
					P[n].x = x; 
					P[n].y = 350-y; 
					n++;
				}
				tP = false;
			}
	}
	glutPostRedisplay(); //Обновить текущее окно
}
 
void MenuChek(int v) {
	if (v == 0 && n > 0)
		n--; 
	else if (v == 1)
		n = 0;
	glutPostRedisplay(); //Обновить текущее окно
}
 
void Line ( ) {
	glPointSize(5);
	glColor3f(0,0,0);
 
//Контрольные точки
	glBegin(GL_POINTS);
		for (i = n-1; i >= 0; i--)
			glVertex2f(P[i].x, P[i].y);
	glEnd();
 
//Пунктирные линии
	glLineStipple(2,58360);
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);
		for (i = 0; i < n-1; i++) {
			glVertex2f(P[i].x, P[i].y);
			glVertex2f(P[i+1].x, P[i+1].y);
		}
	glEnd();
	glDisable(GL_LINE_STIPPLE);
	glPointSize(1);
}
 
void CurveBezier( ) {
		glColor3f(1,0,0);
		glPointSize(2);
		glBegin(GL_POINTS);
			for(float t = 0.0; t <= 1.0; t += 0.001) {
				point p;
    			p.x = 0; p.y = 0;   
    			for (int i = 0; i<n; i++)
    			{
       				 p.x = p.x + binomial_coff((float)(n - 1), (float)i) * pow(t, (double)i) * pow((1 - t), (n - 1 - i)) * P[i].x;
       				 p.y = p.y + binomial_coff((float)(n - 1), (float)i) * pow(t, (double)i) * pow((1 - t), (n - 1 - i)) * P[i].y;
    			}
				glVertex3f(p.x, p.y, 0);
			}
		glEnd();
	}