/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'

import { TourNavigation } from './tour_navigation'

import * as style from './rewards_tour.style'

interface Props {
  rewardsEnabled: boolean
  onDone: () => void
}

function basicStep (
  className: string,
  heading: React.ReactNode,
  text: React.ReactNode
) {
  return (
    <style.stepPanel className={`tour-step-${className}`}>
      <style.stepHeader>{heading}</style.stepHeader>
      <style.stepText>{text}</style.stepText>
      <style.stepGraphic className={`tour-graphic-${className}`} />
    </style.stepPanel>
  )
}

function stepWelcome () {
  return basicStep(
    'welcome',
    'Welcome to Brave Rewards!',
    (
      <>
        Brave private ads rewards you with tokens to support content creators
        unlike traditional ads, all while keeping your personal information
        private.
      </>
    ))
}

function stepAds () {
  return basicStep(
    'ads',
    'Where do ads show up?',
    (
      <>
        Brave private ads will appear as a normal notification. You control how
        often you see these ads in settings.
      </>
    ))
}

function stepSchedule () {
  return basicStep(
    'schedule',
    'When do you receive rewards',
    (
      <>
        Your earned tokens from Brave ads throughout the current month will arrive
        on the 5th of the next month.
      </>
    ))
}

function stepAC () {
  return basicStep(
    'ac',
    'Support creators on autopilot',
    (
      <>
        Creators are automatically rewarded with the earned tokens based on your
        browsing/engagement levels that we measure as ‘Attention’.
      </>
    ))
}

function stepTipping () {
  return basicStep(
    'tipping',
    'Say thank you with tips',
    (
      <>
        Tipping is a way to personally encourage and support content or creators
        that you love. Get tippin!
      </>
    ))
}

function stepRedeem () {
  return basicStep(
    'redeem',
    'What can you do with tokens?',
    (
      <>
        Tokens can be used beyond supporting creators. You can buy digital goods
        and giftcards with a cashback bonus.
      </>
    ))
}

function stepComplete () {
  return basicStep(
    'complete',
    (
      <>
        WOOOOHOOOOO!<br />You’re done.
      </>
    ),
    (
      <>
        By using Brave Rewards, you are protecting your privacy and helping make
        the web a better place for everyone. And that’s awesome!
      </>
    ))
}

const stepPanels = [
  stepWelcome,
  stepAds,
  stepSchedule,
  stepAC,
  stepTipping,
  stepRedeem,
  stepComplete
]

export function RewardsTour (props: Props) {
  const [currentStep, setCurrentStep] = React.useState(0)

  if (stepPanels.length === 0) {
    return null
  }

  const onSkip = () => {
    if (props.rewardsEnabled) {
      setCurrentStep(stepPanels.length - 1)
    } else {
      props.onDone()
    }
  }

  return (
    <style.root>
      {stepPanels[currentStep]()}
      <style.nav>
        <TourNavigation
          rewardsEnabled={props.rewardsEnabled}
          stepCount={stepPanels.length}
          currentStep={currentStep}
          onSelectStep={setCurrentStep}
          onDone={props.onDone}
          onSkip={onSkip}
        />
      </style.nav>
    </style.root>
  )
}
