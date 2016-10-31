package recepcjoniści;

import hotel.Pokój;
import hotel.Recepcjonista;
import hotel.Zamówienie;

public class Perfekcjonista extends Recepcjonista {
	public Perfekcjonista(String imięINazwisko){
		super(imięINazwisko);
	}

	@Override
	public Pokój akceptuj(Pokój[] pokoje, Zamówienie zamówienie) {
		Pokój proponowanyPokój = null;
		for(Pokój pokój : pokoje){
			if(pokój.porównaj(zamówienie.preferowanyPokój())){
				if(proponowanyPokój == null){
					proponowanyPokój = pokój;
				}
				proponowanyPokój = (proponowanyPokój.numer() > pokój.numer() ? pokój : proponowanyPokój);
			}
		}
		return proponowanyPokój;
	}

	@Override
	public String nazwa() {
		return "perfekcjonistyczna";
	}
}
