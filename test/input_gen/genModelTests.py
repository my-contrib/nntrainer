#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0
##
# Copyright (C) 2020 Jihoon Lee <jhoon.it.lee@samsung.com>
#
# @file getModelTests.py
# @date 13 October 2020
# @brief Generate tc using KerasRecorder
# @author Jihoon lee <jhoon.it.lee@samsung.com>

import warnings

from .recorder import KerasRecorder

with warnings.catch_warnings():
    warnings.filterwarnings("ignore", category=FutureWarning)
    import numpy as np
    import tensorflow as tf
    from tensorflow.python import keras as K

if __name__ == "__main__":
    inp = K.Input(shape=(3, 3))
    a = K.layers.Dense(5)(inp)
    b = K.layers.Dense(5)(a)
    c = K.layers.Dense(10)(b)
    d = K.layers.Activation("softmax")(c)

    KerasRecorder(
        file_name="a.info",
        inputs=inp,
        outputs=[inp, a, b, c, d],
        input_shape=(3, 3),
        label_shape=(3, 10),
        loss_fn=tf.keras.losses.CategoricalCrossentropy(),
    ).run(2)
