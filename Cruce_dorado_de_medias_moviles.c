/*
https://www.tecnicasdetrading.com/2013/06/estrategia-del-cruce-dorado-de-medias-moviles.html

Cooding with:
https://zorro-project.com/
*/

vars PriceAVG;
bool flagLong = false;
bool flagShort = false;

vars EMA_X(var periodo){
	vars Solucion = series(EMA(PriceAVG, periodo), 2);
	return Solucion;
}

void EShort(){
	enterShort();
	Stop = priceH() + 30*PIP;
	TakeProfit = priceH() - 90*PIP;
}

void ELong(){
	enterLong();
	Stop = priceL() - 30*PIP;
	TakeProfit = priceL() + 90*PIP;
}

function run(){
	BarPeriod = 15;
	LookBack = 209;
	
	PriceAVG = series(price());
	
	vars ema10, ema20, ema30, ema144, ema169;
	
	ema10 = EMA_X(10);
	ema20 = EMA_X(20);
	ema30 = EMA_X(30);
	ema144 = EMA_X(144);
	ema169 = EMA_X(169);
	
	if(!TradeIsOpen){
		if(flagLong){
			flagLong = false;
			ELong();

		}
		else if(flagShort){
			flagShort = false;
			EShort();
		}
		
		
		/*
		La primera entrada se produce cuando el EMA 10 
		cruza las dos EMA más lentas (144 y 169 periodos). 
		En este caso, la dirección de la posición (long o short) 
		es la misma que presenta el cruce de medias móviles.
		*/
		else if(crossOver(ema10, ema144) && crossOver(ema10, ema169) ){
			ELong();
		}
		else if(crossUnder(ema10, ema144) && crossUnder(ema10, ema169) ){
			EShort();
		}
		
		
		
		/*
		La segunda entrada es la operación principal. Se abre 
		una posición después de una corrección del precio junto 
		con el cruce de las EMA de 20 y 30 periodos sobre las EMA 
		más lentas (144 y 169 periodos).
		*/

		else if(crossOver(ema20, ema144) && crossOver(ema30, ema144)
		&& crossOver(ema30, ema169) && crossOver(ema30, ema169)){
			flagLong = true; 
		}
		else if(crossUnder(ema20, ema144) && crossUnder(ema30, ema144)
		&& crossUnder(ema30, ema169) && crossUnder(ema30, ema169)){
			flagShort = true;
		}
	}
	
	if(TradeIsOpen){
		plot("Stop Loss", Stop, LINE, RED);
		plot("Take Profit", TakeProfit, LINE, GREEN);
	}
	
	
	plot("EMA10", ema10, LINE, CYAN);
	plot("EMA20", ema20, LINE, LIGHTBLUE);
	plot("EMA30", ema30, LINE, PURPLE);
	plot("EMA144", ema144, LINE, YELLOW);
	plot("EMA169", ema169, LINE, MAGENTA);
	
	
}
