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

pub struct Modify_Const_Generic_Type_To_Custom_289;

impl Mutator for Modify_Const_Generic_Type_To_Custom_289 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Type_To_Custom_289"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut custom_types = HashSet::new();
        for item in &file.items {
            match item {
                Item::Type(item_type) => {
                    custom_types.insert(item_type.ident.to_string());
                }
                Item::Struct(item_struct) => {
                    custom_types.insert(item_struct.ident.to_string());
                }
                _ => {}
            }
        }
        let mut selected_type = None;
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if selected_type.is_none() {
                    for param in &item_fn.sig.generics.params {
                        if let GenericParam::Const(const_param) = param {
                            if const_param.ty == parse_quote!(usize) {
                                selected_type = Some(const_param.ident.clone());
                                break;
                            }
                        }
                    }
                }
                if let Some(ref type_ident) = selected_type {
                    if item_fn
                        .sig
                        .generics
                        .params
                        .iter()
                        .any(|param| match param {
                            GenericParam::Const(const_param) => {
                                &const_param.ident == type_ident
                            }
                            _ => false,
                        })
                    {
                        let new_type = custom_types
                            .iter()
                            .next()
                            .map(|ty| Ident::new(ty, Span::call_site()))
                            .unwrap_or_else(|| Ident::new("usize", Span::call_site()));
                        let mut new_generics = item_fn.sig.generics.clone();
                        for param in &mut new_generics.params {
                            if let GenericParam::Const(const_param) = param {
                                if const_param.ident == *type_ident {
                                    const_param.ty = parse_quote!(#new_type);
                                }
                            }
                        }
                        item_fn.sig.generics = new_generics;
                        let mut new_stmts = Vec::new();
                        for stmt in &item_fn.block.stmts {
                            new_stmts.push(stmt.clone());
                        }
                        let new_expr: Expr = parse_quote! { #type_ident > 10 };
                        new_stmts.insert(0, parse_quote! { let _ = #new_expr; });
                        item_fn.block.stmts = new_stmts;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with generic const parameters of primitive types (like usize) and replaces them with custom types (type aliases or newtypes) defined in the module. It modifies the function's generic signature and injects logic in the function body that uses the const parameter in a context expecting a primitive type. This transformation stresses the compiler's type resolution and const evaluation mechanisms by altering the expected type semantics of const generics."
    }
}