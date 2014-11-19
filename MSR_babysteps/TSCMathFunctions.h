#pragma once
#include <math.h>
#include <iostream>
#include "TF1.h"
#include "TF2.h"
#include "TF3.h"
#include <cmath>
using namespace std;





class M {
public :
static double pi;
static double hbar; // eV*s
static double h; // eV*s
static double nmass; // eV
static double hovernmass;
static double eVperJ;
static double K_a; //1/mol
static double K_b;  //eV/K
static double q;
static double c;
static double deg2rad;  //rad/deg
static double rad2deg;  //deg/rad
static double sqrtpi;

static double GetSigma(double density,double mass_in_units,double sigma){
	return density*M::K_a/mass_in_units*sigma*1e-24;
}

static double GetAttinuation(double neutronspeed, double depth, double Sigma_scat, double Sigma_abs){
	double Sigma=Sigma_scat+Sigma_abs/neutronspeed*2200.;
	return exp(-depth*Sigma);
}
static int zaid2Z(int zaid){
	return (int)zaid/1000;
}
static int zaid2A(int zaid){
	return (int)zaid%1000;
}
static int ZA2zaid(int Z,int A){
	return (int)Z*1000+A;
}
};
double M::sqrtpi=sqrt(3.141592653589793238462);
double M::pi=3.141592653589793238462;
double M::c=299792458.; // m/s
double M::hbar=6.58211928e-16; // eV*s
double M::h=4.135667516e-15; // eV*s
double M::nmass=939565378.; // eV
double M::hovernmass=M::h/M::nmass*M::c*M::c; // s
double M::eVperJ=6.24150934e18;
double M::K_a=6.02214129e23; //1/mol
double M::K_b=8.6173324e-5;  //eV/K
double M::q=1.60217657e-19;  //eV/K
double M::deg2rad=M::pi/180.;  //rad/deg
double M::rad2deg=180./M::pi;  //deg/rad



double IkedaCarpenterFoldedWithGaussian(double *t,double *par){
	// 0 intensity, 1 slowing down ratio, 2 t0 shift, 3 alpha, 4 beta, 5 sigma gauss, 6 flat background
	if(par[0]<=0||par[5]<=0)return 0;
	if(par[3]==par[4])return 0;
	double time=t[0]-par[2];

	double Csum=par[3]-par[4];
	double isum=1./Csum;
	double o1=par[3]*par[3]*(1-par[1]);
	double A0b=par[3]*par[3]*par[4]*par[1]*isum*isum*isum;
	double A0a=o1*time*time
				-A0b*(1+Csum*time+.5*Csum*Csum*time*time);
	double A1=2.*o1*time-A0b*Csum*(1+Csum*time);
	double A2=o1-.5*A0b*Csum*Csum;
	
	double normalizer=1./par[5]/sqrt(2.*M::pi);
	double gamma=.5/par[5]/par[5];
	double sgamma=sqrt(gamma);
	double igamma=1./gamma;
	double isgamma=1./sgamma;
	
	
	double aogmgt=.5*par[3]*isgamma-sgamma*time;
	double _exp=exp(par[3]*par[3]*.25*igamma);
	double _erfc=TMath::Erfc(aogmgt);
	double _exp_b=exp(par[4]*par[4]*.25*igamma);
	double _erfc_b=TMath::Erfc(par[4]*.5*isgamma-sgamma*time);

	double f0a=.5*M::sqrtpi*isgamma*_exp*_erfc;
	double f0b=.5*M::sqrtpi*isgamma*_exp_b*_erfc_b;

	double f1=.5*igamma*exp(time*(par[3]-gamma*time))
		-.25*M::sqrtpi*igamma*isgamma*par[3]*_exp*_erfc;

	double f2=.125*igamma*igamma*isgamma
				*(
					+M::sqrtpi*(par[3]*par[3]+2.*gamma)*exp(time*(par[3]-gamma*time)+aogmgt*aogmgt)*_erfc
					-2.*sgamma*(par[3]+2.*gamma*time)*exp(time*(par[3]-gamma*time))
				);

	return par[6]+par[0]*par[3]/2.*normalizer
				*(
					A0b*exp(-par[4]*time)*f0b
					+exp(-par[3]*time)
						*(
						A0a*f0a
						+A1*f1
						+A2*f2
						)
				); 

}

