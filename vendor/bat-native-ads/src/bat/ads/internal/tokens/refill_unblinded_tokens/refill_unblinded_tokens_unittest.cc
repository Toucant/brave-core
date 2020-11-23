/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/tokens/refill_unblinded_tokens/refill_unblinded_tokens.h"

#include <memory>

#include "net/http/http_status_code.h"
#include "bat/ads/internal/account/wallet/wallet.h"
#include "bat/ads/internal/account/wallet/wallet_info.h"
#include "bat/ads/internal/privacy/unblinded_tokens/unblinded_tokens.h"
#include "bat/ads/internal/tokens/refill_unblinded_tokens/get_signed_tokens_url_request_builder.h"
#include "bat/ads/internal/tokens/refill_unblinded_tokens/refill_unblinded_tokens_delegate_mock.h"
#include "bat/ads/internal/tokens/refill_unblinded_tokens/request_signed_tokens_url_request_builder.h"
#include "bat/ads/internal/unittest_base.h"
#include "bat/ads/internal/unittest_util.h"

// npm run test -- brave_unit_tests --filter=BatAds*

using ::testing::NiceMock;

namespace ads {

class BatAdsRefillUnblindedTokensTest : public UnitTestBase {
 protected:
  BatAdsRefillUnblindedTokensTest()
      : refill_unblinded_tokens_(std::make_unique<RefillUnblindedTokens>()),
        refill_unblinded_tokens_delegate_mock_(std::make_unique<
            NiceMock<RefillUnblindedTokensDelegateMock>>()) {
    refill_unblinded_tokens_->set_delegate(
        refill_unblinded_tokens_delegate_mock_.get());
  }

  ~BatAdsRefillUnblindedTokensTest() override = default;

  privacy::UnblindedTokens* get_unblinded_tokens() {
    return ConfirmationsState::Get()->get_unblinded_tokens();
  }

  WalletInfo GetWallet() {
    Wallet wallet;
    wallet.Set("27a39b2f-9b2e-4eb0-bbb2-2f84447496e7",
        "x5uBvgI5MTTVY6sjGv65e9EHr8v7i+UxkFB9qVc5fP0=");

    return wallet.Get();
  }

