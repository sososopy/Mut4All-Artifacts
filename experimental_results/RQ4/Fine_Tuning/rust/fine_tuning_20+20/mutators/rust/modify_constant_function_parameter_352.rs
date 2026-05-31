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

pub struct Modify_Constant_Function_Parameter_352;

impl Mutator for Modify_Constant_Function_Parameter_352 {
    fn name(&self) -> &str {
        "Modify_Constant_Function_Parameter_352"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_const_expr = false;
        for item in &mut file.items {
            if let syn::Item::Const(_) = item {
                has_const_expr = true;
                break;
            }
        }
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut modified = false;
                for arg in &mut func.sig.generics.params {
                    if let syn::GenericParam::Const(const_param) = arg {
                        if !modified {
                            const_param.ty = syn::parse_quote!(usize);
                            modified = true;
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let mut modified = false;
                        for arg in &mut func.sig.generics.params {
                            if let syn::GenericParam::Const(const_param) = arg {
                                if !modified {
                                    const_param.ty = syn::parse_quote!(usize);
                                    modified = true;
                                }
                            }
                        }
                    }
                }
            }
        }
        if has_const_expr {
            return;
        }
        let new_const: syn::Item = syn::parse_quote! {
            const NEW_PARAM: usize = 2 * 2;
        };
        file.items.insert(0, new_const);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with constant generic parameters, particularly those used in generic const expressions. It modifies one of these parameters to a compile-time evaluable expression, such as a simple arithmetic operation. If no const expressions are present in the seed program, it introduces a new const declaration to ensure the mutation remains valid. This transformation stresses the compiler's constant evaluation and generic resolution capabilities."
    }
}