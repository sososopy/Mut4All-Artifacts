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

pub struct Modify_Pattern_Matching_In_Match_Statements_202;

impl Mutator for Modify_Pattern_Matching_In_Match_Statements_202 {
    fn name(&self) -> &str {
        "Modify_Pattern_Matching_In_Match_Statements_202"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct MatchVisitor {
            replacements: Vec<(Span, Expr)>,
        }
        impl Visit<'_> for MatchVisitor {
            fn visit_expr_match(&mut self, node: &ExprMatch) {
                let mut new_arms = Vec::new();
                for arm in &node.arms {
                    let new_pat = match &arm.pat {
                        Pat::Ident(pat_ident) => {
                            let ident_str = pat_ident.ident.to_string();
                            let new_expr: Expr = match ident_str.as_str() {
                                "FOO_REF_REF" => parse_quote! { (10, 0) },
                                _ => parse_quote! { (0, 0) },
                            };
                            self.replacements.push((arm.body.span(), new_expr.clone()));
                            Pat::Tuple(PatTuple {
                                attrs: Vec::new(),
                                paren_token: Paren(Span::call_site()),
                                elems: Punctuated::from_iter(vec![Pat::TupleStruct(
                                    PatTupleStruct {
                                        attrs: Vec::new(),
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![
                                                PathSegment {
                                                    ident: Ident::new("Tuple", Span::call_site()),
                                                    arguments: PathArguments::None,
                                                },
                                            ]),
                                        },
                                        qself: None,
                                        paren_token: Paren(Span::call_site()),
                                        elems: Punctuated::from_iter(vec![Pat::Wild(PatWild {
                                            attrs: Vec::new(),
                                            underscore_token: token::Underscore(Span::call_site()),
                                        })]),
                                    },
                                )]),
                            })
                        }
                        Pat::Tuple(pat_tuple) => {
                            let mut new_elems = Vec::new();
                            for elem in &pat_tuple.elems {
                                let new_elem = match elem {
                                    Pat::Range(pat_range) => {
                                        let new_range: Pat = match &pat_range.end {
                                            syn::ExprRangeEnd::Included(_) => parse_quote! { 0..=100 },
                                            syn::ExprRangeEnd::Excluded(_) => parse_quote! { 0..100 },
                                        };
                                        new_range
                                    }
                                    _ => elem.clone(),
                                };
                                new_elems.push(new_elem);
                            }
                            Pat::Tuple(PatTuple {
                                attrs: pat_tuple.attrs.clone(),
                                paren_token: pat_tuple.paren_token,
                                elems: Punctuated::from_iter(new_elems),
                            })
                        }
                        _ => arm.pat.clone(),
                    };
                    new_arms.push(Arm {
                        attrs: arm.attrs.clone(),
                        pat: new_pat,
                        guard: arm.guard.clone(),
                        fat_arrow_token: arm.fat_arrow_token,
                        body: arm.body.clone(),
                        comma: arm.comma,
                    });
                }
                let new_match = ExprMatch {
                    attrs: node.attrs.clone(),
                    match_token: node.match_token,
                    expr: node.expr.clone(),
                    brace_token: node.brace_token,
                    arms: new_arms,
                };
                self.replacements.push((node.span(), parse_quote! { #new_match }));
            }
        }
        let mut visitor = MatchVisitor {
            replacements: Vec::new(),
        };
        visitor.visit_file(file);
        for (span, replacement) in visitor.replacements {
            let _ = std::panic::catch_unwind(|| {
                let _ = replacement.to_tokens(&mut proc_macro2::TokenStream::new());
            });
            let _ = std::panic::catch_unwind(|| {
                let _ = replacement.to_token_stream();
            });
            let _ = std::panic::catch_unwind(|| {
                let _ = replacement.clone().into_token_stream();
            });
            let _ = std::panic::catch_unwind(|| {
                let _ = quote::quote!(#replacement);
            });
            let _ = std::panic::catch_unwind(|| {
                let _ = quote::quote_spanned!(span => #replacement);
            });
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets `match` statements with complex patterns, particularly those using ranges or bindings. It replaces range patterns with overlapping ranges or single values and substitutes bindings with explicit values that may overlap with other patterns. This transformation aims to create pattern overlaps or redundancies, provoking compiler warnings or errors related to pattern exhaustiveness and unreachable patterns, thereby increasing the likelihood of triggering compiler bugs."
    }
}