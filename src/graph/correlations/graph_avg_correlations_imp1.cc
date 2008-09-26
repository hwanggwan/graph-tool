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


void graph_avg_correlations_imp1(const GraphInterface& g, python::object& avg,
                                 python::object& dev,
                                 python::object& ret_bins,
                                 boost::any deg1, boost::any deg2,
                                 boost::any weight,
                                 const vector<long double>& bins)
{
    typedef DynamicPropertyMapWrap<long double, GraphInterface::edge_t>
        wrapped_weight_t;
    run_action<>()(g, get_avg_correlation<GetNeighboursPairs>
                   (avg, dev, bins, ret_bins),
                   all_selectors(), all_selectors(),
                   mpl::vector<wrapped_weight_t>())
        (deg1, deg2, weight);
}