TH1D* IkedaCarpenterFoldedWithGaussian_convoluted_histogram=0;
double IkedaCarpenterFoldedWithGaussian_convoluted(double *t, double *par){
	// if a histogram named IkedaCarpenterFoldedWithGaussian_convoluted_histogram exist, then this will be the target of the convolusion
	// else the convolution will be a square pulse where: par 7 is the pulse length and par 8 is the number of steps 
	if(!IkedaCarpenterFoldedWithGaussian_convoluted_histogram){
		if(par[8]<=2){cout<<"ERROR in \"IkedaCarpenterFoldedWithGaussian_convoluted\" too few steps in convolution"<<endl;return 0;}
		if(par[7]<=0){cout<<"ERROR in \"IkedaCarpenterFoldedWithGaussian_convoluted\" pulse length < 0"<<endl;return 0;}
		double time[1];
		double result=0;
		double denom=0;
		for(int i=0;i<par[8]+1;i++){
			time[0]=t[0]-i*par[7]/par[8];
			result+=IkedaCarpenterFoldedWithGaussian(time,par);
			denom++;
		}
		return result/denom;
	} else cout<<"ERROR in \"IkedaCarpenterFoldedWithGaussian_convoluted\" pulsethis mode have not been made yet..."<<endl;
	return 0;


}

double IkedaCarpenter2(double *t,double *par){
	if(par[0]==0)return 0;
	if(par[3]==par[4])return 0;
	double time=t[0]-par[2];
	if(time<=0)return par[5]+par[6]/time;
	return
		par[5]+par[6]/time
		+
		par[0]*pow(par[3],3)/2.
			*(
				(1-par[1])*time*time*exp(-par[3]*time)
				+
				2*par[1]*par[4]*pow(par[3]-par[4],-3)
					*(exp(-par[4]*time)-exp(-par[3]*time)
						*(pow((par[3]-par[4])*time,2)/2+(par[3]-par[4])*time+1)
					)
			);
}

double IkedaCarpenter2_hammerconvoluted(double *t,double *par){
	// par[7] is the pulse length, and par[8] is the number of steps
	if(par[0]==0)return 0;
	if(par[3]==par[4])return 0;
	double result=0;
	for(int i=0;i<par[8];i++){
	double time=t[0]-par[2]-i*par[7]/par[8];
	if(time>0){
		result+=par[5]+par[6]/time
		+
		par[0]*pow(par[3],3)/2.
			*(
				(1-par[1])*time*time*exp(-par[3]*time)
				+
				2*par[1]*par[4]*pow(par[3]-par[4],-3)
					*(exp(-par[4]*time)-exp(-par[3]*time)
						*(pow((par[3]-par[4])*time,2)/2+(par[3]-par[4])*time+1)
					)
			);
		} else result+=par[5]+par[6]/time;
	}
	return result/par[8]; 
}

double IkedaCarpenter2_hammerconvoluted2(double *t,double *par){
	// par[7] is the pulse length, and par[8] is the number of steps
	if(par[0]==0)return 0;
	if(par[3]==par[4])return 0;
	double result=0;
	for(int i=0;i<par[8];i++){
	double time=t[0]-par[2]-i*sqrt(169+par[7]*par[7])/par[8];
	if(time>0){
		result+=par[5]+par[6]/time
		+
		par[0]*pow(par[3],3)/2.
			*(
				(1-par[1])*time*time*exp(-par[3]*time)
				+
				2*par[1]*par[4]*pow(par[3]-par[4],-3)
					*(exp(-par[4]*time)-exp(-par[3]*time)
						*(pow((par[3]-par[4])*time,2)/2+(par[3]-par[4])*time+1)
					)
			);
		} else result+=par[5]+par[6]/time;
	}
	return result/par[8]; 
}

double IkedaCarpenter(double *t,double *par){
	if(par[0]==0)return 0;
	if(par[3]==par[4])return 0;
	double time=t[0]-par[2];
	if(time<=0)return 0;
	double Slow=pow(par[3]*time,2)*exp(-par[3]*time);
	double Store=2.*par[3]*par[3]*par[4]*pow(par[3]-par[4],-3.)
		*(
		exp(-par[4]*time)-exp(-par[3]*time)*(1+(par[3]-par[4])*time+pow((par[3]-par[4])*time,2.)
		));
	return par[0]*(par[3]/2.*(1-par[1])*Slow+par[1]*Store)+par[5]+par[6]/time;
}
//double dummalkdsfmlafa=0;
double IkedaCarpenter_serrie_39orders(double *t,double *par){
	if(t[0]<1)return 0;
	//if(dummalkdsfmlafa!=0)return 0;
	//dummalkdsfmlafa=1;
	double inpar[7];
	double result=0;
	for(int i=1;i<39;i++){
//		if(i==2||i==6||i==10||i==14||i==18||i==22||i==26)i++;
		if(par[3+(i-1)*4]==0)continue;
		inpar[5]=0;
		inpar[6]=0;
		inpar[2]=par[2]/i;
		inpar[0]=par[3+(i-1)*4];
		inpar[1]=par[4+(i-1)*4];
		inpar[3]=par[5+(i-1)*4];
		inpar[4]=par[6+(i-1)*4];
		if(IkedaCarpenter(t,inpar))result+=IkedaCarpenter(t,inpar);
		//cout<<inpar[0]<<" "<<inpar[1]<<" "<<inpar[2]<<" "<<inpar[3]<<" "<<inpar[4]<<" "<<inpar[5]<<" "<<inpar[6]<<" "<<inpar[7]<<" "<<endl;
		
		/*
		TCanvas *adkfmmal=new TCanvas();
		TF1 *dalfkmadlf=new TF1("asdvakdmcmasd",IkedaCarpenter,1,25000,7);
		dalfkmadlf->SetParameters(inpar);
		dalfkmadlf->DrawClone();*/
	}
	return	par[0]+par[1]/t[0]+result;
}

