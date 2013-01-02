#include "pakDataTypes.h"

//Defaults for item XML data
#define DEFAULT_ABSPOSITION				0
#define DEFAULT_ALLOWDIRECTIONALLIGHT	1
#define DEFAULT_ALLOWEXPLODESTREAKS		1
#define DEFAULT_ANIMTHRESHOLD			0
#define DEFAULT_BURNSLOWSANIM			1
#define DEFAULT_CANGETPLAGUE			1
#define DEFAULT_COLLIDEENVIRONMENT		1
#define DEFAULT_COLLIDEITEMS			1
#define DEFAULT_COSTSTAMPS				0
#define DEFAULT_DESCKEY					2
#define DEFAULT_ENABLEFREEZEPOSTANIM	1
#define DEFAULT_ENABLEHFLIP				1
#define DEFAULT_FLOORWALLSHADOW			1
#define DEFAULT_FREEZEONCOLLIDE			0
#define DEFAULT_ILLUMINATE				0
#define DEFAULT_INITIALBURNEXPORTID		0
#define DEFAULT_INITIALBURNPERGROUP		0
#define DEFAULT_INSTASHDOESSPLAT		0
#define DEFAULT_INSTASHSOUNDRESID		0
#define DEFAULT_INSTANTEFFECTS			0
#define DEFAULT_ITEMIDSTRID				0
#define DEFAULT_MODXAMPMAX				0
#define DEFAULT_MODXAMPMIN				0
#define DEFAULT_MODXFREQMAX				1
#define DEFAULT_MODXFREQMIN				1
#define DEFAULT_MODXPHASEMAX			0
#define DEFAULT_MODXPHASEMIN			0
#define DEFAULT_MODXSPEEDMAX			0
#define DEFAULT_MODXSPEEDMIN			0
#define DEFAULT_MODYAMPMAX				0
#define DEFAULT_MODYAMPMIN				0
#define DEFAULT_MODYFREQMAX				1
#define DEFAULT_MODYFREQMIN				1
#define DEFAULT_MODYPHASEMAX			0
#define DEFAULT_MODYPHASEMIN			0
#define DEFAULT_MODYSPEEDMAX			0
#define DEFAULT_MODYSPEEDMIN			0
#define DEFAULT_MONEYITEM				0
#define DEFAULT_MOTORTHRESHOLD			-1
#define DEFAULT_MOUSEGRABSOUNDRESID		0
#define DEFAULT_MOUSEGRABBABLE			1
#define DEFAULT_NAMEKEY					1
#define DEFAULT_ORBITALGRAVITY			0
#define DEFAULT_PLAGUEONCOLLIDE			0
#define DEFAULT_POPSCOINS				1
#define DEFAULT_QUANTITY				1
#define DEFAULT_SCALEVARIANCE			0
#define DEFAULT_SPAWNLIMITBURNEXPORTID	0
#define DEFAULT_SPLITJUMPLASTFRAME		0
#define DEFAULT_UNIQUEIGNITESOUNDRESID	0
#define DEFAULT_UNLISTED				0
#define DEFAULT_VALUESTAMPS				0


wstring getNameFromAnim(wstring sAnimName)
{
	sAnimName.erase(sAnimName.rfind(TEXT(".anim.xml")));	//Erase the ending off the wstring
	size_t start = sAnimName.find_last_of('/') + 1;	//Find last forward slash
	sAnimName.erase(0,start);						//Erase everything up to and including this last slash
	return sAnimName;								//Done
}

//DEBUG Convert wstring to uppercase
/*wstring stoupper( const wstring s )
{
  wstring result = s;
  for(unsigned int i = 0; i < s.size(); i++)
  {
	wchar_t c = s[i];
	if( (c >= 'a') && (c <= 'z') )
	{
		c -= 'a' - 'A';
		result[i] = c;
	}
  }
  
  return result;
}*/

