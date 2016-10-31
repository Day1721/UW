package recepcjoniści;

import hotel.Pokój;
import hotel.Recepcjonista;
import hotel.Zamówienie;

public class Złośliwy extends Recepcjonista {

	public Złośliwy(String imięINazwisko) {
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
		for(int i = 1; i < pokoje.length; i++){
			if(porównajPokoje(proponowanyPokój, zamówienie.preferowanyPokój()) <= porównajPokoje(pokoje[i], zamówienie.preferowanyPokój())){
				if(porównajPokoje(proponowanyPokój, zamówienie.preferowanyPokój()) == porównajPokoje(pokoje[i], zamówienie.preferowanyPokój())){
					if(proponowanyPokój.cena() < pokoje[i].cena()){
						proponowanyPokój = pokoje[i];
					}
					else if(proponowanyPokój.cena() == pokoje[i].cena()){
						proponowanyPokój = (proponowanyPokój.numer() > pokoje[i].numer() ? proponowanyPokój : pokoje[i]);
					}
				}
				else{
					proponowanyPokój = pokoje[i];
				}
			}
		}
		return proponowanyPokój;
	}

	@Override
	public String nazwa() {
		return "złośliwa";
	}

}
