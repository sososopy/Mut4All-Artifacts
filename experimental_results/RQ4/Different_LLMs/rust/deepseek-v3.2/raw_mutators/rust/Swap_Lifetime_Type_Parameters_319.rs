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

pub struct Swap_Lifetime_Type_Parameters_319;

impl Mutator for Swap_Lifetime_Type_Parameters_319 {
    fn name(&self) -> &str {
        "Swap_Lifetime_Type_Parameters_319"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                let mut trait_generics = Vec::new();
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if let Some(segment) = path.segments.last() {
                        if let PathArguments::AngleBracketed(args) = &segment.arguments {
                            for arg in &args.args {
                                match arg {
                                    GenericArgument::Type(_) => trait_generics.push(GenericParamKind::Type),
                                    GenericArgument::Lifetime(_) => trait_generics.push(GenericParamKind::Lifetime),
                                    GenericArgument::Const(_) => trait_generics.push(GenericParamKind::Const),
                                    _ => {}
                                }
                            }
                        }
                    }
                }
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let mut new_params = Punctuated::<GenericParam, Comma>::new();
                        let mut type_params: Vec<TypeParam> = Vec::new();
                        let mut lifetime_params: Vec<LifetimeParam> = Vec::new();
                        let mut const_params: Vec<ConstParam> = Vec::new();
                        for param in &func.sig.generics.params {
                            match param {
                                GenericParam::Type(tp) => type_params.push(tp.clone()),
                                GenericParam::Lifetime(lp) => lifetime_params.push(lp.clone()),
                                GenericParam::Const(cp) => const_params.push(cp.clone()),
                            }
                        }
                        let mut type_iter = type_params.iter();
                        let mut lifetime_iter = lifetime_params.iter();
                        let mut const_iter = const_params.iter();
                        for kind in &trait_generics {
                            match kind {
                                GenericParamKind::Type => {
                                    if let Some(lp) = lifetime_iter.next() {
                                        new_params.push(GenericParam::Lifetime(lp.clone()));
                                    } else if let Some(tp) = type_iter.next() {
                                        new_params.push(GenericParam::Type(tp.clone()));
                                    } else {
                                        let fresh_lifetime = LifetimeParam {
                                            attrs: Vec::new(),
                                            lifetime: Lifetime::new(&format!("'a{}", thread_rng().gen::<u32>()), Span::call_site()),
                                            colon_token: None,
                                            bounds: Punctuated::new(),
                                        };
                                        new_params.push(GenericParam::Lifetime(fresh_lifetime));
                                    }
                                }
                                GenericParamKind::Lifetime => {
                                    if let Some(tp) = type_iter.next() {
                                        new_params.push(GenericParam::Type(tp.clone()));
                                    } else if let Some(lp) = lifetime_iter.next() {
                                        new_params.push(GenericParam::Lifetime(lp.clone()));
                                    } else {
                                        let fresh_type = TypeParam {
                                            attrs: Vec::new(),
                                            ident: Ident::new(&format!("T{}", thread_rng().gen::<u32>()), Span::call_site()),
                                            colon_token: None,
                                            bounds: Punctuated::new(),
                                            eq_token: None,
                                            default: None,
                                        };
                                        new_params.push(GenericParam::Type(fresh_type));
                                    }
                                }
                                GenericParamKind::Const => {
                                    if let Some(cp) = const_iter.next() {
                                        new_params.push(GenericParam::Const(cp.clone()));
                                    } else {
                                        let fresh_const = ConstParam {
                                            attrs: Vec::new(),
                                            const_token: token::Const { span: Span::call_site() },
                                            ident: Ident::new(&format!("C{}", thread_rng().gen::<u32>()), Span::call_site()),
                                            colon_token: token::Colon { spans: [Span::call_site()] },
                                            ty: Type::Path(TypePath {
                                                qself: None,
                                                path: SynPath::from(Ident::new("usize", Span::call_site())),
                                            }),
                                            eq_token: None,
                                            default: None,
                                        };
                                        new_params.push(GenericParam::Const(fresh_const));
                                    }
                                }
                            }
                        }
                        func.sig.generics.params = new_params;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps lifetime and type generic parameters in trait implementation method signatures relative to the trait declaration. It analyzes the trait's generic parameters to determine the expected order, then reorders the impl method's parameters by swapping lifetimes and types while preserving const parameters. When mismatches occur, fresh parameters are generated. This transformation aims to expose internal compiler errors during generic argument instantiation and type-checking of mismatched parameter kinds."
    }
}

enum GenericParamKind {
    Type,
    Lifetime,
    Const,
}