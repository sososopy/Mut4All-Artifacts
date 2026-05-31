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

pub struct Modify_Generic_Slice_Ref_To_Boxed_Slice_217;

impl Mutator for Modify_Generic_Slice_Ref_To_Boxed_Slice_217 {
    fn name(&self) -> &str {
        "Modify_Generic_Slice_Ref_To_Boxed_Slice_217"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut found_slice_param = false;
                for input in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(type_reference) = &*pat_type.ty {
                            if let Type::Slice(type_slice) = &*type_reference.elem {
                                let new_type: Type = parse_quote! { Box<[#type_slice]> };
                                pat_type.ty = Box::new(new_type);
                                found_slice_param = true;
                            }
                        }
                    }
                }
                if found_slice_param {
                    let mut new_where_clause = item_fn
                        .sig
                        .generics
                        .where_clause
                        .clone()
                        .unwrap_or_else(|| WhereClause {
                            where_token: Default::default(),
                            predicates: Punctuated::new(),
                        });
                    new_where_clause
                        .predicates
                        .push(parse_quote! { [T]: Clone });
                    item_fn.sig.generics.where_clause = Some(new_where_clause);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Locate generic functions with slice reference parameters and modify them to use boxed slices instead, adding a `where [T]: Clone` clause to ensure compatibility with boxed slice requirements."
    }
}