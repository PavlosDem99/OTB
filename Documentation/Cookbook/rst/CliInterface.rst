Command-line interface
======================

OTB ships with more than 100 ready to use applications for remote sensing tasks.
They usually expose existing processing functions from the underlying C++
library, or integrate them into high level pipelines. OTB applications allow the user 
to:

-  Combine two or more functions from the Orfeo ToolBox,

-  Provide a high level interface to handle: input and output data, 
   definition of parameters and communication with the user.

OTB applications can be launched in different ways, and accessed from different
entry points. While the framework can be extended, the Orfeo ToolBox ships with the following:

-  A command-line launcher, to call applications from the terminal,

-  A graphical launcher, with an auto-generated QT interface, providing
   ergonomic parameters setting, display of documentation, and progress
   reporting,

-  A SWIG interface, which means that any application can be loaded,
   set-up and executed into a high-level language such as Python
   for instance.

-  A :doc:`QGISInterface` based on the providers is built on top of the
   SWIG/Python interface and is available with seamless integration
   within `QGIS <http://www.qgis.org/>`_.

The complete list of applications is described in the Chapter :ref:`apprefdoc`.

All standard applications share the same implementation and automatically expose
generated interfaces.
However they are accessed in a slightly different way: the command-line interface is prefixed by ``otbcli_``, while the Qt interface is prefixed by
``otbgui_``. For instance, calling ``otbcli_DynamicConvert`` will launch the
command-line interface of the DynamicConvert application, while
``otbgui_DynamicConvert`` will launch the GUI.

Command-line launcher
---------------------

The command-line application launcher loads an application
plugin, allows for its parameters to be set, and can then be executed from the command line.
Launching the ``otbApplicationLauncherCommandLine`` without any arguments provided,
results in the following help to be displayed:

::

    $ otbApplicationLauncherCommandLine
    Usage: ./otbApplicationLauncherCommandLine module_name [MODULEPATH] [arguments]

The ``module_name`` parameter corresponds to the application name. The
``[MODULEPATH]`` argument is optional and allows the path to the shared library 
(or plugin) corresponding to the ``module_name`` to be passed to the launcher.

It is also possible to set this path with the environment variable
``OTB_APPLICATION_PATH``, making the ``[MODULEPATH]`` optional. This
variable is checked by default when no ``[MODULEPATH]`` argument is
given. When using multiple paths in ``OTB_APPLICATION_PATH``, one must
ensure that the standard path separator of the target system is used, which
is ``:`` on Unix and ``;`` on Windows.

An error in the application name (i.e. in parameter ``module_name``)
will make the ``otbApplicationLauncherCommandLine`` list the name of
all applications found in the available path (either ``[MODULEPATH]``
and/or ``OTB_APPLICATION_PATH``).

To ease the use of the applications, and to avoid extensive
environment customizations; ready-to-use scripts are provided by the OTB
installation to launch each application. They take care of adding the
standard application installation path to the ``OTB_APPLICATION_PATH``
environment variable.

These scripts are named ``otbcli_<ApplicationName>`` and do not need any
path settings. For example, you can start the Orthorectification
application with the script called ``otbcli_Orthorectification``.

Launching an application without parameters, or with incomplete parameters, will cause the
launcher to display a summary of the parameters. This summary will display the minimum set
of parameters that are required to execute the application. Here is an
example based on the OrthoRectification application:

::

    $ otbcli_OrthoRectification
    ERROR: Waiting for at least one parameter.


    This is the Ortho-rectification (OrthoRectification) application, version 6.7.0

    This application allows ortho-rectifying optical and radar images from supported sensors.
    Complete documentation: https://www.orfeo-toolbox.org/CookBook/Applications/app_OrthoRectification.html or -help

    Parameters:
            -io                      <group>          Input and output data
    MISSING -io.in                   <string>         Input Image  (mandatory)
    MISSING -io.out                  <string> [pixel] Output Image  [pixel=uint8/uint16/int16/uint32/int32/float/double/cint16/cint32/cfloat/cdouble] (default value is float) (mandatory)
            -map                     <string>         Map Projection [utm/lambert2/lambert93/wgs/epsg] (mandatory, default value is utm)
            -map.utm.zone            <int32>          Zone number  (mandatory, default value is 31)
            -map.utm.northhem        <boolean>        Northern Hemisphere  (mandatory, default value is false)
            -map.epsg.code           <int32>          EPSG Code  (mandatory, default value is 4326)
            -outputs                 <group>          Output Image Grid
            -outputs.mode            <string>         Parameters estimation modes [auto/autosize/autospacing/outputroi/orthofit] (mandatory, default value is auto)
    MISSING -outputs.ulx             <float>          Upper Left X  (mandatory)
    MISSING -outputs.uly             <float>          Upper Left Y  (mandatory)
    MISSING -outputs.sizex           <int32>          Size X  (mandatory)
    MISSING -outputs.sizey           <int32>          Size Y  (mandatory)
    MISSING -outputs.spacingx        <float>          Pixel Size X  (mandatory)
    MISSING -outputs.spacingy        <float>          Pixel Size Y  (mandatory)
            -outputs.lrx             <float>          Lower right X  (optional, off by default)
            -outputs.lry             <float>          Lower right Y  (optional, off by default)
            -outputs.ortho           <string>         Model ortho-image  (optional, off by default)
            -outputs.isotropic       <boolean>        Force isotropic spacing by default  (mandatory, default value is true)
            -outputs.default         <float>          Default pixel value  (optional, off by default, default value is 0)
            -elev                    <group>          Elevation management
            -elev.dem                <string>         DEM directory  (optional, off by default)
            -elev.geoid              <string>         Geoid File  (optional, off by default)
            -elev.default            <float>          Default elevation  (mandatory, default value is 0)
            -interpolator            <string>         Interpolation [bco/nn/linear] (mandatory, default value is bco)
            -interpolator.bco.radius <int32>          Radius for bicubic interpolation  (mandatory, default value is 2)
            -opt                     <group>          Speed optimization parameters
            -opt.rpc                 <int32>          RPC modeling (points per axis)  (optional, off by default, default value is 10)
            -opt.ram                 <int32>          Available RAM (MB)  (optional, off by default, default value is 128)
            -opt.gridspacing         <float>          Resampling grid spacing  (optional, off by default, default value is 4)
            -progress                <boolean>        Report progress
            -help                    <string list>    Display long help (empty list), or help for given parameters keys

    Use -help param1 [... paramN] to see detailed documentation of those parameters.

    Examples:
    otbcli_OrthoRectification -io.in QB_TOULOUSE_MUL_Extract_500_500.tif -io.out QB_Toulouse_ortho.tif



