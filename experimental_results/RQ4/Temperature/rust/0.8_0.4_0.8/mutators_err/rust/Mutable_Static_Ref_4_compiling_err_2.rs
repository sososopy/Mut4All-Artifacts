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

pub struct Mutable_Static_Ref_4;

impl Mutator for Mutable_Static_Ref_4 {
    fn name(&self) -> &str {
        "Mutable_Static_Ref_4"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Static(item_static) = item {
                if let Some(_) = item_static.mutability {
                    let static_name = &item_static.ident;
                    let static_type = &item_static.ty;
                    
                    let new_fn_name = Ident::new(&format!("modify_{}", static_name), Span::call_site());
                    let new_fn: ItemFn = parse_quote! {
                        fn #new_fn_name() {
                            unsafe {
                                #static_name += 1;
                            }
                        }
                    };

                    file.items.push(Item::Fn(new_fn));

                    if let Some(main_fn) = file.items.iter_mut().find_map(|item| {
                        if let Item::Fn(item_fn) = item {
                            if item_fn.sig.ident == "main" {
                                return Some(item_fn);
                            }
                        }
                        None
                    }) {
                        main_fn.block.stmts.push(parse_quote! {
                            #new_fn_name();
                        });
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}