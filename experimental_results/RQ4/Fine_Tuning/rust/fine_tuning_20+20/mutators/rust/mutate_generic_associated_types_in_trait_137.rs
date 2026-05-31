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

pub struct Mutate_Generic_Associated_Types_In_Trait_137;

impl Mutator for Mutate_Generic_Associated_Types_In_Trait_137 {
    fn name(&self) -> &str {
        "Mutate_Generic_Associated_Types_In_Trait_137"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                if item_trait.ident != "Base" {
                    continue;
                }
                for item in &mut item_trait.items {
                    if let TraitItem::Type(ty) = item {
                        if ty.ident == "Base" {
                            ty.generics
                                .params
                                .push(GenericParam::Type(parse_quote!(Extra)));
                        }
                    }
                }
                for item in &mut item_trait.items {
                    if let TraitItem::Fn(func) = item {
                        if func.sig.ident == "fmap" {
                            func.sig
                                .generics
                                .params
                                .push(GenericParam::Type(parse_quote!(Extra)));
                            if let ReturnType::Type(_, ref mut ty) = func.sig.output {
                                *ty = Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: {
                                            let mut segs = Punctuated::new();
                                            segs.push(PathSegment {
                                                ident: Ident::new("Self", Span::call_site()),
                                                arguments: PathArguments::None,
                                            });
                                            segs.push(PathSegment {
                                                ident: Ident::new("Base", Span::call_site()),
                                                arguments: PathArguments::AngleBracketed(
                                                    AngleBracketedGenericArguments {
                                                        colon2_token: None,
                                                        lt_token: token::Lt::default(),
                                                        args: {
                                                            let mut args = Punctuated::new();
                                                            args.push(GenericArgument::Type(
                                                                Type::Path(TypePath {
                                                                    qself: None,
                                                                    path: SynPath::from(Ident::new(
                                                                        "B",
                                                                        Span::call_site(),
                                                                    )),
                                                                }),
                                                            ));
                                                            args.push(GenericArgument::Type(
                                                                Type::Path(TypePath {
                                                                    qself: None,
                                                                    path: SynPath::from(Ident::new(
                                                                        "Extra",
                                                                        Span::call_site(),
                                                                    )),
                                                                }),
                                                            ));
                                                            args
                                                        },
                                                        gt_token: token::Gt::default(),
                                                    },
                                                ),
                                            });
                                            segs
                                        },
                                    },
                                }));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets traits with generic associated types (GATs), specifically modifying the `Base` trait to introduce an additional generic parameter `Extra` in its associated type. It then updates the `fmap` function to utilize this new parameter in its return type, transforming `Self::Base<B>` into `Self::Base<B, Extra>`. This mutation stresses the compiler's handling of GATs by altering the trait's type signature and return type expectations, potentially exposing bugs in generic type resolution and trait implementation."
    }
}