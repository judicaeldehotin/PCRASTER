The Modflow 2005 executable
^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. versionchanged:: 4.2
   Previously, Modflow 2000 was used.

PCRaster Modflow executable
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The Modflow executable (mf2005) shipped with our PCRaster Modflow extension is based on the USGS version 1.12.00:

Harbaugh, A.W., Langevin, C.D., Hughes, J.D., Niswonger, R.N., and Konikow, L. F., 2017, MODFLOW-2005 version 1.12.00, the U.S. Geological Survey modular groundwater model: U.S. Geological Survey Software Release, 03 February 2017, http://dx.doi.org/10.5066/F7RF5S7G

You can obtain the original source code from the USGS |usgs_modflow|, or browse our |pcr_gh|.

We used gfortran and gcc version 7.2 to compile the code on Linux and macOS.
On Windows the executable distributed by the USGS will be used.


.. |pcr_gh| raw:: html

   <a href="https://github.com/pcraster/pcraster/tree/master/source/modflow/mf2005" target="_blank"> GitHub page</a>

.. |usgs_modflow| raw:: html

   <a href="https://water.usgs.gov/ogw/modflow/mf2005.html" target="_blank">Modflow 2005 page</a>



USGS Disclaimers and Notices
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Software and related material (data and (or) documentation), contained in or furnished in connection with a software distribution, are made available by the U.S. Geological Survey (USGS) to be used in the public interest and in the advancement of science. You may, without any fee or cost, use, copy, modify, or distribute this software, and any derivative works thereof, and its supporting documentation, subject to the following restrictions and understandings.

If you distribute copies or modifications of the software and related material, make sure the recipients receive a copy of this notice and receive or can get a copy of the original distribution. If the software and (or) related material are modified and distributed, it must be made clear that the recipients do not have the original and they must be informed of the extent of the modifications. For example, modified files must include a prominent notice stating the modifications made, the author of the modifications, and the date the modifications were made. This restriction is necessary to guard against problems introduced in the software by others, reflecting negatively on the reputation of the USGS.

The software is public property and you therefore have the right to the source code, if desired.

You may charge fees for distribution, warranties, and services provided in connection with the software or derivative works thereof. The name USGS can be used in any advertising or publicity to endorse or promote any products or commercial entity using this software if specific written permission is obtained from the USGS.

The user agrees to appropriately acknowledge the authors and the USGS in publications that result from the use of this software or in products that include this software in whole or in part.

Because the software and related material are free (other than nominal materials and handling fees) and provided "as is," the authors, the USGS, and the United States Government have made no warranty, express or implied, as to accuracy or completeness and are not obligated to provide the user with any support, consulting, training or assistance of any kind with regard to the use, operation, and performance of this software nor to provide the user with any updates, revisions, new versions or "bug fixes".

The user assumes all risk for any damages whatsoever resulting from loss of use, data, or profits arising in connection with the access, use, quality, or performance of this software.

(|usgs_notice|, Accessed 3 Jan 2018)

.. |usgs_notice| raw:: html

   <a href="http://water.usgs.gov/software/help/notice/" target="_blank">USGS</a>