For a detailed description of the application behaviour and parameters,
please check the application reference documentation presented
in chapter :ref:`apprefdoc`  or follow the ``DOCUMENTATION``
hyperlink provided in the output of ``otbApplicationLauncherCommandLine``.
Parameters are passed to the application using the parameter key (which
might include one or several ``.`` character), prefixed by a ``-``.
Command-line examples are provided in the chapter :ref:`apprefdoc`.

Load and save parameters to XML
-------------------------------

OTB application parameters can be saved and loaded to
an XML file using the special ``-inxml`` and ``-outxml`` parameters.
Those parameters are available in all applications.
For example:

::

    otbcli_BandMath -il input_image_1 input_image_2
                    -exp "abs(im1b1 - im2b1)"
                    -out output_image
                    -outxml saved_applications_parameters.xml

Then, you can run the applications with the same parameters using the
output XML file previously saved. For this, you have to use the inxml
parameter:

::

    otbcli_BandMath -inxml saved_applications_parameters.xml

Note that you can also add additional parameters to the command line at the same
time

::

    otbcli_BandMath -inxml saved_applications_parameters.xml
                    -exp "(im1b1 - im2b1)"

In this case it will use as mathematical expression “(im1b1 - im2b1)”
instead of “abs(im1b1 - im2b1)”.

Parallel execution with MPI
---------------------------

Provided that Orfeo ToolBox has been built with MPI and SPTW modules
activated, it is possible to use MPI for massive parallel computation
and writing of an output image. A simple call to ``mpirun`` before the
command-line activates this behaviour, with the following logic. MPI
writing is only triggered if:

- OTB is built with MPI and SPTW,

- The number of MPI processes is greater than 1,

- The output filename is ``.tif`` or ``.vrt``


In this case, the output image will be divided into several tiles
according to the number of MPI processes specified to the ``mpirun``
command, and all tiles will be computed in parallel.

If the output filename extension is ``.tif``, tiles will be written in
parallel to a single Tiff file using SPTW (Simple Parallel Tiff Writer).

If the output filename extension is ``.vrt``, each tile will be
written to a separate Tiff file, and a global VRT_ file will be written.

.. _VRT: http://gdal.org/gdal_vrttut.html

Here is an example of MPI call on a cluster::

  $ mpirun -np $nb_procs --hostfile $PBS_NODEFILE  \
    otbcli_BundleToPerfectSensor \
    -inp $ROOT/IMG_PHR1A_P_001/IMG_PHR1A_P_201605260427149_ORT_1792732101-001_R1C1.JP2 \
    -inxs $ROOT/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201605260427149_ORT_1792732101-002_R1C1.JP2 \
    -out $ROOT/pxs.tif uint16 -ram 1024

    ------------ JOB INFO 1043196.tu-adm01 -------------

    JOBID           : 1043196.tu-adm01
    USER            : michelj
    GROUP           : ctsiap
    JOB NAME        : OTB_mpi
    SESSION         : 631249
    RES REQSTED     : mem=1575000mb,ncpus=560,place=free,walltime=04:00:00
    RES USED        : cpupercent=1553,cput=00:56:12,mem=4784872kb,ncpus=560,vmem=18558416kb,
    walltime=00:04:35
    BILLING         : 42:46:40 (ncpus x walltime)
    QUEUE           : t72h
    ACCOUNT         : null
    JOB EXIT CODE   : 0

  ------------ END JOB INFO 1043196.tu-adm01 ---------

One can see that the registration and pan-sharpening of the
panchromatic and multi-spectral bands of a Pleiades image has been split
between 560 CPUs and only took 56 seconds to complete.

Note that this MPI parallel invocation of applications is only
available for command-line calls to OTB applications, and only for
images output parameters.
