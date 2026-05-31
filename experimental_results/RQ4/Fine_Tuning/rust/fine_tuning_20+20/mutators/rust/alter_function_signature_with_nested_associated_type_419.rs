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

pub struct Alter_Function_Signature_With_Nested_Associated_Type_419;

impl Mutator for Alter_Function_Signature_With_Nested_Associated_Type_419 {
    fn name(&self) -> &str {
        "Alter_Function_Signature_With_Nested_Associated_Type_419"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut has_associated_type = false;
                for input in &item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if type_path.path.segments.iter().any(|seg| {
                                matches!(seg.arguments, PathArguments::AngleBracketed(_))
                            }) {
                                has_associated_type = true;
                                break;
                            }
                        }
                    }
                }
                if has_associated_type {
                    let mut new_inputs = Punctuated::new();
                    for input in &item_fn.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Path(type_path) = &*pat_type.ty {
                                let mut new_segments = Punctuated::new();
                                for seg in &type_path.path.segments {
                                    if let PathArguments::AngleBracketed(angle_args) =
                                        &seg.arguments
                                    {
                                        let mut new_args = Punctuated::new();
                                        for arg in &angle_args.args {
                                            if let GenericArgument::Type(Type::Path(type_path)) =
                                                arg
                                            {
                                                let mut new_segments = Punctuated::new();
                                                for seg in &type_path.path.segments {
                                                    if seg.ident == "Type" {
                                                        new_segments.push(seg.clone());
                                                        new_segments.push(seg.clone());
                                                    } else {
                                                        new_segments.push(seg.clone());
                                                    }
                                                }
                                                let new_type_path = TypePath {
                                                    qself: type_path.qself.clone(),
                                                    path: SynPath {
                                                        leading_colon: type_path
                                                            .path
                                                            .leading_colon,
                                                        segments: new_segments,
                                                    },
                                                };
                                                new_args.push(GenericArgument::Type(
                                                    Type::Path(new_type_path),
                                                ));
                                            } else {
                                                new_args.push(arg.clone());
                                            }
                                        }
                                        let new_angle_args = AngleBracketedGenericArguments {
                                            colon2_token: angle_args.colon2_token,
                                            lt_token: angle_args.lt_token,
                                            args: new_args,
                                            gt_token: angle_args.gt_token,
                                        };
                                        new_segments.push(PathSegment {
                                            ident: seg.ident.clone(),
                                            arguments: PathArguments::AngleBracketed(new_angle_args),
                                        });
                                    } else {
                                        new_segments.push(seg.clone());
                                    }
                                }
                                let new_type_path = TypePath {
                                    qself: type_path.qself.clone(),
                                    path: SynPath {
                                        leading_colon: type_path.path.leading_colon,
                                        segments: new_segments,
                                    },
                                };
                                new_inputs.push(FnArg::Typed(PatType {
                                    attrs: pat_type.attrs.clone(),
                                    pat: pat_type.pat.clone(),
                                    colon_token: pat_type.colon_token,
                                    ty: Box::new(Type::Path(new_type_path)),
                                }));
                            } else {
                                new_inputs.push(input.clone());
                            }
                        } else {
                            new_inputs.push(input.clone());
                        }
                    }
                    item_fn.sig.inputs = new_inputs;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets functions with associated types in their signatures. It modifies the function signature by deepening the nesting of associated types, particularly focusing on those involving bound lifetimes. This transformation aims to stress the compiler's handling of bound regions and associated type resolution, increasing the likelihood of triggering ICEs related to lifetime inference and type substitution."
    }
}