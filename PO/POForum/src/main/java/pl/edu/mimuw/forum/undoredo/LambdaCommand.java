package pl.edu.mimuw.forum.undoredo;

public class LambdaCommand implements ICommand {

	private Runnable undo;
	private Runnable redo;
	
	public LambdaCommand(Runnable undo, Runnable redo){
		this.undo = undo;
		this.redo = redo;
	}
	
	@Override
	public void undo() {
		undo.run();
		
	}

	@Override
	public void redo() {
		redo.run();
	}

}
