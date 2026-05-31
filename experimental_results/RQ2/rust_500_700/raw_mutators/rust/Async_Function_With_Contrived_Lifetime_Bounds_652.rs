use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Async_Function_With_Contrived_Lifetime_Bounds_652;

impl Mutator for Async_Function_With_Contrived_Lifetime_Bounds_652 {
    fn name(&self) -> &str {
        "Async_Function_With_Contrived_Lifetime_Bounds_652"
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
                            another_temp
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