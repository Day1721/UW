package hotel;

import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Random;

public class Hotel {
	private ArrayList<Pokój> pokoje;
	private ArrayList<Recepcjonista> recepcjoniści;
	private ArrayDeque<Zamówienie> kolejka = new ArrayDeque<Zamówienie>();
	public Hotel(Pokój[] pokoje, Recepcjonista[] recepcjoniści){
		this.pokoje = new ArrayList<Pokój>();
		for(Pokój pokój : pokoje){
			this.pokoje.add(pokój);
		}
		this.recepcjoniści = new ArrayList<Recepcjonista>();
		for(Recepcjonista recepcjonista : recepcjoniści){
			this.recepcjoniści.add(recepcjonista);
		}
	}
	public void akceptuj(Zamówienie[] zamówienia, Pokój[] pokoje, Recepcjonista[] recepcjoniści){
		for(Zamówienie zamówienie : zamówienia){
			kolejka.add(zamówienie);
		}
		for(Pokój pokój : pokoje){
			if(!this.pokoje.contains(pokój)){
				System.out.println("Nielegalny pokój w tablicy wejściowej!");
				return;
			}
		}
		for(Recepcjonista recepcjonista : recepcjoniści){
			if(!this.recepcjoniści.contains(recepcjonista)){
				System.out.println("Nielegalny recepcjonista w tablicy wejściowej!");
				return;
			}
		}
		int i = 0;
		while(!kolejka.isEmpty()){
			Zamówienie zamówienie = kolejka.poll();
			Pokój proponowanyPokój = recepcjoniści[i].akceptuj(pokoje, zamówienie);
			System.out.println("Recepcjonista: " + recepcjoniści[i].toString() + ".\n");
			System.out.println("Zamówienie: " + zamówienie.toString() + "\n");
			System.out.println("Proponowany pokój: " + (proponowanyPokój != null ? proponowanyPokój.wypiszOpis() : "") + "\n");
			System.out.println("Klient: " + zamówienie.klient().toString() + ".\n");
			boolean decyzja = zamówienie.klient().czyAkceptujeszPropozycję(proponowanyPokój, zamówienie);
			System.out.println((decyzja ? "tak\n" : "nie\n"));
			if(decyzja){
				proponowanyPokój.dodajPrzedział(zamówienie.przedziałCzasowy());
			}
			else{
				zamówienie.zwiększLicznik();
				if(zamówienie.licznik() < 3){
					kolejka.add(zamówienie);
				}
			}
			i = (i == recepcjoniści.length - 1 ? 0 : i + 1);
		}
	}
	
	public static Zamówienie przyjmijZamówienie(Klient klient, Ankieta ankieta){
		return new Zamówienie(klient, ankieta.preferowanyPokój(), ankieta.dataPrzyjazdu(), ankieta.liczbaDni());
	}
	
	public static void main(String[] args){
		Random random = new Random();
		Pokój[] pokoje = new Pokój[100];
		for(int i = 0; i < 100; i++){
			pokoje[i] = new Pokój(i + 1, random.nextInt(4) + 1, 100 * (random.nextInt(10) + 2), 
					Styl.values()[random.nextInt(5)], Kolorystyka.values()[random.nextInt(6)], 
					Kierunek.values()[random.nextInt(4)], random.nextBoolean());
		}
		String[] imiona = {"Michał", "Marcin", "Janusz", "Adam", "Marek", "Wojciech", "Tomasz"};
		String[] nazwiska = {"Kowalski", "Nowak", "Wiśniewski", "Lewandowski", "Dąbrowski", "Pawłowski"};
		Recepcjonista[] recepcjoniści = new Recepcjonista[10];
		for(int i = 0; i < 10; i++){
			switch(random.nextInt(4)){
			case 0:
				recepcjoniści[i] = new recepcjoniści.Perfekcjonista(imiona[random.nextInt(7)] + " " + nazwiska[random.nextInt(6)]);
				break;
			case 1:
				recepcjoniści[i] = new recepcjoniści.Losowy(imiona[random.nextInt(7)] + " " + nazwiska[random.nextInt(6)]);
				break;
			case 2:
				recepcjoniści[i] = new recepcjoniści.Złośliwy(imiona[random.nextInt(7)] + " " + nazwiska[random.nextInt(6)]);
				break;
			case 3:
				recepcjoniści[i] = new recepcjoniści.Aproksymacyjny(imiona[random.nextInt(7)] + " " + nazwiska[random.nextInt(6)]);
				break;
			}
		}
		Klient[] klienci = new Klient[50];
		for(int i = 0; i < 50; i++){
			Calendar kalendarz = Calendar.getInstance();
			kalendarz.set(2018, 1 + random.nextInt(12), 1 + random.nextInt(28));
			Pokój pokój = new Pokój(0, random.nextInt(4) + 1, 100 * (random.nextInt(10) + 2), 
					Styl.values()[random.nextInt(5)], Kolorystyka.values()[random.nextInt(6)], 
					Kierunek.values()[random.nextInt(4)], random.nextBoolean());
			switch(random.nextInt(5)){
			case 0:
				klienci[i] = new klienci.Perfekcjonista(imiona[random.nextInt(7)] + " " + nazwiska[random.nextInt(6)], 
						kalendarz, 1 + random.nextInt(10), pokój);
				break;
			case 1:
				klienci[i] = new klienci.Ugodowy(imiona[random.nextInt(7)] + " " + nazwiska[random.nextInt(6)], 
						kalendarz, 1 + random.nextInt(10), pokój);
				break;
			case 2:
				klienci[i] = new klienci.Widokowy(imiona[random.nextInt(7)] + " " + nazwiska[random.nextInt(6)], 
						kalendarz, 1 + random.nextInt(10), pokój);
				break;
			case 3:
				klienci[i] = new klienci.Połowiczny(imiona[random.nextInt(7)] + " " + nazwiska[random.nextInt(6)], 
						kalendarz, 1 + random.nextInt(10), pokój);
				break;
			case 4:
				klienci[i] = new klienci.Budżetowy(imiona[random.nextInt(7)] + " " + nazwiska[random.nextInt(6)], 
						kalendarz, 1 + random.nextInt(10), pokój);
				break;
			}
		}
		Hotel hotel = new Hotel(pokoje, recepcjoniści);
		Zamówienie[] zamówienia = new Zamówienie[50];
		for(int i = 0; i < 50; i++){
			zamówienia[i] = przyjmijZamówienie(klienci[i], klienci[i].wypełnijAnkietę());
		}
		hotel.akceptuj(zamówienia, pokoje, recepcjoniści);
	}
}
