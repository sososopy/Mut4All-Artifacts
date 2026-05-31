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

pub struct Modify_Return_Type_To_Impl_Generator_38;

impl Mutator for Modify_Return_Type_To_Impl_Generator_38 {
    fn name(&self) -> &str {
        "Modify_Return_Type_To_Impl_Generator_38"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    *ty = Box::new(syn::Type::ImplTrait(syn::TypeImplTrait {
                        impl_token: token::Impl {
                            span: Span::call_site(),
                        },
                        bounds: {
                            let mut bounds = Punctuated::new();
                            bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                                paren_token: None,
                                modifier: syn::TraitBoundModifier::None,
                                lifetimes: None,
                                path: syn::Path {
                                    leading_colon: None,
                                    segments: {
                                        let mut segs = Punctuated::new();
                                        segs.push(syn::PathSegment {
                                            ident: Ident::new("Iterator", Span::call_site()),
                                            arguments: syn::PathArguments::None,
                                        });
                                        segs
                                    },
                                },
                            }));
                            bounds
                        },
                    }));
                    let yield_expr: Expr = parse_quote! { yield };
                    func.block.stmts.insert(0, Stmt::Expr(yield_expr));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator changes the return type of non-main functions to `impl Iterator` and introduces a `yield` statement at the beginning of the function body. This transformation is intended to test the compiler's handling of impl Trait returns and generator constructs, potentially leading to type resolution issues or ICEs due to the syntactically incorrect generator usage."
    }
}