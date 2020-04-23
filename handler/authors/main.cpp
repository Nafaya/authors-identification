#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "authorsRecognition.h"
#include <vector>
#include <unordered_map>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <thread>
#include <future>

using namespace std;
int run()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rnd(seed);
    //AuthorsRecognition::BookStatistics stat;
    //stat.n_grams_frequencies_patches.push_back(vector<double>());
    //stat.n_grams_frequencies_patches[0].push_back(1);
    //stat.n_grams_frequencies_patches[0].push_back(2);
    //stat.n_grams_frequencies_patches[0].push_back(3);
    //AuthorsRecognition::BookStatistics stat2;
    //stat2.n_grams_frequencies_patches.push_back(vector<double>());
    //stat2.n_grams_frequencies_patches[0].push_back(1);
    //stat2.n_grams_frequencies_patches[0].push_back(2);
    //cout << AuthorsRecognition::pstatistic_distance(stat2.n_grams_frequencies_patches[0], stat.n_grams_frequencies_patches[0]) << '\n';
    //parameters
    const string alphabet = "abcdefghijklmnopqrstuvwxyz";
    const string lists_directory = "../../data/gutenberg/catalog";
    //const string lists_directory = "C:/Users/ASUS/anaconda_projects/authors/data/gutenberg/catalog";
    //const string books_directory = "C:/Users/ASUS/anaconda_projects/authors/data/gutenberg/books";
    const string books_directory = "../../data/gutenberg/books";
    //const string books_directory = "C:/Users/ASUS/anaconda_projects/authors/data/gutenberg/books";
    const int number_of_text_patches = 7;
    const int n_grams_size = 2;
    const int min_book_length = 50000;
    const int max_book_length = 100000000;
    const int min_number_of_books_of_author = 40;
    const int number_of_learning_books_per_author = 30;
    const int number_of_precict_books_per_author = 10;
    const int max_n_grams_amount = 200000 ;
    const double max_n_grams_dev = 1 ;
    const double n_grams_distances_amount = 10000000 ;
    //parameters
    vector<string> n_grams = AuthorsRecognition::build_n_grams(alphabet, n_grams_size);
    //vector<string> n_grams = {"gup","cco","oki","dnt","hno","rnd","kdo","kis","abo","bac","ash","wan","oer","won","dli","cri","may","ylo","get","gfa","khe","oka","ike","ced","uni","okh","nga","sib","nki","lik","tme","ook","lya","lld","tgo","ons","cci","ecr","cka","few","fel","sai","oee","ehy","way","awa","ivd","eis","ull","eco","wni","sre","epl","ddo","dly","twr","ugh","oug","inm","eor","ksa","ink","goa","xci","pre","fco","tnd","ybu","ket","ats","wwa","goi","upt","hin","hak","hyh","bou","shw","gru","ied","mil","epr","ckl","hho","thm","boy","aid","big","ldl","hom","kes","izo","knd","ebo","rep","rwr","ewm","owl","nkw","iva","cid","pfo","veg","ign","oin","nwo","upw","oko","eer","lov","fmi","oot","hyw","occ","dcl","dow","got","ped","wid","tel","ddr","tra","rhy","aft","afe","vew","gda","keh","hdw","meh","kwe","abl","dnd","beg","hbu","bew","gsh","rly","ndl","ing","loo","stn","ifw","kca","jac","tly","mak","wor","oyh","mom","pin","kwa","rwa","tst","ovd","dri","oze","rew","osi","tno","gve","thn","wto","lln","atc","bso","uri","wix","dgo","spo","pno","pto","sia","opp","bho","dba","rki","gso","ile","raf","adl","ttl","fal","mwo","mon","wly","pro","cie","tup","pyi","thy","kto","dne","upe","osw","pyd","rwo","rul","gri","gab","kas","pby","fas","con","ply","ysh","rou","cks","xtm","dez","dhy","led","ake","yan","app","vdh","dkn","lyh","jus","its","ohe","lnd","tli","kli","dke","oww","osh","lds","ack","wkw","utt","ifi","hyy","oyw","owh","ghi","ief","shd","fei","eow","low","kor","avo","usa","myl","llh","wmo","kfu","sho","llw","isa","wst","stm","ibl","uwe","eep","hal","avy","kst","shy","rvd","old","ble","kle","cir","apr","gsa","ndh","wsa","rue","itw","ngs","gon","llm","fyo","gcr","dma","gho","nyt","lri","rhe","wlo","row","her","tcr","doa","ndm","obs","nbu","omp","edo","cte","tsh","idt","lke","rig","snd","gnd","ssi","ads","gba","evo","fmy","gaz","sse","own","gfo","esa","ksi","wri","dbo","ntk","ega","gai","gst","ula","rlo","lwo","kit","hou","acc","wna","oan","voi","wwh","twi","epe","pra","ath","ezd","sec","ckw","nsi","riz","mpo","eje","ydo","wnd","rcy","akn","sha","ldn","ove","nec","gto","onl","ent","kll","ndt","ckh","hel","ere","ywr","tke","byk","spu","dfa","ows","dob","ksn","ppe","whe","eup","kea","dot","sap","mfa","uss","rfo","cky","ddi","dbr","bse","rmd","opy","pwa","ind","ngr","rje","rtu","lno","ial","awr","ick","kel","irw","oly","hdo","gif","woe","rdn","ngo","yis","sju","sac","via","daf","rgr","kwi","lls","ces","ndn","rwi","now","cup","pri","uof","rce","ins","dfy","but","sxe","hyb","non","icw","qua","ldm","oeh","rgi","hat","mpr","six","scr","nbe","sce","dep","lat","eel","tho","ppd","and","iml","wli","rso","tut","erv","equ","zdo","par","ezl","obo","ooh","dbl","uvr","wak","icr","fee","mie","lll","tch","kdi","jen","gly","lre","rpl","dgl","tec","ppy","heb","try","ued","ewa","tht","kwh","yup","twa","kse","ayy","kan","ycl","pof","any","zce","zno","rap","gdo","lbe","tkn","yaf","kth","gge","ezv","sli","ssh","kye","etr","vec","gin","ele","kbe","tan","kup","nez","apa","ctl","owa","npr","sph","ghs","yhy","avi","uld","pan","eke","svo","jel","alt","ife","ulw","unt","nco","ale","pte","ixm","vez","rnh","yri","som","ayh","hgo","owi","ppr","yif","ght","yll","asa","zpa","hby","swi","pbu","dup","ssu","yli","awl","snt","kbo","res","ube","nje","win","iny","irh","dsh","pid","aki","ngt","xis","orw","zvo","awe","poi","nfy","zqu","hya","ghe","vou","xtr","xpa","thl","llo","ngi","rey","kna","hoo","ztr","jou","eeh","oks","bid","ese","otr","wmi","eca","fup","kod","kda","onc","mnd","tbl","uzz","jai","atm","uev","ybr","sod","myh","rtt","val","htl","wil","myk","uis","nme","ofy","rcr","wee","ost","gne","hol","wby","llt","mbr","nyo","say","yla","cta","cwo","hta","ezt","ffr","pli","eyl","uej","ful","lyl","pel","dgi","efy","gbo","ece","imp","olr","ood","wab","zde","byy","uen","mur","goo","lpo","ymy","xeu","ckt","gpa","epo","ayw","oyi","cla","anc","ypa","gpl","dsm","mpl","usp","lxi","zmo","cso","tfe","tpo","jum","yco","yxe","ebe","yhe","wmy","jug","rnt","tki","pth","ezc","hye","mco","ngh","ybe","pou","kut","yez","eur","edl","tas","ivo","fly","odd","adw","dgm","nna","dwr","ewr","lth","sje","gtr","gan","gca","npo","wel","eak","ije","hyf","gbu","nfh","moi","kni","egi","scu","hus","rld","pyr","por","gey","xst","cep","dbe","wha","ues","evi","jes","reh","cop","wma","tyo","eta","igg","tbe","vis","tco","ieu","eoi","lep","ede","mfo","omm","oke","hro","onn","dje","dsw","ftl","roy","ezp","vre","alk","zbi","hbe","gfe","xle","nvi","cic","tai","yyo","ker","yot","dll","svu","iat","mli","emo","yho","kcr","ldw","ewi","nym","cul","eva","npl","tua","yas","wnt","tmo","lus","hhy","kss","usc","oid","ork","fwe","yil","wgr","dam","wup","igh","ous","rir","atw","ctu","ntr","idw","hor","zla","lye","ver","ezm","exi","rgt","blu","wsi","abh","ois","rmy","rre","ybo","que","iav","gtm","ead","ehe","gro","eso","uec","nmy","ifd","amh","sma","tmy","rvi","csa","sey","aho","vit","ywi","wns","irs","liv","hyd","dsa","hed","ghl","ezr","jev","zma","elu","hmy","fme","uir","ais","eag","wki","uia","sui","xit","rtn","pue","com","eda","dst","thi","tou","wsh","oow","ekn","yno","utw","giv","zon","mai","nct","tre","sur","lki","loi","zdi","uly","nlo","lui","bup","fcr","bie","wea","rms","nte","ofm","mea","ygo","adh","xpr","ksy","rsh","onv","cko","aym","emi","ctd","xso","lje","ala","yme","iui","dit","stt","rqu","bon","ada","otm","wne","kfa","sne","men","jep","soi","zpr","veh","eyc","wut","rli","hyo","pym","ezs","oiq","ily","lyo","tqu","cel","ixa","erw","rui","ser","xau","vot","wal","dom","pfr","odl","iso","amd","mer"};
    //vector<string> n_grams = {"gup","cco","oki","dnt","hno","rnd","kdo","kis","abo","bac","ash","wan","oer","won","dli","cri","may","ylo","get","gfa","khe","oka","ike","ced","uni","okh","nga","sib","nki","lik","tme","ook","lya","lld","tgo","ons","cci","ecr","cka","few","fel","sai","oee","ehy","way","awa","ivd","eis","ull","eco","wni","sre","epl","ddo","dly","twr","ugh","oug","inm","eor","ksa","ink","goa","xci","pre","fco","tnd","ybu","ket","ats","wwa","goi","upt","hin","hak","hyh","bou","shw","gru","ied","mil","epr","ckl","hho","thm","boy","aid","big","ldl","hom","kes","izo","knd","ebo","rep","rwr","ewm","owl","nkw","iva","cid","pfo","veg","ign","oin","nwo","upw","oko","eer","lov","fmi","oot","hyw","occ","dcl","dow","got","ped","wid","tel","ddr","tra","rhy","aft","afe","vew","gda","keh","hdw","meh","kwe","abl","dnd","beg","hbu","bew","gsh","rly","ndl","ing","loo","stn","ifw","kca","jac","tly","mak","wor","oyh","mom","pin","kwa","rwa","tst","ovd","dri","oze","rew","osi","tno","gve","thn","wto","lln","atc","bso","uri","wix","dgo","spo","pno","pto","sia","opp","bho","dba","rki","gso","ile","raf","adl","ttl","fal","mwo","mon","wly","pro","cie","tup","pyi","thy","kto","dne","upe","osw","pyd","rwo","rul","gri","gab","kas","pby","fas","con","ply","ysh","rou","cks","xtm","dez","dhy","led","ake","yan","app","vdh","dkn","lyh","jus","its","ohe","lnd","tli","kli","dke","oww","osh","lds","ack","wkw","utt","ifi","hyy","oyw","owh","ghi","ief","shd","fei","eow","low","kor","avo","usa","myl","llh","wmo","kfu","sho","llw","isa","wst","stm","ibl","uwe","eep","hal","avy","kst","shy","rvd","old","ble","kle","cir","apr","gsa","ndh","wsa","rue","itw","ngs","gon","llm","fyo","gcr","dma","gho","nyt","lri","rhe","wlo","row","her","tcr","doa","ndm","obs","nbu","omp","edo","cte","tsh","idt","lke","rig","snd","gnd","ssi","ads","gba","evo","fmy","gaz","sse","own","gfo","esa","ksi","wri","dbo","ntk","ega","gai","gst","ula","rlo","lwo","kit","hou","acc","wna","oan","voi","wwh","twi","epe","pra","ath","ezd","sec","ckw","nsi","riz","mpo","eje","ydo","wnd","rcy","akn","sha","ldn","ove","nec","gto","onl","ent","kll","ndt","ckh","hel","ere","ywr","tke","byk","spu","dfa","ows","dob","ksn","ppe","whe","eup","kea","dot","sap","mfa","uss","rfo","cky","ddi","dbr","bse","rmd","opy","pwa","ind","ngr","rje","rtu","lno","ial","awr","ick","kel","irw","oly","hdo","gif","woe","rdn","ngo","yis","sju","sac","via","daf","rgr","kwi","lls","ces","ndn","rwi","now","cup","pri","uof","rce","ins","dfy","but","sxe","hyb","non","icw","qua","ldm","oeh","rgi","hat","mpr","six","scr","nbe","sce","dep","lat","eel","tho","ppd","and","iml","wli","rso","tut","erv","equ","zdo","par","ezl","obo","ooh","dbl","uvr","wak","icr","fee","mie","lll","tch","kdi","jen","gly","lre","rpl","dgl","tec","ppy","heb","try","ued","ewa","tht","kwh","yup","twa","kse","ayy","kan","ycl","pof","any","zce","zno","rap","gdo","lbe","tkn","yaf","kth","gge","ezv","sli","ssh","kye","etr","vec","gin","ele","kbe","tan","kup","nez","apa","ctl","owa","npr","sph","ghs","yhy","avi","uld","pan","eke","svo","jel","alt","ife","ulw","unt","nco","ale","pte","ixm","vez","rnh","yri","som","ayh","hgo","owi","ppr","yif","ght","yll","asa","zpa","hby","swi","pbu","dup","ssu","yli","awl","snt","kbo","res","ube","nje","win","iny","irh","dsh","pid","aki","ngt","xis","orw","zvo","awe","poi","nfy","zqu","hya","ghe","vou","xtr","xpa","thl","llo","ngi","rey","kna","hoo","ztr","jou","eeh","oks","bid","ese","otr","wmi","eca","fup","kod","kda","onc","mnd","tbl","uzz","jai","atm","uev","ybr","sod","myh","rtt","val","htl","wil","myk","uis","nme","ofy","rcr","wee","ost","gne","hol","wby","llt","mbr","nyo","say","yla","cta","cwo","hta","ezt","ffr","pli","eyl","uej","ful","lyl","pel","dgi","efy","gbo","ece","imp","olr","ood","wab","zde","byy","uen","mur","goo","lpo","ymy","xeu","ckt","gpa","epo","ayw","oyi","cla","anc","ypa","gpl","dsm","mpl","usp","lxi","zmo","cso","tfe","tpo","jum","yco","yxe","ebe","yhe","wmy","jug","rnt","tki","pth","ezc","hye","mco","ngh","ybe","pou","kut","yez","eur","edl","tas","ivo","fly","odd","adw","dgm","nna","dwr","ewr","lth","sje","gtr","gan","gca","npo","wel","eak","ije","hyf","gbu","nfh","moi","kni","egi","scu","hus","rld","pyr","por","gey","xst","cep","dbe","wha","ues","evi","jes","reh","cop","wma","tyo","eta","igg","tbe","vis","tco","ieu","eoi","lep","ede","mfo","omm","oke","hro","onn","dje","dsw","ftl","roy","ezp","vre","alk","zbi","hbe","gfe","xle","nvi","cic","tai","yyo","ker","yot","dll","svu","iat","mli","emo","yho","kcr","ldw","ewi","nym","cul","eva","npl","tua","yas","wnt","tmo","lus","hhy","kss","usc","oid","ork","fwe","yil","wgr","dam","wup","igh","ous","rir","atw","ctu","ntr","idw","hor","zla","lye","ver","ezm","exi","rgt","blu","wsi","abh","ois","rmy","rre","ybo","que","iav","gtm","ead","ehe","gro","eso","uec","nmy","ifd","amh","sma","tmy","rvi","csa","sey","aho","vit","ywi","wns","irs","liv","hyd","dsa","hed","ghl","ezr","jev","zma","elu","hmy","fme","uir","ais","eag","wki","uia","sui","xit","rtn","pue","com","eda","dst","thi","tou","wsh","oow","ekn","yno","utw","giv","zon","mai","nct","tre","sur","lki","loi","zdi","uly","nlo","lui","bup","fcr","bie","wea","rms","nte","ofm","mea","ygo","adh","xpr","ksy","rsh","onv","cko","aym","emi","ctd","xso","lje","ala","yme","iui","dit","stt","rqu","bon","ada","otm","wne","kfa","sne","men","jep","soi","zpr","veh","eyc","wut","rli","hyo","pym","ezs","oiq","ily","lyo","tqu","cel","ixa","erw","rui","ser","xau","vot","wal","dom","pfr","odl","iso","amd","mer","rfe","jyv","fcl","kfl","xll","swr","ibi","cke","yet","eno","eyw","vsa","ceq","ggi","hfo","myd","hly","ava","uil","ise","nqu","sij","htn","job","hyc","udd","pyo","eui","kqu","mxc","txt","wyl","fyy","bbl","hki","sly","nue","ayl","goe","mdt","ans","ksf","xou","rme","arp","ubi","oul","hss","ysl","dwo","aze","umo","vey","mme","ckd","odr","tmw","bef","bea","hso","hts","naf","nup","sye","mel","sez","myo","orm","ujo","rve","lue","myb","uel","ndr","uce","urd","odm","oir","eai","eiv","ods","dxe","ont","don","squ","ugo","ezb","beh","wls","usl","rez","ibu","kdh","xdu","cet","ywo","wbr","med","suy","lez","mdo","xpo","rpo","bbi","wya","ldb","esc","lwe","rgo","dth","kft","ipo","son","nta","tir","opd","tje","wfa","mps","kew","rec","ged","epa","qui","dha","icu","omy","swa","lvo","mno","les","tvr","asd","cen","avu","lem","mly","xje","ewe","ezj","tce","suc","psq","iez","ony","npa","ire","del","hnd","sar","dft","ida","oth","ako","dan","una","neu","apo","use","smy","dos","ejo","wro","aqu","uxa","eux","usv","ytr","rgd","owm","you","egl","ipa","hen","ure","htt","ppo","hpu","cne","hsw","ych","yma","upo","uxs","aga","xli","nmo","ubb","fma","nly","uiv","ify","lle","end","lum","sav","tlu","eil","ngw","ggy","ico","kmy","act","hbo","efh","deu","utr","yut","ooa","ift","xan","ien","hme","sob","kem","nto","lys","quo","ccl","dsn","uid","yxi","cse","ysw","pcr","sil","zzl","ond","rbl","iil","ldd","ldt","prs","nll","ith","tud","tos","urq","xme","ank","gdr","ksh","tha","idd","eth","ucd","itu","dev","cve","ejy","rav","ncc","did","htw","daw","tth","lav","thw","xco","rys","fdi","nxe","wsl","ywe","sby","orl","rma","vdo","adr","shi","vdw","ndd","hha","siv","zip","dix","tru","thd","swu","ghy","iss","dlo","kco","wnh","hhe","ngd","ayo","ven","kfo","fso","nua","eed","lec","ifh","tfo","wre","nep","xmi","kno","uth","anq","smm","hlo","lkn","aww","emu","oix","yca","oup","odo","cip","xet","azd","hys","ela","ear","woo","lap","pui","tok","ump","nds","oag","pas","alh","see","etq","lyn","lyg","xac","dfe","ilc","pde","uje","obu","ppi","ita","cil","nss","erm","hce","gla","wis","hth","akw","eha","khi","tju","ucy","asw","ueh","rai","spr","inq","til","mpa","idn","nty","ppa","wno","usd","nne","ruy","zle","ust","nom","owt","rem","jta","seh","ean","sdi","ecd","ihi","lyf","iju","syo","sco","uon","ura","lks","ouv","spa","uxp","sen","kdn","eun","esh","peu","ovi","sky","tde","oie","tsa","ery","sdu","gmi","ama","tty","vdt","nai","rik","swe","ydh","prv","oym","kof","uan","joy","ion","eny","ndy","rsq","tpa","tvo","bem","oud","cmo","pwi","nrh","cun","ode","cia","des","lai","wit","vol","uta","sde","uxl","gaf","plu","iqu","ugu","ipp","gwh","clo","ilr","foi","vri","ong","mav","for","nsd","ffh","ewo","ypl","ghh","lst","eym","lay","aff","rrd","rde","gdt","lwi","def","yna","eul","ait","wfo","dfl","mwh","dte","vdm","uoi","aux","vai","lhy","nrd","oma","ubd","uwo","rin","aty","hsh","vdf","aks","txi","gch","uso","wbu","nkm","tpe","llb","all","she","vra","cdw","ndw","ntd","ral","dpr","ybi","sep","was","ili","une","tta","yin","tue","ego","tne","ifc","smo","usq","rvo","mec","tem","nsl","yav","zra","chi","ils","tjy","odw","nvr","ymo","yda","pic","new","jem","llr","xse","ora","eys","mew","nui","ija","bee","ysa","hyt","eyd","soy","auv","upi","bsi","yrc","ghw","wca","laa","epu","wsm","igu","tol","hgi","cam","oye","wsn","yto","lqu","fli","ant","nef","vos","nfi","blo","ecl","lnl","omb","ror","iol","iai","oah","uxo","psd","hec","eud","tob","pen","flu","isq","cpa","ter","olu","dme","chy","nyl","tbu","agi","htr","hyg","dis","eck","uwa","eek","nes","rab","noh","ota","itl","xmo","unm","jea","izz","riv","yai","tab","asn","tla","gav","aum","ezq","url","opr","zto","toh","ixl","ims","npe","ebu","why","ape","fxe","thr","opu","gth","ite","rks","jeu","eau","ofl","yor","war","rwe","lex","nbl","tsd","ipl","dlt","asr","sir","oys","quu","mbl","uve","unp","ixh","lpa","gna","odu","yab","nmm","lev","dho","ybl","xui","hhi","dec","ber","lam","aij","lyi","rbu","itp","hdh","tvi","agl","vda","ive","oyl","tja","mlo","zga","ryi","nsp","urw","cra","gha","nir","idy","xem","uxc","lwh","oen","eod","icd","etl","muc","erg","ilv","sgo","duc","svi","nli","gue","awo","hev","oje","dab","udo","tsu","mha","hee","wsw","oia","enc","kly","roi","irk","nkc","bes","nks","sot","tfy","ses","scn","ado","upd","uaa","org","txe","hui","ddl","itd","pwh","umi","vdn","faf","aaj","esd","rie","ppu","ksr","lme","not","lyw","ltn","lyt","tle","aon","cha","gbe","uva","yni","rho","tak","ydu","uip","yes","liq","aet","lal","dys","deg","vys","esr","uep","cin","uxd","tvu","ghd","piq","rbo","sla","hai","oui","oba","onm","evr","upa","aua","rth","hsm","nwr","rvl","kfr","vas","ixs","bei","tav","dfh","uwr","ema","asp","vla","zdj","yhu","nts","req","emm","nap","ayn","lna","uun","vdi","nko","pme","lwa","hwo","xpl","lsh","hym","xar","etd","sie","num","bhi","iun","rya","ndg","stw","kbu","sle","sif","tar","enb","xca","heu","dun","zfa","doe","hey","gbr","vue","lmy","eyo","oig","ehi","odh","thh","gyo","irc","ywh","nbr","aya","sem","tog","aai","var","pag","uxe","usn","eli","aws","dui","ntv","rxe","hyl","mup","ill","plo","eit","wth","nyf","tmu","xdo","asi","wnf","ilp","agg","wof","sow","ygr","elq","fte","lit","iap","kic","aco","lob","ksb","sin","dso","lfn","nve","oij","mys","eme","nav","nri","otg","rub","usr","ggl","orh","ynd","ymi","mmy","oic","psi","nsq","okd","geq","hew","jec","www","urv"};


    auto start = chrono::steady_clock::now(), end = chrono::steady_clock::now();

    cout << "Loading authors" << endl;
    start = chrono::steady_clock::now();
    unordered_map<string, AuthorsRecognition::Author> authors = AuthorsRecognition::load_authors_data(lists_directory);
    end = chrono::steady_clock::now();
    cout << "Loading finished. Time: " << chrono::duration <double, milli> (end-start).count() << " ms" << endl << endl;

    cout << "Filtering min_book_length: " << min_book_length << ", min_number_of_books_of_author: " << min_number_of_books_of_author << endl;
    start = chrono::steady_clock::now();
    for(auto i =authors.begin();i != authors.end();) {
        if (i->first == "Various" || i->first == "Anonymous" || i->first == "Unknown" || i->first == "!Unknown") {
            i = authors.erase(i);
            continue;
        }

        i->second.books.erase(std::remove_if(i->second.books.begin(), i->second.books.end(),
        [](const AuthorsRecognition::Book& book) {
            return book.length<min_book_length;
        }), i->second.books.end());
        if (i->second.books.size()<min_number_of_books_of_author) {
            i = authors.erase(i);
            continue;
        }

        std::random_device rd;
        std::mt19937 g(rd());

        //std::shuffle(i->second.books.begin(), i->second.books.end(), rnd);
        if(i->second.books.size() > number_of_learning_books_per_author+number_of_precict_books_per_author) i->second.books.erase(i->second.books.begin()+number_of_learning_books_per_author+number_of_precict_books_per_author, i->second.books.end());
        ++i;
    }
    end = chrono::steady_clock::now();
    //for(const auto& [name, author] : authors) {
    //    cout << name<< ": [";
    //    for(const auto& book : author.books) cout << book.id << ',';
    //    cout << "]\n";
    //}
    cout << "Filtering finished. Time: " << chrono::duration <double, milli> (end-start).count() << " ms" << endl << endl;

    vector<string> authorsList;
    for(const auto& elem : authors) authorsList.push_back(elem.first);
    sort(authorsList.begin(), authorsList.end(), [&authors](const string& a1, const string& a2){ return authors[a1].books.size() > authors[a2].books.size(); });
    cout << "Authors amount: " << authorsList.size() << '\n' ;
    for(const auto& elem : authorsList) cout << elem << '\n' ;
    cout << '\n';

    cout << "Loading and parsing books" << endl;
    start = chrono::steady_clock::now();
    vector<std::thread> threads;
    for(const auto&name:authorsList){
        //cout << name << "\n";
        auto& author = authors[name];
        threads.push_back(std::thread([&author, &books_directory, &alphabet, &n_grams, &n_grams_distances_amount](){
            auto start1 = chrono::steady_clock::now();
            auto diff2 = chrono::steady_clock::now()-start1;
            for(auto ibook = author.books.begin();ibook != author.books.end();){
                rapidjson::Document d;
                ifstream ifs(books_directory+'/'+(ibook->id)+"/bookData.json");
                rapidjson::IStreamWrapper isw(ifs);

                d.ParseStream(isw);
                if (d.HasParseError()) throw std::runtime_error("Error while parsing "+books_directory+'/'+(ibook->id)+"/bookData.json");
                if (!d.IsObject()) throw std::runtime_error("Wrong format(not an object) "+books_directory+'/'+(ibook->id)+"/bookData.json");

                if(d["title"].IsNull()) {
                    ibook = author.books.erase(ibook);
                    continue;
                }
                ibook->title = d["title"].GetString();
                auto start2 = chrono::steady_clock::now();
                /*const string text = AuthorsRecognition::filter_text(
                       AuthorsRecognition::read_text(books_directory+'/'+(ibook->id)+"/book.txt"),
                       alphabet
                    );

                (AuthorsRecognition::BookStatistics&)(*ibook) = AuthorsRecognition::get_text_statisticts(
                    text,
                    n_grams,
                    number_of_text_patches
                );*/
                (AuthorsRecognition::BookStatistics&)(*ibook) = AuthorsRecognition::get_text_statisticts(
                    books_directory+'/'+(ibook->id)+"/book.txt",
                    alphabet,
                    n_grams,
                    number_of_text_patches,
                    max_book_length,
                    n_grams_distances_amount
                );
                auto end2 = chrono::steady_clock::now();
                diff2 += end2 - start2;
                //cout << chrono::duration <double, milli> (end2 - start2).count() << " ms" << endl << endl;
                if (ibook->length < 10000) {
                    ibook = author.books.erase(ibook);
                    continue;
                }
                ++ibook;
            }
            auto end1 = chrono::steady_clock::now();
            auto diff1 = end1 - start1;
            //cout << chrono::duration <double, milli> (diff2).count() << "/" << chrono::duration <double, milli> (diff1).count() << " ms" << endl << endl;
        }));
    }
    for(auto& t: threads) t.join();
    threads.clear();
    end = chrono::steady_clock::now();
    cout << "Loading and parsing books finished. Time: " << chrono::duration <double, milli> (end-start).count() << " ms" << endl << endl;



    cout << "Splitting data into learning and prediction sets." << '\n';
    unordered_map<string, vector<AuthorsRecognition::Book>> learning_data;
    unordered_map<string, vector<AuthorsRecognition::Book>> predict_data;
    for(const auto& name : authorsList){
        learning_data[name] = vector<AuthorsRecognition::Book>();
        std::copy(authors[name].books.begin(), authors[name].books.begin()+number_of_learning_books_per_author, std::back_inserter(learning_data[name]));
        predict_data[name] = vector<AuthorsRecognition::Book>();
        std::copy(authors[name].books.begin()+number_of_learning_books_per_author, authors[name].books.end(), std::back_inserter(predict_data[name]));
    }
    cout << "Ok." << '\n';

    cout << "Sorting n_grams\n";
    start = chrono::steady_clock::now();
    vector<double> n_grams_importances = AuthorsRecognition::get_n_grams_importances(learning_data);
    end = chrono::steady_clock::now();
    vector<pair<string, double>> n_grams_with_importances ;
    for(unsigned long i=0;i<n_grams.size();++i ) n_grams_with_importances.push_back(pair<string, double>(n_grams[i], n_grams_importances[i]));
    sort(n_grams_with_importances.begin(), n_grams_with_importances.end(), [](const auto& x, const auto& y){ return x.second <  y.second; });
    //for(long i=0;i<n_grams.size();++i ) cout << n_grams_with_importances[i].first << " : "<< n_grams_with_importances[i].second << '|' ; cout << '\n' ;
    cout << "Sort finished. Time: "<< chrono::duration <double, milli> (end-start).count() << " ms" << endl << endl;


    if(false){
        n_grams.clear();
        for(long i=0; i < max_n_grams_amount && n_grams_with_importances[i].second < max_n_grams_dev; ++i) n_grams.push_back(n_grams_with_importances[i].first);
        cout << "n_grams : " ;
        for(const auto& x: n_grams) cout << x << ' ';
        cout << '\n';
        cout << "n_grams amount: " << n_grams.size() << '\n' ;
        cout << "Loading and parsing books" << endl;
        start = chrono::steady_clock::now();
        vector<std::thread> threads;
        for(const auto&name:authorsList){
            //cout << name << "\n";
            auto& author = authors[name];
            threads.push_back(std::thread([&author, &books_directory, &alphabet, &n_grams, &n_grams_distances_amount](){
                auto start1 = chrono::steady_clock::now();
                auto diff2 = chrono::steady_clock::now()-start1;
                for(auto ibook = author.books.begin();ibook != author.books.end();){
                    rapidjson::Document d;
                    ifstream ifs(books_directory+'/'+(ibook->id)+"/bookData.json");
                    rapidjson::IStreamWrapper isw(ifs);

                    d.ParseStream(isw);
                    if (d.HasParseError()) throw std::runtime_error("Error while parsing "+books_directory+'/'+(ibook->id)+"/bookData.json");
                    if (!d.IsObject()) throw std::runtime_error("Wrong format(not an object) "+books_directory+'/'+(ibook->id)+"/bookData.json");

                    if(d["title"].IsNull()) {
                        ibook = author.books.erase(ibook);
                        continue;
                    }
                    ibook->title = d["title"].GetString();
                    auto start2 = chrono::steady_clock::now();
                    /*const string text = AuthorsRecognition::filter_text(
                           AuthorsRecognition::read_text(books_directory+'/'+(ibook->id)+"/book.txt"),
                           alphabet
                        );

                    (AuthorsRecognition::BookStatistics&)(*ibook) = AuthorsRecognition::get_text_statisticts(
                        text,
                        n_grams,
                        number_of_text_patches
                    );*/
                    (AuthorsRecognition::BookStatistics&)(*ibook) = AuthorsRecognition::get_text_statisticts(
                        books_directory+'/'+(ibook->id)+"/book.txt",
                        alphabet,
                        n_grams,
                        number_of_text_patches,
                        max_book_length,
                        n_grams_distances_amount
                    );
                    auto end2 = chrono::steady_clock::now();
                    diff2 += end2 - start2;
                    //cout << chrono::duration <double, milli> (end2 - start2).count() << " ms" << endl << endl;
                    if (ibook->length < 10000) {
                        ibook = author.books.erase(ibook);
                        continue;
                    }
                    ++ibook;
                }
                auto end1 = chrono::steady_clock::now();
                auto diff1 = end1 - start1;
                //cout << chrono::duration <double, milli> (diff2).count() << "/" << chrono::duration <double, milli> (diff1).count() << " ms" << endl << endl;
            }));
        }
        for(auto& t: threads) t.join();
        threads.clear();
        end = chrono::steady_clock::now();
        cout << "Loading and parsing books finished. Time: " << chrono::duration <double, milli> (end-start).count() << " ms" << endl << endl;

        cout << "Splitting data into learning and prediction sets." << '\n';
        learning_data.clear();
        predict_data.clear();
        for(const auto& name : authorsList){
            learning_data[name] = vector<AuthorsRecognition::Book>();
            std::copy(authors[name].books.begin(), authors[name].books.begin()+number_of_learning_books_per_author, std::back_inserter(learning_data[name]));
            predict_data[name] = vector<AuthorsRecognition::Book>();
            std::copy(authors[name].books.begin()+number_of_learning_books_per_author, authors[name].books.end(), std::back_inserter(predict_data[name]));
        }
        cout << "Ok." << '\n';
    }

    cout << "Calculating split deviations." << '\n';
    start = chrono::steady_clock::now();
    double fr_global_split_deviations = AuthorsRecognition::calculate_global_split_deviation(learning_data, AuthorsRecognition::get_frequencies_ethalon, AuthorsRecognition::frequencies_distance);
    double ps_global_split_deviations = AuthorsRecognition::calculate_global_split_deviation(learning_data, AuthorsRecognition::get_pstatistic_ethalon, AuthorsRecognition::pstatistic_distance);
    //double ps_2_global_split_deviations = AuthorsRecognition::calculate_global_split_deviation(learning_data, AuthorsRecognition::get_pstatistic_2_ethalon, AuthorsRecognition::pstatistic_distance_2);
    unordered_map<string, double> fr_split_deviations = AuthorsRecognition::calculate_split_deviations(learning_data, AuthorsRecognition::get_frequencies_ethalon, AuthorsRecognition::frequencies_distance);
    unordered_map<string, double> ps_split_deviations = AuthorsRecognition::calculate_split_deviations(learning_data, AuthorsRecognition::get_pstatistic_ethalon, AuthorsRecognition::pstatistic_distance);
    //unordered_map<string, double> ps_2_split_deviations = AuthorsRecognition::calculate_split_deviations(learning_data, AuthorsRecognition::get_pstatistic_2_ethalon, AuthorsRecognition::pstatistic_distance_2);
    cout << "Global: " << fr_global_split_deviations << "(fr), " << ps_global_split_deviations << "(ps)" << '\n';
    for(const auto& name : authorsList){
        //cout << name << ": " << fr_split_deviations[name] << "(fr), " << ps_split_deviations[name] << "(ps)" << ps_2_split_deviations[name] << "(ps 2)" << '\n';
        cout << name << ": " << fr_split_deviations[name] << "(fr), " << ps_split_deviations[name] << "(ps)" << '\n';
    }
    end = chrono::steady_clock::now();
    cout << "Calculating split deviations finished. Time: " << chrono::duration <double, milli> (end-start).count() << " ms" << endl << endl;

    cout << "Calculating ethalons." << '\n';
    start = chrono::steady_clock::now();
    unordered_map<string, vector<AuthorsRecognition::BookStatistics>> grouped_frequencies_ethalons;
    unordered_map<string, vector<AuthorsRecognition::BookStatistics>> grouped_pstatistic_ethalons;
    //unordered_map<string, vector<AuthorsRecognition::BookStatistics>> grouped_pstatistic_2_ethalons;
    for(const auto& name : authorsList){
        //cout << name << '\n';
        //grouped_frequencies_ethalons[name] = vector<AuthorsRecognition::BookStatistics>();
        //grouped_frequencies_ethalons[name].push_back(AuthorsRecognition::get_frequencies_ethalon(learning_data[name]));
        //grouped_frequencies_ethalons[name] = calculate_grouped_ethalons(learning_data[name], fr_split_deviations[name], AuthorsRecognition::get_frequencies_ethalon, AuthorsRecognition::frequencies_distance);
        grouped_frequencies_ethalons[name] = calculate_grouped_ethalons2(learning_data[name], fr_split_deviations[name], AuthorsRecognition::get_frequencies_ethalon, AuthorsRecognition::frequencies_distance);
        //grouped_frequencies_ethalons[name] = calculate_grouped_ethalons2(learning_data[name], fr_global_split_deviations, AuthorsRecognition::get_frequencies_ethalon, AuthorsRecognition::frequencies_distance);


        //cout << name << '\n';
        //copy(learning_data[name].begin(), learning_data[name].end(), std::back_inserter(grouped_pstatistic_ethalons[name]));
        //grouped_pstatistic_ethalons[name] = vector<AuthorsRecognition::BookStatistics>();
        //grouped_pstatistic_ethalons[name].push_back(AuthorsRecognition::get_pstatistic_ethalon(learning_data[name]));
        grouped_pstatistic_ethalons[name] = calculate_grouped_ethalons(learning_data[name], ps_split_deviations[name], AuthorsRecognition::get_pstatistic_ethalon, AuthorsRecognition::pstatistic_distance);
        //grouped_pstatistic_ethalons[name] = calculate_grouped_ethalons2(learning_data[name], ps_split_deviations[name], AuthorsRecognition::get_pstatistic_ethalon, AuthorsRecognition::pstatistic_distance);
        //grouped_pstatistic_ethalons[name] = calculate_grouped_ethalons2(learning_data[name], ps_global_split_deviations, AuthorsRecognition::get_pstatistic_ethalon, AuthorsRecognition::pstatistic_distance);

        //grouped_pstatistic_2_ethalons[name] = calculate_grouped_ethalons(learning_data[name], ps_split_deviations[name], AuthorsRecognition::get_pstatistic_2_ethalon, AuthorsRecognition::pstatistic_distance_2);
        //grouped_pstatistic_ethalons[name] = calculate_grouped_ethalons(learning_data[name], ps_global_split_deviations, AuthorsRecognition::get_pstatistic_2_ethalon, AuthorsRecognition::pstatistic_distance_2);
    }
    cout << "Frequency groups:" << '\n';
    for(const auto& name : authorsList){
        cout << name << ": " << grouped_frequencies_ethalons[name].size() << '\n';
    } cout << '\n' ;
    cout << "Pstatistic groups:" << '\n';
    for(const auto& name : authorsList){
        cout << name << ": " << grouped_pstatistic_ethalons[name].size()  << '\n';
    } cout << '\n' ;
    /*cout << "Pstatistic 2 groups:" << '\n';
    for(const auto& name : authorsList){
        cout << name << ": " << grouped_pstatistic_2_ethalons[name].size()  << '\n';
    }*/
    end = chrono::steady_clock::now();
    cout << "Calculating ethalons finished. Time: " << chrono::duration <double, milli> (end-start).count() << " ms" << endl << endl;


    cout << "mean ethalon distances\n";
    vector<double> own_distances_learning;
    vector<double> own_distances_predict;
    vector<double> foreign_distances_learning;
    vector<double> foreign_distances_predict;
    for(const auto& name : authorsList){
        double mean_own_learning = 0;
        for(const auto& book: learning_data[name]){
            double distance=std::numeric_limits<double>::infinity();
            for(const auto& ethalon: grouped_frequencies_ethalons[name]){
                const double d = AuthorsRecognition::frequencies_distance(ethalon, book);
                if (distance>d) distance = d;
            }
            own_distances_learning.push_back(distance);
            mean_own_learning+=distance;
        }
        mean_own_learning /=learning_data[name].size();
        double mean_foreign_learning = 0;
        double mean_foreign_learning_text_amount = 0;
        for(const auto& foreign_name : authorsList){
            if(foreign_name == name) continue;
            mean_foreign_learning_text_amount +=learning_data[foreign_name].size();
            for(const auto& book: learning_data[foreign_name]){
                double distance=std::numeric_limits<double>::infinity();
                for(const auto& ethalon: grouped_frequencies_ethalons[name]){
                    const double d = AuthorsRecognition::frequencies_distance(ethalon, book);
                    if (distance>d) distance = d;
                }
                foreign_distances_learning.push_back(distance);
                mean_foreign_learning+=distance;
            }
        }
        mean_foreign_learning /= mean_foreign_learning_text_amount;

        double mean_own_predict = 0;
        for(const auto& book: predict_data[name]){
            double distance=std::numeric_limits<double>::infinity();
            for(const auto& ethalon: grouped_frequencies_ethalons[name]){
                const double d = AuthorsRecognition::frequencies_distance(ethalon, book);
                if (distance>d) distance = d;
            }
            own_distances_predict.push_back(distance);
            mean_own_predict+=distance;
        }
        mean_own_predict /=predict_data[name].size();
        double mean_foreign_predict = 0;
        double mean_foreign_predict_text_amount = 0;
        for(const auto& foreign_name : authorsList){
            if(foreign_name == name) continue;
            mean_foreign_predict_text_amount +=predict_data[foreign_name].size();
            for(const auto& book: predict_data[foreign_name]){
                double distance=std::numeric_limits<double>::infinity();
                for(const auto& ethalon: grouped_frequencies_ethalons[name]){
                    const double d = AuthorsRecognition::frequencies_distance(ethalon, book);
                    if (distance>d) distance = d;
                }
                foreign_distances_predict.push_back(distance);
                mean_foreign_predict+=distance;
            }
        }
        mean_foreign_predict /= mean_foreign_predict_text_amount;
        cout << name << " & " << mean_own_learning << " & "<< mean_own_predict << " & " << mean_foreign_learning << " & "<< mean_foreign_predict <<"\\" << '\n';

    }
    cout << "mean ethalon distances end\n\n";
    cout << "own_distances_learning\n";
    //cout << "[";for(const auto& d: own_distances_learning) cout << d<<',';cout << "]\n";
    cout << "own_distances_learning end\n\n";

    cout << "own_distances_predict\n";
    //cout << "[";for(const auto& d: own_distances_predict) cout << d<<',';cout << "]\n";
    cout << "own_distances_predict end\n\n";

    cout << "foreign_distances_learning\n";
    //cout << "[";for(const auto& d: foreign_distances_learning) cout << d<<',';cout << "]\n";
    cout << "foreign_distances_learning end\n\n";

    cout << "foreign_distances_predict\n";
    //cout << "[";for(const auto& d: foreign_distances_predict) cout << d<<',';cout << "]\n";
    cout << "foreign_distances_predict end\n\n";


    cout << "Predicting fr learning data." << '\n';
    start = chrono::steady_clock::now();
    unsigned long total_amount = 0;
    unsigned long predicted_amount = 0;
    std::mutex m;
    for(const auto& name : authorsList){
        total_amount += learning_data[name].size();
        threads.push_back(std::thread([&m, &learning_data, &name, &grouped_frequencies_ethalons, &predicted_amount](){
            for(const auto& book : learning_data[name]){
                string predicted_author = AuthorsRecognition::predict_author(book, grouped_frequencies_ethalons, AuthorsRecognition::frequencies_distance);
                //cout << book.id << " "<< book.author << "("<< book.title<<") ----- "  << predicted_author << '\n' ;
                if (name == predicted_author) {
                    m.lock();
                    ++predicted_amount;
                    m.unlock();
                }
            }
        }));
    }
    for(auto& t: threads) t.join();
    threads.clear();
    end = chrono::steady_clock::now();
    cout << "Predicting fr learning data finished. "<<predicted_amount<<"/"<<total_amount<<' '<<(double(predicted_amount)/total_amount)<<" predicted. Time: " << chrono::duration <double, milli> (end-start).count() << " ms" << endl << endl;

    cout << "Predicting ps learning data." << '\n';
    start = chrono::steady_clock::now();
    total_amount = 0;
    predicted_amount = 0;
    for(const auto& name : authorsList){
        total_amount += learning_data[name].size();
        threads.push_back(std::thread([&m, &learning_data, &name, &grouped_frequencies_ethalons, &predicted_amount](){
            for(const auto& book : learning_data[name]){
                string predicted_author = AuthorsRecognition::predict_author(book, grouped_frequencies_ethalons, AuthorsRecognition::pstatistic_distance);
                //cout << book.id << " "<< book.author << "("<< book.title<<") ----- "  << predicted_author << '\n' ;
                if (name == predicted_author) ++predicted_amount;
            }
        }));
    }
    for(auto& t: threads) t.join();
    threads.clear();
    end = chrono::steady_clock::now();
    cout << "Predicting ps learning data finished. "<<predicted_amount<<"/"<<total_amount<<' '<<(double(predicted_amount)/total_amount)<<" predicted. Time: " << chrono::duration <double, milli> (end-start).count() << " ms" << endl << endl;


    /*cout << "Predicting ps 2 learning data." << '\n';
    start = chrono::steady_clock::now();
    total_amount = 0;
    predicted_amount = 0;
    for(const auto& name : authorsList){
        total_amount += learning_data[name].size();
        threads.push_back(std::thread([&m, &learning_data, &name, &grouped_pstatistic_2_ethalons, &predicted_amount](){
            for(const auto& book : learning_data[name]){
                string predicted_author = AuthorsRecognition::predict_author(book, grouped_pstatistic_2_ethalons, AuthorsRecognition::pstatistic_distance_2);
                //cout << book.id << " "<< book.author << "("<< book.title<<") ----- "  << predicted_author << '\n' ;
                if (name == predicted_author) ++predicted_amount;
            }
        }));
    }
    for(auto& t: threads) t.join();
    threads.clear();
    end = chrono::steady_clock::now();
    cout << "Predicting ps 2 learning data finished. "<<predicted_amount<<"/"<<total_amount<<" predicted. Time: " << chrono::duration <double, milli> (end-start).count() << " ms" << endl << endl;
    */

    cout << "Predicting fr test data." << '\n';
    start = chrono::steady_clock::now();
    total_amount = 0;
    predicted_amount = 0;
    for(const auto& name : authorsList){
        total_amount += predict_data[name].size();
        threads.push_back(std::thread([&m, &predict_data, &name, &grouped_frequencies_ethalons, &predicted_amount](){
            for(const auto& book : predict_data[name]){
                string predicted_author = AuthorsRecognition::predict_author(book, grouped_frequencies_ethalons, AuthorsRecognition::frequencies_distance);
                //cout << book.id << " "<< book.author << "("<< book.title<<") ----- "  << predicted_author << '\n' ;
                if (name == predicted_author) ++predicted_amount;
            }
        }));
    }
    for(auto& t: threads) t.join();
    threads.clear();
    end = chrono::steady_clock::now();
    cout << "Predicting fr test data finished. "<<predicted_amount<<"/"<<total_amount<<' '<<(double(predicted_amount)/total_amount)<<" predicted. Time: " << chrono::duration <double, milli> (end-start).count() << " ms" << endl << endl;

    cout << "Predicting ps test data." << '\n';
    start = chrono::steady_clock::now();
    total_amount = 0;
    predicted_amount = 0;
    for(const auto& name : authorsList){
        total_amount += predict_data[name].size();
        threads.push_back(std::thread([&m, &predict_data, &name, &grouped_frequencies_ethalons, &predicted_amount](){
            for(const auto& book : predict_data[name]){
                string predicted_author = AuthorsRecognition::predict_author(book, grouped_frequencies_ethalons, AuthorsRecognition::pstatistic_distance);
                //cout << book.id << " "<< book.author << "("<< book.title<<") ----- "  << predicted_author << '\n' ;
                if (name == predicted_author) ++predicted_amount;
            }
        }));
    }
    for(auto& t: threads) t.join();
    threads.clear();
    end = chrono::steady_clock::now();
    cout << "Predicting ps test data finished. "<<predicted_amount<<"/"<<total_amount<<' '<<(double(predicted_amount)/total_amount)<<" predicted. Time: " << chrono::duration <double, milli> (end-start).count() << " ms" << endl << endl;

    /*cout << "Predicting ps 2 test data." << '\n';
    start = chrono::steady_clock::now();
    total_amount = 0;
    predicted_amount = 0;
    for(const auto& name : authorsList){
        total_amount += predict_data[name].size();
        threads.push_back(std::thread([&m, &predict_data, &name, &grouped_pstatistic_2_ethalons, &predicted_amount](){
            for(const auto& book : predict_data[name]){
                string predicted_author = AuthorsRecognition::predict_author(book, grouped_pstatistic_2_ethalons, AuthorsRecognition::pstatistic_distance_2);
                //cout << book.id << " "<< book.author << "("<< book.title<<") ----- "  << predicted_author << '\n' ;
                if (name == predicted_author) ++predicted_amount;
            }
        }));
    }
    for(auto& t: threads) t.join();
    threads.clear();
    end = chrono::steady_clock::now();
    cout << "Predicting ps 2 test data finished. "<<predicted_amount<<"/"<<total_amount<<" predicted. Time: " << chrono::duration <double, milli> (end-start).count() << " ms" << endl << endl;
    */
    return 0;
}


