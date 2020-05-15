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

SET default_tablespace = '';

--
-- Name: bal_n bal_n_pkey; Type: CONSTRAINT; Schema: public; Owner: rfp
--

ALTER TABLE ONLY public.bal_n
    ADD CONSTRAINT bal_n_pkey PRIMARY KEY (sym);


--
-- Name: bal bal_pkey; Type: CONSTRAINT; Schema: public; Owner: rfp
--

ALTER TABLE ONLY public.bal
    ADD CONSTRAINT bal_pkey PRIMARY KEY (sym);


--
-- Name: btc_hist btc_hist_pkey; Type: CONSTRAINT; Schema: public; Owner: rfp
--

ALTER TABLE ONLY public.btc_hist
    ADD CONSTRAINT btc_hist_pkey PRIMARY KEY (day);


--
-- Name: goal_n goal_n_pkey; Type: CONSTRAINT; Schema: public; Owner: rfp
--

ALTER TABLE ONLY public.goal_n
    ADD CONSTRAINT goal_n_pkey PRIMARY KEY (sym);


--
-- Name: goal goal_pkey; Type: CONSTRAINT; Schema: public; Owner: rfp
--

ALTER TABLE ONLY public.goal
    ADD CONSTRAINT goal_pkey PRIMARY KEY (sym);


--
-- Name: hold_hist hold_hist_pkey; Type: CONSTRAINT; Schema: public; Owner: rfp
--

ALTER TABLE ONLY public.hold_hist
    ADD CONSTRAINT hold_hist_pkey PRIMARY KEY (day);


--
-- Name: mkt_h mkt_h_pkey; Type: CONSTRAINT; Schema: public; Owner: rfp
--

ALTER TABLE ONLY public.mkt_h
    ADD CONSTRAINT mkt_h_pkey PRIMARY KEY (stime, sym, cur);


--
-- Name: ord ord_pkey; Type: CONSTRAINT; Schema: public; Owner: rfp
--

ALTER TABLE ONLY public.ord
    ADD CONSTRAINT ord_pkey PRIMARY KEY (uuid);


--
-- PostgreSQL database dump complete
--

