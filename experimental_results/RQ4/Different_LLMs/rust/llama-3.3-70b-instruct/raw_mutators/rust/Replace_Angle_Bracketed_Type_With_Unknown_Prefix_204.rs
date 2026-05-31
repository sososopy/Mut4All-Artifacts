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

pub struct Replace_Angle_Bracketed_Type_With_Unknown_Prefix_204;

impl Mutator for Replace_Angle_Bracketed_Type_With_Unknown_Prefix_204 {
    fn name(&self) -> &str {
        "Replace_Angle_Bracketed_Type_With_Unknown_Prefix_204"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, ref mut ty) = &mut func.sig.output {
                    self.replace_angle_bracketed_type(ty);
                }
                for arg in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = arg {
                        self.replace_angle_bracketed_type(&mut pat_type.ty);
                    }
                }
            } else if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let syn::ReturnType::Type(_, ref mut ty) = &mut func.sig.output {
                            self.replace_angle_bracketed_type(ty);
                        }
                        for arg in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = arg {
                                self.replace_angle_bracketed_type(&mut pat_type.ty);
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic type parameters in Rust code. It replaces angle-bracketed types with an unknown prefix. This transformation simulates bugs related to the Rust compiler's handling of generic types and their prefixes."
    }
}

impl Replace_Angle_Bracketed_Type_With_Unknown_Prefix_204 {
    fn replace_angle_bracketed_type(&self, ty: &mut syn::Type) {
        if let syn::Type::Path(TypePath {
            path: SynPath { segments, .. },
            ..
        }) = ty
        {
            if let Some(segment) = segments.iter().next() {
                if let PathArguments::AngleBracketed(args) = &segment.arguments {
                    let mut rng = thread_rng();
                    let prefix: String = (0..10)
                        .map(|_| rng.gen_range('a'..='z'))
                        .collect();
                    let new_ident = Ident::new(&prefix, Span::call_site());
                    let new_segment = PathSegment {
                        ident: new_ident,
                        arguments: PathArguments::None,
                    };
                    *segments = Punctuated::from_iter(vec![new_segment]);
                }
            }
        }
    }
}