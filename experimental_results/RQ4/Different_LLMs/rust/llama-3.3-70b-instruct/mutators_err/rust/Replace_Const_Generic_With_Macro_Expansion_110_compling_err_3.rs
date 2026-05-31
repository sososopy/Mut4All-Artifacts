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

pub struct Replace_Const_Generic_With_Macro_Expansion_110;

impl Mutator for Replace_Const_Generic_With_Macro_Expansion_110 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_Macro_Expansion_110"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                if let Some(generics) = struct_item.generics {
                    for param in generics.params {
                        if let GenericParam::Const(const_param) = param {
                            if let Some(const_value) = const_param.default {
                                if let Expr::Macro(mac) = const_value {
                                    if let Some(mac_ident) = mac.mac.path.get_ident() {
                                        if mac_ident.to_string() == "wrap" {
                                            if let Expr::Path(path) = &mac.mac.tokens[0] {
                                                if let Some(path_ident) = path.path.get_ident() {
                                                    let new_const_value = parse_quote! { #path_ident };
                                                    const_param.default = Some(new_const_value);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic values with their macro-expanded equivalents. It targets code constructs that use const generics in conjunction with macros, aiming to expose bugs related to the interaction between these two features."
    }
}