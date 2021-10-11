#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#define SIZE 256
#define QSIZE 25000000


struct node						// Filmleri ve oyuncular� yerle�tirilmek i�in kullan�l�r.
{
	int fil_oy;    				// Film ise 0 oyuncu ise 1
	int id;						// Yerle�tirildikleri adresleri tutar.
    char isim[150];				// �simleri tutar.
    struct node* next;			// Bir sonraki node u tutar.
    int level;					// BFS ile arama yaparken Bacon Say�s� i�in level tutar.
    struct node* backtrack;		// Bacon say�s� bulunduktan sonra geldi�i yolu yazd�rmak i�in kullan�l�r.
};

struct queue {					// BFS i�in kullan�lan kuyruk.
    struct node *items[QSIZE];
    int front;
    int rear;
}; 

struct Graph					// Film ve oyuncu graphlar�n� olu�turmak i�in kullan�l�r.
{
    int node_sayisi;
    struct node** adjLists;		// Kom�uluklar� tutar.
    int* visited;				// Ziyaret durumlar�n� tutar. Daha �nceden u�ranm�� ise u�rama yap�lmaz.
};




struct queue* createQueue();
void enqueue(struct queue* q, struct node *value);
struct node* dequeue(struct queue* q);
void display(struct queue* q);
int isEmpty(struct queue* q);


//bfs fonksiyonu oyuncu ve filmler graphlar�n�, ba�lang�� ve biti� nodelar�n� al�r.
// Breadth First Search algoritmas�n�n ta kendisidir.


void bfs(struct Graph* oyuncular,struct Graph* filmler,struct node *startVertex ,struct node *endVertex) {
    int yaz_count=0;
    int bulunan_level=0;
    char *buff_film;
    char *buff_oyuncu;
    
    if(strcmp(startVertex->isim,endVertex->isim)==0){	// E�er ayn� node ile ba�lanacak ise sonu� s�f�rd�r.
    	printf("Bacon sayisi : 0");
    	return;
	}
    
    startVertex->level=0;								// Ba�lanan node un leveli s�f�rd�r.
	struct queue* q = createQueue();					// Kuyruk olu�tur.
    
    oyuncular->visited[startVertex->id] = 1;			// �lk eleman�n ziyaret edildi�ini i�aretle.
    enqueue(q, startVertex);							// Sonras�nda kuyru�a ekle.
    
    while(!isEmpty(q)){
        //printQueue(q);
        struct node* currentVertex = dequeue(q);		// Kuyrukta s�radaki eleman� �ek.
        if(currentVertex->fil_oy==0){				    // 0 ise film nodudur. Demek ki kom�ulukta oyuncular var.
        	
    		struct node* oyuncu = filmler->adjLists[currentVertex->id]->next;  // �lk node filmin ad�yd� ikinciden itibaren oyuncular listelenecektir.
    
       		while(oyuncu) {								// Null olmad��� s�rece :
            	int oyuncu_id = oyuncu->id;				// Kom�uluktan al�nan oyuncular�n hashte yerle�ti�i yerleri al.
            
            	if(oyuncular->visited[oyuncu_id] == 0){ 		// E�er ziyaret edilmemi� ise :
            		if(strcmp(oyuncular->adjLists[oyuncu_id]->isim,endVertex->isim)==0){ // �stenilen elemana gelmi� isem.
            			bulunan_level=filmler->adjLists[currentVertex->id]->level;		 // Bacon say�s�n� tut.
            			if(bulunan_level>6){											 // 6 dan b�y�k ise kullan�y� bilgilendir.
            				printf("Bulunan Kevin Bacon sayisi 6 dan buyuktur.\n");
            				return ;
						}
            			
            			printf("Bacon Sayisi : %d\n\n",bulunan_level);					// De�ilse yazd�r.
            			printf("%s ",oyuncular->adjLists[oyuncu_id]->isim);				// Gelinmek istenen node u yaz.
            			while(currentVertex!=NULL){										// NULL de�ilse : 
            				if(currentVertex->fil_oy==0)								// Film ise buffera al, oyuncu ise yazd�r.
            					buff_film=currentVertex->isim;							// Bu i�lem ekrana d�zg�n �ekilde g�stermek i�in yap�l�r.
            				else{
            					buff_oyuncu=currentVertex->isim;						// Ekrana backtracking �eklinde yazd�rma k�s�mlar�.
								printf("| %s ->",currentVertex->isim);
            					printf(" %s\n",buff_film);	
            					yaz_count++;
            					if(yaz_count<bulunan_level)
            						printf("%s",buff_oyuncu);
            				}
            				if(yaz_count<bulunan_level)
								currentVertex=currentVertex->backtrack;
							else
								break;
						
						}
					}
            		// E�er hala bulunmam�� ise oyuncunun i�lemleri yap�lacak.	
                	oyuncular->visited[oyuncu_id] = 1;							// Ziyaret edildiklerini i�aretle
                	oyuncular->adjLists[oyuncu_id]->level=currentVertex->level;	// Her filmden oyuncuya giderken level say�lar�n� oldu�u gibi aktar.
                	oyuncular->adjLists[oyuncu_id]->backtrack=currentVertex;	// Nereden geldiklerini i�aretle.
                	enqueue(q, oyuncular->adjLists[oyuncu_id]);					// Kuyru�a ekle.
            	}
            oyuncu = oyuncu->next;
       		}		
		}
		else{																   // E�er fil_oy==1 ise oyunculara eri�iyorum demektir.
		
    		struct node* film = oyuncular->adjLists[currentVertex->id]->next;  // �lk node oyuncuydu ikinciden itibaren filmler listelenecektir.
    
       		while(film) {														// NULL olmad��� s�rece : 
            	int film_id = film->id;											// Filmin hashte yerle�ti�i adresi al.
            
            	if(filmler->visited[film_id] == 0){								// Ziyaret edilmemi� ise : 
                	filmler->visited[film_id] = 1;								// Ziyaret edildi diye i�aretle.
                	enqueue(q, filmler->adjLists[film_id]);						// Kuyru�a ekle.
                	filmler->adjLists[film_id]->level=currentVertex->level+1;	// Her oyuncudan filme giderken level say�lar�n� bir art�r.
                	filmler->adjLists[film_id]->backtrack=currentVertex;		// Backtracking i�in i�aretle.														
            	}
            film = film->next;
       		}	
		}
    }
    //printf("Kevin Bacon baglantisi bulunamadi \n");
}
 
