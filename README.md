# Alias
[![Latest release](https://img.shields.io/github/v/release/aliascash/alias-wallet?label=Release&color=%2300bf00)](https://github.com/aliascash/alias-wallet/releases/latest)
[![Latest develop build](https://img.shields.io/github/v/release/aliascash/alias-wallet?include_prereleases&label=Develop-Build)](https://github.com/aliascash/alias-wallet/releases)
[![Discord](https://img.shields.io/discord/426769724018524161?logo=discord)](https://discord.gg/ckkrb8m)
[![Reddit](https://img.shields.io/badge/reddit-join-orange?logo=reddit)](https://www.reddit.com/r/AliasCash/)
[![Build Status Master](https://github.com/aliascash/alias-wallet/actions/workflows/build-master.yml/badge.svg)](https://github.com/aliascash/alias-wallet/actions)
[![Build Status Develop](https://github.com/aliascash/alias-wallet/actions/workflows/build-develop.yml/badge.svg)](https://github.com/aliascash/alias-wallet/actions)

Alias is a Secure Proof-of-Stake (PoSv3) Network with Anonymous Transaction Capability.

Alias utilizes a range of proven cryptographic techniques to achieve un-linkable,
un-traceable and anonymous transactions on its underlaying blockchain and also protects
the users identity by running all the network nodes as Tor hidden services.

# Licensing

- SPDX-FileCopyrightText: © 2020 Alias Developers
- SPDX-FileCopyrightText: © 2016 SpectreCoin Developers
- SPDX-FileCopyrightText: © 2014 ShadowCoin Developers
- SPDX-FileCopyrightText: © 2014 BlackCoin Developers
- SPDX-FileCopyrightText: © 2013 NovaCoin Developers
- SPDX-FileCopyrightText: © 2011 PPCoin Developers
- SPDX-FileCopyrightText: © 2009 Bitcoin Developers

SPDX-License-Identifier: MIT

# Social
- Visit our website [Alias](https://alias.cash/) (ALIAS)
- Please join us on our [Discord](https://discord.gg/ckkrb8m) server
- Read the latest [News](https://alias.cash/news/)
- Visit our thread at [BitcoinTalk](https://bitcointalk.org/index.php?topic=2103301.0)

## Key Privacy Technology

Anonymous token creation: Through the use of dual key stealth technology, Alias provides
the ability to generate ‘private coins’ by consuming 'public coins'. Private coins can then be
sent through an implementation of ring signatures based on the Cryptonote protocol
to eliminate any transaction history. The wallet offers the opportunity to transfer your
balance between public coins and private coins. We are currently working
on improving this technology for better functionality and privacy.

Built in Tor: The Alias software offers a full integration of [Tor](https://www.torproject.org/)
so that the Alias client runs exclusively as a Tor hidden service using a .onion
address to connect to other clients in the network. Your real IP address is
therefore protected at all times.

## Basic Coin Specs V3/V4
<table>
<tr><td>Algo</td><td>PoSv3/PoAS</td></tr>
<tr><td>Block Time</td><td>96 Seconds</td></tr>
<tr><td>Difficulty Retargeting</td><td>Every Block (Moving average of last 24 hours)</td></tr>
<tr><td>Initial Coin Supply</td><td>20,000,000 ALIAS</td></tr>
<tr><td>Funding Coin Supply</td><td>3,000,000 ALIAS</td></tr>
<tr><td>Max Coin Supply (PoS Phase)</td><td>3 private or 2 public coins reward per block, depending on what you're staking</td></tr>
<tr><td>Min Stake Maturity</td><td>450 blocks (~12 hours)</td></tr>
<tr><td>Min ALIAS Confirmations</td><td>10 blocks</td></tr>
<tr><td>Base Fee</td><td>0.0001 ALIAS</td></tr>
<tr><td>Max Anon Output</td><td>1000</td></tr>
<tr><td>Ring Size</td><td>fix 10</td></tr>
</table>

## Further documentation

For detailed description and further documentation have a look at our [Wiki](https://github.com/aliascash/documentation/wiki).

 Like to build from source? Go straight to these pages there:
* Build on and for [Windows](https://github.com/aliascash/documentation/wiki/Build-Windows)
* Build on and for [Mac](https://github.com/aliascash/documentation/wiki/Build-Mac)
* Separate pages for different flavours of Linux are also available there

#### UI development

The following files where maintained on the separate Git repository
[alias-wallet-ui](https://github.com/aliascash/alias-wallet-ui):
* src/qt/res/assets/*
* src/qt/res/index.html
* spectre.qrc

**Do not modify them here!**
