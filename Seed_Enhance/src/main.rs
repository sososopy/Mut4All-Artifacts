#![feature(macro_metavar_expr)]
#![feature(macro_metavar_expr_concat)]
// #![feature(concat_idents)]
#![feature(box_into_inner)]
#![feature(min_specialization)]
// #![allow(incomplete_features)]
// #![feature(generic_const_exprs)]
// #![feature(trace_macros)]
// trace_macros!(true);

use std::{
    env::temp_dir,
    error::Error,
    fs::{OpenOptions, create_dir_all},
    io::Write,
    path::PathBuf,
    sync::{
        Arc, Barrier, LazyLock, Mutex,
        atomic::{AtomicBool, AtomicUsize, Ordering},
    },
    thread,
};

use clap::Parser;
use conf::{Args, FuzzerJob, JobType, set_log_level};
use fuzz::{
    feature_list::FEATURES,
    fuzzbase::{DummyFuzzer, FResult, Fuzzer},
};
use ice_process::{ICEFilter, flagbisect::filter_flags};
use signal_hook::{
    consts::{SIGINT, SIGTERM},
    iterator::Signals,
};
// use strategies::models::ModelFuzzer;
#[allow(unused_imports)]
use strategies::{splicer::SplicerFuzzer, syn::SynFuzzer};
use util::gen_alnum;

mod conf;
mod fuzz;
mod ice_process;
mod util;

mod strategies;

const THREAD_SET_CNT: usize = 3;
const STACK_SIZE: usize = 1024 * 1024 * 128; // 128MB

static EXTRA_ARGS: [&str; 9] = [
    "-C",
    "opt-level=3",
    "--edition",
    "2024",
    "--emit=mir",
    "-Zno-codegen",
    "-Zmir-opt-level=4",
    "-Zvalidate-mir",
    "--crate-type=lib",
];

static STOP: LazyLock<AtomicBool> = LazyLock::new(|| AtomicBool::new(false));

enum JobCB {
    Null,
    InformICE(bool), // bool : is dup
}

