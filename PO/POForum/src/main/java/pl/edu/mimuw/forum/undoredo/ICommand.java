package pl.edu.mimuw.forum.undoredo;

public interface ICommand {
	public void undo();
	public void redo();
}
