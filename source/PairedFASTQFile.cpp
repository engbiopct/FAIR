#include "SingleFASTQFile.cpp"

class PairedFASTQFile
{
	string file;
	SingleFASTQFile forward, reverse, identAdapter, adaptersVec;
	PairedFASTQ pairedSequence;
	pair<string, string> adapters;

	string textOutputIdentify;

public:
	bool openFASTQInputFile(string forward, string reverse, int quality);
	bool openFASTQOutputFile(string file, string file2);
	bool hasNext();
	PairedFASTQ getNext();
	pair<string, string> identifyAdapters();
	void trim(int minQuality, int minSequenceLength);
	void removeAdapters(bool onlyRemove, string adapter1, string adapter2, int mismatchMax);
	void write();
	void closeOutput(string typeOperation);

	bool hasNextSearchAdapters();
	void insertAdapters(string adapter1, string adapter2, bool toInsert, double adapterErrorRate, bool adapterInsertionLeft, bool adapterRandomPosition);

};

bool PairedFASTQFile::openFASTQInputFile(string forward, string reverse, int quality)
{
	if (this->forward.openFASTQInput(forward, quality) && this->reverse.openFASTQInput(reverse, quality))
	{
		return true;
	}

	cerr << "Failed To Open Forward & Reverse Files." << endl;
	return false;
}


bool PairedFASTQFile::openFASTQOutputFile(string file, string file2)
{
	this->file = file;

	if (this->forward.openFASTQOutput(file) && this->reverse.openFASTQOutput(file2) == true)
	{
		return true;
	}

	return false;
}

bool PairedFASTQFile::hasNext()
{
	return (forward.hasNext() && reverse.hasNext());
}

bool PairedFASTQFile::hasNextSearchAdapters()
{
	if(forward.hasNextSearchAdapters("forward") && reverse.hasNextSearchAdapters("reverse"))
	{
		return true;
	}

	forward.writeOnlyIdentifyHeader("forward");
	reverse.writeOnlyIdentifyHeader("reverse");

	for (int i = 0; i < forward.getAdaptersVec().size(); ++i)
	{
		cerr << "Forward: " << forward.getAdaptersVec()[i] << "\t" << forward.getAdaptersVecQuant()[i] << endl;
		textOutputIdentify = forward.getAdaptersVec()[i] + "\t" + std::to_string(forward.getAdaptersVecQuant()[i]);
		forward.writeOnlyIdentify(textOutputIdentify);

	}
	for (int i = 0; i < reverse.getAdaptersVec().size(); ++i)
	{
		cerr << "Reverse: " << reverse.getAdaptersVec()[i] << "\t" << reverse.getAdaptersVecQuant()[i] << endl;
		textOutputIdentify = reverse.getAdaptersVec()[i] + "\t" + std::to_string(reverse.getAdaptersVecQuant()[i]);
		reverse.writeOnlyIdentify(textOutputIdentify);
	}

	return false;
}

PairedFASTQ PairedFASTQFile::getNext()
{
	pairedSequence.setPair(forward.getNext(), reverse.getNext());
	return pairedSequence;
}

pair<string, string> PairedFASTQFile::identifyAdapters()
{
	adapters.first = forward.identifyAdapter();
	adapters.second = reverse.identifyAdapter();
	return adapters;
}

void PairedFASTQFile::trim(int minQuality, int minSequenceLength)
{
	forward.trim(minQuality, minSequenceLength);
	reverse.trim(minQuality, minSequenceLength);
}

void PairedFASTQFile::removeAdapters(bool onlyRemove, string adapter1, string adapter2, int mismatchMax)
{
	forward.removeAdapter(onlyRemove, adapter1, mismatchMax);
	reverse.removeAdapter(onlyRemove, adapter2, mismatchMax);
}

void PairedFASTQFile::insertAdapters(string adapter1, string adapter2, bool toInsert, double adapterErrorRate, bool adapterInsertionLeft, bool adapterRandomPosition)
{
	forward.insertAdapter(adapter1, toInsert, adapterErrorRate, adapterInsertionLeft, adapterRandomPosition);
	reverse.insertAdapter(adapter2, toInsert, adapterErrorRate, adapterInsertionLeft, adapterRandomPosition);
}

void PairedFASTQFile::write()
{
	forward.write();
	reverse.write();
}

void PairedFASTQFile::closeOutput(string typeOperation)
{
	forward.closeOutput(typeOperation);
	reverse.closeOutput(typeOperation);
}


