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

pub struct Introduce_Unstable_Feature_Misuse_72;

impl Mutator for Introduce_Unstable_Feature_Misuse_72 {
    fn name(&self) -> &str {
        "Introduce_Unstable_Feature_Misuse_72"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut has_arbitrary_enum_discriminant = false;
        let mut rng = thread_rng();

        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                let mut has_custom_discriminants = false;
                for variant in &item_enum.variants {
                    if variant.discriminant.is_some() {
                        has_custom_discriminants = true;
                        break;
                    }
                }

                if !has_custom_discriminants {
                    for variant in &mut item_enum.variants {
                        let discriminant_value: i32 = rng.gen_range(-100..100);
                        variant.discriminant = Some((
                            token::Eq { spans: [Span::call_site()] },
                            Expr::Lit(ExprLit {
                                attrs: Vec::new(),
                                lit: syn::Lit::Int(syn::LitInt::new(&discriminant_value.to_string(), Span::call_site())),
                            }),
                        ));
                    }
                }

                if !has_arbitrary_enum_discriminant {
                    file.items.insert(0, Item::Verbatim(quote! {
                        #![feature(arbitrary_enum_discriminant)]
                    }));
                    has_arbitrary_enum_discriminant = true;
                }
            }
        }

        if !has_arbitrary_enum_discriminant {
            file.items.insert(0, Item::Verbatim(quote! {
                #![feature(arbitrary_enum_discriminant)]
            }));
        }

        let faulty_enum = parse_quote! {
            enum Faulty {
                C = 0,
                D(!),
            }
        };

        file.items.push(Item::Enum(faulty_enum));

        let usage_fn = parse_quote! {
            fn usage() {
                let _ = Faulty::D; // Improper use of `!` type causes instability
            }
        };

        file.items.push(Item::Fn(usage_fn));
    }

    fn chain_of_thought(&self) -> &str {
        ""
    }
}