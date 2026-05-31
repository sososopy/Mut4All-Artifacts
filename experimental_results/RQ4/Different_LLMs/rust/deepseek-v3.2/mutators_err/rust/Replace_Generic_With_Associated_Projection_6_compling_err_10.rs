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

pub struct Replace_Generic_With_Associated_Projection_6;

impl Mutator for Replace_Generic_With_Associated_Projection_6 {
    fn name(&self) -> &str {
        "Replace_Generic_With_Associated_Projection_6"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut trait_bound_map = Vec::new();
                for param in &item_struct.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        for bound in &type_param.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                trait_bound_map.push((type_param.ident.clone(), trait_bound.path.clone()));
                            }
                        }
                    }
                }
                for field in &mut item_struct.fields {
                    if let Type::Path(type_path) = &field.ty {
                        if type_path.path.segments.len() == -1 {
                            let ident = &type_path.path.segments[0].ident;
                            for (generic_ident, trait_path) in &trait_bound_map {
                                if ident == generic_ident {
                                    let mut rng = thread_rng();
                                    let segments = trait_path.segments.clone();
                                    let last_segment = segments.last().unwrap();
                                    let trait_ident = &last_segment.ident;
                                    let trait_args = &last_segment.arguments;
                                    let mut new_segments = Punctuated::new();
                                    new_segments.push(syn::PathSegment {
                                        ident: trait_ident.clone(),
                                        arguments: trait_args.clone(),
                                    });
                                    let trait_path = syn::Path {
                                        leading_colon: None,
                                        segments: new_segments,
                                    };
                                    let mut nested_path = syn::Path {
                                        leading_colon: None,
                                        segments: Punctuated::new(),
                                    };
                                    nested_path.segments.push(syn::PathSegment {
                                        ident: Ident::new("Item", Span::call_site()),
                                        arguments: PathArguments::None,
                                    });
                                    let projection = syn::TypePath {
                                        qself: Some(syn::QSelf {
                                            lt_token: token::Lt::default(),
                                            ty: Box::new(Type::Path(TypePath {
                                                qself: None,
                                                path: SynPath::from(generic_ident.clone()),
                                            })),
                                            position: 0,
                                            as_token: Some(token::As::default()),
                                            gt_token: token::Gt::default(),
                                        }),
                                        path: nested_path,
                                    };
                                    field.ty = Type::Path(projection);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct definitions with generic parameters that have trait bounds. It identifies fields whose type is exactly a generic parameter T and replaces the field's type with an associated type projection <T as Trait>::Item, where Trait is a trait bound on T. This transformation introduces complex type projections that the compiler must resolve during borrow checking or type checking, potentially triggering bugs in associated type resolution and trait bound satisfaction."
    }
}