TF1 *Fit_IkedaCarpenter_serrie(TH1D *inh,const double *initialpar,int Orders2Fit,int simplecut=100){
	TH1D *h=(TH1D*)inh->Clone();
	TF1 *funk=new TF1("Fit_IkedaCarpenter_serrie_fitFunk",IkedaCarpenter,0,25000,7);
	funk->SetNpx(10000);
	TF1 *funk_out=new TF1("Fit_IkedaCarpenter_serrie_outputFunk",IkedaCarpenter_serrie_39orders,0,25000,300);
	TH1D *PeakRoot=new TH1D("Fit_IkedaCarpenter_serrie_RootOfPeak","",400,0,40);
	TH1D *SDSpeed=new TH1D("Fit_IkedaCarpenter_serrie_SlowingDownSpeed","",400,0,40);
	funk_out->SetNpx(10000);
	double peakstart=0;
	double peakweight=0;
	double outpar[7];
	double longpar[300];
	for(int j=0;j<300;j++)longpar[j]=0;
	for(int i=1;i<Orders2Fit+1;i++){
//	for(int i=1;i<10+1;i++){
		if(i==2||i==6||i==10||i==14||i==18||i==22||i==26)continue;
		funk->SetParameter(0,initialpar[0]/i);
		funk->SetParLimits(0,0,initialpar[0]/i*4);
		if(i>simplecut){
			funk->FixParameter(1,0);
		} else {
			funk->SetParameter(1,initialpar[1]/i/i);
			funk->SetParLimits(1,.0001,.999);
		}
		funk->FixParameter(2,(12671.6+i*4.90439)/i);//initialpar[2]/i);
		//funk->SetParLimits(2,(initialpar[2]-250.)/i,(initialpar[2]+250.)/i);
		funk->FixParameter(3,0.0417248+i*0.0213725);//initialpar[3]);
		//funk->SetParLimits(3,initialpar[3]/2,initialpar[3]*2.);
		funk->FixParameter(4,initialpar[4]);
		//funk->SetParLimits(4,initialpar[4]/2,initialpar[4]*2.);
		funk->FixParameter(5,initialpar[5]*0.98);
		funk->FixParameter(6,initialpar[6]);
		h->GetXaxis()->SetRangeUser(initialpar[2]-500,initialpar[2]+3000);
		if(i!=1)h->GetXaxis()->SetRangeUser(initialpar[2]/i-100/i,initialpar[2]/(i-1));
		h->Fit(funk,"NQWL");
		//cout<<funk->GetParameter(2)<<" -- \t"<<i<<"\t ::  "<<funk->GetParameter(2)*i<<endl;
		TCanvas *akdfa=new TCanvas();
		char dummy[100];
		sprintf(dummy,"peak order %d",i);
		h->SetTitle(dummy);
		h->DrawClone();
		funk->SetLineColor(1);
		funk->DrawClone("same");
		PeakRoot->Fill(i,funk->GetParameter(2)*i);
		SDSpeed->Fill(i,1./funk->GetParameter(3));
		funk->GetParameters(outpar);
		cout<<outpar[0]<<"  "<<outpar[1]<<"  "<<outpar[3]<<"  "<<outpar[4]<<"  "<<endl;
		longpar[3+(i-1)*4]=outpar[0];
		longpar[4+(i-1)*4]=outpar[1];
		longpar[5+(i-1)*4]=outpar[3];
		longpar[6+(i-1)*4]=outpar[4];
		peakstart+=outpar[2]/i;
		peakweight+=1./i/i;
	}
	h->GetXaxis()->SetRangeUser(0,25000);
	for(int i=1;i<10;i++)cout<<longpar[3+(i-1)*4]<<" "<<longpar[4+(i-1)*4]<<" "<<longpar[5+(i-1)*4]<<"  "<<longpar[6+(i-1)*4]<<endl;
	cout<<"Peak fits to: "<<peakstart/peakweight<<endl;
	longpar[0]=initialpar[5];
	longpar[1]=initialpar[6];
	longpar[2]=initialpar[2];//peakstart/peakweight;
	for(int j=0;j<100;j++)cout<<longpar[j]<<endl;
	funk_out->SetParameters(longpar);
	TCanvas *ajkdfa=new TCanvas();
	PeakRoot->Draw();
	TCanvas *ajksdfdfa=new TCanvas();
	SDSpeed->Draw();
	TCanvas *asfdsadcawsdf=new TCanvas();
	funk_out->DrawClone();
	return funk_out;
}
double TSC_TimeModel_Cold(double time,double lambda){
	if(time<0)return 0;
	double normalizer=1./0.0028;
	if(time<0.0028)return normalizer*(1-exp(-(1.20988e4/lambda/lambda+2.31967e3*exp(-4.24902e-2*lambda))*time));
	return normalizer*(exp((1.20988e4/lambda/lambda+2.31967e3*exp(-4.24902e-2*lambda))*0.0028)-1)*exp(-(1.20988e4/lambda/lambda+2.31967e3*exp(-4.24902e-2*lambda))*time);
}

