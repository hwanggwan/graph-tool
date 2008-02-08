// graph-tool -- a general graph modification and manipulation thingy
//
// Copyright (C) 2007  Tiago de Paula Peixoto <tiago@forked.de>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "graph_filtering.hh"

#include <boost/lambda/bind.hpp>

#include "graph.hh"
#include "histogram.hh"
#include "graph_selectors.hh"
#include "graph_properties.hh"

#include "graph_correlations.hh"

using namespace std;
using namespace boost;
using namespace boost::lambda;
using namespace graph_tool;

// implementations spread across different compile units to minimize memory
// usage during compilation
void graph_correlations_imp1(const GraphInterface& g, hist2d_t& hist,
                             boost::any deg1, boost::any deg2,
                             boost::any weight);
void graph_correlations_imp2(const GraphInterface& g, hist2d_t& hist,
                             boost::any deg1, boost::any deg2,
                             boost::any weight);
void graph_correlations_imp3(const GraphInterface& g, hist2d_t& hist,
                             boost::any deg1, boost::any deg2,
                             boost::any weight);

typedef ConstantPropertyMap<int,GraphInterface::edge_t> cweight_map_t;

hist2d_t
GraphInterface::GetVertexCorrelationHistogram(GraphInterface::deg_t deg1,
                                              GraphInterface::deg_t deg2,
                                              string weight) const
{
    hist2d_t hist;

    try
    {        
        any weight_prop;
        if (weight != "")
            weight_prop = prop(weight, _edge_index, _properties);
        else
            weight_prop = cweight_map_t(1);

        run_action<>()(*this, get_correlation_histogram<hist2d_t>(hist), 
                       all_selectors(), all_selectors(), 
                       mpl::vector<cweight_map_t>())
            (degree_selector(deg1, _properties),
             degree_selector(deg2, _properties), weight);
        graph_correlations_imp1(*this, hist, degree_selector(deg1, _properties),
                                degree_selector(deg2, _properties), weight);
        graph_correlations_imp2(*this, hist, degree_selector(deg1, _properties),
                                degree_selector(deg2, _properties), weight);
        graph_correlations_imp3(*this, hist, degree_selector(deg1, _properties),
                                degree_selector(deg2, _properties), weight);

    }
    catch (dynamic_get_failure &e)
    {
        throw GraphException("error getting scalar property: " +
                             string(e.what()));
    }

    return hist;
}
