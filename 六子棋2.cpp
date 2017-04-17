#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <vector>
#include <queue>
#include <string.h>
#include <windows.h>
#include <fstream>
using namespace std;
enum paint_char{black=1,white=2,empty=-1,nempty=-2,side=3,sum=3};
enum vertion{player,computer};
const int chessbored_size=21;//���̳ߴ� 
int chessbored[chessbored_size][chessbored_size];//���� 
int cursor_x,cursor_y;//������� 
int player_color=black;//���������ɫ
int computer_color=white; //����������ɫ 
const int max2=2147483647,fen_max=99999999;
const int min2=-2147483647,fen_min=-99999999;
int wide=4,depth=6;//��������Ⱥ͹�� 
int regret_array[180][chessbored_size][chessbored_size],regret_num;//�����¼����
struct node
{
	int i,j;
	int value;
	int c[2][4];//�������γɵ����ӵ����� 
	friend bool operator < (node a,node b)
	{
		return a.value<b.value;
	}
	friend bool operator > (node a,node b)
	{
		return a.value>b.value;
	}
} ;
struct sixnode
{
	int x1,y1,x2,y2;
};
//priority_queue< node,vector<node>,less<node> > empty_player;
//priority_queue< node,vector<node>,less<node> > empty_player;
vector<node> empty_node;
vector<node> regret_empty_node[180];//���� 
int szfz(int a[chessbored_size][chessbored_size],int b[chessbored_size][chessbored_size])
{
	int i,j;
	for(i=0;i<chessbored_size;i++)
		for(j=0;j<chessbored_size;j++)
			b[i][j]=a[i][j];
	return 0;
}
int ini_chessbored()//��ʼ������ 
{
	memset(chessbored,-1,sizeof(chessbored));
	int i;
	for(i=0;i<chessbored_size;i++)
	{
		chessbored[i][0]=side;
		chessbored[0][i]=side;
		chessbored[chessbored_size-1][i]=side;
		chessbored[i][chessbored_size-1]=side;
	} 
	return 0;	
} 
string get_style(int i,int j)//�õ�������ַ� 
{
	int a=chessbored[i][j];
	if(i==cursor_x&&j==cursor_y)return "  ";
	if(a==black)return "��";
	if(a==white)return "��";
	if(i>1)
	{
		if(i<chessbored_size-2)
		{
			if(j>1)
			{
				if(j<chessbored_size-2)
				{
					return "��";
				}
				else
				{
					return "��";
				}
			}
			else 
			{
				return "��";
			}
		}
		else 
		{
			if(j>1)
			{
				if(j<chessbored_size-2)
				{
					return "��";
				}
				else
				{
					return "��";
				}
			}
			else 
			{
				return "��";
			}
		} 
	}
	else 
	{
		if(j>1)
		{
			if(j<chessbored_size-2)
			{
				return "��";
			}
			else
			{
				return "��";
			}
		}
		else 
		{
			return "��";
		}
	}
} 
int paint()//��ͼ 
{
	string canvas[chessbored_size];
	int i,j;
	for(i=1;i<chessbored_size-1;i++)
	{
		canvas[i]=canvas[i]+" ";
		for(j=1;j<chessbored_size-1;j++)
			canvas[i]=canvas[i]+get_style(i,j);
		canvas[i]=canvas[i]+" ";
	}
	for(i=0;i<chessbored_size;i++)	
	{
		canvas[0]=canvas[0]+" ";
		canvas[chessbored_size-1]=canvas[chessbored_size-1]+" ";
	}
	system("cls");
	for(i=0;i<chessbored_size;i++)
		cout<<canvas[i]<<endl;
	return 0;	
}
int near_num(int x,int y)//���ش˵��γɵ���������� 
{
	int s,m,k,n=chessbored[x][y];
	s=1;
	for(k=1;k<6&&chessbored[x][y-k]==n;k++)s++;
	for(k=1;k<6&&chessbored[x][y+k]==n;k++)s++;
	m=s;
	s=1;
	for(k=1;k<6&&chessbored[x-k][y]==n;k++)s++;
	for(k=1;k<6&&chessbored[x+k][y]==n;k++)s++;
	if(s>m)m=s;
	s=1;
	for(k=1;k<6&&chessbored[x-k][y-k]==n;k++)s++;
	for(k=1;k<6&&chessbored[x+k][y+k]==n;k++)s++;
	if(s>m)m=s;
	s=1;
	for(k=1;k<6&&chessbored[x-k][y+k]==n;k++)s++;
	for(k=1;k<6&&chessbored[x+k][y-k]==n;k++)s++;
	if(s>m)m=s;
	return m;
}
int empty_value_sub(node e,int a)//a��ʾ�ӽ�����һ��ǵ��� ���ո����ֺ����� 
{
	int temp=chessbored[e.i][e.j];
	chessbored[e.i][e.j]=a;
	if(near_num(e.i,e.j)>5)
	{
		chessbored[e.i][e.j]=temp;
		return fen_max;
	}
	chessbored[e.i][e.j]=temp;
	  int b=1,i,j,k,s=0,n,fa=black+white-a,b2=0;
	  for(k=1;k<6;k++)
	  {
			if(chessbored[e.i][e.j-k]==a)b*=4;
			else if(chessbored[e.i][e.j-k]<0)
			{				
				b2++;
				if(b2>1)
				{
					break;
				} 
				b*=2;					
			}
			else 
				break;
      }
      b2=0;
      for(k=1;k<6;k++)
	  {
			if(chessbored[e.i][e.j+k]==a)b*=4;
			else if(chessbored[e.i][e.j+k]<0)
			{
				b2++;
				if(b2>1)
				{
					break;
				} 
				b*=2;
			}
			else 
				break;
      }
   	  n=0;
      for(k=1;k<6&&chessbored[e.i][e.j-k]!=fa&&chessbored[e.i][e.j-k]!=side;k++)
 		n++;
	  for(k=1;k<6&&chessbored[e.i][e.j+k]!=fa&&chessbored[e.i][e.j+k]!=side;k++)
 		n++;
	  if(n<5)b=0;
	  if(n==5&&b==512)b=1024;
  	  s=s+b;
  	  
  	  b=1;b2=0;
  	  for(k=1;k<6;k++)
	  {
			if(chessbored[e.i-k][e.j]==a)b*=4;
			else if(chessbored[e.i-k][e.j]<0)
			{
				b2++;
				if(b2>1)
				{
					break;
				} 
				b*=2;
			}
			else 
				break;
      }
      b2=0;
      for(k=1;k<6;k++)
	  {
			if(chessbored[e.i+k][e.j]==a)b*=4;
			else if(chessbored[e.i+k][e.j]<0)
			{
				b2++;
				if(b2>1)
				{
					break;
				} 
				b*=2;
			}
			else 
				break;
      }
  	  n=0;
      for(k=1;k<6&&chessbored[e.i-k][e.j]!=fa&&chessbored[e.i-k][e.j]!=side;k++)
 		n++;
	  for(k=1;k<6&&chessbored[e.i+k][e.j]!=fa&&chessbored[e.i+k][e.j]!=side;k++)
 		n++;
	  if(n<5)b=0;
	  if(n==5&&b==512)b=1024;
  	  s=s+b;
  	  
  	  b=1;b2=0;
  	  for(k=1;k<6;k++)
	  {
			if(chessbored[e.i-k][e.j-k]==a)b*=4;
			else if(chessbored[e.i-k][e.j-k]<0)
			{
				b2++;
				if(b2>1)
				{
					break;
				} 
				b*=2;
			}
			else 
				break;
      }
      b2=0;
      for(k=1;k<6;k++)
	  {
			if(chessbored[e.i+k][e.j+k]==a)b*=4;
			else if(chessbored[e.i+k][e.j+k]<0)
			{
				b2++;
				if(b2>1)
				{
					break;
				} 
				b*=2;
			}
			else 
				break;
      }
  	  n=0;
      for(k=1;k<6&&chessbored[e.i-k][e.j-k]!=fa&&chessbored[e.i-k][e.j-k]!=side;k++)
 		n++;
	  for(k=1;k<6&&chessbored[e.i+k][e.j+k]!=fa&&chessbored[e.i+k][e.j+k]!=side;k++)
 		n++;
	  if(n<5)b=0;
	  if(n==5&&b==512)b=1024;
  	  s=s+b;
  	  
  	   b=1;b2=0;
  	  for(k=1;k<6;k++)
	  {
			if(chessbored[e.i-k][e.j+k]==a)b*=4;
			else if(chessbored[e.i-k][e.j+k]<0)
			{
				b2++;
				if(b2>1)
				{
					break;
				} 
				b*=2;
			}
			else 
				break;
      }
      b2=0;
      for(k=1;k<6;k++)
	  {
			if(chessbored[e.i+k][e.j-k]==a)b*=4;
			else if(chessbored[e.i+k][e.j-k]<0)
			{
				b2++;
				if(b2>1)
				{
					break;
				} 
				b*=2;
			}
			else 
				break;
      }
  	  n=0;
      for(k=1;k<6&&chessbored[e.i-k][e.j+k]!=fa&&chessbored[e.i-k][e.j+k]!=side;k++)
 		n++;
	  for(k=1;k<6&&chessbored[e.i+k][e.j-k]!=fa&&chessbored[e.i+k][e.j-k]!=side;k++)
 		n++;
	  if(n<5)b=0;
	  if(n==5&&b==512)b=1024;
  	  s=s+b;
  	  
  	  return s;
}
int empty_value(node &e,int a)
{
	int my,you;
	my=empty_value_sub(e,a);
	you=empty_value_sub(e,player_color+computer_color-a);
	e.value=my+you;
	return my+you;
}
int empty_value2(node &e,int a)//���ֺ��� 
{
	int my,you;
	my=empty_value_sub(e,a);
	you=empty_value_sub(e,player_color+computer_color-a);
	e.value=my-you/2;
	return my-you/2;
}
int pingfen(int pc)//�������� 
{
	vector<node>::iterator the_iterator;
	node e;
	int max3=fen_min;
	the_iterator = empty_node.begin();
 	while( the_iterator != empty_node.end() ) 
	{
		e=*the_iterator;
		empty_value2(e,pc);
		if(e.value>max3)
			max3=e.value;
 	   	the_iterator++;
   }
   return max3;
}
int display()
{
	vector<node>::iterator the;
	the=empty_node.begin();
	while(the!=empty_node.end())
	{
		cout<<(*the).i<<" "<<(*the).j<<" "<<(*the).value<<endl;
		the++;
	}
	return 0;
}
int before_move(int x,int y)
{
	if(chessbored[x][y]==nempty)
	{
		vector<node>::iterator the_iterator;
		node e;
		the_iterator = empty_node.begin();
	    while( the_iterator != empty_node.end() ) 
		{
			e=*the_iterator;
			if(e.i==x&&e.j==y)
			{
				empty_node.erase(the_iterator);
				break;
			}
 	    	the_iterator++;
 	    }
	}
	return 0;
}
int updata_empty_node(int x,int y)//����������Χ�Ŀյ� 
{
	int i,j;
	for(i=-3;i<4;i++)
		for(j=-3;j<4;j++)
			if(chessbored[x+i][y+j]==empty&&x+i>0&&x+i<20&&y+j>0&&y+j<20)
			{
				node e;
				e.i=x+i;
				e.j=y+j;
				empty_node.push_back(e);
				//cout<<e.i<<" "<<e.j<<endl; 
				chessbored[x+i][y+j]=nempty;
			}
	return 0;
}
int youfen(int pc,int depth2,int jz);
int myfen(int pc,int depth2,int jz)//�ҵķ��� 
{
	if(depth2>=depth)
	{
		int a=pingfen(pc);
		return a;
	}
	int fa=player_color+computer_color-pc,c;
	int n=empty_node.size();
    int i,j;
    vector<node> en;
	en=empty_node;
    node e,e2;
    priority_queue< node ,vector<node> ,less<node> >  pen;
    for(i=0;i<n;i++)
    {
    	e=en.back();
    	en.pop_back();
    	empty_value(e,pc);
    	pen.push(e);
    }
    int max_value=min2;
    int wide2=wide;
    if(wide2>n)wide2=n;
    node te[50];
 	for(i=0;i<wide2;i++)
 	{
	 	te[i]=pen.top();
		pen.pop();	
	}
	sixnode se[20];
	int secnt=0;
 	for(i=0;i<wide2;i++)
 		for(j=i+1;j<wide2;j++)
 		{
	 		se[secnt].x1=te[i].i;
	 		se[secnt].y1=te[i].j;
	 		se[secnt].x2=te[j].i;
	 		se[secnt].y2=te[j].j;
	 		secnt++;
		}
	int chessbored2[chessbored_size][chessbored_size];
	szfz(chessbored,chessbored2);
    for(i=0;i<secnt;i++)
	{
		vector<node> empty_node2;
		empty_node2=empty_node;
		before_move(se[i].x1,se[i].y1);
		before_move(se[i].x2,se[i].y2);
		chessbored[se[i].x1][se[i].y1]=pc;
		chessbored[se[i].x2][se[i].y2]=pc;
		if(near_num(se[i].x1,se[i].y1)>5)
		{		
			empty_node=empty_node2;
			szfz(chessbored2,chessbored);
			return fen_max;
		}
		if(near_num(se[i].x2,se[i].y2)>5)
		{		
			empty_node=empty_node2;
			szfz(chessbored2,chessbored);
			return fen_max;
		}
		updata_empty_node(se[i].x1,se[i].y1);
		updata_empty_node(se[i].x2,se[i].y2);	
		int fenshu;
		fenshu=youfen(pc,depth2+1,max_value);
		if(fenshu>max_value)
		{
			max_value=fenshu;
		} 
		empty_node=empty_node2;
		szfz(chessbored2,chessbored);
	} 
	return max_value;
} 
int youfen(int pc,int depth2,int jz)//��ķ��� ,jz��֦ 
{
	int fa=player_color+computer_color-pc;
	int n=empty_node.size();
    int i,j,i2;
    vector<node> en;
	en=empty_node;
    node e,e2;
    priority_queue< node ,vector<node> ,less<node> >  pen;
    for(i=0;i<n;i++)
    {
    	e=en.back();
    	en.pop_back();
    	empty_value(e,fa);
    	pen.push(e);
    }
    int min_value=max2,c;
    int wide2=wide;
    if(wide2>n)wide2=n;
    node te[50];
 	for(i=0;i<wide2;i++)
 	{
	 	te[i]=pen.top();
		pen.pop();	
	}
	sixnode se[20];
	int secnt=0;
 	for(i=0;i<wide2;i++)
 		for(j=i+1;j<wide2;j++)
 		{
	 		se[secnt].x1=te[i].i;
	 		se[secnt].y1=te[i].j;
	 		se[secnt].x2=te[j].i;
	 		se[secnt].y2=te[j].j;
	 		secnt++;
		}
	int chessbored2[chessbored_size][chessbored_size];
	szfz(chessbored,chessbored2);
    for(i=0;i<secnt;i++)
	{
		vector<node> empty_node2;
		empty_node2=empty_node;
		before_move(se[i].x1,se[i].y1);
		before_move(se[i].x2,se[i].y2);
		chessbored[se[i].x1][se[i].y1]=fa;
		chessbored[se[i].x2][se[i].y2]=fa;
		if(near_num(se[i].x1,se[i].y1)>5)
		{		
			empty_node=empty_node2;
			szfz(chessbored2,chessbored);
			return fen_min;
		}
		if(near_num(se[i].x2,se[i].y2)>5)
		{		
			empty_node=empty_node2;
			szfz(chessbored2,chessbored);
			return fen_min;
		}
		updata_empty_node(se[i].x1,se[i].y1);
		updata_empty_node(se[i].x2,se[i].y2);	
		int fenshu;
		fenshu=myfen(pc,depth2+1,min_value);
		if(fenshu<min_value)
		{
			min_value=fenshu;
		} 
		empty_node=empty_node2;
		szfz(chessbored2,chessbored);
	} 
	return min_value;
}
sixnode get_bestnode(int pc)//�õ����ŵ� ,pc��ʾ����һ��Ϊ�Լ� 
{
	int n=empty_node.size();
    int i,c1,c2,j,i2;
    vector<node> en;
	en=empty_node;
    sixnode se2;
    node e;
    priority_queue< node ,vector<node> ,less<node> >  pen;
    for(i=0;i<n;i++)
    {
    	e=en.back();
    	en.pop_back();
    	empty_value(e,pc);
    	pen.push(e);
    }
    int max_value=min2;
 //   display();    
 	int wide2=wide;
    if(wide2>n)wide2=n;    
 	node te[50];
 	for(i=0;i<wide2;i++)
 	{
	 	te[i]=pen.top();
		pen.pop();	
	}
	sixnode se[20];
	int secnt=0;
 	for(i=0;i<wide2;i++)
 		for(j=i+1;j<wide2;j++)
 		{
	 		se[secnt].x1=te[i].i;
	 		se[secnt].y1=te[i].j;
	 		se[secnt].x2=te[j].i;
	 		se[secnt].y2=te[j].j;
	 		secnt++;
		}
	int chessbored2[chessbored_size][chessbored_size];
	szfz(chessbored,chessbored2);
    for(i=0;i<secnt;i++)
	{
		vector<node> empty_node2;
		empty_node2=empty_node;
		before_move(se[i].x1,se[i].y1);
		before_move(se[i].x2,se[i].y2);
		chessbored[se[i].x1][se[i].y1]=pc;
		chessbored[se[i].x2][se[i].y2]=pc;
		if(near_num(se[i].x1,se[i].y1)>5)
		{		
			empty_node=empty_node2;
			szfz(chessbored2,chessbored);
			return se[i];
		}
		if(near_num(se[i].x2,se[i].y2)>5)
		{		
			empty_node=empty_node2;
			szfz(chessbored2,chessbored);
			return se[i];
		}
		updata_empty_node(se[i].x1,se[i].y1);
		updata_empty_node(se[i].x2,se[i].y2);	
		int fenshu;
		fenshu=youfen(pc,1,max_value);
		if(fenshu>max_value)
		{
			i2=i;
			max_value=fenshu;
		} 
		empty_node=empty_node2;
		szfz(chessbored2,chessbored);
	} 
	return se[i2];
}
int computer_move(int pc)
{
	sixnode se;
	se=get_bestnode(pc);
	before_move(se.x1,se.y1);
	before_move(se.x2,se.y2);
	chessbored[se.x1][se.y1]=pc;
	chessbored[se.x2][se.y2]=pc;
	paint();
	if(near_num(se.x1,se.y1)>5)
		return 1;
	if(near_num(se.x2,se.y2)>5)
		return 1;
	updata_empty_node(se.x1,se.y1);
	updata_empty_node(se.x2,se.y2);
	return 0;
}
int regret_save() //���������Ϣ 
{
	regret_num++;
	szfz(chessbored,regret_array[regret_num]);
	regret_empty_node[regret_num]=empty_node;	
	return 0;
}
int regret()//���� 
{
	regret_num--;
	if(regret_num<0)return 0;
	szfz(regret_array[regret_num],chessbored); 
	empty_node=regret_empty_node[regret_num];
	return 0;
}
int player_move()
{
	int key,i;
	while(1)
	{
		key=getch();
		if(key==224)//������µ��Ƿ����
		{
			key=getch();
			if(key==72)// �� 
			{
				cursor_x--;
				if(cursor_x<1)
					cursor_x=chessbored_size-2;
			}
			else if(key==80)//�� 
			{
				cursor_x++;
				if(cursor_x>chessbored_size-2)
					cursor_x=1;
			}
			else if(key==75)//�� 
			{
				cursor_y--;
				if(cursor_y<1)
					cursor_y=chessbored_size-2;
			}
			else if(key==77)//�� 
			{
				cursor_y++;
				if(cursor_y>chessbored_size-2)
					cursor_y=1;
			}
			paint();
		} 
		else if(key==32&&chessbored[cursor_x][cursor_y]<0)//������µ��ǿո� 
		{
			before_move(cursor_x,cursor_y);
			chessbored[cursor_x][cursor_y]=player_color;
			paint();
			if(near_num(cursor_x,cursor_y)>5)
				return 1;
			updata_empty_node(cursor_x,cursor_y);
			return 0;
		}
		else if(key==27)
		{
			regret();
			paint();
		}
	}
} 
int main()
{
	system("title ������ ���������������");//���ñ���
  //  system("mode con cols=45 lines=24");//���ô��ڴ�С
    system("color b0");//������ɫ
    while(1) 
    {
    	ini_chessbored();
    	regret_num=-1;
		cursor_x=chessbored_size/2;
		cursor_y=chessbored_size/2;
//		cursor_x=2;
//		cursor_y=2;
		empty_node.clear();
		cout<<"1���֣�2����"<<endl;
		int cnt;
		cin>>cnt;
		if(cnt==2)
		{
			int f=chessbored_size/2;
			before_move(f,f);
			chessbored[f][f]=computer_color;
			updata_empty_node(f,f);
			paint();
		}
		else
		{
			paint();
			player_move();
			computer_move(computer_color);
		} 
		while(1)
		{
			
			int a;
//			a=computer_move(player_color);
			regret_save();
			player_move();
			a=player_move();			
			if(a)
			{
				cout<<"���ʤ��"<<endl;
				int temp=1;
				if(getch()==27)
				{
					regret();
					paint();
					player_move();
					temp=0;
				}
				if(temp)				
					break; 
			}
//			system("pause");
			a=computer_move(computer_color);
			if(a)
			{
				cout<<"����ʤ��"<<endl;
				int temp=1;
				if(getch()==27)
				{
					regret();
					paint();
					temp=0;
				}
				if(temp)				
					break;  
			}
//			system("pause");
		}	
    }	
	return 0;
}