  std::unique_ptr<RefillUnblindedTokens> refill_unblinded_tokens_;
  std::unique_ptr<RefillUnblindedTokensDelegateMock>
      refill_unblinded_tokens_delegate_mock_;
};

TEST_F(BatAdsRefillUnblindedTokensTest,
    RefillUnblindedTokens) {
  // Arrange
  const URLEndpoints endpoints = {
    {
      // Request signed tokens
      R"(/v1/confirmation/token/27a39b2f-9b2e-4eb0-bbb2-2f84447496e7)", {
        {
          net::HTTP_CREATED, R"(
            {
              "nonce": "2252d421-bc40-45f2-8cfe-6959566ed480"
            }
          )"
        }
      }
    },
    {
      // Get signed tokens
      R"(/v1/confirmation/token/27a39b2f-9b2e-4eb0-bbb2-2f84447496e7?nonce=2252d421-bc40-45f2-8cfe-6959566ed480)", {
        {
          net::HTTP_OK, R"(
            {
              "batchProof": "dZCaxFcE0H6uQSO+q01pNxyYajwaCodqRJuCB5qTTQJe2T7tpM6T1z8Sxl37ndlXRVLgfhs/Mu6KEGzYE9XhAg==",
              "signedTokens": [
                "LJZWvz0xWR0vy7W8YyadiinArF5/wchmfd9lY6RO51s=",
                "2v/DdaoYGM62lZlqUTJDSc32AzLD70fIAiJ0uAH52EU=",
                "+DFsV4f2EwupZuchtTOPI0qfsayMYZTsE2YE5TxTK1U=",
                "ciEgtdjrCeZk+H4YZtUxWRRA7oUQBk+qKpvra+iIh0Y=",
                "yJR+iAUlRG31+lx96pe7o0DU7SEeJtdyKNmeNskCF0E=",
                "qqgItEEDfsuPLxDflrtAH/656ohgTWjEpJFcULkLXVQ=",
                "TOsHmd44NVccbhau5b5aMuKrh7n8wqcgIoxYAMqkYx8=",
                "4Ml59vLKwAOsn0B0bq1Q+tv/rpjBmDfeaEAv2ymsqH0=",
                "2tUGTd0gvCFGGXf6mMAjIoorPgLHciGBM77dlucGy1A=",
                "nk5hVRSViFJ5v4ko8jRxK6omOd3sabs+S0/aYF+651A=",
                "dNUjM9lC5CmvmCZ8mtwd5IpGhBme+mqEApdI6cAdMl4=",
                "mJLYHUu4i+sJ2jgpqcDhGcPltIXXdlR5L2JfpG6rF28=",
                "PK1EAnVsHsx52huE6YYitXml6wkgdRBBb/l+QGGInU8=",
                "fKPIs2SLeybMbQ1ceJt5+tmaWrx4CmWQMaMzWc3btRI=",
                "pEiw1RJsjRDkTqg4hxKS5cNO7TNP3FP9VJUyjZ5iUS0=",
                "9kpOCc3ewE5xwqAhn15B9stDpP1cCrMr2nXlIx6dM0Y=",
                "sO06IaLiSAiZl1A6fOfTJt8pNMTGM9DJlGEtYfc9H3w=",
                "OEEs6vDrk2eIwiwpwhp0ncMp1ucU5ar5kF4hn3NHaGU=",
                "vN7G7wqRAtg/m7iGd/DL4lPfTTQ3kFcNHNdibQko6xc=",
                "5NcVc4oqNw3hJJHsLrolWL3/ltAkOq+MSBHY1a7K3R8=",
                "Pldqk4NnkxE4aQasrBsvNmK6DvLidegFnBHjt3dnXUc=",
                "3qpLSK3VmmQrkRhRPL6n9C7cb83ssw+ohwCGyyJ2MF8=",
                "8PZ44RM8v6LoorROAoCYaaeoaBTqDZsU/tZdMVHyhAk=",
                "HhDq9bcyt75PucAL1SfHwtEVbOunsRky1hYpS4kQXUU=",
                "qA2JtWo1cxX3hbPOlLNwuqm3Zy7sKMmPeF3+S62NEkk=",
                "OPshRZUV/2zbsXdMdk6ecY2pGajCqXrvfgmOpqKbbDM=",
                "rMObNdrA3dG1aGfyUJJ/6zm9XFVz/srXfGqIikGBXyQ=",
                "lp1jKE5Zx+ZPbWy4vnktX5iuYNsgodeulvZbHm5B02I=",
                "KiEmqCxtQbWr2jA0AM8zIz+4ZFVia1TJxQeGSFp6vnc=",
                "/LeSlFv55dDAYfI6jalHy+dQaBQfj+Z6RI1vjIB43Uw=",
                "gkWhBBR1V8NaRhfdHhBK+GMvUSKREUkzenu5ZvfrClE=",
                "pv1NDV+AsyzgCM7/pNGBbXLTKqgwEFpfmpa6L1cVFFU=",
                "tKcSH2pPHywsL9bikVltXoYn7fVpg9fMV+ulD8IN4Hw=",
                "rqZM91vpauPsCak11k+L2htLk27zKGXxmXpkTbXPGgk=",
                "CHN+2EUrdVbsEryzQrA10DV9INKykIW+YPPrs457LDs=",
                "xpB9aCSDiGEMQ/pibSdvjxFdzUnfDl78wA+OhgYC9ig=",
                "IiBhyIpsZX0cb4u01rnwv05PEFVi8L4pSZM6OzJMaBM=",
                "FiOKipXpXGZ6WfgfAKmfe/eybfrJs5YOx4w9MMDPC1w=",
                "tKtO+AM5BUFra261HuySiwi3Jvz2weJT1/l0vwu6KEY=",
                "bMw56eGmu+dvfdTrciF8QiGWaOkragdmR/I7l05gZUs=",
                "oHU/WNNmwfTVBEJi/oLdlfpI9BsGDxEcMkkEt91omEY=",
                "tEV+vLONEgE6KHDoAW/pKBY1+SJes+T2I9b/mVZJuhY=",
                "tuD9j3j3pwnkgxzDcvpaKgxCOtil7TUGQqeEMt6inXg=",
                "9sCctFX9AzwZ3nQ36/gFHdtdjYEMVGnsc+vGQoKJu2g=",
                "1jsKtkWmz2Y/rOFGjNDyS2NJYnVKXuquQGnXrAHqR3s=",
                "WHOe52t8vLq9IPYnnIixNVTJeew//TxCQ5bOQrVYYDU=",
                "DhzgSl7l+wKe01QLjyVsycOJrODpSuNyQRCFYV6XJx4=",
                "WEA7dyBRnVFZP8g7tM90TbhnUsYN/P/xdr/jFv7XezQ=",
                "8LCTm9+so2IWdm8EugzohgRgW19KljEHI8jXMF7s2h8=",
                "ElFNFb6ld6C8WP4y+7sv+HeOFRCrb46yTmnuHY2YIHE="
              ],
              "publicKey": "crDVI1R6xHQZ4D9cQu4muVM5MaaM1QcOT4It8Y/CYlw="
            }
          )"
        }
      }
    }
  };

  MockUrlRequest(ads_client_mock_, endpoints);

  // Act
  EXPECT_CALL(*refill_unblinded_tokens_delegate_mock_,
      OnDidRefillUnblindedTokens()).Times(1);

  EXPECT_CALL(*refill_unblinded_tokens_delegate_mock_,
      OnFailedToRefillUnblindedTokens()).Times(0);

  EXPECT_CALL(*refill_unblinded_tokens_delegate_mock_,
      OnDidRetryRefillingUnblindedTokens()).Times(0);

  const WalletInfo wallet = GetWallet();
  refill_unblinded_tokens_->MaybeRefill(wallet);

  // Assert
  EXPECT_EQ(50, get_unblinded_tokens()->Count());
}

// Retry
// Invalid wallet
// Should refill
// Invalid catalog issuers
// Valid catalog issuers but mismatching public key

}  // namespace ads
