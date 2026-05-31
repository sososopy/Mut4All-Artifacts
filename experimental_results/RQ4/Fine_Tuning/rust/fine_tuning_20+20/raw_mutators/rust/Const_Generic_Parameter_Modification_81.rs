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

pub struct Const_Generic_Parameter_Modification_81;

impl Mutator for Const_Generic_Parameter_Modification_81 {
    fn name(&self) -> &str {
        "Const_Generic_Parameter_Modification_81"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for item in &mut item_trait.items {
                    if let TraitItem::Type(type_item) = item {
                        if let Some(bounds) = &mut type_item.generics.params.first_mut() {
                            if let GenericParam::Const(const_param) = bounds {
                                if const_param.ty == parse_quote!(i32) {
                                    const_param.ty = parse_quote!(u32);
                                }
                            }
                        }
                    }
                    if let TraitItem::Fn(method) = item {
                        if let Some(bounds) = &mut method.sig.generics.params.first_mut() {
                            if let GenericParam::Const(const_param) = bounds {
                                if const_param.ty == parse_quote!(i32) {
                                    const_param.ty = parse_quote!(u32);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets traits that use const generics in their associated types or methods, specifically changing the type of a const generic parameter from `i32` to `u32`. By altering the const generic type, it tests the compiler's ability to handle type changes in generic contexts, potentially uncovering issues related to const evaluation and type inference."
    }
}