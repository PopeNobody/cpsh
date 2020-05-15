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
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


--
-- Name: mkt_round(); Type: FUNCTION; Schema: public; Owner: rfp
--

CREATE FUNCTION public.mkt_round() RETURNS bigint
    LANGUAGE sql
    AS $$select round_to(utime(now()),15*60)$$;


ALTER FUNCTION public.mkt_round() OWNER TO rfp;

--
-- Name: round_to(bigint, bigint); Type: FUNCTION; Schema: public; Owner: rfp
--

CREATE FUNCTION public.round_to(bigint, bigint) RETURNS bigint
    LANGUAGE sql
    AS $_$select $1-$1%$2$_$;


ALTER FUNCTION public.round_to(bigint, bigint) OWNER TO rfp;

--
-- Name: utime(timestamp without time zone); Type: FUNCTION; Schema: public; Owner: rfp
--

CREATE FUNCTION public.utime(timestamp without time zone) RETURNS bigint
    LANGUAGE sql
    AS $_$select cast(extract(epoch from $1) as bigint)$_$;


ALTER FUNCTION public.utime(timestamp without time zone) OWNER TO rfp;

--
-- Name: utime(timestamp with time zone); Type: FUNCTION; Schema: public; Owner: rfp
--

CREATE FUNCTION public.utime(timestamp with time zone) RETURNS bigint
    LANGUAGE sql
    AS $_$select cast(extract(epoch from $1) as bigint)$_$;


ALTER FUNCTION public.utime(timestamp with time zone) OWNER TO rfp;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: bal; Type: TABLE; Schema: public; Owner: rfp
--

CREATE TABLE public.bal (
    sym text NOT NULL,
    bal numeric(16,8) NOT NULL,
    pend numeric(16,8) NOT NULL,
    avail numeric(16,8) NOT NULL
);


ALTER TABLE public.bal OWNER TO rfp;

--
-- Name: bal_n; Type: TABLE; Schema: public; Owner: rfp
--

CREATE TABLE public.bal_n (
    sym text NOT NULL,
    bal numeric(16,8) NOT NULL,
    pend numeric(16,8) NOT NULL,
    avail numeric(16,8) NOT NULL
);


ALTER TABLE public.bal_n OWNER TO rfp;

--
-- Name: btc_hist; Type: TABLE; Schema: public; Owner: rfp
--

CREATE TABLE public.btc_hist (
    day date NOT NULL,
    open numeric(16,8),
    high numeric(16,8),
    low numeric(16,8),
    close numeric(16,8),
    adj_close numeric(16,8),
    volume numeric(20,8)
);


ALTER TABLE public.btc_hist OWNER TO rfp;

--
-- Name: goal; Type: TABLE; Schema: public; Owner: rfp
--

CREATE TABLE public.goal (
    sym text NOT NULL,
    weight numeric(16,8) NOT NULL
);


ALTER TABLE public.goal OWNER TO rfp;

--
-- Name: goal_n; Type: TABLE; Schema: public; Owner: rfp
--

CREATE TABLE public.goal_n (
    sym text NOT NULL,
    weight numeric(16,8) NOT NULL
);


ALTER TABLE public.goal_n OWNER TO rfp;

--
-- Name: hold_hist; Type: TABLE; Schema: public; Owner: rfp
--

CREATE TABLE public.hold_hist (
    day date NOT NULL,
    price numeric(16,8),
    btc numeric(16,8),
    usd numeric(16,8),
    val numeric(16,8),
    del_btc numeric(16,8),
    del_usd numeric(16,8)
);


ALTER TABLE public.hold_hist OWNER TO rfp;

--
-- Name: mkt_h; Type: TABLE; Schema: public; Owner: rfp
--

CREATE TABLE public.mkt_h (
    stime bigint NOT NULL,
    sym text NOT NULL,
    cur text NOT NULL,
    ask numeric(16,8),
    bid numeric(16,8),
    last numeric(16,8),
    etime bigint
);


ALTER TABLE public.mkt_h OWNER TO rfp;

--
-- Name: mkt; Type: VIEW; Schema: public; Owner: rfp
--

CREATE VIEW public.mkt AS
 SELECT mkt_h.cur,
    mkt_h.sym,
    mkt_h.ask,
    mkt_h.bid,
    mkt_h.last
   FROM public.mkt_h
  WHERE (mkt_h.etime IS NULL);


ALTER TABLE public.mkt OWNER TO rfp;

--
-- Name: mkt_hv; Type: VIEW; Schema: public; Owner: rfp
--

CREATE VIEW public.mkt_hv AS
 SELECT mkt_h.sym,
    mkt_h.cur,
    mkt_h.ask,
    mkt_h.bid,
    mkt_h.last
   FROM public.mkt_h
  WHERE (mkt_h.etime IS NULL);


ALTER TABLE public.mkt_hv OWNER TO rfp;

--
-- Name: mkt_save; Type: TABLE; Schema: public; Owner: rfp
--

CREATE TABLE public.mkt_save (
    stime bigint,
    sym text,
    cur text,
    ask numeric(16,8),
    bid numeric(16,8),
    last numeric(16,8),
    etime bigint
);


ALTER TABLE public.mkt_save OWNER TO rfp;

--
-- Name: mkt_time_dups; Type: VIEW; Schema: public; Owner: rfp
--

CREATE VIEW public.mkt_time_dups AS
 SELECT m1.stime,
    m1.etime AS mtime,
    m2.etime,
    m1.sym,
    m1.cur
   FROM public.mkt_h m1,
    public.mkt_h m2
  WHERE ((m1.etime = m2.stime) AND (m1.sym = m2.sym) AND (m1.cur = m2.cur) AND (m1.bid = m2.bid) AND (m1.ask = m2.ask) AND (m1.last = m2.ask));


ALTER TABLE public.mkt_time_dups OWNER TO rfp;

--
-- Name: mkt_time_join; Type: VIEW; Schema: public; Owner: rfp
--

CREATE VIEW public.mkt_time_join AS
 SELECT m1.stime,
    m1.etime AS mtime,
    m2.etime,
    m1.sym,
    m1.cur
   FROM public.mkt_h m1,
    public.mkt_h m2
  WHERE ((m1.etime = m2.stime) AND (m1.sym = m2.sym) AND (m1.cur = m2.cur));


ALTER TABLE public.mkt_time_join OWNER TO rfp;

--
-- Name: next_hist; Type: VIEW; Schema: public; Owner: rfp
--

CREATE VIEW public.next_hist AS
 SELECT min(btc_hist.day) AS day
   FROM public.btc_hist
  WHERE (NOT (btc_hist.day IN ( SELECT hold_hist.day
           FROM public.hold_hist)));


ALTER TABLE public.next_hist OWNER TO rfp;

--
-- Name: ord; Type: TABLE; Schema: public; Owner: rfp
--

CREATE TABLE public.ord (
    uuid text NOT NULL,
    closed timestamp without time zone,
    commission numeric(16,8),
    condition text,
    condition_target numeric(16,8),
    exchange text,
    immediate_or_cancel boolean,
    is_conditional boolean,
    xlimit numeric(16,8),
    order_type text,
    price numeric(16,8),
    price_per_unit numeric(16,8),
    quantity numeric(16,8),
    quantity_remaining numeric(16,8),
    stamptime timestamp without time zone
);


ALTER TABLE public.ord OWNER TO rfp;

--
-- PostgreSQL database dump complete
--

