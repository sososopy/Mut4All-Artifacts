use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Insert_Async_Closure_With_Capture_Mismatch_21;

impl Mutator for Insert_Async_Closure_With_Capture_Mismatch_21 {
    fn name(&self) -> &str {
        "Insert_Async_Closure_With_Capture_Mismatch_21"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    continue;
                }
                let mut candidates = Vec::new();
                for input in &item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        candidates.push(pat_type.pat.clone());
                    }
                }
                let mut visitor = LocalVarVisitor {
                    locals: Vec::new(),
                };
                visitor.visit_block(&item_fn.block);
                candidates.extend(visitor.locals);
                if candidates.is_empty() {
                    continue;
                }
                let captured_var = candidates.choose(&mut rng).unwrap();
                let param_names = generate_unique_param_names(&item_fn.block, &mut rng);
                let closure_expr: Expr = parse_quote! {
                    async |#param_names| {
                        #captured_var.wrapping_add(1);
                    }
                };
                let stmt = Stmt::Expr(closure_expr, None);
                let insert_pos = rng.gen_range(0..item_fn.block.stmts.len() + 1);
                item_fn.block.stmts.insert(insert_pos, stmt);
            }
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        if func.sig.ident == "main" {
                            continue;
                        }
                        let mut candidates = Vec::new();
                        for input in &func.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                candidates.push(pat_type.pat.clone());
                            }
                        }
                        let mut visitor = LocalVarVisitor {
                            locals: Vec::new(),
                        };
                        visitor.visit_block(&func.block);
                        candidates.extend(visitor.locals);
                        if candidates.is_empty() {
                            continue;
                        }
                        let captured_var = candidates.choose(&mut rng).unwrap();
                        let param_names = generate_unique_param_names(&func.block, &mut rng);
                        let closure_expr: Expr = parse_quote! {
                            async |#param_names| {
                                #captured_var.wrapping_add(1);
                            }
                        };
                        let stmt = Stmt::Expr(closure_expr, None);
                        let insert_pos = rng.gen_range(0..func.block.stmts.len() + 1);
                        func.block.stmts.insert(insert_pos, stmt);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts an async closure inside non-async functions and methods. The closure captures a variable from the outer scope (parameter or local) but does not use all of its own parameters, creating a mismatch between formal parameters and captured upvars. This transformation aims to stress the compiler's async closure capture analysis, potentially triggering bugs related to leftover child captures and capture environment handling."
    }
}

struct LocalVarVisitor {
    locals: Vec<Pat>,
}

impl Visit<'static> for LocalVarVisitor {
    fn visit_local(&mut self, local: &'static Local) {
        self.locals.push(local.pat.clone());
        syn::visit::visit_local(self, local);
    }
}

fn generate_unique_param_names(block: &Block, rng: &mut impl Rng) -> Punctuated<Ident, Comma> {
    let base_names = ["p1", "p2"];
    let mut used_names = HashSet::new();
    let mut visitor = IdentCollector {
        names: HashSet::new(),
    };
    visitor.visit_block(block);
    used_names.extend(visitor.names);
    let mut params = Punctuated::new();
    for base in base_names.iter() {
        let mut candidate = Ident::new(base, Span::call_site());
        while used_names.contains(&candidate.to_string()) {
            let suffix = rng.gen_range(0..1000);
            candidate = Ident::new(&format!("{}{}", base, suffix), Span::call_site());
        }
        used_names.insert(candidate.to_string());
        params.push(candidate);
    }
    params
}

struct IdentCollector {
    names: HashSet<String>,
}

impl Visit<'static> for IdentCollector {
    fn visit_ident(&mut self, ident: &'static Ident) {
        self.names.insert(ident.to_string());
        syn::visit::visit_ident(self, ident);
    }
}