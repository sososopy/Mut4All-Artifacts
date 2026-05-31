use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemEnum, ItemUnion, ItemTrait, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Swap_Defaulted_Generic_Parameter_Order_348;

impl Mutator for Swap_Defaulted_Generic_Parameter_Order_348 {
    fn name(&self) -> &str {
        "Swap_Defaulted_Generic_Parameter_Order_348"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Struct(item_struct) => {
                    let generics = &mut item_struct.generics;
                    if generics.params.len() >= 2 {
                        let params = &mut generics.params;
                        for i in 0..params.len() - 1 {
                            let (left, right) = params.split_at_mut(i + 1);
                            let first = &mut left[i];
                            let second = &mut right[0];
                            if let (GenericParam::Const(first), GenericParam::Const(second)) = (first, second) {
                                if first.default.is_some() && second.default.is_none() {
                                    std::mem::swap(first, second);
                                    break;
                                }
                            } else if let (GenericParam::Type(first), GenericParam::Type(second)) = (first, second) {
                                if first.default.is_some() && second.default.is_none() {
                                    std::mem::swap(first, second);
                                    break;
                                }
                            } else if let (GenericParam::Const(first), GenericParam::Type(second)) = (first, second) {
                                if first.default.is_some() && second.default.is_none() {
                                    std::mem::swap(first, second);
                                    break;
                                }
                            } else if let (GenericParam::Type(first), GenericParam::Const(second)) = (first, second) {
                                if first.default.is_some() && second.default.is_none() {
                                    std::mem::swap(first, second);
                                    break;
                                }
                            }
                        }
                    }
                }
                Item::Enum(item_enum) => {
                    let generics = &mut item_enum.generics;
                    if generics.params.len() >= 2 {
                        let params = &mut generics.params;
                        for i in 0..params.len() - 1 {
                            let (left, right) = params.split_at_mut(i + 1);
                            let first = &mut left[i];
                            let second = &mut right[0];
                            if let (GenericParam::Const(first), GenericParam::Const(second)) = (first, second) {
                                if first.default.is_some() && second.default.is_none() {
                                    std::mem::swap(first, second);
                                    break;
                                }
                            } else if let (GenericParam::Type(first), GenericParam::Type(second)) = (first, second) {
                                if first.default.is_some() && second.default.is_none() {
                                    std::mem::swap(first, second);
                                    break;
                                }
                            } else if let (GenericParam::Const(first), GenericParam::Type(second)) = (first, second) {
                                if first.default.is_some() && second.default.is_none() {
                                    std::mem::swap(first, second);
                                    break;
                                }
                            } else if let (GenericParam::Type(first), GenericParam::Const(second)) = (first, second) {
                                if first.default.is_some() && second.default.is_none() {
                                    std::mem::swap(first, second);
                                    break;
                                }
                            }
                        }
                    }
                }
                Item::Union(item_union) => {
                    let generics = &mut item_union.generics;
                    if generics.params.len() >= 2 {
                        let params = &mut generics.params;
                        for i in 0..params.len() - 1 {
                            let (left, right) = params.split_at_mut(i + 1);
                            let first = &mut left[i];
                            let second = &mut right[0];
                            if let (GenericParam::Const(first), GenericParam::Const(second)) = (first, second) {
                                if first.default.is_some() && second.default.is_none() {
                                    std::mem::swap(first, second);
                                    break;
                                }
                            } else if let (GenericParam::Type(first), GenericParam::Type(second)) = (first, second) {
                                if first.default.is_some() && second.default.is_none() {
                                    std::mem::swap(first, second);
                                    break;
                                }
                            } else if let (GenericParam::Const(first), GenericParam::Type(second)) = (first, second) {
                                if first.default.is_some() && second.default.is_none() {
                                    std::mem::swap(first, second);
                                    break;
                                }
                            } else if let (GenericParam::Type(first), GenericParam::Const(second)) = (first, second) {
                                if first.default.is_some() && second.default.is_none() {
                                    std::mem::swap(first, second);
                                    break;
                                }
                            }
                        }
                    }
                }
                Item::Trait(item_trait) => {
                    let generics = &mut item_trait.generics;
                    if generics.params.len() >= 2 {
                        let params = &mut generics.params;
                        for i in 0..params.len() - 1 {
                            let (left, right) = params.split_at_mut(i + 1);
                            let first = &mut left[i];
                            let second = &mut right[0];
                            if let (GenericParam::Const(first), GenericParam::Const(second)) = (first, second) {
                                if first.default.is_some() && second.default.is_none() {
                                    std::mem::swap(first, second);
                                    break;
                                }
                            } else if let (GenericParam::Type(first), GenericParam::Type(second)) = (first, second) {
                                if first.default.is_some() && second.default.is_none() {
                                    std::mem::swap(first, second);
                                    break;
                                }
                            } else if let (GenericParam::Const(first), GenericParam::Type(second)) = (first, second) {
                                if first.default.is_some() && second.default.is_none() {
                                    std::mem::swap(first, second);
                                    break;
                                }
                            } else if let (GenericParam::Type(first), GenericParam::Const(second)) = (first, second) {
                                if first.default.is_some() && second.default.is_none() {
                                    std::mem::swap(first, second);
                                    break;
                                }
                            }
                        }
                    }
                }
                Item::Fn(item_fn) => {
                    let generics = &mut item_fn.sig.generics;
                    if generics.params.len() >= 2 {
                        let params = &mut generics.params;
                        for i in 0..params.len() - 1 {
                            let (left, right) = params.split_at_mut(i + 1);
                            let first = &mut left[i];
                            let second = &mut right[0];
                            if let (GenericParam::Const(first), GenericParam::Const(second)) = (first, second) {
                                if first.default.is_some() && second.default.is_none() {
                                    std::mem::swap(first, second);
                                    break;
                                }
                            } else if let (GenericParam::Type(first), GenericParam::Type(second)) = (first, second) {
                                if first.default.is_some() && second.default.is_none() {
                                    std::mem::swap(first, second);
                                    break;
                                }
                            } else if let (GenericParam::Const(first), GenericParam::Type(second)) = (first, second) {
                                if first.default.is_some() && second.default.is_none() {
                                    std::mem::swap(first, second);
                                    break;
                                }
                            } else if let (GenericParam::Type(first), GenericParam::Const(second)) = (first, second) {
                                if first.default.is_some() && second.default.is_none() {
                                    std::mem::swap(first, second);
                                    break;
                                }
                            }
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps the order of a generic parameter with a default value and a subsequent generic parameter without a default value in struct, enum, union, trait, or function definitions. It targets generic items with at least two parameters where the first has a default and the second does not, swapping their positions while preserving names, types, bounds, and defaults. This transformation aims to trigger substitution index miscalculations and ICEs when defaulted parameters are not trailing, stressing the compiler's generic parameter ordering logic."
    }
}