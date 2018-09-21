//Libs do M5Stack e DHT
#include <M5Stack.h>
#include <SimpleDHT.h>

///////////////////////////////////////////////////////////
#include <WiFi.h>
#include <HTTPClient.h>
 
const char* ssid = "espacioRES";
const char* password =  "octubre2014res";
///////////////////////////////////////////////////////////


// definição das cores que serão utilizadas
#define BLACK           0x0000
#define RED             0xF800
#define CYAN            0x07FF
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

#define DHTPIN 5 //pino de dados do DHT

//construtor do objeto para comunicar com o sensor
SimpleDHT11 dht11(DHTPIN);

//variável responsavel por contar o número de leituras realizadas e indicador do valor no eixo X
int leituraAtual = 1;

/*/////////////////// X-axis and Y-axis positioning definitions, /////////
temperature and humidity data display coordinates,
as well as the variables that will store the values read.
We still point to the variable for printing the chart on the display
 */

//definições do posicionamento dos eixos X e Y
#define POS_X_GRAFICO       30
#define POS_Y_GRAFICO       3
#define ALTURA_GRAFICO      180
#define COMPRIMENTO_GRAFICO 270

//definição da coordenada onde escreveremos os dados de temperatura e umidade
#define POS_X_DADOS 30
#define POS_Y_DADOS 200

//variáveis que armazenarão os valores lidos da umidade e temperatura
int umidade = 0;
int temperatura = 0;

//variável que armazenará o valor da coordenada Y para desenharmos uma linha de exemplo
//que varia os valores de 1 em 1
int linhaExemplo = 20;
int fator = 1; //indicará se somaremos ou subtrairemos uma unidade na variável linhaExemplo

