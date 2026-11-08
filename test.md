# Showcase Test Report — updated 9 November 2026

## Scope and evidence

This report combines:

- a live-site Chrome test pass supplied by the tester; and
- reproducible local checks run on 8 and 9 November 2026; and
- a live cache audit supplied by the tester on 9 November 2026.

It does not claim that password-dependent flows, real-phone behaviour, production database integration, or staff moderation on production were tested. Those items are listed as outstanding.

## Result summary

| Area | Result | Evidence |
|---|---|---|
| Public visitor journey | Pass | Live Chrome test pass |
| Guest review submission, duplicate recovery, receipt and withdrawal | Pass | Live Chrome/API test pass |
| Input validation, permissions, privacy and cache policy | Pass | Live Chrome/API test pass |
| Offline packs and offline drafts | Pass in Chrome; real-phone test outstanding | Live Chrome test pass |
| Backend static checks and unit tests | Pass | Local run: 8 Nov |
| Public frontend build and unit tests | Pass | Local run: 8 Nov |
| Staff and ops builds/tests | Pass | Local run: 8 Nov |
| Backend dependency audit and tracked-file secret scan | Pass | Local run: 8 Nov |
| Frontend dependency audit | Attention required | Four low-severity, transitive findings |
| Public-read caching and offline cache boundaries | Pass, with a Vercel 304 header defect fixed in source | Live cache audit; local regression checks on 9 Nov |

## Findings to resolve or hide before the showcase

| Priority | Finding | Risk / demo effect | Recommended action |
|---|---|---|---|
| P0 | Two test reviews are visible in the public feed: **“Online media check”** by **“Media Check”** on Bandarban and **“iuniuhibuyhb”** on Cox’s Bazar. | Public demo data looks unfinished. | Hide or remove these test reviews before the presentation. |
| P0 | The first test review, ID `4625336d-c64d-4621-b8c7-ecb1ef7aaae1`, contains XSS text in its title and is pending moderation. | Do not publish it on production. | Reject it, or publish only on staging/local to confirm that it renders as literal text. |
| P1 | The receipt secret field is a password input; Chrome autofill placed a saved email in the Review ID field. | Users can submit the wrong data and lose confidence in the receipt flow. | Change the receipt secret to a normal text input and set `autocomplete="off"` (and appropriate field-specific autocomplete attributes on both receipt inputs). Re-test in Chrome. |
| P1 | A staff link in the live public app points to `localhost:5175`. | Broken link during demo; could confuse users. | Set the staff dashboard URL in the Vercel/public-app environment, redeploy, and verify it opens the intended hosted dashboard. If the staff dashboard is not part of the demo, hide the link. |
| P1 | One live API request returned `503`; retry succeeded. Backend notes identify a small shared database connection cap as an open risk. | A repeat during the demo could interrupt public pages. | Keep a recorded fallback/demo path ready. Before presenting, inspect production connection-pool usage and deploy settings; avoid opening many app tabs or dashboards simultaneously. |
| P1 | A signed-in superadmin without completed TOTP receives `403 MFA_REQUIRED` when opening their own member-review detail and edit pages. | Superadmins cannot perform ordinary member review actions until two-step verification is set up; this blocked full live XSS coverage. | Decide whether this is intentional. If not, separate ordinary owner-read routes from privileged staff routes; otherwise use a normal-member test account for member-review testing. |
| P1 — fixed in source, deployment/retest pending | Vercel’s external-origin rewrite replaced the backend’s `public, max-age=60` with `public, max-age=0, must-revalidate` on proxied `304 Not Modified` responses. | After the first revalidation, browser copies became immediately stale, causing unnecessary API/database requests and increasing the chance of a 503 under load. | Added targeted Vercel rewrite caching and a Vercel CDN TTL for public reads. Deploy backend and frontend, then confirm a proxied `304` retains browser `max-age=60`. |
| P2 | Language toggle and several sidebar icons have no accessible names. | Screen-reader users cannot identify controls. | Add visible text or `aria-label` values and verify with browser accessibility inspection. |
| P2 | Signed-in members see guest-oriented review wording. | Misleading product state. | Render receipt/guest wording only for guests; show member-specific submission wording for signed-in users. |
| P2 | Offline-pack **Update** gives no visible feedback. | User cannot tell whether an update started, succeeded or failed. | Show progress plus a success/error toast or inline status. |
| P2 | Unknown app routes fall through to the landing page. | A broken shared link looks silently valid. | Add a route-level Not Found page; destination-specific invalid links already behave correctly. |

## Live-site checks that passed

### Visitor journey

