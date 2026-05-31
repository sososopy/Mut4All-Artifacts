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

pub struct Modify_Generator_Return_Type_11;

impl Mutator for Modify_Generator_Return_Type_11 {
    fn name(&self) -> &str {
        "Modify_Generator_Return_Type_11"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Type(impl_type) = impl_item {
                        if impl_type.ident == "Gen" {
                            if let Type::ImplTrait(type_impl_trait) = &impl_type.ty {
                                let mut new_bounds = Punctuated::new();
                                for bound in &type_impl_trait.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        if trait_bound.path.is_ident("Generator") {
                                            let mut new_trait_bound = trait_bound.clone();
                                            for segment in &mut new_trait_bound.path.segments {
                                                if segment.ident == "Generator" {
                                                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                                        for arg in &mut args.args {
                                                            if let GenericArgument::Type(Type::Path(type_path)) = arg {
                                                                if type_path.path.segments.last().unwrap().ident == "Return" {
                                                                    *arg = parse_quote!(Option<()>);
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                            new_bounds.push(TypeParamBound::Trait(new_trait_bound));
                                        } else {
                                            new_bounds.push(bound.clone());
                                        }
                                    } else {
                                        new_bounds.push(bound.clone());
                                    }
                                }
                                impl_type.ty = Box::new(Type::ImplTrait(TypeImplTrait {
                                    impl_token: token::Impl { span: Span::call_site() },
                                    bounds: new_bounds,
                                }));
                            }
                        }
                    }
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if let syn::ReturnType::Type(_, ref mut ty) = method.sig.output {
                            if let Type::ImplTrait(type_impl_trait) = &**ty {
                                for bound in &type_impl_trait.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        if trait_bound.path.is_ident("Generator") {
                                            let mut new_block = method.block.clone();
                                            new_block.stmts.push(parse_quote!(Some(())));
                                            method.block = new_block;
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
        ""
    }
}