double calculate_n_grams_correlation(string bookid, int n_grams_size1, int n_grams_size2){
    long number_of_text_patches = 20;
    long max_book_length = 100000;
    long n_grams_distances_amount = 10000000;
    const string books_directory = "../../data/gutenberg/books";
    //const string alphabet = "abc";
    const string alphabet = "abcdefghijklmnopqrstuvwxyz";
    vector<string> n_grams1 = AuthorsRecognition::build_n_grams(alphabet, n_grams_size1);
    vector<string> n_grams2 = AuthorsRecognition::build_n_grams(alphabet, n_grams_size2);

    cout << "calculate_n_grams_correlation 1\n";
    AuthorsRecognition::BookStatistics stats1 = AuthorsRecognition::get_text_statisticts(
        books_directory+'/'+(bookid)+"/book.txt",
        alphabet,
        n_grams1,
        number_of_text_patches,
        max_book_length,
        n_grams_distances_amount
    );
    cout << "calculate_n_grams_correlation 2\n";
    AuthorsRecognition::BookStatistics stats2 = AuthorsRecognition::get_text_statisticts(
        books_directory+'/'+(bookid)+"/book.txt",
        alphabet,
        n_grams2,
        number_of_text_patches,
        max_book_length,
        n_grams_distances_amount
    );
    cout << "calculate_n_grams_correlation 3\n";

    unordered_map<string, double> n_grams_fr1;
    unordered_map<string, double> n_grams_fr2;
    unordered_map<string, double> n_grams_fr2_analog;

    for(unsigned long i=0;i<n_grams1.size();++i) {
        //cout << n_grams1[i] << ':'<<stats1.n_grams_frequencies[i]<<'|';
        n_grams_fr1[n_grams1[i]] = stats1.n_grams_frequencies[i];
    }
    for(unsigned long i=0;i<n_grams2.size();++i) n_grams_fr2[n_grams2[i]] = stats2.n_grams_frequencies[i];

    cout << "calculate_n_grams_correlation 4\n";
    for(unsigned long i=0;i<n_grams2.size();++i) {
        const string& n_gram2 = n_grams2[i];
        double fr = 1;
        for(int j=0;j<=(n_grams_size2-n_grams_size1);++j){
            //cout << j << ' '<< (j+n_grams_size1-1) << '|'<< n_gram2.substr(j, n_grams_size1) << '|' <<'\n';
            fr*=n_grams_fr1[n_gram2.substr(j, n_grams_size1)];
        }
        //cout << n_gram2 << ' '<< fr << ' '<< n_grams_fr2[n_grams2[i]]<<'\n';
        n_grams_fr2_analog[n_gram2] = fr;
    }

    cout << "calculate_n_grams_correlation 5\n";
    double A=0, B=0, C=0, D=0, E=0;
    for(unsigned long i=0;i<n_grams2.size();++i) {
        const string& n_gram2 = n_grams2[i];
        const double& x = n_grams_fr2[n_gram2];
        const double& y = n_grams_fr2_analog[n_gram2];
        //cout << x << ' '<< y << '\n' ;
        A += x*y;
        B+=x;
        C+=y;
        D+=pow(x,2);
        E+=pow(y,2);
    }
    cout << "calculate_n_grams_correlation 6\n";

    const unsigned long n = n_grams2.size();
    A /= n;
    B /= n;
    C /= n;
    D = sqrt(D/n - B*B);
    E = sqrt(E/n - C*C);

    if(D== 0 || E==0) return 0;
    return (A-B*C)/D/E;
}
double calculate_n_grams_correlation2(string bookid, unsigned long n_grams_size1, unsigned long n_grams_size2){
    long number_of_text_patches = 20;
    long max_book_length = 100000;
    long n_grams_distances_amount = 10000000;
    const string books_directory = "../../data/gutenberg/books";
    //const string alphabet = "abc";
    const string alphabet = "abcdefghijklmnopqrstuvwxyz";
    vector<string> n_grams1 = AuthorsRecognition::build_n_grams(alphabet, n_grams_size1);
    vector<string> n_grams2 = AuthorsRecognition::build_n_grams(alphabet, n_grams_size2);
    vector<string> n_grams3 = AuthorsRecognition::build_n_grams(alphabet, n_grams_size2-n_grams_size1);

    cout << "calculate_n_grams_correlation2 1\n";
    AuthorsRecognition::BookStatistics stats1 = AuthorsRecognition::get_text_statisticts(
        books_directory+'/'+(bookid)+"/book.txt",
        alphabet,
        n_grams1,
        number_of_text_patches,
        max_book_length,
        n_grams_distances_amount
    );
    cout << "calculate_n_grams_correlation2 2\n";
    AuthorsRecognition::BookStatistics stats2 = AuthorsRecognition::get_text_statisticts(
        books_directory+'/'+(bookid)+"/book.txt",
        alphabet,
        n_grams2,
        number_of_text_patches,
        max_book_length,
        n_grams_distances_amount
    );
    AuthorsRecognition::BookStatistics stats3 = AuthorsRecognition::get_text_statisticts(
        books_directory+'/'+(bookid)+"/book.txt",
        alphabet,
        n_grams3,
        number_of_text_patches,
        max_book_length,
        n_grams_distances_amount
    );
    cout << "calculate_n_grams_correlation2 3\n";

    unordered_map<string, double> n_grams_fr1;
    unordered_map<string, double> n_grams_fr2;
    unordered_map<string, double> n_grams_fr3;
    unordered_map<string, double> n_grams_fr2_analog;

    for(unsigned long i=0;i<n_grams1.size();++i) {
        //cout << n_grams1[i] << ':'<<stats1.n_grams_frequencies[i]<<'|';
        n_grams_fr1[n_grams1[i]] = stats1.n_grams_frequencies[i];
    }
    for(unsigned long i=0;i<n_grams2.size();++i) n_grams_fr2[n_grams2[i]] = stats2.n_grams_frequencies[i];
    for(unsigned long i=0;i<n_grams3.size();++i) n_grams_fr3[n_grams3[i]] = stats3.n_grams_frequencies[i];

    cout << "calculate_n_grams_correlation2 4\n";
    for(unsigned long i=0;i<n_grams2.size();++i) {
        const string& n_gram2 = n_grams2[i];
        double fr = 1;
        for(unsigned long j=0;j<=(n_grams_size2-n_grams_size1);++j){
            //cout << j << ' '<< (j+n_grams_size1-1) << '|'<< n_gram2.substr(j, n_grams_size1) << '|' <<'\n';
            fr*=n_grams_fr1[n_gram2.substr(j, n_grams_size1)];
        }
        //cout << n_gram2 << ' '<< fr << ' '<< n_grams_fr2[n_grams2[i]]<<'\n';
        n_grams_fr2_analog[n_gram2] = n_grams_fr1[n_gram2.substr(0, n_grams_size1)]*n_grams_fr3[n_gram2.substr(n_grams_size1, n_grams_size2-n_grams_size1)]
        +n_grams_fr3[n_gram2.substr(0, n_grams_size2-n_grams_size1)]*n_grams_fr1[n_gram2.substr(n_grams_size2-n_grams_size1, n_grams_size1)];
    }

    cout << "calculate_n_grams_correlation2 5\n";
    double A=0, B=0, C=0, D=0, E=0;
    for(unsigned long i=0;i<n_grams2.size();++i) {
        const string& n_gram2 = n_grams2[i];
        const double& x = n_grams_fr2[n_gram2];
        const double& y = n_grams_fr2_analog[n_gram2];
        //cout << x << ' '<< y << '\n' ;
        A += x*y;
        B+=x;
        C+=y;
        D+=pow(x,2);
        E+=pow(y,2);
    }
    cout << "calculate_n_grams_correlation2 6\n";

    const long n = n_grams2.size();
    A /= n;
    B /= n;
    C /= n;
    D = sqrt(D/n - B*B);
    E = sqrt(E/n - C*C);

    if(D== 0 || E==0) return 0;
    return (A-B*C)/D/E;
}
int main()
{
    /*double d1 = calculate_n_grams_correlation("100", 1, 3);
    double d2 = calculate_n_grams_correlation("100", 1, 2);
    double d3 = calculate_n_grams_correlation("100", 2, 3);
    double d34 = calculate_n_grams_correlation("100", 3, 4);
    double d4 = calculate_n_grams_correlation2("100", 1, 3);
    double d5 = calculate_n_grams_correlation2("100", 1, 2);
    double d6 = calculate_n_grams_correlation2("100", 2, 3);
    double d341 = calculate_n_grams_correlation2("100", 3, 4);
    cout << "calculate_n_grams_correlation() = "<<d1 <<' '<<' '<<d2 <<' '<<d3 << '\n';
    cout << "calculate_n_grams_correlation2() = "<<d4 <<' '<<' '<<d5 <<' '<<d6 << '\n';
    cout << "calculate_n_grams_correlation() = "<<d34 << '\n';
    cout << "calculate_n_grams_correlation() = "<<d341 << '\n';*/
    for(int i=0;i<1;i++)run();

    return 0;
}
