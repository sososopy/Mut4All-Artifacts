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

pub struct Alter_Specialization_Impl_28;

impl Mutator for Alter_Specialization_Impl_28 {
    fn name(&self) -> &str {
        "Alter_Specialization_Impl_28"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Type(type_item) = impl_item {
                        if let Some(_defaultness) = &type_item.defaultness {
                            if let Type::Path(type_path) = &type_item.ty {
                                let last = type_path.path.segments.last().unwrap();
                                if last.ident == "Vec" {
                                    type_item.ty = parse_quote!(Option<Self>);
                                }
                            }
                        }
                    }
                }
            } else if let Item::Struct(item_struct) = item {
                if item_struct.ident == "Wrapper" {
                    continue;
                }
                let wrapper_struct: ItemStruct = parse_quote! {
                    struct Wrapper<T>(T);
                };
                file.items.push(Item::Struct(wrapper_struct));
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}