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

pub struct Replace_Const_Generic_With_Function_Ptr_293;

impl Mutator for Replace_Const_Generic_With_Function_Ptr_293 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Function_Ptr_293"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut functions = Vec::new();
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                functions.push(func);
            }
        }

        if functions.is_empty() {
            return;
        }

        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                if let Some(const_param) = find_first_const_param(&mut struct_item.generics) {
                    if let GenericParam::Const(const_param) = const_param {
                        let func = functions.choose(&mut thread_rng()).unwrap();
                        let func_name = func.sig.ident.to_string();
                        const_param.ident = Ident::new(&func_name, const_param.ident.span());

                        let mut inputs = Punctuated::new();
                        for arg in &func.sig.inputs {
                            if let FnArg::Typed(pat_type) = arg {
                                let ty = pat_type.ty.clone();
                                let bare_fn_arg = syn::BareFnArg {
                                    ty: *ty,
                                    ..Default::default()
                                };
                                inputs.push(bare_fn_arg);
                            }
                        }
                        let output = func.sig.output.clone();
                        let bare_fn = syn::TypeBareFn {
                            unsafety: syn::Unsafety::Normal,
                            fn_token: token::Fn::default(),
                            lifetimes: None,
                            paren_token: Paren::default(),
                            inputs,
                            output,
                            variadic: None,
                            abi: None,
                        };
                        const_param.ty = Box::new(syn::Type::BareFn(bare_fn));
                    }
                }
            } else if let Item::Impl(impl_item) = item {
                if let Some(const_param) = find_first_const_param(&mut impl_item.generics) {
                    if let GenericParam::Const(const_param) = const_param {
                        let func = functions.choose(&mut thread_rng()).unwrap();
                        let func_name = func.sig.ident.to_string();
                        const_param.ident = Ident::new(&func_name, const_param.ident.span());

                        let mut inputs = Punctuated::new();
                        for arg in &func.sig.inputs {
                            if let FnArg::Typed(pat_type) = arg {
                                let ty = pat_type.ty.clone();
                                let bare_fn_arg = syn::BareFnArg {
                                    ty: *ty,
                                    ..Default::default()
                                };
                                inputs.push(bare_fn_arg);
                            }
                        }
                        let output = func.sig.output.clone();
                        let bare_fn = syn::TypeBareFn {
                            unsafety: syn::Unsafety::Normal,
                            fn_token: token::Fn::default(),
                            lifetimes: None,
                            paren_token: Paren::default(),
                            inputs,
                            output,
                            variadic: None,
                            abi: None,
                        };
                        const_param.ty = Box::new(syn::Type::BareFn(bare_fn));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces a valid const generic parameter in a struct or impl block with a function pointer type using an existing function from the seed program. This transformation invalidates the const parameter by substituting it with a function pointer, which is not a valid const generic parameter. This may trigger ICEs or errors in the compiler's handling of const generics and function pointer types."
    }
}

fn find_first_const_param(generics: &mut Generics) -> Option<&mut GenericParam> {
    for param in &mut generics.params {
        if let GenericParam::Const(_) = param {
            return Some(param);
        }
    }
    None}