#[derive(Clone)]
struct Job {
    fuzzer: Arc<Mutex<Box<dyn Fuzzer>>>,
    job: JobType,
    tmp_source: PathBuf,
    tmp_bin: PathBuf,
}
impl Job {
    pub fn new(fuzzer: Box<dyn Fuzzer>, job: JobType) -> Self {
        Self {
            fuzzer: Arc::new(Mutex::new(fuzzer)),
            job,
            tmp_source: temp_dir().join(format!("nfuzz_{}.rs", gen_alnum(4))),
            tmp_bin: temp_dir().join(format!("nfuzz_{}.bin", gen_alnum(4))),
        }
    }
    pub fn deep_clone(&self) -> Self {
        let mut lock = self.fuzzer.lock().map_err(|e| e.to_string()).unwrap();
        let fuzzer = lock.as_mut();
        let fuzzer = dyn_clone::clone_box(&*fuzzer);
        let fuzzer = Arc::new(Mutex::new(fuzzer));
        Self {
            fuzzer,
            job: self.job.clone(),
            tmp_source: self.tmp_source.clone(),
            tmp_bin: self.tmp_bin.clone(),
        }
    }
}
impl Job {
    pub fn do_once(
        &self,
        args: &Args,
        idx: usize,
        step_idx: usize,
        mut code: Vec<Vec<u8>>,
        filters: &mut Vec<Arc<Mutex<Box<dyn ICEFilter>>>>,
    ) -> Result<(JobCB, Vec<Vec<u8>>), Box<dyn Error>> {
        let mut lock = self.fuzzer.lock().map_err(|e| e.to_string())?;
        match self.job {
            JobType::Gen => {
                let new_code = lock.generate()?;
                code.push(new_code);
            }
            JobType::Mask => {
                let maskfuzzer = lock.as_mut().as_mask_fuzzer_mut()?;
                let last = code.pop().ok_or("No code to mask")?;
                let (code_prefix, code_suffix) = maskfuzzer.mask(&last)?;
                code.push(code_prefix);
                code.push(code_suffix);
            }
            JobType::Infill => {
                let infillfuzzer = lock.as_mut().as_infill_fuzzer_mut()?;
                let suffix = code.pop().ok_or("No code to infill")?;
                let prefix = code.pop().ok_or("No code to infill")?;
                let infill_code = infillfuzzer.infill(&prefix, &suffix)?;
                code.push(infill_code);
            }
            JobType::Fuzz => {
                if code.is_empty() {
                    Err("You can't fuzz without code")?;
                }
                let compile_res =
                    DummyFuzzer::compile(&code[0], &self.tmp_source, &self.tmp_bin, &EXTRA_ARGS);
                let compile_res = match compile_res {
                    Ok(res) => res,
                    Err(e) => {
                        error!("Read thread error...: {e}");
                        return Ok((JobCB::Null, code));
                    }
                };
                info!("{idx} - CRES: {}", compile_res.1);
                // return Ok((JobCB::Null, code));

                if !matches!(compile_res.1, FResult::InternalCompileError(..))
                    && !matches!(compile_res.1, FResult::HangOnCompile)
                {
                    return Ok((JobCB::Null, code));
                }

                if matches!(compile_res.1, FResult::HangOnCompile) {
                    if args.skip_hang {
                        info!("\t\tHang: Skip hang on compile");
                        return Ok((JobCB::Null, code));
                    }
                    info!("\t\tHang: Hang on compile");
                    let p = args.output.join(format!("gen_{idx}.rs"));
                    info!(
                        "\t\tDump to: {}",
                        p.as_os_str().to_str().unwrap_or_default()
                    );

                    <DummyFuzzer as Fuzzer>::dump(&code[0], &p)?;
                    let mut addon_f = OpenOptions::new().append(true).open(&p)?;
                    addon_f.write_fmt(format_args!(
                        "\n\n// Compile Args: {}",
                        EXTRA_ARGS.join(" ")
                    ))?;
                    addon_f.write_fmt(format_args!(
                        "\n\n// Original Flags: {}",
                        FEATURES.join(" ")
                    ))?;
                    return Ok((JobCB::InformICE(false), code));
                }

                let mut filter_pass = 0;
                for filter in filters {
                    let mut lock = filter.lock().map_err(|s| s.to_string())?;
                    if !lock.filter(&compile_res.1) {
                        filter_pass += 1;
                        let _ = lock.add(&compile_res.1);
                    }
                }
                if filter_pass == 0 {
                    info!("\t\tICE already exists, filtering out");
                    return Ok((JobCB::InformICE(true), code));
                }
                let p = args.output.join(format!("gen_{idx}.rs"));
                info!("\t\tICE: New ICE found");
                info!(
                    "\t\tDump to: {}",
                    p.as_os_str().to_str().unwrap_or_default()
                );
                <DummyFuzzer as Fuzzer>::dump(&code[0], &p)?;
                let mut addon_f = OpenOptions::new().append(true).open(&p)?;
                let flags = FEATURES.iter().map(|x| x.to_string()).collect::<Vec<_>>();
                let minimized_flag = filter_flags::<DummyFuzzer>(
                    flags,
                    &code[0],
                    &self.tmp_source,
                    &self.tmp_bin,
                    &EXTRA_ARGS,
                )?;
                addon_f.write_fmt(format_args!(
                    "\n\n// Compile Args: {}",
                    EXTRA_ARGS.join(" ")
                ))?;
                addon_f.write_fmt(format_args!(
                    "\n\n// Original Flags: {}",
                    FEATURES.join(" ")
                ))?;
                addon_f.write_fmt(format_args!(
                    "\n\n// Compile Flags: {}",
                    minimized_flag.join(" ")
                ))?;
                return Ok((JobCB::InformICE(false), code));
            }
            JobType::Dump => {
                let p = args.output.join(format!("gen_{step_idx}_{idx}.rs"));
                info!(
                    "\t\tDump to: {}",
                    p.as_os_str().to_str().unwrap_or_default()
                );
                // dump code use "\n// <Break Here>\n"
                let mut f = OpenOptions::new().write(true).create(true).open(&p)?;
                let mut flag = false;
                for i in &code {
                    if flag {
                        f.write_all(b"<|[Break Here]|>")?;
                    }
                    f.write_all(&i)?;
                    flag = true;
                }
            }
        }
        Ok((JobCB::Null, code))
    }
}

#[derive(Clone)]
struct JobHolder {
    jobs: Vec<Job>,
    filters: Vec<Arc<Mutex<Box<dyn ICEFilter>>>>,
}
impl JobHolder {
    pub fn new() -> Self {
        Self {
            jobs: Vec::new(),
            filters: Vec::new(),
        }
    }
    pub fn deep_clone(&self) -> Self {
        let mut jobs = Vec::new();
        for job in &self.jobs {
            jobs.push(job.deep_clone());
        }

        let filters = self.filters.clone();
        Self { jobs, filters }
    }
    pub fn add_job(&mut self, args: &Args, job: FuzzerJob) -> Result<(), Box<dyn Error>> {
        let fuzzer = job.fuzzer.new_fuzzer(args)?;
        let job = Job::new(fuzzer, job.job);
        self.jobs.push(job);
        Ok(())
    }
    pub fn add_filter(&mut self, filter: Box<dyn ICEFilter>) -> Result<(), Box<dyn Error>> {
        let filter = Arc::new(Mutex::new(filter));
        self.filters.push(filter);
        Ok(())
    }
}
impl JobHolder {
    pub fn do_once(&mut self, args: &Args, idx: usize) -> Result<(), Box<dyn Error>> {
        let mut code = Vec::new();
        for (sidx, job) in self.jobs.iter().enumerate() {
            let (cb, n_code) = job.do_once(args, idx, sidx, code, &mut self.filters)?;
            code = n_code;
            match cb {
                JobCB::Null => {}
                JobCB::InformICE(dup) => {
                    for i in 0..sidx {
                        self.jobs[i]
                            .fuzzer
                            .lock()
                            .unwrap()
                            .inform_ice(&code[0], dup)?;
                    }
                }
            }
        }
        Ok(())
    }
}