struct node* node_olustur(int v,char *ad,int film_oyuncu){	// Al�nan parametrelerle node olu�turup d�nd�r�r.
	
    struct node* newNode = malloc(sizeof(struct node));
    newNode->id = v;
    strcpy(newNode->isim,ad);
    newNode->next = NULL;
    newNode->backtrack=NULL;
    newNode->fil_oy=film_oyuncu;
    return newNode;
}
 
struct Graph* graph_olustur(int node_sayisi)				// Verilen say�ya g�re graph olu�turup d�nd�r�r.
{
    int i;
	struct Graph* graph = malloc(sizeof(struct Graph));
    graph->node_sayisi = node_sayisi;
 
    graph->adjLists = malloc(node_sayisi * sizeof(struct node*));
    graph->visited = malloc(node_sayisi * sizeof(int));
    
 
    
    for (i = 0; i < node_sayisi; i++) {
        graph->adjLists[i] = NULL;
        graph->visited[i] = 0;
    }
 
    return graph;
}


int asal_bul(int sayi){									// Verilen de�erden b�y�k en k���k asal say�y� hesapla ve d�nd�r.
	int flag=1,i;										// Hashing i�in boyut hesaplamada kullan�ld�.
	int asal=sayi+1;
	
	while(flag){
		int sayac=0;
		for(i=2;i<asal;i++)
       		if(asal%i==0)
           		sayac++;
       
    	if(sayac==0)
      		return asal;                  				// Asal say� bulununca d�n.
    	else											// Say� asal de�ilse bir art�r ve tekrar dene.
    		asal++;
	}
}

struct queue* createQueue() {							// Kuyruk olu�turur ve d�nd�r�r.
    struct queue* q = malloc(sizeof(struct queue));
    q->front = -1;
    q->rear = -1;
    return q;
}


int isEmpty(struct queue* q) {							// Kuyru�un durumunu kontrol eder ve ilgili fonksiyonu bilgilendirir.
    if(q->rear == -1) 
        return 1;
    else 
        return 0;
}


void enqueue(struct queue* q, struct node* value){		// Kuyru�a ekleme yapar.
    if(q->rear == QSIZE-1)
        printf("\nKuyruk dolu");
    else {
        if(q->front == -1)
            q->front = 0;
        q->rear++;
        q->items[q->rear] = value;
    }
}

struct node* dequeue(struct queue* q){					// Kuyruktan eleman �eker.
    struct node* item;
    if(isEmpty(q)){
        printf("Queue bos\n");
       // item = -1;
    }
    else{
        item = q->items[q->front];
        q->front++;
        if(q->front > q->rear){
            //printf("Resetting queue");
            q->front = q->rear = -1;
        }
    }
    return item;
}


