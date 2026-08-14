# Seedance2AI API authentication and local-secret guide

- Status: historical, domain-bound operator guide for `seedance2ai.io`; later
  `.io` canaries do not create current transfer or spending authority
- Public-source review: 2026-08-08, with provider-identity correction on
  2026-08-10 (America/Los_Angeles)
- Provider transfer authorized: false
- Provider spending authorized: false

This guide explains how the account operator can create a key for the
independent `seedance2ai.io` service and keep it out of Git. It does not create
an account or key, inspect an authenticated account, change an account setting,
buy credits, test a credential over the network, or submit a generation job.
Possessing a credential is not authorization to transfer project material or
spend credits.

> **Provider identity correction, 2026-08-10:** `seedance2ai.io` and
> `seedance2ai.ai` are distinct independently operated services. This guide and
> the existing local key apply only to `.io`. Never send that key to `.ai` and
> never change the adapter hostname as a migration shortcut. The newly reviewed
> `.ai` site exposes different model names and credit rates, and no public video
> API/key contract was found. See `docs/development/sessions/explore_futures.20260811.md` for the
> comparison and the required account-domain preflight.

The availability of a user-controlled account is user-supplied context. Codex
has not signed in to or verified that account. The public provider pages were
reviewed without authentication.

## Service and evidence boundary

`seedance2ai.io` says that it is an independent platform operated by SixBryan
LLC and is not affiliated with ByteDance or the model developers named on the
site. The credential described here is therefore a key for the SixBryan LLC
platform, not a ByteDance Seed API key. Keep that distinction in filenames,
receipts, and user notices.

The following separates current public evidence from details that could not be
observed:

| Item | Evidence status on 2026-08-08 |
|---|---|
| A user-controlled account is available | User-supplied context; not authenticated or independently verified by Codex |
| Accounts with credits may create API keys, with no separate public API plan gate | Observed in the public provider FAQ |
| A new key is displayed once | Observed in the public provider quickstart |
| Requests use an HTTPS Bearer authorization header | Observed in the public provider documentation |
| The API base origin is `https://seedance2ai.io/api/v1` | Observed in the provider's public API text |
| Exact authenticated dashboard controls and button labels | UNAVAILABLE |
| Key scopes, expiration, prefix contract, per-account key limit, and MFA support | UNAVAILABLE |
| A quote, balance, or no-cost credential-validation endpoint | UNAVAILABLE in the public API documentation |
| An API key was created or tested for izzi | No |

The provider's examples call their shell variable `SEEDANCE_API_KEY`. Izzi uses
the more specific name `SEEDANCE2AI_API_KEY` to avoid confusing this independent
platform with other Seedance providers. An environment-variable name is a local
convention; the protocol requirement is the Bearer value in the HTTPS
`Authorization` header.

## Generate the key in the provider dashboard

These are operator actions in a browser, outside izzi:

1. Review the current provider pricing, terms, privacy policy, refund policy,
   and commercial-license status for the account's exact offering.
2. Open the provider's [API key dashboard](https://seedance2ai.io/app/api) and
   sign in to the adult account that will own the jobs and credit charges.
3. Confirm that this is the intended personal account. The public FAQ says API
   calls use the key owner's personal credit balance, including for members of
   a Team account.
4. Use the dashboard's key-creation control. The authenticated interface was
   not inspected, so this guide does not invent a button name, scope selector,
   or expiration option. If the dashboard offers scopes or expiration, select
   the narrowest and shortest settings appropriate for this project.
5. Copy the value when the dashboard displays it. The public quickstart says it
   is shown once.
6. Put it directly in an operating-system credential store, or use the private
   JSON fallback below. Do not paste it into chat, email, an issue, a commit
   message, a shell command, a screenshot, or a tracing receipt.

Generating a key does not authorize a provider request. Do not use a generation
call merely to find out whether the key works: the public API does not document
a no-cost validation endpoint, and generation requests can consume credits.