fn run(args: &Args, idx: Arc<AtomicUsize>, mut holder: JobHolder) -> Result<(), Box<dyn Error>> {
    loop {
        let cidx = idx.fetch_add(1, Ordering::Relaxed);
        if !args._infloop && cidx >= args._loopcnt {
            break;
        }
        if STOP.load(Ordering::Relaxed) {
            break;
        }

        info!("Current index: {cidx}");

        holder.do_once(args, cidx)?;
    }
    info!("Work thread exited!");
    Ok(())
}

fn check_stop(mut signals: Signals) {
    for sig in signals.forever() {
        match sig {
            SIGINT | SIGTERM => {
                info!("Received signal: {sig}");
                STOP.store(true, Ordering::Relaxed);
                break;
            }
            _ => unreachable!(),
        }
    }
    info!("Signal thread exited!");
}

fn main() -> Result<(), Box<dyn Error>> {
    let args = Args::parse();
    let args = Box::leak(args.into());
    set_log_level(&args.log_level);

    debug!("{:#?}", args);

    // let fuzzer = FuzzerType::new(&args)?;

    // let mut filters: Vec<_> = vec![
    //     QueryStackFilter::new(),
    //     PanicFuncFilter::new(),
    //     DummyFilter::new(),
    // ]
    // .into_iter()
    // .collect();
    // for filter in &mut filters {
    //     filter.import(args)?;
    //     info!("Filter imported");
    // }

    let mut worker = JobHolder::new();
    let mut filters = Vec::new();
    for f in &args.filters {
        info!("Creating filter: {:?}", f);
        filters.push(f.new_filter(&args)?);
    }
    for mut filter in filters {
        filter.import(args)?;
        info!(
            "Filter imported {}",
            std::any::type_name_of_val(filter.as_ref())
        );
        worker.add_filter(filter)?;
    }

    for job in &args.jobs {
        worker.add_job(args, job.clone())?;
        info!("Job added {:#?}", job);
    }

    create_dir_all(&args.output)?;
    info!("Begin generating...");

    let idx: Arc<AtomicUsize> = Arc::new(0.into());

    if args.nthread <= 1 {
        let idx = idx.clone();
        let worker = worker.clone();
        let args_ref = &*args;
        let handle = thread::Builder::new()
            .name(format!("Worker-0"))
            .stack_size(STACK_SIZE)
            .spawn(move || -> Result<(), Box<dyn Error + Send + Sync>> {
                let begin_time = std::time::Instant::now();
                let res = run(args_ref, idx, worker).map_err(|e| e.to_string().into());
                let end_time = std::time::Instant::now();
                let duration = end_time.duration_since(begin_time);
                warn!("Total Execution Time: {:?}", duration);
                res
            })?;
        let sig = Signals::new(&[SIGINT, SIGTERM])?;
        let _ = thread::spawn(move || {
            check_stop(sig);
        });
        // Sleep for a while to let the threads finish
        thread::sleep(std::time::Duration::from_secs(1));
        let re = handle.join();
        let re = match re {
            Ok(re) => re,
            Err(e) => {
                error!("Thread atexit error: {e:?}");
                Ok(())
            }
        };
        if let Err(e) = re {
            error!("Thread error: {e}");
        }
    } else {
        let mut handles = Vec::default();
        let barrier = Arc::new(Barrier::new(args.nthread));
        let mut i = 0;

        let args = &*args;
        info!(
            "Due to multi-thread needs to copy some data, the spawn procedure could be as slow as the init process :("
        );
        info!("Please wait patiently~ :P");

        while i < args.nthread {
            let worker_in_set = worker.deep_clone();
            for _ in 0..THREAD_SET_CNT {
                if i >= args.nthread {
                    break;
                }
                let worker_scope = worker_in_set.clone();
                let idx = idx.clone();
                let barrier = barrier.clone();
                let handle = thread::Builder::new()
                    .name(format!("Worker-{i}"))
                    .stack_size(STACK_SIZE)
                    .spawn(move || -> Result<(), Box<dyn Error + Send + Sync>> {
                        barrier.wait();
                        run(args, idx, worker_scope).map_err(|e| e.to_string().into())
                    })?;
                handles.push(handle);
                info!("Thread {i} spawned~");
                i += 1;
            }
        }
        info!("Begin to execute w~");
        info!("Press Ctrl+C to stop");
        let sig = Signals::new(&[SIGINT, SIGTERM])?;
        let _ = thread::spawn(move || {
            check_stop(sig);
        });
        // Sleep for a while to let the threads finish
        thread::sleep(std::time::Duration::from_secs(1));

        while let Some(handle) = handles.pop() {
            let res = handle.join();
            let res = match res {
                Ok(res) => res,
                Err(e) => {
                    error!("Thread atexit error: {e:?}");
                    Ok(())
                }
            };
            if let Err(e) = res {
                error!("Thread error: {e}");
            }
        }
        info!("All threads exited!");
    }

    for filter in &worker.filters {
        let lock = filter.lock().map_err(|s| s.to_string())?;
        lock.export(args)?;
    }
    info!("Filter exported");

    Ok(())
}
