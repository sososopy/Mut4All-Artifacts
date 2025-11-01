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

pub struct Modify_Const_Generics_151;

impl Mutator for Modify_Const_Generics_151 {
    fn name(&self) -> &str {
        "Modify_Const_Generics_151"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let generics = &item_struct.generics;
                let mut has_const_generic = false;
                let mut new_generics = Punctuated::new();

                for param in generics.params.iter() {
                    if let GenericParam::Const(const_param) = param {
                        if const_param.default.is_some() {
                            has_const_generic = true;
                            let new_param: GenericParam = parse_quote! { const M: usize = 2 };
                            new_generics.push(new_param);

                            let new_default: Expr = parse_quote! { M + 1 };
                            let mut modified_param = const_param.clone();
                            modified_param.default = Some(new_default);
                            new_generics.push(GenericParam::Const(modified_param));
                        } else {
                            new_generics.push(param.clone());
                        }
                    } else {
                        new_generics.push(param.clone());
                    }
                }

                if has_const_generic {
                    item_struct.generics.params = new_generics;
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets struct definitions that use const generics with default values. It modifies the default value of an existing const generic by introducing a dependency on a new const generic parameter. Specifically, it adds a new const generic parameter and changes the existing const generic's default value to depend on this new parameter through an arithmetic expression."
    }
}