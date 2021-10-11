#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#define SIZE 256
#define QSIZE 25000000


struct node						// Filmleri ve oyuncularý yerleþtirilmek için kullanýlýr.
{
	int fil_oy;    				// Film ise 0 oyuncu ise 1
	int id;						// Yerleþtirildikleri adresleri tutar.
    char isim[150];				// Ýsimleri tutar.
    struct node* next;			// Bir sonraki node u tutar.
    int level;					// BFS ile arama yaparken Bacon Sayýsý için level tutar.
    struct node* backtrack;		// Bacon sayýsý bulunduktan sonra geldiði yolu yazdýrmak için kullanýlýr.
};

struct queue {					// BFS için kullanýlan kuyruk.
    struct node *items[QSIZE];
    int front;
    int rear;
}; 

struct Graph					// Film ve oyuncu graphlarýný oluþturmak için kullanýlýr.
{
    int node_sayisi;
    struct node** adjLists;		// Komþuluklarý tutar.
    int* visited;				// Ziyaret durumlarýný tutar. Daha önceden uðranmýþ ise uðrama yapýlmaz.
};




struct queue* createQueue();
void enqueue(struct queue* q, struct node *value);
struct node* dequeue(struct queue* q);
void display(struct queue* q);
int isEmpty(struct queue* q);


//bfs fonksiyonu oyuncu ve filmler graphlarýný, baþlangýç ve bitiþ nodelarýný alýr.
// Breadth First Search algoritmasýnýn ta kendisidir.