double TSC_YModel(double y,double height, double center){
	double integral=1; //insert integral from c-h/2 to c+h/2
	if(y<center-height/2.||y>center+height/2.)return 0;
	return 1./integral*(exp(-2.2e-001*(y-center-height/2.))+exp(-4.5e-002*height+2.2e-001*(y-center+height/2.)))*(exp(50./sqrt(height)*(y-center-height/2.))-1)*(exp(-50./sqrt(height)*(y-center+height/2.))-1);
}



double V2E(double v){  //ToF in micro secunds
	return v*v*M::nmass/2e6/M::c/M::c;
}

double E2V(double E){  //ToF in micro secunds
	return sqrt(E*2e6/M::nmass)*M::c; // Å (e10*e-6)
}


double E2T(double E){ //MeV to Kelvin
	return 11604.5e6*E;
}
double T2E(double T){ //MeV to Kelvin
	return T/11604.5e6;
}
double E2L(double E){ // MeV to Angstrom
	return 0.000286299/sqrt(E);
}

double L2E(double l){ // Angstrom to MeV
	return 0.000286299*0.000286299/(l*l);
}
double T2L(double T){
	return E2L(T2E(T));
}
double L2T(double T){
	return E2T(L2E(T));
}

double V2T(double v){
	return E2T(V2E(v));
}
double T2V(double T){
	return E2V(T2E(T));
}

double V2L(double v){
	return E2L(V2E(v));
}

double L2V(double l){
	return E2V(L2E(l));
}



double E2K(double e){  //ToF in micro secunds
	return e*1.e4/M::c/M::h; // Å (e10*e-6)
}
double K2E(double k){  //ToF in micro secunds
	return k*M::c*M::h*1e-4; // Å (e10*e-6)
}
double L2K(double l){  //ToF in micro secunds
	return E2L(K2E(l)); // Å (e10*e-6)
}
double K2L(double k){  //ToF in micro secunds
	return E2K(L2E(k)); // Å (e10*e-6)
}
double V2K(double k){  //ToF in micro secunds
	return E2K(V2E(k)); // Å (e10*e-6)
}
double K2V(double k){  //ToF in micro secunds
	return E2V(K2E(k)); // Å (e10*e-6)
}



double func_L2E_meV(double *l,double *par){ // Angstrom to meV
	return 28.6299*2.86299/(l[0]*l[0]);
}

double dVdE(double E){  //ToF in micro secunds
	return 1./sqrt(E*2e6/M::nmass)*M::c/2.; // Å (e10*e-6)
}
double dEdV(double v){  //ToF in micro secunds
	return 1.e-6*v/M::c/M::c*M::nmass; // Å (e10*e-6)
}
double dTdE(double E){ //MeV to Kelvin
	return 11604.5e6;
}
double dEdT(double T){ //MeV to Kelvin
	return 1./11604.5e6;
}
double dLdE(double E){ // MeV to Angstrom
	return 0.000286299/sqrt(E)/E/2.;
}
double dEdL(double l){ // Angstrom to MeV
	return 2.*0.000286299*0.000286299/(l*l*l);
}


double PE_IkedaCarpenter(double *t,double *par){
	if(t[0]<1)return 0;
	double t0=-par[0]/par[1];
	double time=t[0]-t0;                  // first order peak position and order
	if(time<0)return par[4]+par[5]/t[0];             // background
	double SigmaV=9.23e6/t0*sqrt(0.98*0.98+pow(V2L(9.23e6/t0)*0.27,2)); // speed * Sigma
		  

	double slow=SigmaV/2.*pow(SigmaV*time,2)*exp(-SigmaV*time);
	double store_1=pow(SigmaV,2)*par[3]*pow(SigmaV-par[3],-3);
	double store_2=exp(-par[3]*time)-exp(-SigmaV*time)*(1+(SigmaV-par[3]+.5*pow((SigmaV-par[3])*time,2)));
	return (1-par[2])*slow+2*par[2]*store_1*store_2;
}


