use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatPath, PatStruct, PatTupleStruct, PatType, Path as SynPath,
    PathArguments, PathSegment, QSelf, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct Introduce_Qualified_Path_In_Pattern_140;

impl Mutator for Introduce_Qualified_Path_In_Pattern_140 {
    fn name(&self) -> &str {
        "Introduce_Qualified_Path_In_Pattern_140"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct PatternVisitor {
            changed: bool,
        }
        impl VisitMut for PatternVisitor {
            fn visit_pat_mut(&mut self, pat: &mut Pat) {
                match pat {
                    Pat::Path(pat_path) => {
                        if pat_path.path.segments.len() == 1 {
                            let segment = &pat_path.path.segments[0];
                            if segment.arguments.is_none() {
                                let ident = &segment.ident;
                                let new_path = SynPath {
                                    leading_colon: None,
                                    segments: {
                                        let mut segs = Punctuated::new();
                                        segs.push(PathSegment {
                                            ident: ident.clone(),
                                            arguments: PathArguments::None,
                                        });
                                        segs
                                    },
                                };
                                let qself = Some(QSelf {
                                    lt_token: token::Lt::default(),
                                    ty: Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: new_path.clone(),
                                    })),
                                    position: 0,
                                    as_token: None,
                                    gt_token: token::Gt::default(),
                                });
                                *pat = Pat::Path(PatPath {
                                    attrs: pat_path.attrs.clone(),
                                    qself,
                                    path: new_path,
                                });
                                self.changed = true;
                            }
                        }
                    }
                    Pat::Struct(pat_struct) => {
                        if pat_struct.path.segments.len() ==,1 {
                            let segment = &pat_struct.path.segments[0];
                            if segment.arguments.is_none() {
                                let ident = &segment.ident;
                                let new_path = SynPath {
                                    leading_colon: None,
                                    segments: {
                                        let mut segs = Punctuated::new();
                                        segs.push(PathSegment {
                                            ident: ident.clone(),
                                            arguments: PathArguments::None,
                                        });
                                        segs
                                    },
                                };
                                let qself = Some(QSelf {
                                    lt_token: token::Lt::default(),
                                    ty: Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: new_path.clone(),
                                    })),
                                    position: 0,
                                    as_token: None,
                                    gt_token: token::Gt::default(),
                                });
                                pat_struct.qself = qself;
                                pat_struct.path = new_path;
                                self.changed = true;
                            }
                        }
                    }
                    Pat::TupleStruct(pat_tuple_struct) => {
                        if pat_tuple_struct.path.segments.len() == 1 {
                            let segment = &pat_tuple_struct.path.segments[0];
                            if segment.arguments.is_none() {
                                let ident = &segment.ident;
                                let new_path = SynPath {
                                    leading_colon: None,
                                    segments: {
                                        let mut segs = Punctuated::new();
                                        segs.push(PathSegment {
                                            ident: ident.clone(),
                                            arguments: PathArguments::None,
                                        });
                                        segs
                                    },
                                };
                                let qself = Some(QSelf {
                                    lt_token: token::Lt::default(),
                                    ty: Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: new_path.clone(),
                                    })),
                                    position: 0,
                                    as_token: None,
                                    gt_token: token::Gt::default(),
                                });
                                pat_tuple_struct.qself = qself;
                                pat_tuple_struct.path = new_path;
                                self.changed = true;
                            }
                        }
                    }
                    _ => {}
                }
                syn::visit_mut::visit_pat_mut(self, pat);
            }
        }
        let mut visitor = PatternVisitor { changed: false };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a qualified path in pattern contexts by adding angle brackets around the enum type. It targets simple enum variant patterns (Path, Struct, TupleStruct) and transforms them into qualified paths like `<EnumName>::VariantName`. This stresses the compiler's handling of qualified paths in patterns, potentially exposing internal consistency errors or out-of-bounds slice accesses during HIR lowering or type checking."
    }
}