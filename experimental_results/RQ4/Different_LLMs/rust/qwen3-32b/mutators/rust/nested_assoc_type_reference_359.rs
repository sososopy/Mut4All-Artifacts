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

pub struct Nested_Assoc_Type_Reference_359;

impl Mutator for Nested_Assoc_Type_Reference_359 {
    fn name(&self) -> &str {
        "Nested_Assoc_Type_Reference_359"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct NestedAssocTypeVisitor {
            in_trait_impl: bool,
        }
        impl<'ast> VisitMut for NestedAssocTypeVisitor {
            fn visit_item_impl_mut(&mut self, i: &mut ItemImpl) {
                if let Some((_, trait_path, _)) = &i.trait_ {
                    self.in_trait_impl = true;
                    syn::visit_mut::visit_item_impl_mut(self, i);
                    self.in_trait_impl = false;
                } else {
                    syn::visit_mut::visit_item_impl_mut(self, i);
                }
            }
            fn visit_type_path_mut(&mut self, i: &mut TypePath) {
                if self.in_trait_impl {
                    if let Some(qself) = &i.qself {
                        let original_path = i.path.clone();
                        let new_qself = QSelf {
                            lt_token: token::Lt::default(),
                            position: 0,
                            ty: Box::new(syn::Type::Path(TypePath {
                                qself: None,
                                path: original_path,
                            })),
                            as_token: Some(token::As::default()),
                            gt_token: token::Gt::default(),
                        };
                        i.qself = Some(new_qself);
                        i.path = SynPath {
                            leading_colon: None,
                            segments: {
                                let mut segs = Punctuated::new();
                                segs.push(PathSegment {
                                    ident: Ident::new("Item", Span::call_site()),
                                    arguments: PathArguments::None,
                                });
                                segs
                            },
                        };
                    }
                }
                syn::visit_mut::visit_type_path_mut(self, i);
            }
        }
        let mut visitor = NestedAssocTypeVisitor {
            in_trait_impl: false,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}