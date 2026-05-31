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

pub struct Replace_Array_Const_Param_With_Type_Alias_286;

impl Mutator for Replace_Array_Const_Param_With_Type_Alias_286 {
    fn name(&self) -> &str {
        "Replace_Array_Const_Param_With_Type_Alias_286"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut alias_count = 0usize;
        for (index, item) in file.items.iter_mut().enumerate() {
            if let Item::Struct(struct_item) = item {
                let mut param_aliases = Vec::new();
                for (param_index, param) in struct_item.generics.params.iter_mut().enumerate() {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Array(array_type) = &const_param.ty {
                            let alias_name = format_ident!("Arr_{}", alias_count);
                            alias_count += 1;
                            let alias: syn::ItemType = parse_quote! {
                                pub type #alias_name = #array_type;
                            };
                            param_aliases.push((param_index, alias_name, alias));
                        }
                    }
                }
                for (param_index, alias_name, alias) in param_aliases.iter().rev() {
                    file.items.insert(index, Item::Type(alias.clone()));
                }
                for (param_index, alias_name, _) in param_aliases {
                    if let Some(param) = struct_item.generics.params.get_mut(param_index) {
                        if let GenericParam::Const(const_param) = param {
                            const_param.ty = parse_quote! { #alias_name };
                        }
                    }
                }
            } else if let Item::Enum(enum_item) = item {
                let mut param_aliases = Vec::new();
                for (param_index, param) in enum_item.generics.params.iter_mut().enumerate() {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Array(array_type) = &const_param.ty {
                            let alias_name = format_ident!("Arr_{}", alias_count);
                            alias_count += 1;
                            let alias: syn::ItemType = parse_quote! {
                                pub type #alias_name = #array_type;
                            };
                            param_aliases.push((param_index, alias_name, alias));
                        }
                    }
                }
                for (param_index, alias_name, alias) in param_aliases.iter().rev() {
                    file.items.insert(index, Item::Type(alias.clone()));
                }
                for (param_index, alias_name, _) in param_aliases {
                    if let Some(param) = enum_item.generics.params.get_mut(param_index) {
                        if let GenericParam::Const(const_param) = param {
                            const_param.ty = parse_quote! { #alias_name };
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces concrete array types in const generic parameters of structs/enums with lazy type aliases. This transformation tests the compiler's handling of const parameter type resolution when using type aliases, particularly during const comparison operations and ADT generic parameter validation. It aims to expose potential issues in the interaction between lazy type aliases and const generics."
    }
}