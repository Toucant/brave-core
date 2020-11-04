/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import styled from 'styled-components'

export const root = styled.div`
  button.main-action {
    color: var(--brave-palette-white);
    background: var(--brave-color-brandBat);
    border: none;
    padding: 5px 20px;
    border-radius: 30px;
    font-weight: 600;
    font-size: 13px;
    line-height: 19px;

    .icon {
      height: 12px;
      vertical-align: middle;
      margin: 0 -6px 2px 2px;
    }

    &:active {
      background: var(--brave-color-brandBatActive);
    }
  }
`

export const stepLinks = styled.div`
  text-align: center;
  display: flex;
  justify-content: center;

  > button {
    margin: 0 2px;
    padding: 0 3px;
    height: 8px;
    background: none;
    border: none;
    cursor: pointer;
    outline: none;

    &::before {
      content: ' ';
      display: block;
      height: 8px;
      width: 8px;
      border-radius: 50%;
      background: var(--brave-palette-grey200);
    }

    &:hover::before {
      background: var(--brave-color-brandBatActive);
    }

    &.selected::before {
      background: var(--brave-color-brandBat);
    }
  }
`

export const actions = styled.div`
  margin-top: 30px;
  display: flex;
  justify-content: space-between;
  align-items: center;

  &[data-step='0'] {
    margin-top: 16px;
  }
`

export const start = styled.div`
  flex: 1 0 auto;
  text-align: center;

  button.main-action {
    padding: 10px 26px;
  }
`

export const skip = styled.div`
  margin-top: 16px;

  button {
    color: var(--brave-palette-neutral600);
    background: none;
    border: none;
    font-weight: 600;
    font-size: 13px;
    line-height: 19px;
    cursor: pointer;
    outline: none;
    font-weight: 600;
    font-size: 13px;
    line-height: 19px;
  }
`

export const forward = styled.div``

export const back = styled.div`
  button {
    border: none;
    background: none;
    margin: 0;
    padding: 5px;
    cursor: pointer;
    color: var(--brave-color-brandBat);

    .icon {
      height: 12px;
      vertical-align: middle;
      margin: 0 2px 2px 0;
    }
  }
`