- Bangla/English preference and light/dark theme work and persist after reload.
- Guest welcome shows the guest tag.
- Bandarban and Cox’s Bazar guides show bilingual content, advisory severity/state, source links, verification dates and working emergency phone links.
- Source links open safely in a new tab.
- Unknown and malformed destination links show a friendly destination-not-found state.

### Reviews and security

- Valid guest submission passed the robot check, returned a receipt and remained non-public while pending.
- Retrying after a deliberately lost response returned the same review and an “already submitted” outcome; no duplicate was created.
- Receipt status and withdrawal with confirmation worked.
- Client and server rejected invalid ratings, short notes, future visit months, missing fields and unexpected fields.
- Signed-out account access and normal-member moderation/admin access were denied.
- Production ignored the development staff header and returned `STAFF_AUTH_NOT_CONFIGURED` for staff features.
- Random receipts and valid receipts combined with the wrong review ID returned the same non-revealing mismatch result.
- Public responses and errors exposed no receipt tokens, user IDs, IP addresses, moderation history, stack traces or SQL details.
- Malformed cursors and identifiers were rejected safely.
- State-changing, receipt and staff requests were not cacheable; public reads used the intended 60-second cache.
- The app could not be embedded in another page.

### CSRF protection — 9 November

Live browser testing confirmed CSRF protection for cookie-authenticated writes:

| Test | Result |
|---|---|
| New member review with no, wrong or empty token | `403 CSRF_INVALID` |
| Same request with a valid token | Passed CSRF; malformed/empty review then correctly returned `422` validation failure |
| Previously issued token after a newer token | Accepted; token rotation does not invalidate older valid tokens |
| Review edit/withdraw, appeals, verification, notification settings and sign-out without a token | `403 CSRF_INVALID` |
| Form-encoded and `text/plain` forged request shapes without a token | `403` |
| Cross-site page attempting to read `/auth/csrf` or `/auth/session` through Vercel or Railway | Browser blocked access, so an attacker could not obtain a token |
| Cross-site hidden forged sign-out request with cookies | Did not sign the browser out |
| Admin routes using the superadmin session without completed TOTP | `403 MFA_REQUIRED` |

One response-ordering note: an empty review report returns a field-validation error before CSRF is evaluated. A complete report without a token is refused, so this does not permit a state change or expose a CSRF bypass.

The browser cannot forge an `Origin` header directly; the cross-site browser test above is the relevant real-world check and was blocked.

### XSS handling — 9 November

The tester submitted a member review using these harmless rendering probes: an image event-handler title, script and SVG event-handler text, a `javascript:` link and a Markdown-style `javascript:` link.

- On **My reviews**, the title appeared as literal text; no image, script, event handler or executable link was created.
- The XSS test review was withdrawn afterward and will not reach moderation.
- Public-feed and staff-moderation rendering are **not yet verified**. The live account is a superadmin without TOTP, so own-review detail/edit pages return `403 MFA_REQUIRED`, and production staff publishing is intentionally unavailable.

Complete the remaining XSS check on local/staging with a moderator account: publish the harmless test record, inspect the public feed and staff queue, confirm literal rendering, then hide/remove the test record.

### Offline

- The PWA was installed and active in Chrome.
- Saving, updating and deleting an offline pack worked; deletion required confirmation.
- Offline cache inspection found public guide/review data only, not sessions or receipts.
- Review drafts survived reload; simulated offline state disabled submission and showed an offline message.

### Cache and service-worker audit — 9 November

The live audit confirmed the intended cache separation:

- Public destination list/detail, review feed, review parameters, pack manifest and pack freshness responses use `public, max-age=60`.
- Destination list, detail, pack manifest and pack freshness support ETag revalidation and return `304 Not Modified` when unchanged.
- Session, health, errors, guest-review writes and receipt-status writes use `no-store`.
- Hashed JS/CSS uses `max-age=31536000, immutable`; HTML/page routes use `max-age=0, must-revalidate`; the service worker and manifest use `no-cache`.
- The current service worker was active, controlled the page, precached 44 files, and did not leave an update waiting.
- Offline API cache inspection found only successful public destination/review reads. It contained no session, receipt, account or `no-store` response.
- Service-worker caching uses a four-second network timeout, accepts only `200` responses, expires entries after seven days, and refuses `no-store` or signed-in responses.
- A Bandarban offline pack matched the server manifest (version 2) and was still valid until 15 November.
- Immediately after the Dhaka midnight rollover, cached guides carried the prior date, while the server generated a new ETag/date. A re-fetch replaced the cached copy, so advisory state was not retained as current after the date changed.

#### Vercel 304 cache-header defect and fix

The backend returned the correct 60-second cache header directly, but Vercel’s `/api` external-origin rewrite returned `public, max-age=0, must-revalidate` on proxied `304` responses. This did not make data incorrect, but it defeated the browser's 60-second freshness window after the first revalidation.

