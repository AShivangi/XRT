---
name: update-claude-md
description: >-
  Maintain CLAUDE.md at the repository root as a living knowledge base about the
  XRT project (what it is, what it does, architecture, components, who uses it,
  build/test workflows, conventions). Use whenever new durable facts about the
  XRT project are discovered or confirmed, when the user asks to record/update
  project knowledge, or at the end of a task that revealed reusable project
  context. CLAUDE.md is auto-loaded by Cursor into every conversation, so keeping
  it accurate gives all future agents full project context.
---

# Update CLAUDE.md (XRT project knowledge base)

`CLAUDE.md` at the repo root is auto-loaded by Cursor into every conversation. It
is the single source of truth for durable knowledge about the XRT project. Keep
it accurate, deduplicated, and concise so every agent starts with full context.

## When to update

Update `CLAUDE.md` when you learn or confirm a **durable** fact, for example:
- What XRT is and what it does (purpose, scope, target hardware/platforms).
- Who uses it (teams, products, downstream consumers, key stakeholders).
- Architecture and major components / directory map.
- Build, test, and run workflows (commands, prerequisites, environment).
- Conventions, gotchas, and non-obvious constraints that recur.

Do **not** record: secrets, transient task state, one-off debugging notes, or
anything that will be stale next week.

## Workflow

1. Read the current root `CLAUDE.md`. If it does not exist, create it using the
   template below.
2. Decide where the new fact belongs (update an existing section; only add a new
   section if none fits).
3. Edit in place: merge with existing content, remove anything now contradicted,
   and keep entries terse (bullet points over prose).
4. Verify facts before writing. Prefer evidence from the repo (code, docs,
   build files) over assumption. If a fact is inferred/uncertain, mark it
   `(unverified)`.
5. Keep the file scannable and ideally under ~300 lines. If a section grows too
   large, summarize and link to the relevant in-repo doc or path.
6. Bump the "Last updated" date at the top.

## Template (use when creating CLAUDE.md)

```markdown
# CLAUDE.md — XRT Project Knowledge Base

> Living context for AI agents. Auto-loaded into every conversation.
> Last updated: YYYY-MM-DD

## What is XRT
- One-line summary.
- Purpose / problem it solves.
- Target hardware, platforms, and OSes.

## Who uses it
- Teams / products / downstream consumers.
- Key stakeholders or maintainers.

## Architecture & components
- High-level architecture.
- Major directories and what they contain.

## Build, test, run
- Prerequisites / dependencies.
- Build commands.
- Test commands.

## Conventions & gotchas
- Coding conventions.
- Non-obvious constraints, pitfalls, recurring issues.

## Glossary
- Term — definition.

## Open questions
- Things not yet confirmed.
```

## Rules

- One canonical location only: the repo-root `CLAUDE.md`. Never fork copies.
- Edit existing sections instead of appending duplicates.
- Be specific (real paths, real commands) and concise.
- Never write secrets, credentials, or tokens into this file.
