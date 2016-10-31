package hotel;

public abstract class Recepcjonista {
	protected String imięINazwisko;
	public Recepcjonista(String imięINazwisko){
		this.imięINazwisko = imięINazwisko;
	}
	public abstract Pokój akceptuj(Pokój[] pokoje, Zamówienie zamówienie);
	public abstract String nazwa();
	
	@Override
	public String toString(){
		return imięINazwisko + ", " + nazwa();
	}
}
