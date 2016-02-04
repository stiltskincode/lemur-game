#include "AnimationSet.h"


#define SAFE_DELETE_ARRAY(p) { if ( p ) delete[] ( p ); ( p )=NULL; }

AnimationSet& AnimationSet::operator=(const AnimationSet& pattern)
{
	skeleton = pattern.skeleton;
	loaded = pattern.loaded;
	bounded = pattern.bounded;
	looped = pattern.looped;
	stopped = pattern.stopped;
	paused = pattern.paused;
	finished = pattern.finished;
	num = pattern.num;
	duration = pattern.duration;
	lastK1 = pattern.lastK1;
	elapsedTime = pattern.elapsedTime;
	weight = pattern.weight;

	anims=new Animation[num];
	for(unsigned int i=0; i<num; i++)	//Dla kazdej animacji.
	{
		anims[i].name = pattern.anims[i].name;
		anims[i].bname = pattern.anims[i].bname;
		anims[i].num = pattern.anims[i].num;

		anims[i].keys = new Key[anims[i].num];

		for(unsigned int j=0; j<anims[i].num; j++)	//Dla kazdego klucza.
		{
			anims[i].keys[j].time = pattern.anims[i].keys[j].time;
			anims[i].keys[j].r = pattern.anims[i].keys[j].r;
			anims[i].keys[j].s = pattern.anims[i].keys[j].s;
			anims[i].keys[j].t = pattern.anims[i].keys[j].t;
		}
	}

	if(bounded)
	{
		srt=new SRT*[skeleton->num];

		for(unsigned int i=0; i<skeleton->num; i++)	//Dla kazdej animacji.
		{
			if(pattern.srt[i])
			{
				srt[i] = new SRT;
			}
			else srt[i]=NULL;
		}

	}

	cout<<"k";

	return (*this);
}

AnimationSet::AnimationSet()
{
	num=0;
	anims=NULL;
	elapsedTime=0;
	skeleton=NULL;
	weight=0.0f;
	duration=0;
	stopped=paused=finished=false;
	loaded=bounded=false;
	looped=true;
	srt=NULL;
	lastK1=0;
}

AnimationSet::AnimationSet(const AnimationSet& pattern)
{
	skeleton = pattern.skeleton;
	loaded = pattern.loaded;
	bounded = pattern.bounded;
	looped = pattern.looped;
	stopped = pattern.stopped;
	paused = pattern.paused;
	finished = pattern.finished;
	num = pattern.num;
	duration = pattern.duration;
	lastK1 = pattern.lastK1;
	elapsedTime = pattern.elapsedTime;
	weight = pattern.weight;

	anims=new Animation[num];
	for(unsigned int i=0; i<num; i++)	//Dla kazdej animacji.
	{
		anims[i].name = pattern.anims[i].name;
		anims[i].bname = pattern.anims[i].bname;
		anims[i].num = pattern.anims[i].num;

		anims[i].keys = new Key[anims[i].num];

		for(unsigned int j=0; j<anims[i].num; j++)	//Dla kazdego klucza.
		{
			anims[i].keys[j].time = pattern.anims[i].keys[j].time;
			anims[i].keys[j].r = pattern.anims[i].keys[j].r;
			anims[i].keys[j].s = pattern.anims[i].keys[j].s;
			anims[i].keys[j].t = pattern.anims[i].keys[j].t;
		}
	}

	if(bounded)
	{
		srt=new SRT*[skeleton->num];

		for(unsigned int i=0; i<skeleton->num; i++)	//Dla kazdej animacji.
		{
			if(pattern.srt[i])
			{
				srt[i] = new SRT;
			}
			else srt[i]=NULL;
		}
	}
}

AnimationSet::~AnimationSet()
{
	for(UINT i=0; i<num; i++)	//Dla kazdej animacji.
	{
		SAFE_DELETE_ARRAY(anims[i].keys);
		anims[i].bone=NULL;
	}
	SAFE_DELETE_ARRAY(anims);	//Usugniecie zbioru animacji.

	if(skeleton && srt)
	{
		for(unsigned int i=0; i<skeleton->num; i++)
		{
			if(srt[i]) delete srt[i];
			srt[i]=NULL;
		}
		delete[] srt; srt=NULL;
	}
	skeleton=NULL;
}

