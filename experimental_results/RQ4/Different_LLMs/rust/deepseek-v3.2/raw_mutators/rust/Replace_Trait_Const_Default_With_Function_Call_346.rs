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

pub struct Replace_Trait_Const_Default_With_Function_Call_346;

impl Mutator for Replace_Trait_Const_Default_With_Function_Call_346 {
    fn name(&self) -> &str {
        "Replace_Trait_Const_Default_With_Function_Call_346"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut existing_functions = HashSet::new();
        let mut trait_const_params = Vec::new();

        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                existing_functions.insert(item_fn.sig.ident.to_string());
            }
        }

        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for param in &mut item_trait.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if const_param.default.is_some() {
                            trait_const_params.push((item_trait, const_param));
                        }
                    }
                }
            }
        }

        if trait_const_params.is_empty() {
            return;
        }

        let (trait_item, const_param) = trait_const_params.choose(&mut rng).unwrap();
        let param_ident = &const_param.ident;
        let param_type = &const_param.ty;
        let fn_name = format!("{}_default", param_ident);
        let fn_ident = Ident::new(&fn_name, Span::call_site());

        let mut function_exists = existing_functions.contains(&fn_name);
        let mut new_fn_item: Option<ItemFn> = None;

        if !function_exists {
            let generic_type = if rng.gen_bool(0.5) {
                let generic_param: GenericParam = parse_quote!(T);
                let return_expr: Expr = parse_quote!(42);
                new_fn_item = Some(parse_quote! {
                    fn #fn_ident<T>() -> #param_type {
                        #return_expr
                    }
                });
                true
            } else {
                let return_expr: Expr = parse_quote!(42);
                new_fn_item = Some(parse_quote! {
                    fn #fn_ident() -> #param_type {
                        #return_expr
                    }
                });
                false
            };
        }

        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                if item_trait.ident == trait_item.ident {
                    for param in &mut item_trait.generics.params {
                        if let GenericParam::Const(const_param) = param {
                            if const_param.ident == *param_ident {
                                let call_expr: Expr = if rng.gen_bool(0.5) {
                                    parse_quote!(#fn_ident())
                                } else {
                                    parse_quote!(#fn_ident::<i32>())
                                };
                                const_param.default = Some(call_expr);
                                break;
                            }
                        }
                    }
                    break;
                }
            }
        }

        if let Some(fn_item) = new_fn_item {
            file.items.push(Item::Fn(fn_item));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait definitions with generic const parameters that have default values. It replaces the default value with a call to a function, either existing or newly introduced. The introduced function may be generic or non-generic, returning a constant expression but potentially causing type mismatches or const evaluation issues. This transformation stresses the compiler's const generic handling, trait resolution, and function call evaluation in default parameter contexts."
    }
}