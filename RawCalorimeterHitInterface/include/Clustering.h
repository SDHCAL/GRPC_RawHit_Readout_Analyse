#ifndef SDHCAL_Clustering_old_HH
#define SDHCAL_Clustering_old_HH

#include "Clustering_bis.h"

namespace SDHCAL
{
  
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
    void clusterize(std::list<Cluster<T> >& cl, typename std::list<Cluster<T> >::iterator start, mergePred pred)
  {
    typename std::list<Cluster<T> >::iterator itCurrentClusterBuild=start;
    
    while (itCurrentClusterBuild != cl.end())
      {
	typename std::list<Cluster<T> >::iterator itOtherCluster=itCurrentClusterBuild;
	++itOtherCluster;

	while (itOtherCluster != cl.end())
	  {
	    if (merge(*itCurrentClusterBuild,*itOtherCluster,pred))
	      {
		if (std::distance(itCurrentClusterBuild,itOtherCluster)>1)
		  {
		    typename std::list<Cluster<T> >::iterator itInsert=itCurrentClusterBuild;
		    ++itInsert;
		    itInsert=cl.insert(itInsert,*itOtherCluster);
		    cl.erase(itOtherCluster);
		    itOtherCluster=itInsert;
		    clusterize(cl,itOtherCluster,pred);
		  }  
		itCurrentClusterBuild->merge(*itOtherCluster);
		itOtherCluster=cl.erase(itOtherCluster);
	      }
	    else
	      ++itOtherCluster;
	  }
	++itCurrentClusterBuild;
      }
  }

  template <class T,class mergePred>
    void clusterize(std::list<Cluster<T> >& cl, mergePred pred)
  {
    clusterize(cl,cl.begin(),pred);
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
