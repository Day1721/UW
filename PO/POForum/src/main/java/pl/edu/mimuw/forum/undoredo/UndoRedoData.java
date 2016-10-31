package pl.edu.mimuw.forum.undoredo;

import java.util.List;

import pl.edu.mimuw.forum.ui.tree.ForumTreeItem;

public class UndoRedoData {
	private Action type;
	private int from;
	private int to;
	private List<ForumTreeItem> elements;
	private ForumTreeItem parent;
	
	public UndoRedoData(Action type, int from, int to, List<ForumTreeItem> elements, ForumTreeItem parent){
		this.elements = elements;
		this.from = from;
		this.to = to;
		this.type = type;
		this.parent = parent;
	}
	
	public UndoRedoData(Action type, int from, List<ForumTreeItem> elements, ForumTreeItem parent){
		this(type, from, from + elements.size() - 1, elements, parent);
	}
	
	public Action getType(){
		return type;
	}
	
	public int getFrom(){
		return from;
	}
	
	public int getTo(){
		return to;
	}
	
	public List<ForumTreeItem> getElements(){
		return elements;
	}
	
	public ForumTreeItem getParent(){
		return parent;
	}
}
