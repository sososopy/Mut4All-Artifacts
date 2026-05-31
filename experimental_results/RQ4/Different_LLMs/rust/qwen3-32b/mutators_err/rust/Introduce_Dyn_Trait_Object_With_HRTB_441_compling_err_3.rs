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

pub struct Introduce_Dyn_Trait_Object_With_HRTB_441;

impl Mutator for Introduce_Dyn_Trait_Object_With_HRTB_441 {
    fn name(&self) -> &str {
        "Introduce_Dyn_Trait_Object_With_HRTB_441"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_deref = false;
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                if trait_item.ident == "Deref" {
                    has_deref = true;
                    break;
                }
            }
        }
        if !has_deref {
            file.items.push(parse_quote! {
                trait Deref<T> {
                    fn deref(&self) -> &T;
                }
            });
        }

        file.items.push(parse_quote! {
            trait MyTrait: for<T> Deref<Target=T> {}
        });

        file.items.push(parse_quote! {
            fn some_function(arg: *mut usize) {}
        });

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    let local = parse_quote! {
                        let _: dyn MyTrait = &();
                    };
                    func.block.stmts.insert(0, Stmt::Local(local));

                    let expr_call = parse_quote! {
                        some_function(obj)
                    };
                    func.block.stmts.push(Stmt::Expr(expr_call, None));
                    break;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}