## Storage policy

An operating-system credential store is preferred. Give the entry a descriptive
service name such as `izzi / seedance2ai.io`, restrict it to the account
operator, and retrieve it only for the lifetime of an authorized client
process. If the credential store can inject an environment variable, inject it
as `SEEDANCE2AI_API_KEY` without printing the value.

This checkout's canonical on-disk fallback is a plain key file **outside the
repository tree**:

```text
${IZZI_PRIVATE_DIR:-$HOME/.config/izzi/private}/seedance2.key
```

The directory is mode `700` and the file mode `600`, owner-only. One key file
per provider lives there (`seedance2.key` today; future providers would use
their own names). A second key for the PRO account (or the resolution-cap
test) lives beside it as `seedance2_pro.key`. Select it with
`SEEDANCE2AI_KEY_NAME=seedance2_pro.key`; the submit/poll scripts default to
`seedance2.key`. The private JSON skeleton below remains as a compatibility
fallback only.

Never put the key in:

- a ten-scene authoring document, resolved plan, provider run-state document,
  title-transition configuration, audio-negotiation document, or manifest;
- Git, GitHub, CI variables that have not been separately approved, build logs,
  trace JSONL, receipts, test fixtures, terminal transcripts, or crash reports;
- chat, email, screenshots, screen recordings, or shared clipboard history;
- a command-line argument, URL, idempotency key, output filename, browser
  bundle, or WebAssembly module.

The fallback below is deliberately outside the creative and execution JSON
contracts. It is local secret input only. A Git ignore rule is an accident
guard, not access control or encryption.

The canonical key file is never placed under the repository tree. The
`scripts/check-tracked-secrets.py` tripwire (Make target
`check-tracked-secrets`) fails the build if any tracked file contains key
material or a key-like path, and it runs as part of `make check`.

## Private JSON fallback

This checkout includes an editable skeleton at:

```text
build/private/seedance2-auth.local.json
```

This checkout's anchored `/build/` ignore rule covers the file and any editor
backup kept in that directory. The initial content is inert:

```json
{
  "schema_version": 1,
  "provider": "seedance2ai.io",
  "credential": {
    "scheme": "bearer",
    "api_key": "REPLACE_ME"
  },
  "provider_transfer_authorized": false
}
```

Replace only `REPLACE_ME` with the value shown by the provider. Keep
`provider_transfer_authorized` false. That field documents the boundary; editing
it is not an accepted authorization mechanism, and a future client must reject
the file if it is not false.

Do not add a configurable API origin to the secret file. A future server-side
client must pin `https://seedance2ai.io/api/v1`, require normal TLS validation,
and refuse to forward the Bearer header across an origin-changing redirect.
Keeping the destination out of the editable credential file prevents a changed
local endpoint from silently receiving the key.

Before and after editing, restrict the directory and file:

```sh
umask 077
chmod 700 build/private
chmod 600 build/private/seedance2-auth.local.json
```

Configure the editor so swap, backup, and recovery files remain under the
ignored `build/private` directory. Do not inspect the completed file with
`cat`, `jq .`, a command that writes shell tracing, or any tool that copies its
contents into a transcript.

## Verify locally without printing the key

Run these checks from the repository root. They validate structure, mode, and
Git disposition without displaying the credential:

```sh
auth_file=build/private/seedance2-auth.local.json

test "$(stat -c '%a' "$auth_file")" = 600

jq -e '
  (keys | sort) ==
    ["credential", "provider", "provider_transfer_authorized", "schema_version"] and
  .schema_version == 1 and
  .provider == "seedance2ai.io" and
  (.credential | keys | sort) == ["api_key", "scheme"] and
  .credential.scheme == "bearer" and
  (.credential.api_key |
    type == "string" and length > 0 and . != "REPLACE_ME") and
  .provider_transfer_authorized == false
' "$auth_file" >/dev/null

git check-ignore -v -- "$auth_file"

if git ls-files --error-unmatch -- "$auth_file" >/dev/null 2>&1; then
  echo "error: the private authentication file is tracked" >&2
  exit 1
fi

git status --short --ignored --untracked-files=all -- "$auth_file"
unset auth_file
```

