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

pub struct Modify_Const_Generics_In_Struct_Declarations_350;

impl Mutator for Modify_Const_Generics_In_Struct_Declarations_350 {
    fn name(&self) -> &str {
        "Modify_Const_Generics_In_Struct_Declarations_350"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut has_const_generics = false;
                for param in &item_struct.generics.params {
                    if let GenericParam::Const(_) = param {
                        has_const_generics = true;
                        break;
                    }
                }
                if !has_const_generics {
                    continue;
                }
                let mut new_params = Punctuated::new();
                for param in &item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        let mut new_const_param = const_param.clone();
                        if new_const_param.ident == "N" {
                            new_const_param.default = Some(Expr::Verbatim(quote! { M + 1 }));
                        }
                        new_params.push(GenericParam::Const(new_const_param));
                    } else {
                        new_params.push(param.clone());
                    }
                }
                new_params.push(parse_quote! { const M: usize = 2 });
                item_struct.generics.params = new_params;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets struct definitions with const generics, specifically those with a const generic named `N` that has a default value. It modifies the default value of `N` to be `M + 1`, where `M` is a new const generic parameter with a default value of 2. This transformation introduces a dependency between const generics, potentially exposing compiler bugs related to const evaluation and generic parameter resolution."
    }
}