bool itemManifestToXML(const wchar_t* cFilename)
{
	//Open the file
	FILE* f = _wfopen(cFilename, TEXT("rb"));
	if(f == NULL)
	{
		cout << "Error: could not open " << cFilename << " for reading." << endl;
		return false;
	}
	
	//Read in the manifest
	itemManifestHeader imh;
	if(fread(&imh, 1, sizeof(itemManifestHeader), f) != sizeof(itemManifestHeader))
	{
		cout << "Error: unable to read itemManifestHeader from file " << cFilename << endl;
		fclose(f);
		return false;
	}
	
	//Read in the itemManifestRecords
	list<itemManifestRecord> lManifestRecords;
	map<u32, wstring> mItemNames;
	fseek(f, imh.itemsManifest.offset, SEEK_SET);
	for(int i = 0; i < imh.itemsManifest.count; i++)
	{
		itemManifestRecord imr;
		if(fread(&imr, 1, sizeof(itemManifestRecord), f) != sizeof(itemManifestRecord))
		{
			cout << "Error: unable to read itemManifestRecord from file " << cFilename << endl;
			fclose(f);
			return false;
		}
		lManifestRecords.push_back(imr);
		mItemNames[imr.itemId] = getNameFromAnim(getName(imr.animResId));
	}
	
	//Read in the normalDependencies
	vector<normalDependency> vNormalDependencies;
	fseek(f, imh.normalDeps.offset, SEEK_SET);
	for(int i = 0; i < imh.normalDeps.count; i++)
	{
		normalDependency nd;
		if(fread(&nd, 1, sizeof(normalDependency), f) != sizeof(normalDependency))
		{
			cout << "Error: unable to read normalDependency from file " << cFilename << endl;
			fclose(f);
			return false;
		}
		vNormalDependencies.push_back(nd);
	}
	
	//Read in the soundDependencies
	vector<soundDependency> vSoundDependencies;
	fseek(f, imh.soundDeps.offset, SEEK_SET);
	for(int i = 0; i < imh.soundDeps.count; i++)
	{
		soundDependency sd;
		if(fread(&sd, 1, sizeof(soundDependency), f) != sizeof(soundDependency))
		{
			cout << "Error: unable to read soundDependency from file " << cFilename << endl;
			fclose(f);
			return false;
		}
		vSoundDependencies.push_back(sd);
	}
	
	//Read in the effectDependencies
	vector<effectDependency> vEffectDependencies;
	fseek(f, imh.effectDeps.offset, SEEK_SET);
	for(int i = 0; i < imh.effectDeps.count; i++)
	{
		effectDependency ed;
		if(fread(&ed, 1, sizeof(effectDependency), f) != sizeof(effectDependency))
		{
			cout << "Error: unable to read effectDependency from file " << cFilename << endl;
			fclose(f);
			return false;
		}
		vEffectDependencies.push_back(ed);
	}
	
	//Read in the itemDependencies
	vector<itemDependency> vItemDependencies;
	fseek(f, imh.itemDeps.offset, SEEK_SET);
	for(int i = 0; i < imh.itemDeps.count; i++)
	{
		itemDependency id;
		if(fread(&id, 1, sizeof(itemDependency), f) != sizeof(itemDependency))
		{
			cout << "Error: unable to read itemDependency from file " << cFilename << endl;
			fclose(f);
			return false;
		}
		vItemDependencies.push_back(id);
	}
	
	//Read in the item data
	vector<itemDataHeader> vItemDataHeaders;
	for(list<itemManifestRecord>::iterator i = lManifestRecords.begin(); i != lManifestRecords.end(); i++)
	{
		fseek(f, imh.itemsBinDataBytes.offset + i->binDataOffsetBytes, SEEK_SET);	//Seek to this position to read
		itemDataHeader idh;
		if(fread(&idh, 1, sizeof(itemDataHeader), f) != sizeof(itemDataHeader))
		{
			cout << "Error: Unable to read itemDataHeader from file " << cFilename << endl;
			fclose(f); 
			return false;
		}
		vItemDataHeaders.push_back(idh);
	}
	
	//TODO Read rest of item data
	
	fclose(f);
	
	//Now, open up the XML file and put all the data into it
	wstring sFilename = cFilename;
	sFilename += TEXT(".xml");
	XMLDocument* doc = new XMLDocument;
	XMLElement* root = doc->NewElement("itemmanifest");	//Create the root element
	int iCurItemData = 0;
	
	//DEBUG: See how often each value occurs and see if there's a resonable default
	//map<wstring, list<wstring> > mOccurrences;
	//
	//ofstream oHash("hash2.txt");
	for(list<itemManifestRecord>::iterator i = lManifestRecords.begin(); i != lManifestRecords.end(); i++)
	{
		XMLElement* elem = doc->NewElement("itemrecord");
		elem->SetAttribute("id", ws2s(mItemNames[i->itemId]).c_str());
		//oHash << "id: " << i->itemId << ", filename: " << mItemNames[i->itemId].c_str() 
		//	  << ", hashed filename: " << hash(mItemNames[i->itemId]) << endl;
		//if(i->itemId == hash(mItemNames[i->itemId]))
		//	oHash << "Hash worked!" << endl;
		//else
		//	oHash << "Hash failed." << endl;
		XMLElement* elem2 = doc->NewElement("animresid");
		elem2->SetAttribute("filename", ws2s(getName(i->animResId)).c_str());
		elem->InsertEndChild(elem2);
		elem2 = doc->NewElement("recentlymodifiedrank");	//TODO Ignore this
		elem2->SetAttribute("value", i->recentlyModifiedRank);
		elem->InsertEndChild(elem2);
		elem2 = doc->NewElement("coloritemicon");
		elem2->SetAttribute("filename", getName(i->catalogIconColorItemTexResId));
		elem->InsertEndChild(elem2);
		elem2 = doc->NewElement("colorbgicon");
		elem2->SetAttribute("filename", getName(i->catalogIconColorBGTexResId));
		elem->InsertEndChild(elem2);
		elem2 = doc->NewElement("greybgicon");
		elem2->SetAttribute("filename", getName(i->catalogIconGreyBGTexResId));
		elem->InsertEndChild(elem2);
		//TODO: binDataOffsetBytes stuff
		//Now insert dependencies for this item
		elem2 = doc->NewElement("depends");
		for(int j = i->firstNormalDepends; j < i->firstNormalDepends + i->numNormalDepends; j++)
		{
			XMLElement* elem3 = doc->NewElement("normal");
			elem3->SetAttribute("filename", getName(vNormalDependencies[j].normalTexResId));
			elem2->InsertEndChild(elem3);
		}
		for(int j = i->firstSoundDepends; j < i->firstSoundDepends + i->numSoundDepends; j++)
		{
			XMLElement* elem3 = doc->NewElement("sound");
			elem3->SetAttribute("id", ws2s(getSoundName(vSoundDependencies[j].soundResId)).c_str());
			//elem3->SetAttribute("id", getName(vSoundDependencies[j].soundResId));
			elem2->InsertEndChild(elem3);
		}
		for(int j = i->firstEffectDepends; j < i->firstEffectDepends + i->numEffectDepends; j++)
		{
			XMLElement* elem3 = doc->NewElement("effect");
			elem3->SetAttribute("id", getName(vEffectDependencies[j].effectResId));
			elem2->InsertEndChild(elem3);
		}
		for(int j = i->firstItemDepends; j < i->firstItemDepends + i->numItemDepends; j++)
		{
			XMLElement* elem3 = doc->NewElement("item");
			elem3->SetAttribute("id", ws2s(mItemNames[vItemDependencies[j].itemResId]).c_str());
			elem2->InsertEndChild(elem3);
		}
		elem->InsertEndChild(elem2);
		
		//Now deal with item data, only writing each if it differs from the default
		elem2 = doc->NewElement("itemdata");
		if(vItemDataHeaders[iCurItemData].absPosition != DEFAULT_ABSPOSITION)
			elem2->SetAttribute("absPosition", vItemDataHeaders[iCurItemData].absPosition);
		if(vItemDataHeaders[iCurItemData].allowDirectionalLight != DEFAULT_ALLOWDIRECTIONALLIGHT)
			elem2->SetAttribute("allowDirectionalLight", vItemDataHeaders[iCurItemData].allowDirectionalLight);
		if(vItemDataHeaders[iCurItemData].allowExplodeStreaks != DEFAULT_ALLOWEXPLODESTREAKS)
			elem2->SetAttribute("allowExplodeStreaks", vItemDataHeaders[iCurItemData].allowExplodeStreaks);
		if(vItemDataHeaders[iCurItemData].animThreshold != DEFAULT_ANIMTHRESHOLD)
			elem2->SetAttribute("animThreshold", vItemDataHeaders[iCurItemData].animThreshold);
		if(vItemDataHeaders[iCurItemData].burnSlowsAnim != DEFAULT_BURNSLOWSANIM)
			elem2->SetAttribute("burnSlowsAnim", vItemDataHeaders[iCurItemData].burnSlowsAnim);
		if(vItemDataHeaders[iCurItemData].canGetPlague != DEFAULT_CANGETPLAGUE)
			elem2->SetAttribute("canGetPlague", vItemDataHeaders[iCurItemData].canGetPlague);
		if(vItemDataHeaders[iCurItemData].collideEnvironment != DEFAULT_COLLIDEENVIRONMENT)
			elem2->SetAttribute("collideEnvironment", vItemDataHeaders[iCurItemData].collideEnvironment);
		if(vItemDataHeaders[iCurItemData].collideItems != DEFAULT_COLLIDEITEMS)
			elem2->SetAttribute("collideItems", vItemDataHeaders[iCurItemData].collideItems);
		elem2->SetAttribute("costCoins", vItemDataHeaders[iCurItemData].costCoins);
		if(vItemDataHeaders[iCurItemData].costStamps != DEFAULT_COSTSTAMPS)
			elem2->SetAttribute("costStamps", vItemDataHeaders[iCurItemData].costStamps);
		elem2->SetAttribute("descid", vItemDataHeaders[iCurItemData].desc.id);
		if(vItemDataHeaders[iCurItemData].desc.key != DEFAULT_DESCKEY)
			elem2->SetAttribute("desckey", vItemDataHeaders[iCurItemData].desc.key);
		if(vItemDataHeaders[iCurItemData].enableFreezePostAnim != DEFAULT_ENABLEFREEZEPOSTANIM)
			elem2->SetAttribute("enableFreezePostAnim", vItemDataHeaders[iCurItemData].enableFreezePostAnim);
		if(vItemDataHeaders[iCurItemData].enableHFlip != DEFAULT_ENABLEHFLIP)
			elem2->SetAttribute("enableHFlip", vItemDataHeaders[iCurItemData].enableHFlip);
		if(vItemDataHeaders[iCurItemData].floorWallShadow != DEFAULT_FLOORWALLSHADOW)
			elem2->SetAttribute("floorWallShadow", vItemDataHeaders[iCurItemData].floorWallShadow);
		if(vItemDataHeaders[iCurItemData].freezeOnCollide != DEFAULT_FREEZEONCOLLIDE)
			elem2->SetAttribute("freezeOnCollide", vItemDataHeaders[iCurItemData].freezeOnCollide);
		elem2->SetAttribute("iconAnimBoundsMaxx", vItemDataHeaders[iCurItemData].iconAnimBoundsMax.x);
		elem2->SetAttribute("iconAnimBoundsMaxy", vItemDataHeaders[iCurItemData].iconAnimBoundsMax.y);
		elem2->SetAttribute("iconAnimBoundsMinx", vItemDataHeaders[iCurItemData].iconAnimBoundsMin.x);
		elem2->SetAttribute("iconAnimBoundsMiny", vItemDataHeaders[iCurItemData].iconAnimBoundsMin.y);
		if(vItemDataHeaders[iCurItemData].illuminate != DEFAULT_ILLUMINATE)
			elem2->SetAttribute("illuminate", vItemDataHeaders[iCurItemData].illuminate);
		if(vItemDataHeaders[iCurItemData].initialBurnExportId != DEFAULT_INITIALBURNEXPORTID)
			elem2->SetAttribute("initialBurnExportId", vItemDataHeaders[iCurItemData].initialBurnExportId);
		if(vItemDataHeaders[iCurItemData].initialBurnPerGroup != DEFAULT_INITIALBURNPERGROUP)
			elem2->SetAttribute("initialBurnPerGroup", vItemDataHeaders[iCurItemData].initialBurnPerGroup);
		if(vItemDataHeaders[iCurItemData].instAshDoesSplat != DEFAULT_INSTASHDOESSPLAT)
			elem2->SetAttribute("instAshDoesSplat", vItemDataHeaders[iCurItemData].instAshDoesSplat);
		if(vItemDataHeaders[iCurItemData].instAshSoundResId != DEFAULT_INSTASHSOUNDRESID)
			elem2->SetAttribute("instAshSoundResId", vItemDataHeaders[iCurItemData].instAshSoundResId);
		if(vItemDataHeaders[iCurItemData].instantEffects != DEFAULT_INSTANTEFFECTS)
			elem2->SetAttribute("instantEffects", vItemDataHeaders[iCurItemData].instantEffects);
		if(vItemDataHeaders[iCurItemData].itemIdStrId != DEFAULT_ITEMIDSTRID)
			elem2->SetAttribute("itemIdStrId", vItemDataHeaders[iCurItemData].itemIdStrId);
		if(vItemDataHeaders[iCurItemData].modXAmpMax != DEFAULT_MODXAMPMAX)
			elem2->SetAttribute("modXAmpMax", vItemDataHeaders[iCurItemData].modXAmpMax);
		if(vItemDataHeaders[iCurItemData].modXAmpMin != DEFAULT_MODXAMPMIN)
			elem2->SetAttribute("modXAmpMin", vItemDataHeaders[iCurItemData].modXAmpMin);
		if(vItemDataHeaders[iCurItemData].modXFreqMax != DEFAULT_MODXFREQMAX)
			elem2->SetAttribute("modXFreqMax", vItemDataHeaders[iCurItemData].modXFreqMax);
		if(vItemDataHeaders[iCurItemData].modXFreqMin != DEFAULT_MODXFREQMIN)
			elem2->SetAttribute("modXFreqMin", vItemDataHeaders[iCurItemData].modXFreqMin);
		if(vItemDataHeaders[iCurItemData].modXPhaseMax != DEFAULT_MODXPHASEMAX)
			elem2->SetAttribute("modXPhaseMax", vItemDataHeaders[iCurItemData].modXPhaseMax);
		if(vItemDataHeaders[iCurItemData].modXPhaseMin != DEFAULT_MODXPHASEMIN)
			elem2->SetAttribute("modXPhaseMin", vItemDataHeaders[iCurItemData].modXPhaseMin);
		if(vItemDataHeaders[iCurItemData].modXSpeedMax != DEFAULT_MODXSPEEDMAX)
			elem2->SetAttribute("modXSpeedMax", vItemDataHeaders[iCurItemData].modXSpeedMax);
		if(vItemDataHeaders[iCurItemData].modXSpeedMin != DEFAULT_MODXSPEEDMIN)
			elem2->SetAttribute("modXSpeedMin", vItemDataHeaders[iCurItemData].modXSpeedMin);
		if(vItemDataHeaders[iCurItemData].modYAmpMax != DEFAULT_MODYAMPMAX)
			elem2->SetAttribute("modYAmpMax", vItemDataHeaders[iCurItemData].modYAmpMax);
		if(vItemDataHeaders[iCurItemData].modYAmpMin != DEFAULT_MODYAMPMIN)
			elem2->SetAttribute("modYAmpMin", vItemDataHeaders[iCurItemData].modYAmpMin);
		if(vItemDataHeaders[iCurItemData].modYFreqMax != DEFAULT_MODYFREQMAX)
			elem2->SetAttribute("modYFreqMax", vItemDataHeaders[iCurItemData].modYFreqMax);
		if(vItemDataHeaders[iCurItemData].modYFreqMin != DEFAULT_MODYFREQMIN)
			elem2->SetAttribute("modYFreqMin", vItemDataHeaders[iCurItemData].modYFreqMin);
		if(vItemDataHeaders[iCurItemData].modYPhaseMax != DEFAULT_MODYPHASEMAX)
			elem2->SetAttribute("modYPhaseMax", vItemDataHeaders[iCurItemData].modYPhaseMax);
		if(vItemDataHeaders[iCurItemData].modYPhaseMin != DEFAULT_MODYPHASEMIN)
			elem2->SetAttribute("modYPhaseMin", vItemDataHeaders[iCurItemData].modYPhaseMin);
		if(vItemDataHeaders[iCurItemData].modYSpeedMax != DEFAULT_MODYSPEEDMAX)
			elem2->SetAttribute("modYSpeedMax", vItemDataHeaders[iCurItemData].modYSpeedMax);
		if(vItemDataHeaders[iCurItemData].modYSpeedMin != DEFAULT_MODYSPEEDMIN)
			elem2->SetAttribute("modYSpeedMin", vItemDataHeaders[iCurItemData].modYSpeedMin);
		if(vItemDataHeaders[iCurItemData].moneyItem != DEFAULT_MONEYITEM)
			elem2->SetAttribute("moneyItem", vItemDataHeaders[iCurItemData].moneyItem);
		if(vItemDataHeaders[iCurItemData].motorThreshold != DEFAULT_MOTORTHRESHOLD)
			elem2->SetAttribute("motorThreshold", vItemDataHeaders[iCurItemData].motorThreshold);
		if(vItemDataHeaders[iCurItemData].mouseGrabSoundResId != DEFAULT_MOUSEGRABSOUNDRESID)
			elem2->SetAttribute("mouseGrabSoundResId", vItemDataHeaders[iCurItemData].mouseGrabSoundResId);
		if(vItemDataHeaders[iCurItemData].mouseGrabbable != DEFAULT_MOUSEGRABBABLE)
			elem2->SetAttribute("mouseGrabbable", vItemDataHeaders[iCurItemData].mouseGrabbable);
		elem2->SetAttribute("nameid", vItemDataHeaders[iCurItemData].name.id);
		if(vItemDataHeaders[iCurItemData].name.key != DEFAULT_NAMEKEY)
			elem2->SetAttribute("namekey", vItemDataHeaders[iCurItemData].name.key);
		if(vItemDataHeaders[iCurItemData].orbitalGravity != DEFAULT_ORBITALGRAVITY)
			elem2->SetAttribute("orbitalGravity", vItemDataHeaders[iCurItemData].orbitalGravity);
		if(vItemDataHeaders[iCurItemData].plagueOnCollide != DEFAULT_PLAGUEONCOLLIDE)
			elem2->SetAttribute("plagueOnCollide", vItemDataHeaders[iCurItemData].plagueOnCollide);
		if(vItemDataHeaders[iCurItemData].popsCoins != DEFAULT_POPSCOINS)
			elem2->SetAttribute("popsCoins", vItemDataHeaders[iCurItemData].popsCoins);
		elem2->SetAttribute("purchaseCooldown", vItemDataHeaders[iCurItemData].purchaseCooldown);
		if(vItemDataHeaders[iCurItemData].quantity != DEFAULT_QUANTITY)
			elem2->SetAttribute("quantity", vItemDataHeaders[iCurItemData].quantity);
		if(vItemDataHeaders[iCurItemData].scaleVariance != DEFAULT_SCALEVARIANCE)
			elem2->SetAttribute("scaleVariance", vItemDataHeaders[iCurItemData].scaleVariance);
		elem2->SetAttribute("shipTimeSec", vItemDataHeaders[iCurItemData].shipTimeSec);
		if(vItemDataHeaders[iCurItemData].spawnLimitBurnExportId != DEFAULT_SPAWNLIMITBURNEXPORTID)
			elem2->SetAttribute("spawnLimitBurnExportId", vItemDataHeaders[iCurItemData].spawnLimitBurnExportId);
		if(vItemDataHeaders[iCurItemData].splitJumpLastFrame != DEFAULT_SPLITJUMPLASTFRAME)
			elem2->SetAttribute("splitJumpLastFrame", vItemDataHeaders[iCurItemData].splitJumpLastFrame);
		if(vItemDataHeaders[iCurItemData].uniqueIgniteSoundResId != DEFAULT_UNIQUEIGNITESOUNDRESID)
			elem2->SetAttribute("uniqueIgniteSoundResId", vItemDataHeaders[iCurItemData].uniqueIgniteSoundResId);
		if(vItemDataHeaders[iCurItemData].unlisted != DEFAULT_UNLISTED)
			elem2->SetAttribute("unlisted", vItemDataHeaders[iCurItemData].unlisted);
		elem2->SetAttribute("valueCoins", vItemDataHeaders[iCurItemData].valueCoins);
		if(vItemDataHeaders[iCurItemData].valueStamps != DEFAULT_VALUESTAMPS)
			elem2->SetAttribute("valueStamps", vItemDataHeaders[iCurItemData].valueStamps);

		
		//DEBUG Now loop back through here and pull data from it all
		//for(const XMLAttribute* att = elem2->FirstAttribute(); att != NULL; att = att->Next())
		//	mOccurrences[att->Name()].push_back(att->Value());
		
		iCurItemData++;
		//TODO: Write rest of XML stuff for rest of item data
		elem->InsertEndChild(elem2);
		root->InsertEndChild(elem);
	}
	//oHash.close();
	doc->InsertFirstChild(root);
	doc->SaveFile(ws2s(sFilename).c_str());
	
	delete doc;
	
	//DEBUG: Save out all the data that we got in our map
	/*ofstream ofile("map2.txt");
	ofstream ofMap("map.txt");
	ofstream ofCode("mapcode.txt");
	for(map<wstring, list<wstring> >::iterator i = mOccurrences.begin(); i != mOccurrences.end(); i++)
	{
		//ofile << i->first << " ";
		map<wstring, unsigned int> mPer;
		
		//i->second.sort();
		for(list<wstring>::iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			if(mPer.count(*j))
				mPer[*j]++;
			else
				mPer[*j] = 1;
		}
		wstring sHighest = "";
		unsigned int iHighest = 0;
		for(map<wstring, unsigned int>::iterator j = mPer.begin(); j != mPer.end(); j++)
		{
			if(j->second > iHighest)
			{
				iHighest = j->second;
				sHighest = j->first;
			}
		}
		if(iHighest > i->second.size() / 2)	//Has to be over half to be a reasonable default
		{
			ofile << i->first << " had the default value " << sHighest << ", which occurred " << iHighest << " times." << endl;
			ofMap << "#define DEFAULT_" << stoupper(i->first) << "\t" << sHighest << endl;
			ofCode << "\tif(vItemDataHeaders[iCurItemData]." << i->first << " != DEFAULT_" << stoupper(i->first) << ")" << endl
				   << "\t\telem2->SetAttribute(\"" << i->first << "\", vItemDataHeaders[iCurItemData]." << i->first << ");" << endl;
		}
		else
		{
			ofCode << "\telem2->SetAttribute(\"" << i->first << "\", vItemDataHeaders[iCurItemData]." << i->first << ");" << endl;
		}
		//ofile << *j << " ";
		//ofile << endl << endl;
	}
	ofile.close();
	ofMap.close();
	ofCode.close();*/
	//
	
	return true;
}