char** kelime_ayir(char*line,int *i){					// Filedan okunan sat�rlar�n bile�enlerine ay�r�r ve d�nd�r�r.
	char **args;
	char*arg;
	args=(char**)malloc(SIZE*sizeof(char*));
	
    	arg=strtok(line, "/\n");						// Verilen parametrelere g�re stringi ay�r�r.
		while(arg!=NULL){
			args[*i]=arg;
			*i+=1;
			arg=strtok(NULL,"/\n");
		}
return args;
}


int key_bul(char *val,int size){						// Hashing i�in verilen stringe g�re key de�eri hesaplar ve d�nd�r�r.
		int i,carpim_katsayisi=1,yedek=1;				// De�erleri istedi�im zaman de�i�tirebilmek i�in de�i�kenlere ba�lad�m.		
	int anahtar=0;
	
	for (i=0;i<strlen(val);i++){
		anahtar+=val[i]*carpim_katsayisi;
		carpim_katsayisi=carpim_katsayisi*yedek;
	}
	
	anahtar=anahtar%size;
return anahtar;
}

// Verilen iki node u birbirine ekler. Bir oyuncu bir filmde oynuyorsa : film -> oyuncu  ve  oyuncu -> film  g�sterimlerini sa�lar.
void node_bagla(struct Graph *filmler,struct Graph *oyuncular, int filmid, int oyuncuid,char* film_isim,char *oyuncu_isim)
{

	struct node* newNode = node_olustur(oyuncuid,oyuncu_isim,1);	// Parametre olan 1 olu�turulan node un bir oyuncu node u oldu�unu belirtir.
    if(filmler->adjLists[filmid]->next!=NULL){
		newNode->next = filmler->adjLists[filmid]->next;			// Listenin sonuna gitmek yerine ilk elemandan sonras�na araya gir.
    	filmler->adjLists[filmid]->next = newNode;					// Listenin ilk eleman� filmin ad�n�, ba�l� elemanlar ise oyuncu adlar�yd�.
	}else{
		filmler->adjLists[filmid]->next = newNode;					// Hen�z ekleme yap�lmam��sa do�rudan yerle�.
	}
	
 	newNode = node_olustur(filmid,film_isim,0);						// Parametre olan 0 olu�turulan node un bir film node u oldu�unu belirtir.
 	if(oyuncular->adjLists[oyuncuid]!=NULL){
 		newNode->next = oyuncular->adjLists[oyuncuid]->next;
    	oyuncular->adjLists[oyuncuid]->next= newNode;
	}else{
		oyuncular->adjLists[oyuncuid]->next= newNode;
	}
    
}

// Verilen oyuncu veya filmi olu�turulmu� graph a hash de�erlerine g�re ekler. Return olarak ekledi�i yerin adresini verir.
int graph_yerlestir(struct Graph *graph,char* ad,int size,int film_oyuncu){ 
																			
	
	int i,j=0,k=0,say=0;
	int key;
	
	key=key_bul(ad,size);			// Key de�erini bul.
	while(graph->adjLists[(key+j)%size]!=NULL&&say<size){	// E�er doluysa bir sonraki g�ze bak.
		k++;
		j=(k*k)%size;										// Y���lmalar� engellemek ama�la quadratic olarak ayarla.
		say++;												// Tablo size � ge�me.
	}
		
	struct node* newNode = node_olustur((key+j)%size,ad,film_oyuncu); // Bulunan yere yerle�.
	graph->adjLists[(key+j)%size]=newNode;
	
return (key+j)%size;										// Yerle�tirilen adresi g�nder.
}


int id_list_search(struct Graph *graph,char*ad,int size){	// Verilen graphta verilen ismi arar.Return olarak buldu�u yeri verir.
	int key=0,say=0;
	int j=0,k=0;
	
	key=key_bul(ad,size);									// Verilen ismin hash de�erini hesapla.
	while(graph->adjLists[(key+j)%size]!=NULL&&say<size){	// Hash de�erinden itibaren aramaya ba�la.
		if(strcmp(graph->adjLists[(key+j)%size]->isim,ad)==0)
			return (key+j)%size;
		k++;
		j=(k*k)%size;
		say++;
	}
	
	return -1;												// Bulamam�� isen kullan�c�y� bilgilendir.
	
}


