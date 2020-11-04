/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import styled from 'styled-components'

import batEcosystemImage from './assets/tour_bat_ecosystem.svg'
import braveAdsImage from './assets/tour_brave_ads.svg'
import batScheduleImage from './assets/tour_bat_schedule.svg'
import acImage from './assets/tour_ac.svg'
import tippingImage from './assets/tour_tipping.svg'
import redeemImage from './assets/tour_redeem.svg'
import completedImage from './assets/tour_completed.svg'

export const root = styled.div`
  font-family: var(--brave-font-heading);
  display: flex;
  flex-direction: column;
  height: 100%;
`

export const stepPanel = styled.div`
  flex: 1 0 auto;
  text-align: center;
  display: flex;
  flex-direction: column;
`

export const stepHeader = styled.div`
  font-weight: 600;
  font-size: 18px;
  line-height: 28px;
  color: var(--brave-palette-black);
`

export const stepSubheader = styled.div`
  margin-top: 3px;
  color: var(--brave-color-brandBat);
  font-size: 14px;
  line-height: 20px;
`

export const stepText = styled.div`
  margin-top: 8px;
  font-size: 14px;
  line-height: 22px;
  color: var(--brave-palette-neutral700);
`

export const stepGraphic = styled.div`
  flex: 1 1 0;
  margin: 10px 0;

  background-repeat: no-repeat;
  background-position: center;
  background-sizing: contain;

  &.tour-graphic-welcome {
    background-image: url('${batEcosystemImage}');
  }

  &.tour-graphic-ads {
    background-image: url('${braveAdsImage}');
  }

  &.tour-graphic-schedule {
    background-image: url('${batScheduleImage}');
  }

  &.tour-graphic-ac {
    background-image: url('${acImage}');
  }

  &.tour-graphic-tipping {
    background-image: url('${tippingImage}');
  }

  &.tour-graphic-redeem {
    background-image: url('${redeemImage}');
  }

  &.tour-graphic-complete {
    background-image: url('${completedImage}');
    background-size: 89px 89px;
  }
`

export const nav = styled.div`
  flex: 0 0 105px;
`
