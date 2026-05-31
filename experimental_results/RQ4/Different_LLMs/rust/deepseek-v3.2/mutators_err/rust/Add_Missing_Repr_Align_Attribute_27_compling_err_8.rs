use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemEnum, ItemUnion, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Add_Missing_Repr_Align_Attribute_27;

impl Mutator for Add_Missing_Rep2_Align_Attribute_27 {
    fn name(&self) -> &str {
        "Add_Missing_Repr_Align_Attribute_27"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut candidates = Vec::new();
        for item in &mut file.items {
            match item {
                Item::Fn(item_fn) => {
                    candidates.push(item_fn);
                }
                Item::Struct(item_struct) => {
                    candidates.push(item_struct);
                }
                Item::Enum(item_enum) => {
                    candidates.push(item_enum);
                }
                Item::Union(item_union) => {
                    candidates.push(item_union);
                }
                Item::Impl(item_impl) => {
                    for impl_item in &mut item_impl.items {
                        match impl_item {
                            ImplItem::Fn(impl_fn) => {
                                candidates.push(impl_fn);
                            }
                            ImplItem::Const(impl_const) => {
                                candidates.push(impl_const);
                            }
                            ImplItem::Type(impl_type) => {
                                candidates.push(impl_type);
                            }
                            _ => {}
                        }
                    }
                }
                Item::Trait(item_trait) => {
                    for trait_item in &mut item_trait.items {
                        match trait_item {
                            TraitItem::Fn(trait_fn) => {
                                candidates.push(trait_fn);
                            }
                            TraitItem::Const(trait_const) => {
                                candidates.push(trait_const);
                            }
                            TraitItem::Type(trait_type) => {
                                candidates.push(trait_type);
                            }
                            _ => {}
                        }
                    }
                }
                _ => {}
            }
        }

        if candidates.is_empty() {
            return;
        }

        let mut rng = thread_rng();
        let target = candidates.choose(&mut rng).unwrap();

        let attr = Attribute {
            pound_token: token::Pound::default(),
            style: AttrStyle::Outer,
            bracket_token: token::Bracket::default(),
            meta: parse_quote!(repr(align)),
        };

        match target {
            ItemFn { attrs, .. } => {
                attrs.push(attr);
            }
            ItemStruct { attrs, .. } => {
                attrs.push(attr);
            }
            ItemEnum { attrs, .. } => {
                attrs.push(attr);
            }
            ItemUnion { attrs, .. } => {
                attrs.push(attr);
            }
            ImplItem::Fn(impl_fn) => {
                impl_fn.attrs.push(attr);
            }
            ImplItem::Const(impl_const) => {
                impl_const.attrs.push(attr);
            }
            ImplItem::Type(impl_type) => {
                impl_type.attrs.push(attr);
            }
            TraitItem::Fn(trait_fn) => {
                trait_fn.attrs.push(attr);
            }
            TraitItem::Const(trait_const) => {
                trait_const.attrs.push(attr);
            }
            TraitItem::Type(trait_type) => {
                trait_type.attrs.push(attr);
            }
            _ => {}
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator selects a random eligible item (function, method, struct, enum, union, trait item, or impl item) and adds a #[repr(align)] attribute without the required integer argument. This malformed attribute triggers internal compiler errors in attribute checking passes, testing the compiler's robustness against incomplete attribute specifications."
    }
}