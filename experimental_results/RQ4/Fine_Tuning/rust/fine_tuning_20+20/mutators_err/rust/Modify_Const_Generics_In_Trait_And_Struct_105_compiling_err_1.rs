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

pub struct Modify_Const_Generics_In_Trait_And_Struct_105;

impl Mutator for Modify_Const_Generics_In_Trait_And_Struct_105 {
    fn name(&self) -> &str {
        "Modify_Const_Generics_In_Trait_And_Struct_105"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                if item_trait.ident == "ConstChunksExactTrait" {
                    for item in &mut item_trait.items {
                        if let TraitItem::Fn(item_fn) = item {
                            if let Some(FnArg::Typed(pat_type)) = item_fn.sig.inputs.first() {
                                if let Type::Path(type_path) = &*pat_type.ty {
                                    if type_path.path.is_ident("usize") {
                                        let mut new_inputs = Punctuated::new();
                                        new_inputs.push(FnArg::Typed(PatType {
                                            attrs: Vec::new(),
                                            pat: Box::new(Pat::Ident(PatIdent {
                                                attrs: Vec::new(),
                                                by_ref: None,
                                                mutability: None,
                                                ident: Ident::new("M", Span::call_site()),
                                                subpat: None,
                                            })),
                                            colon_token: Default::default(),
                                            ty: Box::new(Type::Path(TypePath {
                                                qself: None,
                                                path: SynPath::from(Ident::new(
                                                    "usize",
                                                    Span::call_site(),
                                                )),
                                            })),
                                        }));
                                        item_fn.sig.generics.params = new_inputs;
                                    }
                                }
                            }
                            if let ReturnType::Type(_, ty) = &mut item_fn.sig.output {
                                if let Type::Path(type_path) = &mut **ty {
                                    if let Some(last_segment) =
                                        type_path.path.segments.last_mut()
                                    {
                                        if last_segment.ident == "ConstChunksExact" {
                                            if let PathArguments::AngleBracketed(
                                                angle_bracketed,
                                            ) = &mut last_segment.arguments
                                            {
                                                if let Some(GenericArgument::Const(expr)) =
                                                    angle_bracketed.args.last()
                                                {
                                                    if let Expr::Path(expr_path) = expr {
                                                        if expr_path
                                                            .path
                                                            .is_ident("N")
                                                        {
                                                            angle_bracketed.args.pop();
                                                            angle_bracketed.args.push(
                                                                GenericArgument::Const(
                                                                    Expr::Path(ExprPath {
                                                                        attrs: Vec::new(),
                                                                        qself: None,
                                                                        path: SynPath::from(
                                                                            Ident::new(
                                                                                "N",
                                                                                Span::call_site(),
                                                                            ),
                                                                        ),
                                                                    }),
                                                                ),
                                                            );
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
            if let Item::Struct(item_struct) = item {
                if item_struct.ident == "ConstChunksExact" {
                    let mut new_params = Punctuated::new();
                    new_params.push(GenericParam::Lifetime(LifetimeParam {
                        attrs: Vec::new(),
                        lifetime: Lifetime::new("'rem", Span::call_site()),
                        colon_token: None,
                        bounds: Punctuated::new(),
                    }));
                    new_params.push(GenericParam::Type(TypeParam {
                        attrs: Vec::new(),
                        ident: Ident::new("T", Span::call_site()),
                        colon_token: Some(Default::default()),
                        bounds: {
                            let mut bounds = Punctuated::new();
                            bounds.push(TypeParamBound::Lifetime(Lifetime::new(
                                "'a",
                                Span::call_site(),
                            )));
                            bounds
                        },
                        eq_token: None,
                        default: None,
                    }));
                    new_params.push(GenericParam::Const(ConstParam {
                        attrs: Vec::new(),
                        const_token: Default::default(),
                        ident: Ident::new("M", Span::call_site()),
                        colon_token: Default::default(),
                        ty: Type::Path(TypePath {
                            qself: None,
                            path: SynPath::from(Ident::new("usize", Span::call_site())),
                        }),
                        eq_token: None,
                        default: None,
                    }));
                    item_struct.generics.params = new_params;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets traits and structs that use const generics, specifically those related to the `ConstChunksExactTrait` and `ConstChunksExact` struct. It modifies the const generic parameter in the trait method signature and the struct definition to introduce a mismatch between the declared const parameter and its usage. This transformation tests the compiler's ability to handle const generic inconsistencies, particularly in the context of trait method signatures and struct definitions, by forcing it to resolve const parameters that are declared differently from their usage."
    }
}