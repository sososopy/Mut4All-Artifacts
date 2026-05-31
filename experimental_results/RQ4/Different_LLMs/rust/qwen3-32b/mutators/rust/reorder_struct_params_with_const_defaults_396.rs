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

pub struct Reorder_Struct_Params_With_Const_Defaults_396;

impl Mutator for Reorder_Struct_Params_With_Const_Defaults_396 {
    fn name(&self) -> &str {
        "Reorder_Struct_Params_With_Const_Defaults_396"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let has_type = item_struct.generics.params.iter().any(|p| {
                    matches!(p, GenericParam::Type(_))
                });
                let has_const = item_struct.generics.params.iter().any(|p| {
                    matches!(p, GenericParam::Const(_))
                });

                if has_type && has_const {
                    let (const_params, other_params): (Vec<_>, Vec<_>) = item_struct.generics.params.iter().cloned().partition(|p| matches!(p, GenericParam::Const(_)));

                    let mut new_const_params = Vec::new();
                    for param in const_params {
                        if let GenericParam::Const(mut const_param) = param {
                            if const_param.default.is_none() {
                                const_param.default = Some(parse_quote!({ 0 + 1 }));
                            }
                            new_const_params.push(GenericParam::Const(const_param));
                        }
                    }

                    item_struct.generics.params = new_const_params.into_iter().chain(other_params).collect();
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator reorders struct generic parameters to place const parameters with default expressions before type parameters. It introduces default expressions to const parameters if missing, then reorders them. This transformation stresses the compiler's handling of generic parameter ordering, potentially exposing bugs in parameter resolution and default expression evaluation."
    }
}