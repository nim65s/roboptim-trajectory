// Copyright (C) 2009 by Thomas Moulard, AIST, CNRS, INRIA.
//
// This file is part of the roboptim.
//
// roboptim is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// roboptim is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with roboptim.  If not, see <http://www.gnu.org/licenses/>.

#ifndef ROBOPTIM_TRAJECTORY_VISUALIZATION_TRAJECTORY_HH
# define ROBOPTIM_TRAJECTORY_VISUALIZATION_TRAJECTORY_HH
# include <boost/format.hpp>

# include <roboptim/core/visualization/gnuplot-commands.hh>
# include <roboptim/trajectory/limit-speed.hh>

namespace roboptim
{
  namespace visualization
  {
    namespace gnuplot
    {
      /// \brief Plot a speed limit constraint.
      ///
      /// \return Gnuplot command
      template <unsigned dorder>
      Command plot_limitSpeed
      (const Trajectory<dorder>& trajectory,
       typename Trajectory<dorder>::value_type step = .01);

      namespace detail
      {
	template <typename T>
	struct PlotLimitSpeed
	{
	  PlotLimitSpeed (const T& trajectory, std::string& str)
	    : trajectory_ (trajectory),
	      str_ (str)
	  {}

	  void operator () (const Function::value_type& t)
	  {
	    using namespace boost::numeric::ublas;
	    using boost::format;

	    double tmax = Function::getUpperBound (trajectory_.timeRange ());

	    LimitSpeed<T> speedLimit (t / tmax * tMax, trajectory_);
	    Function::vector_t res = speedLimit (trajectory_.parameters ());
	    str_ += (format ("%1% %2%\n") % t % res[0]).str ();
	  }

	private:
	  const T& trajectory_;
	  std::string& str_;
	};
      } // end of namespace detail.

      template <unsigned dorder>
      Command plot_limitSpeed (const Trajectory<dorder>& trajectory,
			       typename Trajectory<dorder>::value_type step)
      {
	using namespace detail;
	Function::value_type min =
	  Function::getLowerBound (trajectory.timeRange ());
	Function::value_type max =
	  Function::getUpperBound (trajectory.timeRange ());
	Function::discreteInterval_t interval (min, max, step);

	std::string str = (boost::format ("plot '-' title '%1%' with line\n")
			   % trajectory.getName ()).str ();

	trajectory.foreach
	  (interval,
	   PlotLimitSpeed<Trajectory<dorder> > (trajectory, str));
	str += "e\n";
	return Command (str);
      }
    } // end of namespace gnuplot.
  } // end of namespace visualization.
} // end of namespace roboptim.

#endif //! ROBOPTIM_TRAJECTORY_VISUALIZATION_TRAJECTORY_HH
