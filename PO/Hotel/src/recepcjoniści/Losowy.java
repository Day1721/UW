package recepcjoniści;

import java.util.Random;

import hotel.Pokój;
import hotel.Recepcjonista;
import hotel.Zamówienie;

public class Losowy extends Recepcjonista {

	public Losowy(String imięINazwisko) {
		super(imięINazwisko);
	}

	@Override
	public Pokój akceptuj(Pokój[] pokoje, Zamówienie zamówienie) {
		Random random = new Random();
		return pokoje[random.nextInt(pokoje.length)];
	}

	@Override
	public String nazwa() {
		return "losowa";
	}

}