int main(){
	char **argumanlar;				// Verilen stringin par�alar�na ayr�lm�� halini tutar.
	struct Graph* filmler;			// Filmler graph�n� tutar.
	struct Graph* oyuncular;		// Oyuncular graph�n� tutar.
	struct Graph* point;			// Graphlar �zerinde ilerlemek i�in kullan�l�r.
	int indis=0,i,id=0,film_id,gr_size=0,film_size=0,grp_ret_val,secim;
	//indis : sat�rlarda ka� eleman oldu�unu tutar.
	//id : Oyuncunun hashte yerle�ti�i yeri tutar.
	//filmid : Filmin hashte yerle�ti�i yeri tutar.
	//grsize : input filedaki oyuncu ve filmin toplam say�s�n� tutar.
	//filmsize : film say�lar�n� tutar.
	//grp_ret_val : graphta arama yap�ld���nda arama sonucunu tutar.
	char buff[2048];				// Filedan sat�r okuma yapar.
	char ilk_buf[40];				// Kullan�n�n ad�n� ve soyad�n� tutar.
	char iki_buf[25];				// Kullan�c�n ad�n� tutar.
	
	FILE *fptr;
	
	fptr=fopen("input-mpaa.txt","r");

	while(fgets(buff,sizeof(buff),fptr)!=NULL){	// Verilen inputta ka� film ve ka� oyuncu oldu�unu tutmak i�in bo� gez.
		argumanlar=kelime_ayir(buff,&indis);
		film_size++;							// Filmlerin say�s�n� getirir.
		for(i=0;i<indis;i++){
			gr_size++;							// Graph size getirir.
		}
	indis=0;									// Her defas�nda bir sat�rda ka� tane eleman oldu�unu tutar.
	}

	film_size=asal_bul(film_size);				// Hashteki de�eri asal olarak se�
	filmler=graph_olustur(film_size+1);			// Filmler graph�n� olu�tur.
	oyuncular=graph_olustur(gr_size);			// Oyuncular graph�n� olu�tur.
	
	rewind(fptr);								// Bo� gezmeden sonra tekrar ba�a gel.
	
	while(fgets(buff,sizeof(buff),fptr)!=NULL){	// Dosyan�n sonuna kadar oku.
		argumanlar=kelime_ayir(buff,&indis);	// Al�nan sat�r� elemanlar�na ay�r.

		film_id=graph_yerlestir(filmler,argumanlar[0],film_size,0); // Film her zaman sat�rdaki ilk elemand�r.
		
		for(i=1;i<indis;i++){
		
			grp_ret_val=id_list_search(oyuncular,argumanlar[i],gr_size); // Daha �nce eklenmi� mi kontrol et.
			
			if(grp_ret_val==-1){				// Eklenmemi� ise ekle, ekledikten sonra film ve oyuncuyu ba�la.
				id=graph_yerlestir(oyuncular,argumanlar[i],gr_size,1);
				node_bagla(filmler,oyuncular,film_id,id,argumanlar[0],argumanlar[i]);
			}else{
				node_bagla(filmler,oyuncular,film_id,grp_ret_val,argumanlar[0],argumanlar[i]);// Daha �nce eklenmi�se ba�la.
			}
		}
	indis=0;									// Al�nan arguman say�s�n� bir sonraki tur i�in s�f�rla.
	}
	fclose(fptr);
//EL�MDE OYUNCULAR VE F�LMLER VAR BFS BA�LAYAB�L�R�M.

	memset(ilk_buf,0,sizeof(char)*40);
	printf("Kevin Bacon Sayisi Bulma Programi\n----------------------------------------------\n");
	printf("Ilk oyuncu ismini giriniz : ");
	
	gets(ilk_buf);
	//printf("Girilen : %s\n",ilk_buf);
	int ilk_id=id_list_search(oyuncular,ilk_buf,gr_size);// �lk oyuncunun id sini tut.
	
	fflush(stdin);
	memset(ilk_buf,0,sizeof(char)*40);			
	
	printf("Ikinci oyuncu ismini giriniz : ");
	
	
	gets(ilk_buf);
	//printf("Ikinci : %s\n",ilk_buf);
		
	int ikinci_id=id_list_search(oyuncular,ilk_buf,gr_size);//�kinci oyuncunun id sini tut.
	
	
	printf("\n----------------------------------------------\n");
	if(ilk_id!=-1&&ikinci_id!=-1){
	bfs(oyuncular,filmler,oyuncular->adjLists[ilk_id],oyuncular->adjLists[ikinci_id]);	
	}else{
		printf("Girilen oyuncu bulunamadi Inputlari kontrol ediniz.\n");
	}

	printf("\n----------------------------------------------\n");	
	

	
	return 0;
}