`git check-ignore` must identify the anchored `/build/` rule. The final status
must report an ignored path (with `!!`), never a staged or ordinary untracked
file. Do not use `git add -f`. Do not run a diff, checksum command whose output
will be published, or secret scanner in a mode that echoes matching lines.

## Load for a future authorized client

The current izzi examples do not implement provider submission. Once a client
exists and a separately traced provider-transfer and spending authorization is
active, load the canonical key file into the process environment without
printing it:

```sh
set +x
private_dir=${IZZI_PRIVATE_DIR:-${HOME}/.config/izzi/private}
key_name=${SEEDANCE2AI_KEY_NAME:-seedance2.key}
SEEDANCE2AI_API_KEY="$(tr -d '\r\n' < "${private_dir}/${key_name}")"
export SEEDANCE2AI_API_KEY
```

Keep shell tracing disabled. The client should read the environment variable at
the last possible moment, construct the Bearer header internally, redact the
entire header and provider output URL queries, and never include the key in an
exception. Environment variables can be visible to child processes and some
same-user diagnostics, which is another reason to prefer a credential store and
keep the variable's lifetime short.

After the authorized client exits, clear the shell state:

```sh
unset SEEDANCE2AI_API_KEY
unset private_dir
unset key_name
```

Removing the variable does not revoke the provider credential. It only removes
this shell's copy.

## Request and retry boundary

Every future paid `POST` should use a stable, non-secret `Idempotency-Key` and a
persisted hash of the exact request body. A retry after an uncertain network
result must reuse both the same idempotency key and the same body. A changed body
is a new creative attempt and requires a new idempotency key and whatever
authorization the run policy requires.

The idempotency key must not contain the API key, account email, collaborator
email, prompt text, or private asset name. A credential does not authorize a
canary, retry, top-up, subscription change, public asset upload, or generation.
Those remain separate consent and budget decisions under the
[Seedance account integration proposal](seedance.md).

## Rotation, revocation, and suspected exposure

If the key may have appeared in Git, a log, chat, email, screenshot, shell
history, process argument, or another person's possession:

1. Stop provider work; do not wait for evidence of misuse.
2. Revoke the key in the provider dashboard. The exact authenticated revocation
   controls remain UNAVAILABLE to this guide.
3. Unset `SEEDANCE2AI_API_KEY` and remove or replace the local JSON value.
4. Search the authorized local artifacts for the exposure without copying the
   key into a search command or report. Redact any retained evidence.
5. Generate a replacement only after the exposure boundary is understood.
6. Update the credential store or private JSON, restore mode `600`, and rerun
   only the non-printing local checks above.

Revoke a project-specific key when it is no longer needed. Never email a key to
a collaborator; collaborators exchange creative JSON and reviewed assets, not
account credentials.

## Acceptance for this setup step

This setup step is complete when the Markdown guide exists, the placeholder
JSON parses, the local file mode is `600`, Git classifies it as ignored and not
tracked, and the placeholder remains `REPLACE_ME`. Acceptance performs no DNS,
HTTP, login, credential, balance, quote, upload, generation, polling, download,
or other provider operation. Provider transfer and spending remain false.

## Current provider sources

- [Public API guide](https://www.seedance2ai.io/developers)
- [Raw public API text](https://www.seedance2ai.io/llms.txt)
- [API key dashboard; authentication required](https://seedance2ai.io/app/api)
- [Terms of Service and API key security terms](https://www.seedance2ai.io/terms-of-use)
- [Privacy policy](https://www.seedance2ai.io/privacy-policy)

These pages can change. Recheck them before implementing or using the provider
adapter, and preserve observations from the authenticated account as
operator-supplied evidence rather than retroactively treating them as public
documentation.
