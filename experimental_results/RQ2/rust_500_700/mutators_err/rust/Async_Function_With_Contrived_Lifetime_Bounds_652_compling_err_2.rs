use proc_macro2::Span;
use quote::parse_quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    Expr, File, FnArg, GenericParam, Ident, Item, ItemFn, Lifetime, LifetimeParam, PatType,
    ReturnType, Stmt, Type, parse_quote, punctuated::Punctuated, spanned::Spanned, visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct AsyncFunctionWithContrivedLifetimeBounds652;

impl Mutator for AsyncFunctionWithContrivedLifetimeBounds652 {
    fn name(&self) -> &str {
        "AsyncFunctionWithContrivedLifetimeBounds652"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    let generics = &mut func.sig.generics;
                    let new_lifetime_z: LifetimeParam = parse_quote!('z: 'y);
                    let new_lifetime_a: LifetimeParam = parse_quote!('a: 'b);
                    generics.params.push(GenericParam::Lifetime(new_lifetime_z));
                    generics.params.push(GenericParam::Lifetime(new_lifetime_a));

                    if let Some(FnArg::Typed(pat_type)) = func.sig.inputs.first_mut() {
                        if let Type::Reference(ref mut ref_type) = *pat_type.ty {
                            ref_type.lifetime = Some(Lifetime::new("'x", Span::call_site()));
                        }
                    }

                    let new_stmts: Vec<Stmt> = vec![
                        parse_quote! {
                            let temp: &'z str = param;
                        },
                        parse_quote! {
                            let another_temp: &'b str = temp;
                        },
                        parse_quote! {
                            another_temp;
                        }
                    ];

                    func.block.stmts.splice(0..0, new_stmts);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}