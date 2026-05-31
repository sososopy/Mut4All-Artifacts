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

pub struct Modify_Struct_Generics_Order_396;

impl Mutator for Modify_Struct_Generics_Order_396 {
    fn name(&self) -> &str {
        "Modify_Struct_Generics_Order_396"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let generics = &mut item_struct.generics;
                let mut const_generic_index = None;
                let mut type_generic_index = None;

                for (i, param) in generics.params.iter().enumerate() {
                    match param {
                        syn::GenericParam::Const(_) => const_generic_index = Some(i),
                        syn::GenericParam::Type(_) => type_generic_index = Some(i),
                        _ => {}
                    }
                }

                if let (Some(c_idx), Some(t_idx)) = (const_generic_index, type_generic_index) {
                    let mut params_vec: Vec<_> = generics.params.clone().into_iter().collect();
                    params_vec.swap(c_idx, t_idx);
                    generics.params = Punctuated::from_iter(params_vec);

                    if let syn::GenericParam::Const(const_param) = &mut generics.params[c_idx] {
                        if const_param.default.is_none() {
                            const_param.default = Some(parse_quote!({1 + 1}));
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