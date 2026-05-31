use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
use syn::{
    Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam,
    Ident, Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Attribute_Swapper_257;

impl Mutator for Attribute_Swapper_257 {
    fn name(&self) -> &str {
        "Attribute_Swapper_257"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                let mut inner_attrs = Vec::new();
                let mut outer_attrs = Vec::new();

                for attr in &struct_item.attrs {
                    if attr.style == syn::AttrStyle::Inner(syn::token::Not) {
                        inner_attrs.push(attr.clone());
                    } else {
                        outer_attrs.push(attr.clone());
                    }
                }

                struct_item.attrs.clear();

                // Swap inner and outer attributes
                for attr in inner_attrs {
                    struct_item.attrs.push(syn::Attribute {
                        style: syn::AttrStyle::Outer,
                        ..attr
                    });
                }

                for attr in outer_attrs {
                    struct_item.attrs.push(syn::Attribute {
                        style: syn::AttrStyle::Inner(syn::token::Not),
                        ..attr
                    });
                }
            }

            if let syn::Item::Fn(func) = item {
                let mut inner_attrs = Vec::new();
                let mut outer_attrs = Vec::new();

                for attr in &func.attrs {
                    if attr.style == syn::AttrStyle::Inner(syn::token::Not) {
                        inner_attrs.push(attr.clone());
                    } else {
                        outer_attrs.push(attr.clone());
                    }
                }

                func.attrs.clear();

                // Swap inner and outer attributes
                for attr in inner_attrs {
                    func.attrs.push(syn::Attribute {
                        style: syn::AttrStyle::Outer,
                        ..attr
                    });
                }

                for attr in outer_attrs {
                    func.attrs.push(syn::Attribute {
                        style: syn::AttrStyle::Inner(syn::token::Not),
                        ..attr
                    });
                }
            }

            if let syn::Item::Impl(impl_item) = item {
                let mut inner_attrs = Vec::new();
                let mut outer_attrs = Vec::new();

                for attr in &impl_item.attrs {
                    if attr.style == syn::AttrStyle::Inner(syn::token::Not) {
                        inner_attrs.push(attr.clone());
                    } else {
                        outer_attrs.push(attr.clone());
                    }
                }

                impl_item.attrs.clear();

                // Swap inner and outer attributes
                for attr in inner_attrs {
                    impl_item.attrs.push(syn::Attribute {
                        style: syn::AttrStyle::Outer,
                        ..attr
                    });
                }

                for attr in outer_attrs {
                    impl_item.attrs.push(syn::Attribute {
                        style: syn::AttrStyle::Inner(syn::token::Not),
                        ..attr
                    });
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps the positions of inner and outer attributes in Rust code, targeting functions, structs, and modules. This transformation aims to test the compiler's handling of attribute ordering and may expose bugs related to attribute processing."
    }
}