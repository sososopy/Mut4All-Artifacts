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
    ItemStatic,
    StaticMutability,
};

use crate::mutator::Mutator;

pub struct Mutable_Static_Ref_4;

impl Mutator for Mutable_Static_Ref_4 {
    fn name(&self) -> &str {
        "Mutable_Static_Ref_4"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Static(item_static) = item {
                if let StaticMutability::Mut(_) = item_static.mutability {
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

                    new_items.push(Item::Fn(new_fn));
                }
            }
        }

        file.items.extend(new_items);

        let modify_fns: Vec<Ident> = file.items.iter().filter_map(|item| {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident.to_string().starts_with("modify_") {
                    return Some(item_fn.sig.ident.clone());
                }
            }
            None
        }).collect();

        if let Some(main_fn) = file.items.iter_mut().find_map(|item| {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    return Some(item_fn);
                }
            }
            None
        }) {
            for fn_ident in modify_fns {
                main_fn.block.stmts.push(parse_quote! {
                    #fn_ident();
                });
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}