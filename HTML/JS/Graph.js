

function lireFichierTexte1(fichier)
{
	//On lance la requête pour récupérer le fichier
	var fichierBrut = new XMLHttpRequest();
	fichierBrut.open("GET", fichier, false);
	//On utilise une fonction sur l'événement "onreadystate"
	fichierBrut.onreadystatechange = function ()
	{
		if(fichierBrut.readyState === 4)
		{
			//On contrôle bien quand le statut est égal à 0
			if(fichierBrut.status === 200 || fichierBrut.status == 0)
			{
				//On peut récupérer puis traiter le texte du fichier
				var texteComplet = fichierBrut.responseText;
				valGraph1 = texteComplet.split("\n")
			}
		}
	}
	fichierBrut.send(null);
}

function lireFichierTexte2(fichier)
{
	//On lance la requête pour récupérer le fichier
	var fichierBrut = new XMLHttpRequest();
	fichierBrut.open("GET", fichier, false);
	//On utilise une fonction sur l'événement "onreadystate"
	fichierBrut.onreadystatechange = function ()
	{
		if(fichierBrut.readyState === 4)
		{
			//On contrôle bien quand le statut est égal à 0
			if(fichierBrut.status === 200 || fichierBrut.status == 0)
			{
				//On peut récupérer puis traiter le texte du fichier
				var texteComplet = fichierBrut.responseText;
				valGraph2 = texteComplet.split("\n")
			}
		}
	}
	fichierBrut.send(null);
}

function lireFichierTexte3(fichier)
{
	//On lance la requête pour récupérer le fichier
	var fichierBrut = new XMLHttpRequest();
	fichierBrut.open("GET", fichier, false);
	//On utilise une fonction sur l'événement "onreadystate"
	fichierBrut.onreadystatechange = function ()
	{
		if(fichierBrut.readyState === 4)
		{
			//On contrôle bien quand le statut est égal à 0
			if(fichierBrut.status === 200 || fichierBrut.status == 0)
			{
				//On peut récupérer puis traiter le texte du fichier
				var texteComplet = fichierBrut.responseText;
				valGraph3 = texteComplet.split("\n")
			}
		}
	}
	fichierBrut.send(null);
}

function lireFichierTexte4(fichier)
{
	//On lance la requête pour récupérer le fichier
	var fichierBrut = new XMLHttpRequest();
	fichierBrut.open("GET", fichier, false);
	//On utilise une fonction sur l'événement "onreadystate"
	fichierBrut.onreadystatechange = function ()
	{
		if(fichierBrut.readyState === 4)
		{
			//On contrôle bien quand le statut est égal à 0
			if(fichierBrut.status === 200 || fichierBrut.status == 0)
			{
				//On peut récupérer puis traiter le texte du fichier
				var texteComplet = fichierBrut.responseText;
				valGraph4 = texteComplet.split("\n")
			}
		}
	}
	fichierBrut.send(null);
}

function lireFichierTexte5(fichier)
{
	//On lance la requête pour récupérer le fichier
	var fichierBrut = new XMLHttpRequest();
	fichierBrut.open("GET", fichier, false);
	//On utilise une fonction sur l'événement "onreadystate"
	fichierBrut.onreadystatechange = function ()
	{
		if(fichierBrut.readyState === 4)
		{
			//On contrôle bien quand le statut est égal à 0
			if(fichierBrut.status === 200 || fichierBrut.status == 0)
			{
				//On peut récupérer puis traiter le texte du fichier
				var texteComplet = fichierBrut.responseText;
				valGraph5 = texteComplet.split("\n")
			}
		}
	}
	fichierBrut.send(null);
}

function lireFichierTexte6(fichier)
{
	//On lance la requête pour récupérer le fichier
	var fichierBrut = new XMLHttpRequest();
	fichierBrut.open("GET", fichier, false);
	//On utilise une fonction sur l'événement "onreadystate"
	fichierBrut.onreadystatechange = function ()
	{
		if(fichierBrut.readyState === 4)
		{
			//On contrôle bien quand le statut est égal à 0
			if(fichierBrut.status === 200 || fichierBrut.status == 0)
			{
				//On peut récupérer puis traiter le texte du fichier
				var texteComplet = fichierBrut.responseText;
				valGraph6 = texteComplet.split("\n")
			}
		}
	}
	fichierBrut.send(null);
}