The source fix has two deliberately narrow parts:

1. `backend/app/destination_api.py` now sends `Vercel-CDN-Cache-Control: max-age=60` alongside the existing browser header, `Cache-Control: public, max-age=60`, for public-read helpers only. Vercel consumes the targeted directive while the browser keeps the normal 60-second header.
2. `frontend/vercel.json` explicitly enables external-origin rewrite caching only for public destination, pack and review-parameter routes. It does not set a blanket cache header, so backend `no-store` errors and all private routes remain protected.

This requires a backend deployment and a Vercel deployment. The required retest is:

1. Request a public destination through `/api`, record its ETag and `Cache-Control`.
2. Repeat with `If-None-Match` after deployment.
3. Confirm the proxied `304` has `Cache-Control: public, max-age=60` and that an unknown destination still has `Cache-Control: no-store`.

## Local automated checks run on 8–9 November 2026

### Backend

```text
uv run ruff check .                         PASS
uv run mypy                                 PASS (81 source files)
uv run pytest -m "not integration" -q       PASS (390 passed, 1 skipped)
uvx pip-audit ...                           PASS (no known vulnerabilities)
python3 .github/scripts/check_committed_secrets.py
                                             PASS
```

The test run emitted existing dependency deprecation warnings from FastAPI/Starlette and SlowAPI, plus a test-only JWT HMAC key-length warning. They did not fail the suite but should be tracked for dependency maintenance.

### Public frontend

```text
npm ci                                      PASS
npm run build                               PASS
npm test                                    PASS (192 tests on 9 Nov)
```

The build’s isolation check passed: the public `dist` bundle contains no staff/ops API endpoint references or ops token references.

The local machine used Node `23.10.0`; Vitest warned that it officially supports Node `22.12+`, `24`, or `26+`. The build and tests still passed, while CI is configured to use Node 22.

`npm audit --audit-level=moderate` reported **four low-severity transitive vulnerabilities**:

- dependency path: `vite-plugin-pwa` → `workbox-build` → `@rollup/plugin-node-resolve` → `deepmerge`;
- advisory: prototype poisoning in `deepmerge`;
- the suggested automatic fix is breaking (`vite-plugin-pwa@0.2.1`).

Do not use `npm audit fix --force` immediately before the showcase. Create a follow-up upgrade/test task.

### Separate staff and operations applications

```text
staff: npm run build                         PASS
staff: npm test                              PASS (46 tests)
staff: npm audit --audit-level=moderate      PASS (0 vulnerabilities)

ops: npm run build                           PASS
ops: npm test                                PASS (19 tests)
ops: npm audit --audit-level=moderate        PASS (0 vulnerabilities)
```

### Cache-fix regression checks — 9 November

```text
backend: Ruff and strict MyPy                         PASS
backend: destination/pack/parameter targeted suite   PASS (61 tests)
frontend: vercel.json JSON parse                      PASS
frontend: production build                            PASS
frontend: unit tests                                  PASS (192 tests)
```

The new backend unit test verifies that the browser and Vercel cache TTLs remain aligned. The Vercel response itself cannot be verified until both deployments are live.

## Tests intentionally not run

| Test | Why it was not run | Owner / next action |
|---|---|---|
| PostgreSQL integration tests, migrations and schema check | They require `TEST_DATABASE_URL`; running database-writing tests against the available local database could alter a non-test environment. | Run only against a disposable database whose name ends in `_test`. |
| Docker image build | Not run in this local pass. | Run the CI Docker build before release when Docker is available. |
| Registration, email verification, password reset and session expiry | Requires an authorised test account/password and mailbox access. | Product owner/tester. |
| Staff publish/hide/restore lifecycle | Production intentionally has staff features unconfigured. | Test on local or staging with approved staff accounts. |
| Visual XSS confirmation | The XSS-title review is pending and should not be published publicly. | Publish the test record only on staging/local, verify literal rendering, then remove/reject it. |
| Real-phone 360–390 px, installation and airplane-mode verification | Chrome test environment is not a physical phone. | Test on a real Android/iPhone. Confirm an unsaved-but-previously-viewed destination does not misleadingly appear as a saved pack. |
| Accessibility screen-reader pass | Only labels were visually/structurally identified. | Test VoiceOver/TalkBack or NVDA after labels are added. |

## Showcase gate

Proceed after the two public test reviews and pending XSS test record are removed/rejected, the broken staff link is fixed or hidden, and the deployed Vercel `304` cache-header retest passes. If the 503 risk is not resolved, have a short recorded offline/PWA backup ready and avoid parallel dashboard tabs during the demonstration.