/**********************************************************************
 Wczytuje animacje z pliku.
**********************************************************************/
bool AnimationSet::load(std::string fileName)
{
	ifstream file;
	file.open(fileName.c_str(),ios_base::binary);
	if(!file.is_open()) return false;

	file>>name; file.get();	//Odczytanie nazwy zbioru.
	file.read((char*)&num, sizeof(int));	//Odczytanie ilosci animacji.

	anims=new Animation[num];	//Alokacja pamieci dla animacji.

	for(unsigned int i=0; i<num; i++)	//Dla kazdej animacji.
	{
		file>>anims[i].name; file.get();	//Odczytanie nazwy animacji.
		file>>anims[i].bname; file.get();	//Odczytanie nazwy kosci.

		file.read((char*)&anims[i].num, sizeof(int));	//Ilosc kluczy.

		anims[i].keys = new Key[anims[i].num];

		for(unsigned int j=0; j<anims[i].num; j++)	//Dla kazdego klucza.
		{
			file.read((char*)&anims[i].keys[j].time, sizeof(int));
			file.read((char*)anims[i].keys[j].r.q, sizeof(float)*4);
			file.read((char*)anims[i].keys[j].s.v, sizeof(float)*3);
			file.read((char*)anims[i].keys[j].t.v, sizeof(float)*3);

			//Szukamy klucza o najwiekszym czasie wystapienia.
			if(anims[i].keys[j].time>duration)
				duration=anims[i].keys[j].time;
		}
	}

	loaded=true;
	return true;
}

/**********************************************************************
 Laczy animacje z danym szkieletem. Kazde wywolanie funkcji update
 animacji spowoduje modyfikacje kosci polaczonego szkieletu.
**********************************************************************/
void AnimationSet::bind(Skeleton *s)
{
	if(!s || !loaded) return;

	skeleton=s;

	//Alokacja pamieci i inicjalizacja kwaternionow wynikowych.
	srt=new SRT*[s->num];
	for(unsigned int i=0; i<s->num; i++) srt[i]=NULL;

	for(unsigned int i=0; i<num; i++)	//Dla kazdej animacji.
	{
		anims[i].bone = s->find(anims[i].bname);
		if(anims[i].bone)	//Jezeli kosc zostala znaleziona.
		{
			srt[anims[i].bone->id] = new SRT;
		}
	}

	bounded=true;
}

/**********************************************************************
 Akutalizuje animacje modyfikujac szkielet, do ktorego jest ona
 przywiazana.
**********************************************************************/
void AnimationSet::update(float delta)
{
	if(!bounded) return;

	unsigned int k0=0, k1=0;
	Quat q0,q1,qi,qr;
	Vec3 res, diff;
	Mat3 mr, mc;
	float f=0;
	delta*=1000.0f;

	if(paused) return;
	else if(stopped) {elapsedTime=0;}
	else elapsedTime+=delta;

	//Jezeli animacja zakonczyla sie.
	if(elapsedTime>duration)
	{
		lastK1=1;
		if(looped) {elapsedTime=0; finished=false;}
		else finished=true;
		return;
	}

	//Dla kazdej animacji.
	for(unsigned int a=0; a<num; a++)
	{
		//Pomijamy przebieg petli, jezeli animacje nie jest polaczona
		//z koscia.
		if(!anims[a].bone) continue;

		k1=0;
		for(unsigned int k=lastK1; k<anims[a].num; k++)
		{
			if(anims[a].keys[k].time>elapsedTime)
			{
				lastK1=k1=k;
				break;
			}
		}

		//Jezeli znalezlismy pozniejszy klucz, mozna wykonac interpolacje.
		if(k1!=0)
		{
			k0=k1-1;
			f=(float)(anims[a].keys[k1].time-elapsedTime);
			f/=float(anims[a].keys[k1].time-anims[a].keys[k0].time);

			//Rotacja.
			q1=anims[a].keys[k1].r;
			q0=anims[a].keys[k0].r;
			srt[anims[a].bone->id]->r=slerp(q1,q0,f);
			srt[anims[a].bone->id]->r.normalize();

			//Translacja.
			Vec3 v=(anims[a].keys[k0].t-anims[a].keys[k1].t);
			srt[anims[a].bone->id]->t=v*f;
			srt[anims[a].bone->id]->t+=anims[a].keys[k1].t;

			//Skalowanie.
			//srt[anims[a].bone->id]->s=(anims[a].keys[k1].s-anims[a].keys[k0].s)*f+anims[a].keys[k0].s;
		}
	}
}

void AnimationSet::updateSkeleton()
{
	if(!bounded) return;

	for(unsigned int i=0; i<skeleton->num; i++)
	{
		if(!srt[i]) continue;

		(*skeleton->wtbuf[i])=srt[i]->r.toMatrix();
		skeleton->wtbuf[i]->m[3]=srt[i]->t[0];
		skeleton->wtbuf[i]->m[7]=srt[i]->t[1];
		skeleton->wtbuf[i]->m[11]=srt[i]->t[2];
		skeleton->wtbuf[i]->m[15]=1;

		//if(i==b->id) *skeleton->wtbuf[i] *= m;
	}
}

void AnimationSet::reset()
{
	Quat q;
	Vec3 v;

	for(unsigned int i=0; i<skeleton->num; i++)
	{
		if(!srt[i]) continue;

		srt[i]->r = q;
		srt[i]->t = v;
	}

	elapsedTime = 0;
	stopped=paused=finished=false;
}