/*/////// SETUP //////////////////////////////////////////////
In Setup, we initialize the M5Stack. We define commands for drawing the graph,
configure the font as well as text colors, and position the cursor for writing.
*/
void setup(void) {
  Serial.begin(115200);
  
  //Inicializa o M5Stack
  M5.begin();

  //pinta a tela toda de preto
  M5.Lcd.fillScreen(BLACK);

  //os comandos a seguir irão desenhar as linhas dos eixos cartesianos na cor branca
  //drawFastVLine(x,y,width,color) --> linha vertical
  M5.Lcd.drawFastVLine(POS_X_GRAFICO,POS_Y_GRAFICO, ALTURA_GRAFICO,WHITE); //eixo Y
  //drawFastHLine(x,y,width,color) --> linha horizontal
  M5.Lcd.drawFastHLine(POS_X_GRAFICO,ALTURA_GRAFICO+1,COMPRIMENTO_GRAFICO,WHITE); //eixo X

  //configura o tamnaho do texto que escreveremos em tela
  M5.Lcd.setTextSize(3);
  //configura a cor branca para o texto
  M5.Lcd.setTextColor(WHITE);

  //posiciona o cursor para escrita
  M5.Lcd.setCursor(POS_X_DADOS, POS_Y_DADOS);
  M5.Lcd.print("T: "); //indicando a temperatura
  M5.Lcd.setCursor(POS_X_DADOS+105, POS_Y_DADOS);
  M5.Lcd.print(" U: "); //indicando a umidade

  ///////////////////////////////////////////////////////////
  delay(4000);   //Delay needed before calling the WiFi.begin
 
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(1000);
    Serial.println("Connecting to WiFi..");
    }
  Serial.println("Connected to the WiFi network");
  ///////////////////////////////////////////////////////////
  
  }

  /*///////////////////// LOOP ///////////////////////////////////////////////
  In the first part of the Loop, we read the temperature and humidity,
  map the value of the variables for placement on the graph, and
  draw the point on the screen that refers to these values read from the sensor.
  */
  void loop() {
  // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT11...");
  
  //Fazemos a leitura da temperatura e umiade
  byte temp, umid;

  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temp, &umid, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return;
  }
  temperatura = temp;
  umidade = umid;
  
  Serial.print("Sample OK: ");
  Serial.print((int)temperatura); Serial.print(" *C, "); 
  Serial.print((int)umidade); Serial.println(" H");
  
  //mapeando o valor das variáveis para colocar no gráfico
  //necessário pois o display tem 240px de altura e separamos apenas 180 para o gráfico
  //umidade pode ser lida de 0-100
  int temperaturaMapeada = map(temperatura,0,100,0,ALTURA_GRAFICO);
  int umidadeMapeada = map(umidade,0,100,0,ALTURA_GRAFICO);
  //desenha na tela o ponto referente aos valores lidos do sensor
   M5.Lcd.drawPixel(POS_X_GRAFICO+leituraAtual, ALTURA_GRAFICO-temperaturaMapeada, RED);
   M5.Lcd.drawPixel(POS_X_GRAFICO+leituraAtual, ALTURA_GRAFICO-umidadeMapeada, CYAN);
  //desenha na tela o ponto referente a nossa linha de exemplo que fica variando
   M5.Lcd.drawPixel(POS_X_GRAFICO+leituraAtual, ALTURA_GRAFICO-linhaExemplo, YELLOW);

   ////////// Then, we define possibilities of printing the graphic on the display. /////////
   //aqui controlamos nossa linha de exemplo, quando chega no valor máximo decrementamos o valor
  //até um valor mínimo determinado (no nosso caso 10), e a partir daí, incrementa novamente
  if(linhaExemplo == 50) fator = -1;
  else if(linhaExemplo == 10) fator = 1;
  //soma o valor de linhaExemplo
  linhaExemplo += fator;
  //incrementa o contador de leituras realizadas
  leituraAtual++;
  //se a leitura chegar em 270 (número máximo do eixo X) então limparemos a área do gráfico para voltarmos a desenhar.
  if(leituraAtual == 270)
  {
    //limpa a área toda do gráfico
    M5.Lcd.fillRect(POS_X_GRAFICO+1, POS_Y_GRAFICO-1, COMPRIMENTO_GRAFICO, ALTURA_GRAFICO-1, BLACK);
    leituraAtual = 1; //volta o contador de leitura para 1 (nova coordenada X)
  }
  //limpa a área onde colocamos o valor da temperatura e da umidade
  M5.Lcd.fillRect(POS_X_DADOS+50, POS_Y_DADOS, 60, 30, BLACK);
  M5.Lcd.fillRect(POS_X_DADOS+165, POS_Y_DADOS, 90, 30, BLACK);

  //reposiciona o cursor para escrever a temperatura
  M5.Lcd.setCursor(POS_X_DADOS+50, POS_Y_DADOS);
  
  M5.Lcd.setTextColor(RED);
  M5.Lcd.print(temperatura);
  M5.Lcd.print((char)247);

  //reposiciona o cursor para escrever a umidade
  M5.Lcd.setCursor(POS_X_DADOS+165, POS_Y_DADOS);

  M5.Lcd.setTextColor(CYAN);
  M5.Lcd.print(umidade);
  M5.Lcd.print("%");

  ///////////////////////////////////////////////////////////
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
    HTTPClient http;   
    http.begin("http://ec2-34-245-115-53.eu-west-1.compute.amazonaws.com/api/v1/DHT_FROM_M5STACK/attributes");  //Specify destination for HTTP request
    http.addHeader("Content-Type", "application/json");             //Specify content-type header
    int httpResponseCode = http.POST("{\"temperatura\": " + String(temperatura) + ", \"humedad\": " + String(umidade) + "}"); 
    //int httpResponseCode = http.POST("{\"temperatura\": 26.99,\"humedad\" : 60.5}");   //Send the actual POST request
    // int httpResponseCode = http.POST(" "{\"temperatura\": " + String(temperatura) + ",\"humedad\" : " + String(umidade) + "}"");
    if(httpResponseCode>0)  {
      String response = http.getString();                       //Get the response to the request
      Serial.println(httpResponseCode);   //Print return code
      Serial.println(response);           //Print request answer
    }
    else  {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    http.end();  //Free resources
    }
    else
      Serial.println("Error in WiFi connection");  
  ///////////////////////////////////////////////////////////
  
    
    // DHT11 sampling rate is 1HZ.
    delay(1200);  // Some miliseconds above 1000 to avoid errors
  }
