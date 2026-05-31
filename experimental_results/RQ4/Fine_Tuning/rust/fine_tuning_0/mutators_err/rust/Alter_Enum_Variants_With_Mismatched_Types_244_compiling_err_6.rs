use proc_macro2::{Span, DelimSpan};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemEnum, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Alter_Enum_Variants_With_Mismatched_Types_244;

impl Mutator for Alter_Enum_Variants_With_Mismatched_Types_244 {
    fn name(&self) -> &str {
        "Alter_Enum_Variants_With_Mismatched_Types_244"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                let mut rng = thread_rng();
                let new_variant_name = Ident::new("MismatchedVariant", Span::call_site());
                let new_variant = syn::Variant {
                    ident: new_variant_name.clone(),
                    fields: syn::Fields::Unnamed(syn::FieldsUnnamed {
                        paren_token: Paren { span: DelimSpan::from_single(Span::call_site()) },
                        unnamed: Punctuated::from_iter(vec![
                            syn::Field {
                                attrs: Vec::new(),
                                vis: syn::Visibility::Inherited,
                                ident: None,
                                colon_token: None,
                                ty: parse_quote!(isize),
                                mutability: None,
                            },
                            syn::Field {
                                attrs: Vec::new(),
                                vis: syn::Visibility::Inherited,
                                ident: None,
                                colon_token: None,
                                ty: parse_quote!(isize),
                                mutability: None,
                            },
                        ]),
                    }),
                    discriminant: None,
                    attrs: Vec::new(),
                };
                item_enum.variants.push(new_variant);

                for item in &mut file.items {
                    if let Item::Fn(item_fn) = item {
                        for stmt in &mut item_fn.block.stmts {
                            if let Stmt::Expr(Expr::Match(expr_match), _) = stmt {
                                for arm in &mut expr_match.arms {
                                    if let Pat::TupleStruct(pat_tuple_struct) = &mut arm.pat {
                                        if pat_tuple_struct.path.segments.last().unwrap().ident == item_enum.ident {
                                            let new_pat = Pat::TupleStruct(PatTupleStruct {
                                                attrs: Vec::new(),
                                                path: SynPath {
                                                    leading_colon: None,
                                                    segments: {
                                                        let mut segments = pat_tuple_struct.path.segments.clone();
                                                        segments.pop();
                                                        segments.push(syn::PathSegment {
                                                            ident: new_variant_name.clone(),
                                                            arguments: syn::PathArguments::None,
                                                        });
                                                        segments
                                                    },
                                                },
                                                qself: None,
                                                paren_token: Paren { span: DelimSpan::from_single(Span::call_site()) },
                                                elems: Punctuated::from_iter(vec![
                                                    Pat::Ident(syn::PatIdent {
                                                        attrs: Vec::new(),
                                                        by_ref: None,
                                                        mutability: None,
                                                        ident: Ident::new("x", Span::call_site()),
                                                        subpat: None,
                                                    }),
                                                    Pat::Ident(syn::PatIdent {
                                                        attrs: Vec::new(),
                                                        by_ref: None,
                                                        mutability: None,
                                                        ident: Ident::new("y", Span::call_site()),
                                                        subpat: None,
                                                    }),
                                                ]),
                                            });
                                            arm.pat = new_pat;
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
        "This mutator identifies enum definitions and introduces a new variant with a mismatched arity compared to existing ones. It then modifies pattern match arms to use this new variant, deliberately creating a mismatch in expected arity to test the compiler's handling of pattern matching and arity analysis."
    }
}