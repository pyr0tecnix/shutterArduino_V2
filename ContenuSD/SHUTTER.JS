/*Au chargement de la page on récupère les données*/
function launchJsonRequest() {
  console.log("STACK - launchJsonRequest");
  getjSonFromShutter("statut-console-formulaire");
  /*Ajout d'un listener sur la soumission du formulaire de modification de la config*/
  var _form = document.querySelector("#modifier_config");
  _form.addEventListener("submit", function(event){
    /*Annule la soumission classique du formulaire*/
    event.preventDefault();
    formModifierConfig();
  });
  /*Ajout d'un listener sur le changement de la liste de fichier à uploader*/
  var _input = document.querySelector("#file_to_upload");
  _input.addEventListener("change", function() {
    event.preventDefault();
    uploadFile();
  });
}

/*Upload de fichier*/
function uploadFile() {
  console.log("STACK - uploadFile");
  var _input = document.querySelector("#file_to_upload");
  if(_input.files.length > 0) {
    document.querySelector("#p_upload").innerHTML = "Upload de : " + _input.files[0].name;
    var formData = new FormData();
    formData.append('fichier', _input.files[0]);
    sendJsonToShutter(formData);
  }
}

function formModifierConfig() {
  console.log("STACK - formModifierConfig");
  var _form = document.querySelector("#modifier_config");
  var formData = {};
  var date = new Date();
  formData["date_fichier_config"] = date.toDateString();
  /*_form.length - 1 pour ne pas traiter le submit qui est le dernier élément du formulaire*/
  for(cpt = 0; cpt < _form.length - 1; cpt++) {
    if(_form.elements[cpt].type == "select-one") {
      if(_form.elements[cpt].id.indexOf("shutter_verbosite_level") > -1) {
        formData[_form.elements[cpt].id] = _form.elements[cpt].selectedIndex.toString();
      }
      else if(_form.elements[cpt].id.indexOf("shutter_servo_nombre") > -1) {
        formData[_form.elements[cpt].id] = (_form.elements[cpt].selectedIndex  + 1).toString();
      }
      else{
        formData[_form.elements[cpt].id] = _form.elements[cpt].value;
      }
    }
    else if (_form.elements[cpt].type == "checkbox") {
      formData[_form.elements[cpt].id] = _form.elements[cpt].value == "on" ? 1 : 0;
    }
    else {
      formData[_form.elements[cpt].id] = _form.elements[cpt].value;
    }
  }
  console.log(formData);
  sendJsonToShutter(JSON.stringify(formData));
}

/*Recevoir des données de l'arduino*/
function getjSonFromShutter(request){
  console.log("STACK - getjSonFromShutter");
  var xhr = getXMLHttpRequest();
  xhr.open('POST', encodeURI('getdata'));
  xhr.setRequestHeader('contentType', 'application/json');
  xhr.onload = function(){
    try {
      if(xhr.status == 200) {
        var data = JSON.parse(xhr.responseText);
        console.log(data);
        if(request.search("statut") >= 0) {handle_statut(data['statut']);}
        if(request.search("console") >= 0) {handle_console(data['console']);}
        if(request.search("formulaire") >= 0) {handle_formulaire(data['formulaire']);}
      }
    }
    catch(error) {
      console.log("Erreur de réception des données de l'arduino : " + error);
    }
  };
  xhr.send(request);
}

/*Envoyer des données à l'arduino*/
function sendJsonToShutter(request) {
  console.log("STACK - sendJsonToShutter");
  var xhr = getXMLHttpRequest();
  xhr.open('POST', encodeURI('setdata'));
  xhr.setRequestHeader('contentType', 'application/json');
  xhr.send(request);
}

/*Gestion de l'affichage*/

