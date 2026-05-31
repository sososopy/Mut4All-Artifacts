use std::{
    collections::HashSet,
    error::Error,
    hash::Hash,
    mem::swap,
    sync::{Arc, LazyLock, RwLock},
};

use crate::util::glob_range;

#[derive(Debug)]
struct VecHash<T>
where
    T: Hash + Eq,
{
    v: Vec<T>,
    h: HashSet<T>,
}
impl<T> Default for VecHash<T>
where
    T: Hash + Eq,
{
    fn default() -> Self {
        Self {
            v: Vec::default(),
            h: HashSet::default(),
        }
    }
}
#[allow(unused)]
impl<T> VecHash<T>
where
    T: Hash + Eq + Clone,
{
    pub fn insert(&mut self, x: T) {
        if self.h.insert(x.clone()) {
            self.v.push(x);
        }
    }
    pub fn take(&mut self, idx: usize) -> T {
        let elem = self.v.swap_remove(idx);
        self.h.remove(&elem);
        elem
    }
    pub fn take_rand(&mut self) -> T {
        let r = glob_range(0..self.v.len());
        let elem = self.v.swap_remove(r);
        self.h.remove(&elem);
        elem
    }
    pub fn len(&self) -> usize {
        self.v.len()
    }
}

#[allow(unused)]
#[derive(Debug)]
pub struct ASTEnv {
    pub nested: usize,
    outer: Option<Box<Self>>,
    vars: VecHash<String>,
    pub var_cnt: usize,
    funcs: VecHash<String>,
    pub func_cnt: usize,

    structs: VecHash<String>,
    traits: VecHash<String>,

    pub expr_nest: usize,
    pub pat_nest: usize,
    pub stmt_nest: usize,
    pub type_nest: usize,
}

impl Default for ASTEnv {
    fn default() -> Self {
        Self {
            nested: 0,
            outer: None,
            vars: VecHash::default(),
            var_cnt: 0,
            funcs: VecHash::default(),
            func_cnt: 0,

            structs: VecHash::default(),
            traits: VecHash::default(),

            expr_nest: 1,
            pat_nest: 1,
            stmt_nest: 1,
            type_nest: 1,
        }
    }
}

#[allow(unused)]
impl ASTEnv {
    pub fn new(mut outer: Self) -> Self {
        outer.undrop_new();
        outer
    }
    pub fn undrop_new(&mut self) {
        let mut res = Self {
            nested: self.nested + 1,
            var_cnt: self.var_cnt,
            func_cnt: self.func_cnt,
            outer: None,
            vars: VecHash::default(),
            funcs: VecHash::default(),

            structs: VecHash::default(),
            traits: VecHash::default(),

            expr_nest: self.expr_nest,
            pat_nest: self.pat_nest,
            stmt_nest: self.stmt_nest,
            type_nest: self.type_nest,
        };
        swap(self, &mut res);
        self.outer = Some(Box::new(res));
    }
    pub fn take(mut self) -> Result<Self, Box<dyn Error>> {
        let outer = self
            .outer
            .take()
            .ok_or("You cant take the most outside or an already taken object")?;
        let res = Box::into_inner(outer);
        Ok(res)
    }
    pub fn undrop_take(&mut self) -> Result<Self, Box<dyn Error>> {
        let outer = self
            .outer
            .take()
            .ok_or("You cant take the most outside or an already taken object")?;
        let res = Box::into_inner(outer);
        Ok(res)
    }
}
#[allow(unused)]
impl ASTEnv {
    pub fn insert_var(&mut self, name: &str) {
        self.vars.insert(name.to_owned());
        self.var_cnt += 1;
    }
    pub fn insert_func(&mut self, name: &str) {
        self.funcs.insert(name.to_owned());
        self.func_cnt += 1;
    }
    pub fn take_var(&mut self, mut idx: usize) -> Result<String, Box<dyn Error>> {
        if idx < self.vars.len() {
            self.var_cnt -= 1;
            return Ok(self.vars.take(idx));
        }
        idx -= self.vars.len();
        let res = match &mut self.outer {
            Some(outer) => outer.take_var(idx),
            None => Err("Outsize range".into()),
        }?;
        self.func_cnt -= 1;
        Ok(res)
    }
    pub fn take_var_rand(&mut self) -> Result<String, Box<dyn Error>> {
        let r = glob_range(0..self.var_cnt);
        self.take_var(r)
    }
    pub fn take_func(&mut self, mut idx: usize) -> Result<String, Box<dyn Error>> {
        if idx < self.funcs.len() {
            self.func_cnt -= 1;
            return Ok(self.funcs.take(idx));
        }
        idx -= self.funcs.len();
        let res = match &mut self.outer {
            Some(outer) => outer.take_func(idx),
            None => Err("Outsize range".into()),
        }?;
        self.func_cnt -= 1;
        Ok(res)
    }
    pub fn take_func_rand(&mut self) -> Result<String, Box<dyn Error>> {
        let r = glob_range(0..self.func_cnt);
        self.take_var(r)
    }

    pub fn insert_struct(&mut self, name: &str) {
        self.structs.insert(name.to_owned());
    }
    pub fn insert_trait(&mut self, name: &str) {
        self.traits.insert(name.to_owned());
    }
    pub fn take_struct(&mut self, idx: usize) -> String {
        self.structs.take(idx)
    }
    pub fn take_trait(&mut self, idx: usize) -> String {
        self.traits.take(idx)
    }
    pub fn take_struct_rand(&mut self) -> String {
        self.structs.take_rand()
    }
    pub fn take_trait_rand(&mut self) -> String {
        self.traits.take_rand()
    }
}

static CURRENT_ENV_P: LazyLock<Arc<RwLock<ASTEnv>>> =
    LazyLock::new(|| Arc::new(RwLock::new(ASTEnv::default())));

pub fn current_env() -> Arc<RwLock<ASTEnv>> {
    CURRENT_ENV_P.clone()
}

pub fn unnest_env() -> Result<(), Box<dyn Error>> {
    let cur = current_env();
    let mut lock = cur.write().map_err(|e| e.to_string())?;
    let outer = lock.undrop_take()?;
    *lock = outer;
    Ok(())
}

pub fn nest_env() -> Result<(), Box<dyn Error>> {
    let cur = current_env();
    let mut lock = cur.write().map_err(|e| e.to_string())?;
    lock.undrop_new();
    Ok(())
}
