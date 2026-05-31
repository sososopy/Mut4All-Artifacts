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

pub struct TransmuteMismatchedSize_223;

impl Mutator for TransmuteMismatchedSize_223 {
    fn name(&self) -> &str {
        "TransmuteMismatchedSize_223"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct TransmuteVisitor;
        impl VisitMut for TransmuteVisitor {
            fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
                if let Expr::Path(expr_path) = &node.func {
                    if let Some(segment) = expr_path.path.segments.last() {
                        if segment.ident == "transmute" {
                            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                if args.args.len() == 2 {
                                    // Replace source type with Option<T>
                                    if let GenericArgument::Type(ty) = &args.args[0] {
                                        let inner_type = ty.clone();
                                        let option_type = Type::Path(TypePath {
                                            qself: None,
                                            path: SynPath {
                                                leading_colon: None,
                                                segments: {
                                                    let mut segments = Punctuated::new();
                                                    segments.push(PathSegment {
                                                        ident: Ident::new("Option", Span::call_site()),
                                                        arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                                            colon2_token: None,
                                                            lt_token: token::Lt::default(),
                                                            args: Punctuated::from_iter(vec![GenericArgument::Type(inner_type)]),
                                                            gt_token: token::Gt::default(),
                                                        }),
                                                    });
                                                    segments
                                                },
                                            },
                                        });
                                        args.args[0] = Box::new(GenericArgument::Type(option_type));
                                    }
                                    // Replace target type with u64
                                    if let GenericArgument::Type(ty) = &args.args[1] {
                                        let u64_type = Type::Path(TypePath {
                                            qself: None,
                                            path: SynPath {
                                                leading_colon: None,
                                                segments: {
                                                    let mut segments = Punctuated::new();
                                                    segments.push(PathSegment {
                                                        ident: Ident::new("u64", Span::call_site()),
                                                        arguments: PathArguments::None,
                                                    });
                                                    segments
                                                },
                                            },
                                        });
                                        args.args[1] = Box::new(GenericArgument::Type(u64_type));
                                    }
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_call_mut(self, node);
            }
        }
        let mut visitor = TransmuteVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets transmute calls with same-sized types. It replaces the source type with Option<T> (introducing a niche) and the target type with a larger primitive like u64, creating a size/alignment mismatch. This may trigger MIR optimization errors during codegen by violating type layout assumptions."
    }
}