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

pub struct Add_Const_Move_Closure_With_Slice_Pattern_321;

impl Mutator for Add_Const_Move_Closure_With_Slice_Pattern_321 {
    fn name(&self) -> &str {
        "Add_Const_Move_Closure_With_Slice_Pattern_321"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(item_fn) = item {
                if item_fn.sig.constness.is_some() {
                    if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                        if let Type::ImplTrait(_) = &**return_type {
                            let new_block = parse_quote! {
                                {
                                    const move || {
                                        let arr: &[u8] = b"abc";
                                        match arr {
                                            [x, rest @ ..] => {},
                                            [] => {},
                                        }
                                    }
                                }
                            };
                            *item_fn.block = new_block;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}