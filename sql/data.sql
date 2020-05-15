--
-- PostgreSQL database dump
--

-- Dumped from database version 10.12 (Ubuntu 10.12-0ubuntu0.18.04.1)
-- Dumped by pg_dump version 10.12 (Ubuntu 10.12-0ubuntu0.18.04.1)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- Data for Name: bal; Type: TABLE DATA; Schema: public; Owner: rfp
--

COPY public.bal (sym, bal, pend, avail) FROM stdin;
\.


--
-- Data for Name: bal_n; Type: TABLE DATA; Schema: public; Owner: rfp
--

COPY public.bal_n (sym, bal, pend, avail) FROM stdin;
\.


--
-- Data for Name: btc_hist; Type: TABLE DATA; Schema: public; Owner: rfp
--

COPY public.btc_hist (day, open, high, low, close, adj_close, volume) FROM stdin;
\.


--
-- Data for Name: goal; Type: TABLE DATA; Schema: public; Owner: rfp
--

COPY public.goal (sym, weight) FROM stdin;
\.


--
-- Data for Name: goal_n; Type: TABLE DATA; Schema: public; Owner: rfp
--

COPY public.goal_n (sym, weight) FROM stdin;
\.


--
-- Data for Name: hold_hist; Type: TABLE DATA; Schema: public; Owner: rfp
--

COPY public.hold_hist (day, price, btc, usd, val, del_btc, del_usd) FROM stdin;
\.


--
-- Data for Name: mkt_h; Type: TABLE DATA; Schema: public; Owner: rfp
--

COPY public.mkt_h (stime, sym, cur, ask, bid, last, etime) FROM stdin;
\.


--
-- Data for Name: mkt_save; Type: TABLE DATA; Schema: public; Owner: rfp
--

COPY public.mkt_save (stime, sym, cur, ask, bid, last, etime) FROM stdin;
\.


--
-- Data for Name: ord; Type: TABLE DATA; Schema: public; Owner: rfp
--

COPY public.ord (uuid, closed, commission, condition, condition_target, exchange, immediate_or_cancel, is_conditional, xlimit, order_type, price, price_per_unit, quantity, quantity_remaining, stamptime) FROM stdin;
\.


--
-- PostgreSQL database dump complete
--

