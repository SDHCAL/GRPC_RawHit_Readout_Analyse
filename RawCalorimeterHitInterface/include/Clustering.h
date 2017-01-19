#ifndef SDHCAL_Clustering_HH
#define SDHCAL_Clustering_HH

#include <set>
#include <list>

namespace SDHCAL
{
  template <class T> 
    class Cluster : public std::set<const T*>
    {
    public:
    Cluster() : std::set<const T*>() {}
    Cluster(const T& obj) : std::set<const T*>() {add(obj);}
      void add(const T& obj) {this->insert(&obj);}
      void merge(const Cluster<T>& cl) {this->insert(cl.begin(),cl.end());}
    private:
    };
  
  
  template <class T,class iterT>
    void convertToClusterList(iterT inputFirst, iterT inputLast, std::list<Cluster<T> >& output)
  {
    for (iterT it=inputFirst; it != inputLast; ++it)
      output.push_back(Cluster<T>(*it));
  }
  
  //mergePred ia a function or functor implementing bool mergePred(const T&,const T&)
  template <class T,class mergePred>
    bool merge(const Cluster<T>& A, const Cluster<T>& B, mergePred pred)
  {
    for (typename std::set<const T*>::const_iterator itA=A.begin(); itA != A.end(); ++itA)
      for (typename std::set<const T*>::const_iterator itB=B.begin(); itB != B.end(); ++itB)
	if (pred(**itA,**itB)) return true;
    return false;
  }
  
  //this function is intended to take as input a list of cluster with each cluster made
  // of one element
  template <class T,class mergePred>
    void clusterize(std::list<Cluster<T> >& cl, mergePred pred)
  {
    typename std::list<Cluster<T> >::iterator itCurrentClusterBuild=cl.begin();
    
    while (itCurrentClusterBuild != cl.end())
      {
	typename std::list<Cluster<T> >::iterator itOtherCluster=itCurrentClusterBuild;
	++itOtherCluster;
	while (itOtherCluster != cl.end())
	  {
	    if (merge(*itCurrentClusterBuild,*itOtherCluster,pred))
	      {
		itCurrentClusterBuild->merge(*itOtherCluster);
		itOtherCluster=cl.erase(itOtherCluster);
	      }
	    else
	      ++itOtherCluster;
	  }
	++itCurrentClusterBuild;
      }
  }
}
//
//  Example of usage :
//  assuming f is a function bool f(const int &,const int &);
//  int tab[15]; 
//  //fill tab
//  std::list<Cluster<int> > clusters;
//  SDHCAL::convertToClusterList((int*) tab, tab+15, clusters);
//  SDHCAL::clusterize(clusters,f);
//  
//  clusters contains your clusters

#endif