/*
// overloaders == for root TF1 calls ==
double V2L(double *speed,double *par){  //ToF in micro secunds
	return TSC::hovernmass/speed[0]*1e10; // Å (e10*e-6)
}
double E2T(double *E,double *par){ //MeV to Kelvin
	return 11604.5e6*E[0];
}
double T2E(double *T,double *par){ //MeV to Kelvin
	return T[0]/11604.5e6;
}
double E2L(double *E,double *par){ // MeV to Angstrom
	return 0.000286299/sqrt(E[0]);
}
double dldE(double *E,double *par){
	return 0.0001431495/(sqrt(E[0])*E[0]);
}
double L2E_meV(double *l,double *par){ // Angstrom to meV
	return 0.000286299*0.000286299/(l[0]*l[0])*1e9;
}
double L2E(double *l,double *par){ // Angstrom to MeV
	return 0.000286299*0.000286299/(l[0]*l[0]);
}
double dEdl(double *l,double *par){
	return 0.000572598/(l[0]*l[0]*l[0]);
}
double T2L(double *T,double *par){
	return E2L(T2E(T[0]));
}
double L2T(double *T,double *par){
	return E2T(L2E(T[0]));
}*/


/*
double erf(double x)
{
    // constants
    double a1 =  0.254829592;
    double a2 = -0.284496736;
    double a3 =  1.421413741;
    double a4 = -1.453152027;
    double a5 =  1.061405429;
    double p  =  0.3275911;

    // Save the sign of x
    int sign = 1;
    if (x < 0)
        sign = -1;
    x = fabs(x);

    // A&S formula 7.1.26
    double t = 1.0/(1.0 + p*x);
    double y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);

    return sign*y;
}
*/
double Maxwellian(double *l,double *T){
	if(T[0]!=0&&l[0]>0){
		double aOlsqr=949./(T[0]*l[0]*l[0]);
		return 2.*aOlsqr*aOlsqr/l[0]*exp(-aOlsqr);
	}
	return 0;
}
double ScaledMaxwellian(double *l,double *par){
	if(par[1]!=0&&l[0]>0){
		double aOlsqr=949./(par[1]*l[0]*l[0]);
		return par[0]*2.*aOlsqr*aOlsqr/l[0]*exp(-aOlsqr);
	}
	return 0;
}
double Joining(double *x,double *par){ // speed,joinvalue
	return 1/(1+exp(par[1]*(x[0]-par[0])));
}
double Slowingdown(double *l,double *par){
	if(1+exp(par[0]*l[0]-par[1])!=0&&l[0]>0){
	return 1/(1+exp(par[0]*l[0]-par[1]))/(l[0]*l[0]*l[0]);
	}
	return 0;
}
double ScaledSlowingdown(double *l,double *par){
	if(1+exp(par[1]*l[0]-par[2])!=0&&l[0]>0){
		return par[0]/(1+exp(par[1]*l[0]-par[2]))/l[0]*l[0]*l[0];
	}
	return 0;
}
/*
double BispectralFolding(double *l,double *par){ //erf std, cutoff center,decay
	return 1-(+.5+.5*erf(par[0]*(l[0]-par[1])));
	
}
double BispectralFolding_thermal(double *l,double *par){ //erf std, cutoff center,decay
	return 0.9*BispectralFolding(l,par)+.1*exp(-par[2]*l[0]);
//	)+.1*exp(-par[2]*l[0]);
}

double BispectralFolding_cold(double *l,double *par){ //erf std, cutoff center,decay
	//return 0.96*(1-BispectralFolding(l,par));
	return 0.97*(1-BispectralFolding(l,par))-.1*(1-BispectralFolding(l,par))*exp(-par[2]*l[0]);
}
*/
double Joinedexponential(double *l, double *par){
	return (1-1/
		pow((1+exp(par[1]*(l[0]-par[0]))),2.5)
		)*exp(-par[2]*(l[0]-par[0]));
}

double ParaSwitch(double *e,double *par){ // cut, beta, delta
	if(e[0]>=par[0]){
		double x=par[1]*(e[0]-par[0]);
		return 1-exp(-x)*(1+x+.5*x*x);
	}
	return 0;
}
double ParaCut(double *e,double *par){
	if(e[0]>=par[0]){
		double x=par[1]*(e[0]-par[0]);
		return 1+exp(-x)*(1+x+.5*x*x);
	}
	return 0;
}