void bfs(struct Graph* oyuncular,struct Graph* filmler,struct node *startVertex ,struct node *endVertex) {
    int yaz_count=0;
    int bulunan_level=0;
    char *buff_film;
    char *buff_oyuncu;
    
    if(strcmp(startVertex->isim,endVertex->isim)==0){	// Eðer ayný node ile baþlanacak ise sonuç sýfýrdýr.
    	printf("Bacon sayisi : 0");
    	return;
	}
    
    startVertex->level=0;								// Baþlanan node un leveli sýfýrdýr.
	struct queue* q = createQueue();					// Kuyruk oluþtur.
    
    oyuncular->visited[startVertex->id] = 1;			// Ýlk elemanýn ziyaret edildiðini iþaretle.
    enqueue(q, startVertex);							// Sonrasýnda kuyruða ekle.
    
    while(!isEmpty(q)){
        //printQueue(q);
        struct node* currentVertex = dequeue(q);		// Kuyrukta sýradaki elemaný çek.
        if(currentVertex->fil_oy==0){				    // 0 ise film nodudur. Demek ki komþulukta oyuncular var.
        	
    		struct node* oyuncu = filmler->adjLists[currentVertex->id]->next;  // Ýlk node filmin adýydý ikinciden itibaren oyuncular listelenecektir.
    
       		while(oyuncu) {								// Null olmadýðý sürece :
            	int oyuncu_id = oyuncu->id;				// Komþuluktan alýnan oyuncularýn hashte yerleþtiði yerleri al.
            
            	if(oyuncular->visited[oyuncu_id] == 0){ 		// Eðer ziyaret edilmemiþ ise :
            		if(strcmp(oyuncular->adjLists[oyuncu_id]->isim,endVertex->isim)==0){ // Ýstenilen elemana gelmiþ isem.
            			bulunan_level=filmler->adjLists[currentVertex->id]->level;		 // Bacon sayýsýný tut.
            			if(bulunan_level>6){											 // 6 dan büyük ise kullanýyý bilgilendir.
            				printf("Bulunan Kevin Bacon sayisi 6 dan buyuktur.\n");
            				return ;
						}
            			
            			printf("Bacon Sayisi : %d\n\n",bulunan_level);					// Deðilse yazdýr.
            			printf("%s ",oyuncular->adjLists[oyuncu_id]->isim);				// Gelinmek istenen node u yaz.
            			while(currentVertex!=NULL){										// NULL deðilse : 
            				if(currentVertex->fil_oy==0)								// Film ise buffera al, oyuncu ise yazdýr.
            					buff_film=currentVertex->isim;							// Bu iþlem ekrana düzgün þekilde göstermek için yapýlýr.
            				else{
            					buff_oyuncu=currentVertex->isim;						// Ekrana backtracking þeklinde yazdýrma kýsýmlarý.
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
            		// Eðer hala bulunmamýþ ise oyuncunun iþlemleri yapýlacak.	
                	oyuncular->visited[oyuncu_id] = 1;							// Ziyaret edildiklerini iþaretle
                	oyuncular->adjLists[oyuncu_id]->level=currentVertex->level;	// Her filmden oyuncuya giderken level sayýlarýný olduðu gibi aktar.
                	oyuncular->adjLists[oyuncu_id]->backtrack=currentVertex;	// Nereden geldiklerini iþaretle.
                	enqueue(q, oyuncular->adjLists[oyuncu_id]);					// Kuyruða ekle.
            	}
            oyuncu = oyuncu->next;
       		}		
		}
		else{																   // Eðer fil_oy==1 ise oyunculara eriþiyorum demektir.
		
    		struct node* film = oyuncular->adjLists[currentVertex->id]->next;  // Ýlk node oyuncuydu ikinciden itibaren filmler listelenecektir.
    
       		while(film) {														// NULL olmadýðý sürece : 
            	int film_id = film->id;											// Filmin hashte yerleþtiði adresi al.
            
            	if(filmler->visited[film_id] == 0){								// Ziyaret edilmemiþ ise : 
                	filmler->visited[film_id] = 1;								// Ziyaret edildi diye iþaretle.
                	enqueue(q, filmler->adjLists[film_id]);						// Kuyruða ekle.
                	filmler->adjLists[film_id]->level=currentVertex->level+1;	// Her oyuncudan filme giderken level sayýlarýný bir artýr.
                	filmler->adjLists[film_id]->backtrack=currentVertex;		// Backtracking için iþaretle.														
            	}
            film = film->next;
       		}	
		}
    }
    //printf("Kevin Bacon baglantisi bulunamadi \n");
}
 
struct node* node_olustur(int v,char *ad,int film_oyuncu){	// Alýnan parametrelerle node oluþturup döndürür.
	
    struct node* newNode = malloc(sizeof(struct node));
    newNode->id = v;
    strcpy(newNode->isim,ad);
    newNode->next = NULL;
    newNode->backtrack=NULL;
    newNode->fil_oy=film_oyuncu;
    return newNode;
}
 
struct Graph* graph_olustur(int node_sayisi)				// Verilen sayýya göre graph oluþturup döndürür.
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


int asal_bul(int sayi){									// Verilen deðerden büyük en küçük asal sayýyý hesapla ve döndür.
	int flag=1,i;										// Hashing için boyut hesaplamada kullanýldý.
	int asal=sayi+1;
	
	while(flag){
		int sayac=0;
		for(i=2;i<asal;i++)
       		if(asal%i==0)
           		sayac++;
       
    	if(sayac==0)
      		return asal;                  				// Asal sayý bulununca dön.
    	else											// Sayý asal deðilse bir artýr ve tekrar dene.
    		asal++;
	}
}

struct queue* createQueue() {							// Kuyruk oluþturur ve döndürür.
    struct queue* q = malloc(sizeof(struct queue));
    q->front = -1;
    q->rear = -1;
    return q;
}


int isEmpty(struct queue* q) {							// Kuyruðun durumunu kontrol eder ve ilgili fonksiyonu bilgilendirir.
    if(q->rear == -1) 
        return 1;
    else 
        return 0;
}


void enqueue(struct queue* q, struct node* value){		// Kuyruða ekleme yapar.
    if(q->rear == QSIZE-1)
        printf("\nKuyruk dolu");
    else {
        if(q->front == -1)
            q->front = 0;
        q->rear++;
        q->items[q->rear] = value;
    }
}

struct node* dequeue(struct queue* q){					// Kuyruktan eleman çeker.
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


char** kelime_ayir(char*line,int *i){					// Filedan okunan satýrlarýn bileþenlerine ayýrýr ve döndürür.
	char **args;
	char*arg;
	args=(char**)malloc(SIZE*sizeof(char*));
	
    	arg=strtok(line, "/\n");						// Verilen parametrelere göre stringi ayýrýr.
		while(arg!=NULL){
			args[*i]=arg;
			*i+=1;
			arg=strtok(NULL,"/\n");
		}
return args;
}


int key_bul(char *val,int size){						// Hashing için verilen stringe göre key deðeri hesaplar ve döndürür.
		int i,carpim_katsayisi=1,yedek=1;				// Deðerleri istediðim zaman deðiþtirebilmek için deðiþkenlere baðladým.		
	int anahtar=0;
	
	for (i=0;i<strlen(val);i++){
		anahtar+=val[i]*carpim_katsayisi;
		carpim_katsayisi=carpim_katsayisi*yedek;
	}
	
	anahtar=anahtar%size;
return anahtar;
}

// Verilen iki node u birbirine ekler. Bir oyuncu bir filmde oynuyorsa : film -> oyuncu  ve  oyuncu -> film  gösterimlerini saðlar.
void node_bagla(struct Graph *filmler,struct Graph *oyuncular, int filmid, int oyuncuid,char* film_isim,char *oyuncu_isim)
{

	struct node* newNode = node_olustur(oyuncuid,oyuncu_isim,1);	// Parametre olan 1 oluþturulan node un bir oyuncu node u olduðunu belirtir.
    if(filmler->adjLists[filmid]->next!=NULL){
		newNode->next = filmler->adjLists[filmid]->next;			// Listenin sonuna gitmek yerine ilk elemandan sonrasýna araya gir.
    	filmler->adjLists[filmid]->next = newNode;					// Listenin ilk elemaný filmin adýný, baðlý elemanlar ise oyuncu adlarýydý.
	}else{
		filmler->adjLists[filmid]->next = newNode;					// Henüz ekleme yapýlmamýþsa doðrudan yerleþ.
	}
	
 	newNode = node_olustur(filmid,film_isim,0);						// Parametre olan 0 oluþturulan node un bir film node u olduðunu belirtir.
 	if(oyuncular->adjLists[oyuncuid]!=NULL){
 		newNode->next = oyuncular->adjLists[oyuncuid]->next;
    	oyuncular->adjLists[oyuncuid]->next= newNode;
	}else{
		oyuncular->adjLists[oyuncuid]->next= newNode;
	}
    
}

// Verilen oyuncu veya filmi oluþturulmuþ graph a hash deðerlerine göre ekler. Return olarak eklediði yerin adresini verir.
int graph_yerlestir(struct Graph *graph,char* ad,int size,int film_oyuncu){ 
																			
	
	int i,j=0,k=0,say=0;
	int key;
	
	key=key_bul(ad,size);			// Key deðerini bul.
	while(graph->adjLists[(key+j)%size]!=NULL&&say<size){	// Eðer doluysa bir sonraki göze bak.
		k++;
		j=(k*k)%size;										// Yýðýlmalarý engellemek amaçla quadratic olarak ayarla.
		say++;												// Tablo size ý geçme.
	}
		
	struct node* newNode = node_olustur((key+j)%size,ad,film_oyuncu); // Bulunan yere yerleþ.
	graph->adjLists[(key+j)%size]=newNode;
	
return (key+j)%size;										// Yerleþtirilen adresi gönder.
}


int id_list_search(struct Graph *graph,char*ad,int size){	// Verilen graphta verilen ismi arar.Return olarak bulduðu yeri verir.
	int key=0,say=0;
	int j=0,k=0;
	
	key=key_bul(ad,size);									// Verilen ismin hash deðerini hesapla.
	while(graph->adjLists[(key+j)%size]!=NULL&&say<size){	// Hash deðerinden itibaren aramaya baþla.
		if(strcmp(graph->adjLists[(key+j)%size]->isim,ad)==0)
			return (key+j)%size;
		k++;
		j=(k*k)%size;
		say++;
	}
	
	return -1;												// Bulamamýþ isen kullanýcýyý bilgilendir.
	
}


int main(){
	char **argumanlar;				// Verilen stringin parçalarýna ayrýlmýþ halini tutar.
	struct Graph* filmler;			// Filmler graphýný tutar.
	struct Graph* oyuncular;		// Oyuncular graphýný tutar.
	struct Graph* point;			// Graphlar üzerinde ilerlemek için kullanýlýr.
	int indis=0,i,id=0,film_id,gr_size=0,film_size=0,grp_ret_val,secim;
	//indis : satýrlarda kaç eleman olduðunu tutar.
	//id : Oyuncunun hashte yerleþtiði yeri tutar.
	//filmid : Filmin hashte yerleþtiði yeri tutar.
	//grsize : input filedaki oyuncu ve filmin toplam sayýsýný tutar.
	//filmsize : film sayýlarýný tutar.
	//grp_ret_val : graphta arama yapýldýðýnda arama sonucunu tutar.
	char buff[2048];				// Filedan satýr okuma yapar.
	char ilk_buf[40];				// Kullanýnýn adýný ve soyadýný tutar.
	char iki_buf[25];				// Kullanýcýn adýný tutar.
	
	FILE *fptr;
	
	fptr=fopen("input-mpaa.txt","r");

	while(fgets(buff,sizeof(buff),fptr)!=NULL){	// Verilen inputta kaç film ve kaç oyuncu olduðunu tutmak için boþ gez.
		argumanlar=kelime_ayir(buff,&indis);
		film_size++;							// Filmlerin sayýsýný getirir.
		for(i=0;i<indis;i++){
			gr_size++;							// Graph size getirir.
		}
	indis=0;									// Her defasýnda bir satýrda kaç tane eleman olduðunu tutar.
	}

	film_size=asal_bul(film_size);				// Hashteki deðeri asal olarak seç
	filmler=graph_olustur(film_size+1);			// Filmler graphýný oluþtur.
	oyuncular=graph_olustur(gr_size);			// Oyuncular graphýný oluþtur.
	
	rewind(fptr);								// Boþ gezmeden sonra tekrar baþa gel.
	
	while(fgets(buff,sizeof(buff),fptr)!=NULL){	// Dosyanýn sonuna kadar oku.
		argumanlar=kelime_ayir(buff,&indis);	// Alýnan satýrý elemanlarýna ayýr.

		film_id=graph_yerlestir(filmler,argumanlar[0],film_size,0); // Film her zaman satýrdaki ilk elemandýr.
		
		for(i=1;i<indis;i++){
		
			grp_ret_val=id_list_search(oyuncular,argumanlar[i],gr_size); // Daha önce eklenmiþ mi kontrol et.
			
			if(grp_ret_val==-1){				// Eklenmemiþ ise ekle, ekledikten sonra film ve oyuncuyu baðla.
				id=graph_yerlestir(oyuncular,argumanlar[i],gr_size,1);
				node_bagla(filmler,oyuncular,film_id,id,argumanlar[0],argumanlar[i]);
			}else{
				node_bagla(filmler,oyuncular,film_id,grp_ret_val,argumanlar[0],argumanlar[i]);// Daha önce eklenmiþse baðla.
			}
		}
	indis=0;									// Alýnan arguman sayýsýný bir sonraki tur için sýfýrla.
	}
	fclose(fptr);
//ELÝMDE OYUNCULAR VE FÝLMLER VAR BFS BAÞLAYABÝLÝRÝM.

	memset(ilk_buf,0,sizeof(char)*40);
	printf("Kevin Bacon Sayisi Bulma Programi\n----------------------------------------------\n");
	printf("Ilk oyuncu ismini giriniz : ");
	
	gets(ilk_buf);
	//printf("Girilen : %s\n",ilk_buf);
	int ilk_id=id_list_search(oyuncular,ilk_buf,gr_size);// Ýlk oyuncunun id sini tut.
	
	fflush(stdin);
	memset(ilk_buf,0,sizeof(char)*40);			
	
	printf("Ikinci oyuncu ismini giriniz : ");
	
	
	gets(ilk_buf);
	//printf("Ikinci : %s\n",ilk_buf);
		
	int ikinci_id=id_list_search(oyuncular,ilk_buf,gr_size);//Ýkinci oyuncunun id sini tut.
	
	
	printf("\n----------------------------------------------\n");
	if(ilk_id!=-1&&ikinci_id!=-1){
	bfs(oyuncular,filmler,oyuncular->adjLists[ilk_id],oyuncular->adjLists[ikinci_id]);	
	}else{
		printf("Girilen oyuncu bulunamadi Inputlari kontrol ediniz.\n");
	}

	printf("\n----------------------------------------------\n");	
	

	
	return 0;
}
