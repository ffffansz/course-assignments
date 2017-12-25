# !usr/bin/env python
# -*- coding:utf-8 -*-

from BN_Util import *

if __name__ == '__main__':
    PatientAge = Node('PatientAge', ['PatientAge'])
    CTScanResult = Node('CTScanResult', ['CTScanResult'])
    MRIScanResult = Node('MRIScanResult', ['MRIScanResult'])
    Anticoagulants = Node('Anticoagulants', ['Anticoagulants'])
    StrokeType = Node('StrokeType', ['StrokeType', 'CTScanResult', 'MRIScanResult'])
    Mortality = Node('Mortality', ['Mortality', 'StrokeType', 'Anticoagulants'])
    Disability = Node('Disability', ['Disability', 'StrokeType', 'PatientAge'])

    PatientAge.setCpt(CondiPrTable.getInitializedCpt(
        [
            ['0-30', .10],
            ['31-65', .30],
            ['65+', .60]
        ],
        ['PatientAge']
    ))

    CTScanResult.setCpt(CondiPrTable.getInitializedCpt(
        [
            ['Ischemic Stroke', 0.7],
            ['Hemmorraghic Stroke', 0.3]
        ],
        ['CTScanResult']
    ))

    MRIScanResult.setCpt(CondiPrTable.getInitializedCpt(
        [
            ['Ischemic Stroke', 0.7],
            ['Hemmorraghic Stroke', 0.3]
        ],
        ['MRIScanResult']
    ))

    Anticoagulants.setCpt(CondiPrTable.getInitializedCpt(
        [
            ['Used', 0.5],
            ['Not used', 0.5]
        ],
        ['Anticoagulants']
    ))

    StrokeType.setCpt(CondiPrTable.getInitializedCpt(
        [
            ['Ischemic Stroke', 'Ischemic Stroke', 'Ischemic Stroke', 0.8],
            ['Ischemic Stroke', 'Hemmorraghic Stroke', 'Ischemic Stroke', 0.5],
            ['Hemmorraghic Stroke', 'Ischemic Stroke', 'Ischemic Stroke', 0.5],
            ['Hemmorraghic Stroke', 'Hemmorraghic Stroke', 'Ischemic Stroke', 0],

            ['Ischemic Stroke', 'Ischemic Stroke', 'Hemmorraghic Stroke', 0],
            ['Ischemic Stroke', 'Hemmorraghic Stroke', 'Hemmorraghic Stroke', 0.4],
            ['Hemmorraghic Stroke', 'Ischemic Stroke', 'Hemmorraghic Stroke', 0.4],
            ['Hemmorraghic Stroke', 'Hemmorraghic Stroke', 'Hemmorraghic Stroke', 0.9],

            ['Ischemic Stroke', 'Ischemic Stroke', 'Stroke Mimic', 0.2],
            ['Ischemic Stroke', 'Hemmorraghic Stroke', 'Stroke Mimic', 0.1],
            ['Hemmorraghic Stroke', 'Ischemic Stroke', 'Stroke Mimic', 0.1],
            ['Hemmorraghic Stroke', 'Hemmorraghic Stroke', 'Stroke Mimic', 0.1]
        ],
        ['CTScanResult', 'MRIScanResult', 'StrokeType']
    ))

    Mortality.setCpt(CondiPrTable.getInitializedCpt(
        [
            ['Ischemic Stroke', 'Used', 'False', 0.28],
            ['Hemmorraghic Stroke', 'Used', 'False', 0.99],
            ['Stroke Mimic', 'Used', 'False', 0.1],
            ['Ischemic Stroke', 'Not used', 'False', 0.56],
            ['Hemmorraghic Stroke', 'Not used', 'False', 0.58],
            ['Stroke Mimic', 'Not used', 'False', 0.05],

            ['Ischemic Stroke',  'Used', 'True', 0.72],
            ['Hemmorraghic Stroke', 'Used', 'True', 0.01],
            ['Stroke Mimic', 'Used', 'True', 0.9],
            ['Ischemic Stroke',  'Not used', 'True', 0.44],
            ['Hemmorraghic Stroke', 'Not used', 'True', 0.42],
            ['Stroke Mimic', 'Not used', 'True', 0.95]
        ],
        ['StrokeType', 'Anticoagulants', 'Mortality']
    ))

    Disability.setCpt(CondiPrTable.getInitializedCpt(
        [
            ['Ischemic Stroke',     '0-30',  'Negligible', 0.80],
            ['Hemmorraghic Stroke', '0-30',  'Negligible', 0.70],
            ['Stroke Mimic',        '0-30',  'Negligible', 0.9],
            ['Ischemic Stroke',     '31-65', 'Negligible', 0.60],
            ['Hemmorraghic Stroke', '31-65', 'Negligible', 0.50],
            ['Stroke Mimic',        '31-65', 'Negligible', 0.4],
            ['Ischemic Stroke',     '65+',   'Negligible', 0.30],
            ['Hemmorraghic Stroke', '65+',   'Negligible', 0.20],
            ['Stroke Mimic',        '65+',   'Negligible', 0.1],

            ['Ischemic Stroke',     '0-30',  'Moderate', 0.1],
            ['Hemmorraghic Stroke', '0-30',  'Moderate', 0.2],
            ['Stroke Mimic',        '0-30',  'Moderate', 0.05],
            ['Ischemic Stroke',     '31-65', 'Moderate', 0.3],
            ['Hemmorraghic Stroke', '31-65', 'Moderate', 0.4],
            ['Stroke Mimic',        '31-65', 'Moderate', 0.3],
            ['Ischemic Stroke',     '65+',   'Moderate', 0.4],
            ['Hemmorraghic Stroke', '65+',   'Moderate', 0.2],
            ['Stroke Mimic',        '65+',   'Moderate', 0.1],

            ['Ischemic Stroke',     '0-30',  'Severe', 0.1],
            ['Hemmorraghic Stroke', '0-30',  'Severe', 0.1],
            ['Stroke Mimic',        '0-30',  'Severe', 0.05],
            ['Ischemic Stroke',     '31-65', 'Severe', 0.1],
            ['Hemmorraghic Stroke', '31-65', 'Severe', 0.1],
            ['Stroke Mimic',        '31-65', 'Severe', 0.3],
            ['Ischemic Stroke',     '65+',   'Severe', 0.3],
            ['Hemmorraghic Stroke', '65+',   'Severe', 0.6],
            ['Stroke Mimic',        '65+',   'Severe', 0.8]
        ],
        ['StrokeType', 'PatientAge', 'Disability']
    ))

    factors = list()
    factors.append(PatientAge)
    factors.append(CTScanResult)
    factors.append(MRIScanResult)
    factors.append(Anticoagulants)
    factors.append(StrokeType)
    factors.append(Mortality)
    factors.append(Disability)

    model = BayesNet(factors)
    print()