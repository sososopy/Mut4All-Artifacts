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

pub struct Replace_Const_Generic_Default_With_IncompatibleType_91;

impl Mutator for Replace_Const_Generic_Default_With_IncompatibleType_91 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Default_With_IncompatibleType_91"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_items: Vec<(Ident, String)> = Vec::new();
        for item in &file.items {
            if let Item::Const(item_const) = item {
                let ty_tokens = item_const.ty.clone().into_token_stream();
                let ty_str = ty_tokens.to_string();
                const_items.push((item_const.ident.clone(), ty_str));
            }
        }

        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                self.process_generics(&mut item_struct.generics, &const_items);
            } else if let Item::Enum(item_enum) = item {
                self.process_generics(&mut item_enum.generics, &const_items);
            } else if let Item::Fn(item_fn) = item {
                self.process_generics(&mut item_fn.sig.generics, &const_items);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the default value of a const generic parameter with a const item of an incompatible type from the same module. This introduces type mismatches that may trigger substitution errors during trait resolution or well-formed checks, testing the compiler's handling of const generics and type compatibility."
    }
}

impl Replace_Const_Generic_Default_With_IncompatibleType_91 {
    fn process_generics(&self, generics: &mut syn::Generics, const_items: &[(Ident, String)]) {
        for param in &mut generics.params {
            if let GenericParam::Const(const_param) = param {
                if let Some(default) = &const_param.default {
                    let param_type_tokens = const_param.ty.clone().into_token_stream();
                    let param_type_str = param_type_tokens.to_string();
                    if let Some((ident, _)) = const_items.iter().find(|(_, ty)| ty != &param_type_str) {
                        let new_expr = parse_quote!(#ident);
                        *const_param.default = Some(new_expr);
                    }
                }
            }
        }
    }
}