double ShortPulse(double time, double *par){	//2 par: par[0]=tau, par[1]=eta (pulseshape-parameter)
	if(par[0]!=0&&(1-par[1])!=0){
		double tauinvers=1/par[0];
		return (exp(-time*tauinvers)-exp(-par[1]*tauinvers*time))*par[1]/(par[1]-1)*tauinvers;
	}
	if(par[0]==0)return 0;
	cout<<"undefined-shortpulse"<<endl;
	return 0;
}
double LongPulseIExp(double t,double *par){ //[0]=tau, [1]=d;
	if(t<=par[1])return par[0]*(1-exp(-t/par[0]));
	return par[0]*(exp(par[1]/par[0]-t/par[0])-exp(-t/par[0]));
}
double LongPulse(double *t,double *par){ //[0]=tau,[1]=eta,[2]=d;
	if(par[0]!=0&&par[1]!=0&&par[1]!=1){
		double passpar1[2]={par[0],par[2]};
		double passpar2[2]={par[0]/par[1],par[2]};
		return (LongPulseIExp(t[0],passpar1)-LongPulseIExp(t[0],passpar2))*par[1]/(par[1]-1)/par[0]/par[2];
	}
	//if(par[1]==0)cout<<"strange"<<endl;
	//cout<<"undefined-Longpulse"<<endl;
	return 0;
}
double DoubleLongPulse(double *t,double *par){ //[0]=tau_pulse,tau_Reflector,[1]=eta,[2]=d;
	double otherpar[3]={par[0],par[2],par[3]};
	return (LongPulse(t,otherpar)+LongPulse(t,&par[1]))*.5; // Semi normalized!!!
}


// for long pulses:
double Decoupled(double *lt, double *par){ //IntencityM ( temperture : tau, eta, d) : IntencityS (cutoffspeed, cutoffPosition : tau, eta, d) 
	return par[0]*Maxwellian(&lt[0],&par[1])*LongPulse(&lt[1],&par[2])+par[5]*Joining(&lt[0],&par[6])*LongPulse(&lt[1],&par[7]);
}
double Coupled(double *lt, double *par){ //IntencityM ( temperture : tau_pulse,tau_reflector, eta, d) : IntencityS (cutoffspeed, cutoffPosition : tau, eta, d) 
	
	return par[0]*Maxwellian(&lt[0],&par[1])*(DoubleLongPulse(&lt[1],&par[2]))+par[6]*Joining(&lt[0],&par[7])*LongPulse(&lt[1],&par[8]);
}
double Coupled_l(double *l, double *par){ //IntencityM ( temperture : tau_pulse,tau_reflector, eta, d) : IntencityS (cutoffspeed, cutoffPosition : tau, eta, d) 
	double lt[2]={l[0],par[5]};
	return Coupled(lt,par);
}
double Decoupled_l(double *l, double *par){ //IntencityM ( temperture : tau, eta, d) : IntencityS (cutoffspeed, cutoffPosition : tau, eta, d) 
	double lt[2]={l[0],par[4]};
	return Decoupled(lt,par);
}


double MaxMaxSlow(double *l, double *par){
	return par[0]*Maxwellian(l,&par[1])+par[2]*Maxwellian(l,&par[3])+par[4]/((1+exp(par[5]*l[0]-par[6]))*l[0])+par[7]/((1+exp(par[8]*l[0]-par[9]))*l[0]);
}



double LeakingParaH2(double *l, double *par){
	return (1-1/
		pow((1+exp(par[1]*(l[0]-par[0]))),par[2])
		)*exp(-par[3]*l[0]);
}


double BispectralParaH2_ver2(double *l, double *par){
	return par[0]*Maxwellian(l,&par[1])*pow(l[0],par[2])  // Maxwellian
		+par[3]*Joinedexponential(l,&par[4])          // ParaHydrogen model
		+par[7]/((1+exp(par[8]*(l[0]-par[9])))*l[0])  // slowing Down
		+par[10]/((1+exp(par[11]*(l[0]-par[12])))*l[0]); // Slowing Down

}


double BispectralParaH2(double *l, double *par){
	double partherm[2]={par[7],par[9]};
	return par[0]*Maxwellian(l,&par[1])*pow(l[0],par[2])+par[3]*Joinedexponential(l,&par[4])
		+par[7]/((1+exp(par[8]*(l[0]-par[9])))*l[0])+par[10]/((1+exp(par[11]*(l[0]-par[12])))*l[0]);

}

