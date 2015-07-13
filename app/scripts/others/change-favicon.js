var favicon = {
 
change: function(iconURL) {
  if (arguments.length==2) {
    document.title = optionalDocTitle;
  }
  this.addLink(iconURL, "icon");
  this.addLink(iconURL, "shortcut icon");
  console.log(iconURL);
},
 
addLink: function(iconURL, relValue) {
  var link = document.createElement("link");
  link.type = "image/x-icon";
  link.rel = relValue;
  link.href = iconURL;
  this.removeLinkIfExists(relValue);
  this.docHead.appendChild(link);
},
 
removeLinkIfExists: function(relValue) {
  var links = this.docHead.getElementsByTagName("link");
  for (var i=0; i<links .length; i++) {
    var link = links[i];
    if (link.type=="image/x-icon" && link.rel==relValue) {
      this.docHead.removeChild(link);
      return; // Assuming only one match at most.
    }
  }
},
 
docHead:document.getElementsByTagName("head")[0]
}