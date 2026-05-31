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

pub struct Substitute_Const_Generic_Default_With_Out_of_Bounds_Block_87;

impl Mutator for Substitute_Const_Generic_Default_With_Out_of_Bounds_Block_87 {
    fn name(&self) -> &str {
        "Substitute_Const_Generic_Default_With_Out_of_Bounds_Block_87"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                self.mutate_generics(&mut item_struct.generics);
            }
            if let Item::Enum(item_enum) = item {
                self.mutate_generics(&mut item_enum.generics);
            }
            if let Item::Fn(item_fn) = item {
                self.mutate_generics(&mut item_fn.sig.generics);
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        self.mutate_generics(&mut func.sig.generics);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic parameters with default values in structs, enums, and functions. It replaces the default expression with a const block referencing another const generic parameter from the same list, potentially creating self-references or out-of-bounds references. This transformation aims to exploit the compiler's handling of const generic default expressions during substitution, potentially triggering index calculation errors or cyclic dependency issues."
    }
}
impl Substitute_Const_Generic_Default_With_Out_of_Bounds_Block_87 {
    fn mutate_generics(&self, generics: &mut syn::Generics) {
        let const_params_indices: Vec<_> = generics.params.iter().enumerate().filter_map(|(idx, param)| {
            if let GenericParam::Const(const_param) = param {
                Some((idx, const_param.ident.clone()))
            } else {
                None
            }
        }).collect();
        if const_params_indices.len() == 0 {
            return;
        }
        for (param_idx, const_param_ident) in &const_params_indices {
            let const_param = generics.params.iter().find(|p| {
                if let GenericParam::Const(cp) = p {
                    cp.ident == *const_param_ident
                } else {
                    false
                }
            }).unwrap();
            if let GenericParam::Const(const_param) = const_param {
                if let Some(default_expr) = &const_param.default {
                    let target_idx = if const_params_indices.len() > 1 {
                        let mut rng = thread_rng();
                        let candidates = const_params_indices.iter().filter(|(idx, _)| *idx != *param_idx).map(|(idx, _)| idx).collect::<Vec<_>>();
                        if candidates.len() > 0 {
                            *candidates.choose(&mut rng).unwrap()
                        } else {
                            param_idx
                        }
                    } else {
                        param_idx
                    };
                    let target_param_ident = const_params_indices.iter().find(|(idx, ident)| *idx == *target_idx).map(|(_, ident)| ident).unwrap();
                    let new_expr: syn::Expr = parse_quote! { { const { #target_param_ident } } };
                    let mut const_param_mut = generics.params.iter_mut().filter_map(|p| {
                        if let GenericParam::Const(cp) = p {
                            Some(cp)
                        } else {
                            None
                        }
                    }).find(|cp| cp.ident == *const_param_ident).unwrap();
                    const_param_mut.default = Some(new_expr);
                }
            }
        }
    }
}