double TSC_BispectralParaH2Coupled(double *l, double *par){
	double partherm[2]={par[7],par[9]};
	return par[0]*Maxwellian(l,&par[1])*pow(l[0],par[10])+par[2]*Joinedexponential(l,&par[3])+par[6]/(l[0])*(.55*Joining(l,partherm)+.45*Joining(l,&par[8]));
}
double TSC_BispectralParaH2Coupled_Slowingdown(double *l, double *par){
	double partherm[2]={par[7],par[9]};
	return par[6]/(l[0])*(.55*Joining(l,partherm)+.45*Joining(l,&par[8]));
}
double TSC_BispectralParaH2Coupled_Maxwellian(double *l, double *par){
	double partherm[2]={par[7],par[9]};
	return par[0]*Maxwellian(l,&par[1])*pow(l[0],par[10])+par[6]/(l[0])*.55*Joining(l,partherm);
}
double TSC_BispectralParaH2Coupled_Joinedexponential(double *l, double *par){
	return par[2]*Joinedexponential(l,&par[3])+par[6]/(l[0])*.45*Joining(l,&par[8]);
}

double BispectralParaH2Coupled(double *l, double *par){
	return par[0]*Maxwellian(l,&par[1])+par[2]*Joinedexponential(l,&par[3])+par[6]*Slowingdown(l,&par[7]);
}
double BispectralParaH2Coupled_Slowingdown(double *l, double *par){
	return par[6]*Slowingdown(l,&par[7]);
}
double BispectralParaH2Coupled_Maxwellian(double *l, double *par){
	return par[0]*Maxwellian(l,&par[1])+.5*par[6]*Slowingdown(l,&par[7]);
}
double BispectralParaH2Coupled_Joinedexponential(double *l, double *par){
	return par[2]*Joinedexponential(l,&par[3])+.5*par[6]*Slowingdown(l,&par[7]);
}




double BGSpectrum(double *l,double *par){
// [0]normalizer;[1]time;[2]tau;[3]n;[4]d;[5]bg;
	double passpar[3]={par[2],par[3],par[4]};
	double passvar[1]={par[1]};
		
		return par[0]*LongPulse(passvar,passpar)*Slowingdown(l,&par[5]);
}
double BGSpectrum_Time(double *t,double *par){
	double l[1]={par[0]};
	par[0]=t[0];
	return BGSpectrum(t,par);
}
double SingleSpectrum(double *l,double *par){
	//0normalizer;1time;2Tempreture;3tau;4n;5d
	double passpar[3]={par[3],par[4],par[5]};
	double maxpasspar[1]={par[2]};
	double passvar[1]={par[1]};
	return par[0]*Maxwellian(l,maxpasspar)*LongPulse(passvar,passpar);
}
double SingleSpectrum_Time(double *t,double *par){
	double l[1]={par[0]};
	par[0]=t[0];
	return SingleSpectrum(t,par);
}


double Maxwellian_trunk(double *l,double *T){
	if(T[0]!=0&&l[0]>0){
		double a=949./T[0];
		double lsqr=l[0]*l[0];
		return 2.*a*a/lsqr/lsqr/l[0]*exp(-a/lsqr/lsqr);
	}
	return 0;
}
double SpectrumESS(double *l,double *par){
	if(l[0]!=0)return par[0]*Slowingdown(l,&par[1])+par[2]*Maxwellian(l,&par[3])
		+par[4]*Maxwellian(l,&par[5]);
		//+par[4]*Maxwellian_trunc(l,&par[5]);		//*(1+(par[6]*(l[0]-par[7])*(l[0]-par[7])*exp(-(l[0]-par[7])*(l[0]-par[7])/par[8]/par[8])));
	return 0;
}
double SpectrumESS_trunk(double *l,double *par){

	if(l[0]!=0)return par[0]*Slowingdown(l,&par[1])+par[2]*Maxwellian(l,&par[3])/l[0]//*pow(l[0],par[6])
		//+par[4]*Maxwellian(l,&par[5]);
		+par[4]*Maxwellian_trunk(l,&par[5])*pow(l[0],par[7]);				//*(1+(par[6]*(l[0]-par[7])*(l[0]-par[7])*exp(-(l[0]-par[7])*(l[0]-par[7])/par[8]/par[8])));
	return 0;
}
/************************************************
* FFT code from the book Numerical Recipes in C *
* Visit www.nr.com for the licence.             *
************************************************/

// The following line must be defined before including math.h to correctly define M_PI
//#include <math.h>
//#include <stdio.h>
//#include <stdlib.h>


