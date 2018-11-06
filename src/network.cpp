#include "network.h"
#include "random.h"
#include <iostream>

void Network::resize(const size_t& n) {
    values.clear();
    for(size_t i(0); i < n; ++i) { values.push_back(RNG.normal()); }
}

bool Network::add_link(const size_t& i, const size_t& j) {
    if ( i > size()-1 or j > size()-1 or i == j) {return false;}

    for (const auto& a : links) {
      if (a.first == i and a.second == j) { return false; }
    }
    links.insert({ {i, j}, {j, i} });
    return true;
}


size_t Network::random_connect(const double& mean) {
    links.clear();
    size_t result(0);
    size_t a(0);      //pour itérer sur le tableau de noeuds aléatoires
    int deg(0);       //nombre de liens devant être faits
    bool link_not_added(true);          //true-> pas de lien ajouté; false-> lien ajouté
    std::vector<int> random_nodes(size());
    for (size_t i(0); i < size(); ++i) {                    //parcourir chaque noeud existant
        RNG.uniform_int(random_nodes, 0, size() - 1);       //distributtion aléatoire de noeuds à tester
        //deg = RNG.poisson(mean) - degree(i);
        deg = RNG.poisson(mean);
        for (int j (0); j < deg; ++j) {                     //créer "deg" liens
            while(a < size() and link_not_added) {
                if(add_link(i, random_nodes[a])) {
                    link_not_added = false;
                    ++result;
                }
                ++a;
            }
            a = 0;      //on repart à zéro pour itération du tab de noeuds aléatoires
            link_not_added = true;
        }
    }
    return result;
}


size_t Network::set_values(const std::vector<double>& new_values) {
    size_t s(size());
    size_t n(new_values.size());

    if (n < s) {
        for(size_t i(0); i < n; ++i) { values[i] = new_values[i]; }
        return n;
    } else if (n > s) {
        for(size_t i(0); i < s; ++i) { values[i] = new_values[i]; }
        return s;
    } else {
        values = new_values;
        return s;
    }
}


size_t Network::size() const {
    return values.size();
}


size_t Network::degree(const size_t & _n) const {
    return links.count(_n);
}


double Network::value(const size_t & _n) const {
    if (values.size() >= _n + 1)  {return values[_n];}
    return 0;                                                       //par défaut si pas de noeud numéro n
}


std::vector<double> Network::sorted_values() const {
    std::vector<double> v (values.begin(), values.end() );
    std::sort(v.begin(), v.end(), std::greater<double>() );         //greater<type> : opérateur > (ordre décroissant)
    return v;
}


std::vector<size_t> Network::neighbors(const size_t& i) const {
    std::vector<size_t> neighbors_;
    for(const auto& a : links) {
        if (a.first == i) { neighbors_.push_back(a.second); }
    }
    return neighbors_;
}
