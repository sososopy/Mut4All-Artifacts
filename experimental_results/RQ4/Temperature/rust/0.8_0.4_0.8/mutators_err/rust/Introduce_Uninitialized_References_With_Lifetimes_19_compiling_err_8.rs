use proc_macro2::Span;
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command};
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
    parse::Parse,
    parse::{ParseStream, Parser},
};

use crate::mutator::Mutator;

pub struct Introduce_Uninitialized_References_With_Lifetimes_19;

impl Mutator for Introduce_Uninitialized_References_With_Lifetimes_19 {
    fn name(&self) -> &str {
        "Introduce_Uninitialized_References_With_Lifetimes_19"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut has_lifetime = false;
                for input in &func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(type_ref) = &*pat_type.ty {
                            if type_ref.lifetime.is_some() {
                                has_lifetime = true;
                                break;
                            }
                        }
                    }
                }

                if has_lifetime {
                    let new_lifetime: Lifetime = parse_quote!('b);
                    let uninit_var: Stmt = parse_quote!(let uninit_ref: &'b str;);

                    func.block.stmts.insert(0, uninit_var);
                    if let Some(first_stmt) = func.block.stmts.get_mut(1) {
                        let use_uninit: Expr = parse_quote! {
                            let _ = uninit_ref;
                        };
                        func.block.stmts.insert(1, Stmt::Expr(use_uninit, None));
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator introduces uninitialized references with a new lifetime into functions that already have lifetime annotations. By adding a reference with a different lifetime, it challenges the compiler's lifetime checking and inference mechanisms, potentially exposing issues with lifetime mismatches and uninitialized reference usage."
    }
}