void four1(double data[], int nn, int isign){
	int n, mmax, m, j, istep, i;
	double wtemp, wr, wpr, wpi, wi, theta;
	double tempr, tempi;

	n = nn << 1;
	j = 1;
	for (i = 1; i < n; i += 2){
		if (j > i){
			tempr = data[j];
			data[j] = data[i];
			data[i] = tempr;
			tempr = data[j+1];
			data[j+1] = data[i+1];
			data[i+1] = tempr;
		}
		m = n >> 1;
		while (m >= 2 && j > m) {
			j -= m;
			m >>= 1;
		}
		j += m;
	}
	mmax = 2;
	while (n > mmax){
		istep = 2*mmax;
		theta = 2*M::pi/(isign*mmax);
		wtemp = sin(0.5*theta);
		wpr = -2.0*wtemp*wtemp;
		wpi = sin(theta);
		wr = 1.0;
		wi = 0.0;
		for (m = 1; m < mmax; m += 2) {
			for (i = m; i <= n; i += istep) {
				j =i + mmax;
				tempr = wr*data[j]   - wi*data[j+1];
				tempi = wr*data[j+1] + wi*data[j];
				data[j]   = data[i]   - tempr;
				data[j+1] = data[i+1] - tempi;
				data[i] += tempr;
				data[i+1] += tempi;
			}
			wr = (wtemp = wr)*wpr - wi*wpi + wr;
			wi = wi*wpr + wtemp*wpi + wi;
		}
	mmax = istep;
	}
}

void FFT2(double *a,double *ib,int length,bool inverse=0){
	double *o_a=new double[length];
	double *o_ib=new double[length];

	double pi2 = (inverse)?2.0 * (M::pi) :-2.0 * (M::pi);
	double aa,ca,sa;
	double invs = 1.0 /(double)length;
	for(int y = 0;y < length;y++) {
		o_a[y] = 0;
		o_ib[y] = 0;
		for(int x = 0;x < length;x++) {
			aa = pi2 * y * x * invs;
			ca = cos(aa);
			sa = sin(aa);
			o_a[y] += a[x] * ca - ib[x] * sa;
			o_ib[y] += a[x] * sa + ib[x] * ca;

			if(!inverse) {
				o_a[y] *= invs;
			}
		}
		cout<<o_a[y]<<" - "<<o_ib[y]<<endl;
	}
	for(int i=0;i<length;i++){
		a[i]=o_a[i];
		ib[i]=o_ib[i];
	}
}
/*
void four1(double* data, unsigned long nn, int isign)
{
    unsigned long n, mmax, m, j, istep, i;
    double wtemp, wr, wpr, wpi, wi, theta;
    double tempr, tempi;
 
    // reverse-binary reindexing
    n = nn<<1;
    j=1;
    for (i=1; i<n; i+=2) {
        if (j>i) {
			tempr=data[j-1]; data[j-1]=data[i-1]; data[i-1]=tempr;
			tempr=data[j]; data[j]=data[i]; data[i]=tempr;
		}
        m = nn;
        while (m>=2 && j>m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    };
 
    // here begins the Danielson-Lanczos section
    mmax=2;
    while (n>mmax) {
        istep = mmax<<1;
        theta = -(2*pi/(isign*mmax));
        wtemp = sin(0.5*theta);
        wpr = -2.0*wtemp*wtemp;
        wpi = sin(theta);
        wr = 1.0;
        wi = 0.0;
        for (m=1; m < mmax; m += 2) {
            for (i=m; i <= n; i += istep) {
                j=i+mmax;
                tempr = wr*data[j-1] - wi*data[j];
                tempi = wr * data[j] + wi*data[j-1];
 
                data[j-1] = data[i-1] - tempr;
                data[j] = data[i] - tempi;
                data[i-1] += tempr;
                data[i] += tempi;
            }
            wtemp=wr;
            wr += wr*wpr - wi*wpi;
            wi += wi*wpr + wtemp*wpi;
        }
        mmax=istep;
    }
}*/
// takes 2 lists of doubles representing a and b in a complex number c=a+ib
void FFT(double *a, double *ib,int length){
	int Transform=1;
	if(length<0){length=-length;Transform=-1;}
	int Nx=length;
	int NFFT=(int)pow(2.0, ceil(log((double)Nx)/log(2.0)));
	double *X=new double[2*NFFT+1];
	for(int i=0; i<Nx; i++){
		X[2*i+1] = a[i];
		X[2*i+2] = ib[i];
	}
	for(int i=Nx; i<NFFT; i++){
		X[2*i+1] = 0.0;
		X[2*i+2] = 0.0;
	}

	/* calculate FFT */
	
	four1(X, NFFT, Transform);
	if(Transform==-1){
		for(int i=0; i<Nx; i++){
			X[2*i+1] /= NFFT;
			X[2*i+2] /= NFFT;
		}
	}
	double *x=new double[NFFT];
	double *y=new double[NFFT];
	for(int i=0;i<length;i++){
		a[i]=X[2*i+1];
		ib[i]=X[2*i+2];
	}
}

void IFFT(double *a, double *ib,int length){
	return FFT(a, ib,-length);
}