/*Gère l'affichage des commandes directes en fonction du nombre de servomoteur actif*/
function setServomoteurCommandDisplay() {
  console.log("STACK - setServomoteurCommandDisplay");
  var _divControle = document.querySelector("#div_controle");
  /*On ajoute 1 car le tableau commence à 0*/
  var nombreServomoteur = document.querySelector("#shutter_servo_nombre").options[document.querySelector("#shutter_servo_nombre").selectedIndex].text;
  console.log(document.querySelector("#shutter_servo_nombre").options[document.querySelector("#shutter_servo_nombre").selectedIndex].text);

  for(cpt = 0; cpt < nombreServomoteur; cpt++) {
    var _divServo = document.createElement("div");
    _divServo.class = "three column value";
    _divServo.id = ("servomoteur - " + (cpt + 1));
    /*Bloc de commandes*/
    var _html = "";
    _html = "<h6> Servomoteur - " + (cpt + 1) + "</h6>\n";
    _html += "<a class=\"button button-primary\" href=\"javascript:sendJsonToShutter('servomoteur - open - " + (cpt + 1) + "')\">Ouvrir</a>\n";
    _html += "<a class=\"button button-primary\" href=\"javascript:sendJsonToShutter('servomoteur - close - " + (cpt + 1) + "')\">Fermer</a>\n";
    _html += "<a class=\"button button-primary\" href=\"javascript:sendJsonToShutter('servomoteur - position - " + (cpt + 1) + "')\">Position</a>\n";
    _divServo.innerHTML = _html;
    _divControle.appendChild(_divServo);
  }
  /*Ajout des commandes communes*/
  var _divCommun = document.createElement("div");
  _divCommun.class = "two column value";
  _divCommun.id = ("shutter");
  _html = "<br><h6> Shutter </h6>"
  _html += "<a class=\"button button-primary\" href=\"javascript:sendJsonToShutter('shutter - test')\">Test</a>\n";
  _html += "<a class=\"button button-primary\" href=\"javascript:sendJsonToShutter('shutter - reboot')\">Reboot</a>\n";
  _divCommun.innerHTML = _html;
  _divControle.appendChild(_divCommun);
}

/*Fonction d'affichage du statut des différents services du shutter*/
function handle_statut(data) {
  console.log("STACK - handle_statut");
  var value = "";
  /*Statut SD*/
  value = data["statut_SD"] ? "OK" : "NOK";
  document.querySelector("#statut_SD").innerHTML += value;
  /*Statut OSC*/
  value = data["statut_OSC"] ? "OK" : "NOK";
  document.querySelector("#statut_OSC").innerHTML += value;
  /*Statut Bonjour*/
  value = data["statut_Bonjour"] ? "OK" : "NOK";
  document.querySelector("#statut_Bonjour").innerHTML += value;
}

/*Fonction d'affichage du retour console */
function handle_console(data) {
  console.log("STACK - handle_console");
  for(let element in data) {
    if(data.hasOwnProperty(element)) {
      document.querySelector("#console").innerHTML += data[element];
    }
  }
}

/*Fonction permettant d'initialiser les champs du formulaire avec les valeurs
de la configuration actuelle*/
function handle_formulaire(data) {
  console.log("STACK - handle_formulaire");
  for(let element in data) {
    if(data.hasOwnProperty(element)) {
      console.log(element, data[element]);
      try {
        console.log(document.querySelector("#" + element).type);
        if(document.querySelector("#" + element).type == "checkbox") {
          data[element] ? document.querySelector("#" + element).checked = true : document.querySelector("#" + element).checked = false;
        }
        else if(document.querySelector("#" + element).type == "select-one") {
          if(document.querySelector("#" + element).id.indexOf("shutter_servo_nombre") > -1) {
            document.querySelector("#" + element).selectedIndex = (parseInt(data[element]) - 1);
          }
          else {
            document.querySelector("#" + element).selectedIndex = parseInt(data[element]);
          }
        }
        else {
          document.querySelector("#" + element).value = data[element];
        }
      }
      catch (error) {
        console.log("Erreur de pré-remplissage du formulaire " + error);
      }
    }
  }
  /*Module l'affichage des commandes en fonction du nombre de servomoteur*/
  setServomoteurCommandDisplay();
}


/*Fonction d'instanciation de l'objet XMLHttpRequest*/
function getXMLHttpRequest() {
  console.log("STACK - getXMLHttpRequest");
  var xhr = null;

  if (window.XMLHttpRequest || window.ActiveXObject) {
    if (window.ActiveXObject) {
      try {
        xhr = new ActiveXObject("Msxml2.XMLHTTP");
      } catch(e) {
        xhr = new ActiveXObject("Microsoft.XMLHTTP");
      }
    } else {
      xhr = new XMLHttpRequest();
    }
  } else {
    alert("Votre navigateur ne supporte pas l'objet XMLHTTPRequest...");
    return null;
  }

  return xhr;
}
