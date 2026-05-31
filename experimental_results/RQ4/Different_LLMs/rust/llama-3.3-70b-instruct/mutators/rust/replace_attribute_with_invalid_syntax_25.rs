use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, AttrStyle, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument,
    GenericParam, Ident, Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType,
    Path as SynPath, PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type,
    TypeImplTrait, TypeParamBound, TypePath, parse_quote,
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

pub struct Replace_Attribute_With_Invalid_Syntax_25;

impl Mutator for Replace_Attribute_With_Invalid_Syntax_25 {
    fn name(&self) -> &str {
        "Replace_Attribute_With_Invalid_Syntax_25"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for attr in &mut func.attrs {
                    if let AttrStyle::Inner(_) = attr.style {
                        let invalid_attr = Attribute {
                            pound_token: attr.pound_token,
                            style: attr.style,
                            bracket_token: attr.bracket_token,
                            meta: attr.meta.clone(),
                        };
                        *attr = invalid_attr;
                    }
                }
            }
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        for attr in &mut func.attrs {
                            if let AttrStyle::Inner(_) = attr.style {
                                let invalid_attr = Attribute {
                                    pound_token: attr.pound_token,
                                    style: attr.style,
                                    bracket_token: attr.bracket_token,
                                    meta: attr.meta.clone(),
                                };
                                *attr = invalid_attr;
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets attributes in the given code, replacing them with an invalid syntax to trigger potential bugs in the compiler's attribute handling. It applies to any attribute in the code, regardless of its location or the type of attribute."
    }
}