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

pub struct Insert_Generic_Const_Expr_With_Lifetime_In_Async_Array_Length_349;

impl Mutator for Insert_Generic_Const_Expr_With_Lifetime_In_Async_Array_Length_349 {
    fn name(&self) -> &str {
        "Insert_Generic_Const_Expr_With_Lifetime_In_Async_Array_Length_349"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if !item_fn.sig.asyncness.is_some() {
                    continue;
                }
                let has_lifetime = item_fn.sig.generics.params.iter().any(|param| {
                    matches!(param, GenericParam::Lifetime(_))
                });
                if !has_lifetime {
                    let lifetime_param: GenericParam = parse_quote!('a);
                    if item_fn.sig.generics.params.is_empty() {
                        item_fn.sig.generics.params.push(lifetime_param);
                    } else {
                        item_fn.sig.generics.params.insert(0, lifetime_param);
                    }
                }
                let mut array_found = false;
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Pat::Ident(pat_ident) = &local.pat {
                            if let Some(local_init) = &local.init {
                                if let Type::Array(type_array) = &*local_init.ty {
                                    array_found = true;
                                    let new_len: Expr = parse_quote!({
                                        let _: &'a ();
                                        #type_array.len
                                    });
                                    let mut new_type_array = type_array.clone();
                                    new_type_array.len = new_len;
                                    let new_init = local.init.as_mut().unwrap();
                                    new_init.ty = Box::new(Type::Array(new_type_array));
                                }
                            }
                        }
                    }
                }
                if !array_found {
                    let new_stmt: Stmt = parse_quote! {
                        let _arr = [0u8; { let _: &'a (); 1 }];
                    };
                    item_fn.block.stmts.insert(0, new_stmt);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async functions. It ensures the function has a generic lifetime parameter 'a, adding one if missing. Then it searches for array declarations within the function body. If found, it modifies the array length expression by wrapping it in a const block that includes a dummy reference bound to 'a, creating a late-bound variable scenario. If no array exists, it inserts a new array declaration with a const generic length referencing 'a. This aims to produce ConstEvaluatable with escaping bound vars, potentially causing an ICE during trait selection for well-formedness predicates."
    }
}