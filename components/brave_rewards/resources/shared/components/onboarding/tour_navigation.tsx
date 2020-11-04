/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'

import { CaretIcon } from './icons/caret_icon'

import * as style from './tour_navigation.style'

interface Props {
  stepCount: number
  currentStep: number
  rewardsEnabled: boolean
  onSelectStep: (step: number) => void
  onSkip: () => void
  onDone: () => void
}

export function TourNavigation (props: Props) {
  if (props.currentStep < 0 ||
      props.currentStep >= props.stepCount ||
      props.stepCount <= 0) {
    return null
  }

  function stepCallback (step: number) {
    return () => {
      if (step !== props.currentStep &&
          step >= 0 &&
          step < props.stepCount) {
        props.onSelectStep(step)
      }
    }
  }

  function stepActions () {
    if (props.currentStep === 0) {
      const skipClick = () => props.onSkip()
      return (
        <style.start>
          <button className='main-action' onClick={stepCallback(1)}>
            Let’s take a quick tour <CaretIcon direction='right' />
          </button>
          <style.skip>
            <button onClick={skipClick}>
              {props.rewardsEnabled ? 'Skip for now' : 'Skip'}
            </button>
          </style.skip>
        </style.start>
      )
    }

    const isLast = props.currentStep === props.stepCount - 1

    const onContinue = isLast
      ? () => props.onDone()
      : stepCallback(props.currentStep + 1)

    return (
      <>
        <style.back>
          <button onClick={stepCallback(props.currentStep - 1)}>
            <CaretIcon direction='left' />Go Back
          </button>
        </style.back>
        <style.forward>
          <button className='main-action' onClick={onContinue}>
            {
              isLast
                ? <>Done</>
                : <>Continue<CaretIcon direction='right' /></>
            }
          </button>
        </style.forward>
      </>
    )
  }

  return (
    <style.root>
      <style.stepLinks>
        {
          Array(props.stepCount).fill(null).map((_, i) =>
            <button
              key={i}
              className={i === props.currentStep ? 'selected' : ''}
              onClick={stepCallback(i)}
            />
          )
        }
      </style.stepLinks>
      <style.actions data-step={props.currentStep}>
        {stepActions()}
      </style.actions>
    </style.root>
  )
}
