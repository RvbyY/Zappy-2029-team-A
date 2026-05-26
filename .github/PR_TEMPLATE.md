# Pull Request Template

## Summary

- Brief description: What the PR changes (one to three sentences).
- Why: Short justification for the change and its scope.

## Type of change

- Bugfix
- New feature
- Performance improvement
- Refactor
- Documentation
- CI / tooling
- Chore

## Related issues

- Fixes: (e.g. `Fixes rgb(0, 0, 0)`) or related issue links.

## How to test

Provide clear, step-by-step instructions to verify the change locally and in CI. Include commands to run and expected output.

Example:

1. Start the server: `make start`
2. Run end-to-end test: `make test-e2e`
3. Manually verify: open the GUI and check feature X

## Checklist (author)

* I updated/added documentation where needed.
* I added tests that prove the fix/feature works (unit/integration).
* I ran linters and formatting (`make lint`, `make format`).
* CI is passing for this branch.
* I added migration notes if DB/compatibility changes apply.

## Checklist (reviewer)

* Code is readable and follows project conventions.
* Tests cover new behavior and edge cases.
* No obvious performance regressions.
* No sensitive data or credentials included.

## Impact / Notes

Mention breaking changes, database migrations, deployment steps, or runtime config changes.

## Screenshots / Logs (if applicable)

Attach screenshots, GIFs, or log excerpts to help reviewers understand UI/behavioral changes.

---

**Thank you for your contribution! Please fill the sections above and assign reviewers as needed.**