//TODO Fix problems and stuff for next release
bool XMLToItemManifest(const wchar_t* cFilename)
{
	//Open this XML file for parsing
	wstring sXMLFile = cFilename;
	sXMLFile += TEXT(".xml");
	XMLDocument* doc = new XMLDocument;
	int iErr = doc->LoadFile(ws2s(sXMLFile).c_str());
	if(iErr != XML_NO_ERROR)
	{
		cout << "Error parsing XML file " << ws2s(sXMLFile) << ": Error " << iErr << endl;
		delete doc;
		return false;
	}
	
	//Grab root element
	XMLElement* root = doc->RootElement();
	if(root == NULL)
	{
		cout << "Error: Root element NULL in XML file " << ws2s(sXMLFile) << endl;
		delete doc;
		return false;
	}
	
	//Read in all child elements
	list<itemManifestRecord> lItemManifests;
	list<normalDependency> lNormalDeps;
	list<soundDependency> lSoundDeps;
	list<effectDependency> lEffectDeps;
	list<itemDependency> lItemDeps;
	for(XMLElement* elem = root->FirstChildElement("itemrecord"); elem != NULL; elem = elem->NextSiblingElement("itemrecord"))
	{
		itemManifestRecord imr;
		imr.itemId = imr.animResId = imr.recentlyModifiedRank = imr.firstNormalDepends = imr.numNormalDepends = imr.firstSoundDepends = imr.numSoundDepends = imr.firstEffectDepends = imr.numEffectDepends = imr.firstItemDepends = imr.numItemDepends = imr.catalogIconColorItemTexResId = imr.catalogIconColorBGTexResId = imr.catalogIconGreyBGTexResId = imr.binDataOffsetBytes = 0;	//To supress compiler warnings
		const char* id = elem->Attribute("id");
		if(id == NULL)
		{
			cout << "Error: Unable to get id of XML element in file " << ws2s(sXMLFile) << endl;
			delete doc;
			return false;
		}
		imr.itemId = hash(s2ws(id)); //TODO
		imr.recentlyModifiedRank = 1;	//Because why not
		
		//get all the XML children of this
		for(XMLElement* elem2 = elem->FirstChildElement(); elem2 != NULL; elem2 = elem2->NextSiblingElement())
		{
			wstring sName = s2ws(elem2->Name());
			if(sName == TEXT("animresid"))
			{
				const char* cTemp = elem2->Attribute("filename");
				if(cTemp == NULL)
				{
					cout << "Error: Missing filename for animresid in file " << ws2s(sXMLFile) << endl;
					delete doc;
					return false;
				}
				imr.animResId = getResID(s2ws(cTemp));
			}
			else if(sName == TEXT("recentlymodifiedrank"))
			{
				//Completely ignore //TODO Remove test case
			}
			else if(sName == TEXT("coloritemicon"))
			{
				const char* cTemp = elem2->Attribute("filename");
				if(cTemp == NULL)
				{
					cout << "Error: Missing filename for coloritemicon in file " << ws2s(sXMLFile) << endl;
					delete doc;
					return false;
				}
				imr.catalogIconColorItemTexResId = getResID(s2ws(cTemp));
			}
			else if(sName == TEXT("colorbgicon"))
			{
				const char* cTemp = elem2->Attribute("filename");
				if(cTemp == NULL)
				{
					cout << "Error: Missing filename for colorbgicon in file " << ws2s(sXMLFile) << endl;
					delete doc;
					return false;
				}
				imr.catalogIconColorBGTexResId = getResID(s2ws(cTemp));
			}
			else if(sName == TEXT("greybgicon"))
			{
				const char* cTemp = elem2->Attribute("filename");
				if(cTemp == NULL)
				{
					cout << "Error: Missing filename for greybgicon in file " << ws2s(sXMLFile) << endl;
					delete doc;
					return false;
				}
				imr.catalogIconGreyBGTexResId = getResID(s2ws(cTemp));
			}
			//TODO: binDataOffsetBytes
			else if(sName == TEXT("depends"))
			{
				//Store offsets for these dependencies
				imr.firstNormalDepends = lNormalDeps.size();
				imr.firstSoundDepends = lSoundDeps.size();
				imr.firstEffectDepends = lEffectDeps.size();
				imr.firstItemDepends = lItemDeps.size();
				imr.numNormalDepends = 0;
				imr.numSoundDepends = 0;
				imr.numEffectDepends = 0;
				imr.numItemDepends = 0;
				//Get all child dependencies
				for(XMLElement* elem3 = elem2->FirstChildElement(); elem3 != NULL; elem3 = elem3->NextSiblingElement())
				{
					wstring sDependName = s2ws(elem3->Name());
					if(sDependName == TEXT("normal"))
					{
						const char* cTemp = elem3->Attribute("filename");
						if(cTemp == NULL)
						{
							cout << "Error: Missing filename for normal dependency in file " << ws2s(sXMLFile) << endl;
							delete doc;
							return false;
						}
						normalDependency nd;
						nd.normalTexResId = getResID(s2ws(cTemp));
						lNormalDeps.push_back(nd);
						imr.numNormalDepends++;
					}
					else if(sDependName == TEXT("sound"))
					{
						const char* cTemp = elem3->Attribute("id");
						if(cTemp == NULL)
						{
							cout << "Error: Missing id for sound dependency in file " << ws2s(sXMLFile) << endl;
							delete doc;
							return false;
						}
						soundDependency sd;
						sd.soundResId = getSoundId(s2ws(cTemp));
						lSoundDeps.push_back(sd);
						imr.numSoundDepends++;
					}
					else if(sDependName == TEXT("effect"))
					{
						const char* cTemp = elem3->Attribute("id");
						if(cTemp == NULL)
						{
							cout << "Error: Missing id for effect dependency in file " << ws2s(sXMLFile) << endl;
							delete doc;
							return false;
						}
						effectDependency ed;
						ed.effectResId = getResID(s2ws(cTemp));
						lEffectDeps.push_back(ed);
						imr.numEffectDepends++;
					}
					else if(sDependName == TEXT("item"))
					{
						const char* cTemp = elem3->Attribute("id");
						if(cTemp == NULL)
						{
							cout << "Error: Missing id for item dependency in file " << ws2s(sXMLFile) << endl;
							delete doc;
							return false;
						}
						itemDependency id;
						id.itemResId = hash(s2ws(cTemp));
						lItemDeps.push_back(id);
						imr.numItemDepends++;
					}
					else if(sDependName == TEXT(""))
						cout << "Warning: Empty element name for depends in XML file " << ws2s(sXMLFile) << endl;
					else
						cout << "Warning: Unknown name for dependency: " << ws2s(sDependName) << " in XML file " << ws2s(sXMLFile) << ". Ignoring..." << endl;
				}
			}
			else if(sName == TEXT(""))
				cout << "Warning: XML element missing name in file " << ws2s(sXMLFile) << ". Ignoring... " << endl;
			else
				cout << "Warning: Unknown XML element name: " << ws2s(sName) << " in XML file " << ws2s(sXMLFile) << ". Ignoring..." << endl;
		}
		lItemManifests.push_back(imr);
	}
	//Now we have all the item stuff properly in our lists
	delete doc;	//We're done with this
	
	//TODO Finish parsing XML binDataOffsetBytes stuff and write to file
	itemManifestHeader imh;
	size_t curOffset = sizeof(itemManifestHeader);
	imh.itemsManifest.count = lItemManifests.size();
	imh.itemsManifest.offset = curOffset;
	curOffset += sizeof(itemManifestRecord) * lItemManifests.size();
	imh.normalDeps.count = lNormalDeps.size();
	imh.normalDeps.offset = curOffset;
	curOffset += sizeof(normalDependency) * lNormalDeps.size();
	imh.soundDeps.count = lSoundDeps.size();
	imh.soundDeps.offset = curOffset;
	curOffset += sizeof(soundDependency) * lSoundDeps.size();
	imh.effectDeps.count = lEffectDeps.size();
	imh.effectDeps.offset = curOffset;
	curOffset += sizeof(effectDependency) * lEffectDeps.size();
	imh.itemDeps.count = lItemDeps.size();
	imh.itemDeps.offset = curOffset;
	curOffset += sizeof(itemDependency) * lItemDeps.size();
	imh.itemsBinDataBytes.count = 0;	//TODO
	imh.itemsBinDataBytes.offset = curOffset;
	
	//Open output file
	wstring sFilename = cFilename;
	sFilename += TEXT(".derp");	//TODO: Replace original file
	FILE* f = _wfopen(sFilename.c_str(), TEXT("wb"));
	if(f == NULL)
	{
		cout << "Error: Unable to open file " << cFilename << " for writing." << endl;
		return false;
	}
	
	//Write our itemManifestHeader
	fwrite(&imh, 1, sizeof(itemManifestHeader), f);
	
	//Write our itemManifestRecords
	for(list<itemManifestRecord>::iterator i = lItemManifests.begin(); i != lItemManifests.end(); i++)
		fwrite(&(*i), 1, sizeof(itemManifestRecord), f);
	
	//Write our normalDependencies
	for(list<normalDependency>::iterator i = lNormalDeps.begin(); i != lNormalDeps.end(); i++)
		fwrite(&(*i), 1, sizeof(normalDependency), f);
	
	//Write our soundDependencies
	for(list<soundDependency>::iterator i = lSoundDeps.begin(); i != lSoundDeps.end(); i++)
		fwrite(&(*i), 1, sizeof(soundDependency), f);
	
	//Write our effectDependencies
	for(list<effectDependency>::iterator i = lEffectDeps.begin(); i != lEffectDeps.end(); i++)
		fwrite(&(*i), 1, sizeof(effectDependency), f);
	
	//Write our itemDependencies
	for(list<itemDependency>::iterator i = lItemDeps.begin(); i != lItemDeps.end(); i++)
		fwrite(&(*i), 1, sizeof(itemDependency), f);
	
	//TODO Write our binDataOffsetBytes stuff
	
	fclose(f);
	return true;
}


















