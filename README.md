Pineal
======

![screenshot](http://i.imgur.com/3lBNKan.png)

Graphic synthesis engine, designed for live performances.

Like in an audio (subtractive) synthesizer you have some basic oscillators and
then chains of effects and filters, here there are primitive entities and chains
of transformations.


Example
-------

```hy
(background (rgb 0))


(draw (change (cube)
              (no-fill)
              (line-width 2)
              (color (rgb 0 1 0))
              (rotate-y (time 0.5))
              ))


```

Build
-----
- Install OpenFrameworks and make sure to have `projectGenerator` in your PATH.
- TODO: Linux/Debian build dependencies
- Change `build.cfg` with your OF path, and the Python paths depending from
  your system.
- Install Python dependencies with `pip install -r requirements.txt`.
- Run `scripts/build.sh`.


Quick Start
-----------
After building run:
```
scripts/watch.sh examples/first-example.pn`
```
it will start the server (`bin/pineal`) and watch `first-example.pn` for
changes.


Documentation
-------------
References are available on [Readthedocs](http://pineal.readthedocs.org/en/latest/)


License
-------
This project is released under the terms of [GNU AGPL](http://www.gnu.org/licenses/agpl-3.0.html), see [LICENSE](LICENSE) file for details

