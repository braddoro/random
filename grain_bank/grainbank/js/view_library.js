function ViewLibrary() {

	this.test = function(inStr) {
		alert(inStr);
	}	

	this.showIt = function showIt(thing) {
		if (document.getElementById(thing).style.display != 'block') {
			document.getElementById(thing).style.display = 'block';
		} else {
			document.getElementById(thing).style.display = 'none';
		}
	}
}