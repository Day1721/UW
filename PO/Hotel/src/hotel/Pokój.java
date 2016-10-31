package hotel;

import java.util.ArrayList;

public class Pokój {
	private int numer;
	private int liczbaOsób;
	private int cena;
	private Styl styl;
	private Kolorystyka kolor;
	private Kierunek kierunek;
	private boolean internet;
	private ArrayList<PrzedziałCzasowy> kalendarz = new ArrayList<PrzedziałCzasowy>();
	
	public Pokój(int numer, int liczbaOsób, int cena, 
			Styl styl, Kolorystyka kolor, Kierunek kierunek, boolean internet){
		this.numer = numer;
		this.liczbaOsób = liczbaOsób;
		this.cena = cena;
		this.styl = styl;
		this.kolor = kolor;
		this.kierunek = kierunek;
		this.internet = internet;
	}
	
	public int numer(){
		return numer;
	}
	public int liczbaOsób(){
		return liczbaOsób;
	}
	public int cena(){
		return cena;
	}
	public Styl styl(){
		return styl;
	}
	public Kolorystyka kolor(){
		return kolor;
	}
	public Kierunek kierunek(){
		return kierunek;
	}
	public boolean internet(){
		return internet;
	}
	public boolean czyWolny(PrzedziałCzasowy przedział){
		boolean wynik = true;
		for(PrzedziałCzasowy p : kalendarz){
			if(p.porównaj(przedział) == 0){
				wynik = false;
				break;
			}
		}
		return wynik;
	}
	public Pokój dodajPrzedział(PrzedziałCzasowy przedział){
		if(kalendarz.size() == 0){
			kalendarz.add(przedział);
		}
		for(int i = 0; i < kalendarz.size(); i++){
			if(kalendarz.get(i).porównaj(przedział) == -1){
				kalendarz.add(i, przedział);
				return this;
			}
			if(kalendarz.get(i).porównaj(przedział) == 0){
				return this;
			}
		}
		if(kalendarz.get(kalendarz.size() - 1).porównaj(przedział) == -1){
			kalendarz.add(przedział);
		}
		return this;
	}
	
	public String wypiszOpis(){
		return "liczba osób: " + liczbaOsób + ", cena: " + cena + ", styl: " + styl.toString() + ", kolor: " + 
				kolor.toString() + ", kierunek: " + kierunek.toString() + ", dostęp do internetu: " + (internet ? "TAK" : "NIE");
	}
	
	public boolean porównaj(Pokój pokój){
		return liczbaOsób == pokój.liczbaOsób && cena == pokój.cena && styl == pokój.styl && kolor == pokój.kolor &&
				kierunek == pokój.kierunek && internet == pokój.internet; 
	}
}
