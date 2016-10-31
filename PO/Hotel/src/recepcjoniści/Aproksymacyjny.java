package recepcjoniści;

import hotel.Pokój;
import hotel.Recepcjonista;
import hotel.Zamówienie;

public class Aproksymacyjny extends Recepcjonista {

	public Aproksymacyjny(String imięINazwisko) {
		super(imięINazwisko);
	}
	
	private int porównajPokoje(Pokój pokój1, Pokój pokój2){
		return (pokój1.liczbaOsób() <= pokój2.liczbaOsób() ? 0 : 1) + (pokój1.cena() <= pokój2.cena() ? 0 : 1) + 
				(pokój1.styl() == pokój2.styl() ? 0 : 1) + (pokój1.kolor() == pokój2.kolor() ? 0 : 1) + 
				(pokój1.kierunek() == pokój2.kierunek() ? 0 : 1) + (pokój1.internet() == pokój2.internet() ? 0 : 1);
	}

	@Override
	public Pokój akceptuj(Pokój[] pokoje, Zamówienie zamówienie) {
		Pokój proponowanyPokój = pokoje[0];
		for(int i = 0; i < pokoje.length; i++){
			int porównanie1 = porównajPokoje(proponowanyPokój, zamówienie.preferowanyPokój());
			int porównanie2 = porównajPokoje(pokoje[i], zamówienie.preferowanyPokój());
			if(porównanie1 == porównanie2){
				if(proponowanyPokój.cena() == zamówienie.preferowanyPokój().cena()){
					proponowanyPokój = (proponowanyPokój.numer() > pokoje[i].numer() ? pokoje[i] : proponowanyPokój);
				}
				else{
					proponowanyPokój = (proponowanyPokój.cena() < pokoje[i].cena() ? pokoje[i] : proponowanyPokój);
				}
			}
			else{
				proponowanyPokój = (porównanie1 > porównanie2 ? pokoje[i] : proponowanyPokój);
			}
		}
		return proponowanyPokój;
	}

	@Override
	public String nazwa() {
		return "aproksymacyjna";
	}

}
