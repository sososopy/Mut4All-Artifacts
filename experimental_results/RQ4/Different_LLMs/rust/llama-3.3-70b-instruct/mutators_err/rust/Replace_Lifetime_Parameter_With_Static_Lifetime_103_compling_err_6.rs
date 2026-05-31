use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Replace_Lifetime_Parameter_With_Static_Lifetime_103;

impl Mutator for Replace_Lifetime_Parameter_With_Static_Lifetime_103 {
    fn name(&self) -> &str {
        "Replace_Lifetime_Parameter_With_Static_Lifetime_103"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, _) = &func.sig.output {
                    for param in &mut func.sig.inputs {
                        if let FnArg::Typed(pat_type) = param {
                            let ty = &*pat_type.ty;
                            if let Type::Path(TypePath {
                                qself: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments,
                                },
                            }) = ty
                            {
                                for segment in segments {
                                    if let PathSegment {
                                        ident,
                                        arguments:
                                            PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                                colon2_token: None,
                                                lt_token: _,
                                                args,
                                                gt_token: _,
                                            }),
                                    } = segment
                                    {
                                        for arg in args {
                                            if let GenericArgument::Lifetime(Lifetime {
                                                apostrophe: _,
                                                ident: ident_ref,
                                            }) = arg
                                            {
                                                let new_ident = Ident::new("static", Span::call_site());
                                                let new_lifetime = Lifetime {
                                                    apostrophe: Span::call_site(), 
                                                    ident: new_ident,
                                                };
                                                let new_arg = GenericArgument::Lifetime(new_lifetime);
                                                let new_args = args.iter().map(|a| {
                                                    if let GenericArgument::Lifetime(lifetime) = a {
                                                        if *lifetime.ident == *ident_ref {
                                                            new_arg.clone()
                                                        } else {
                                                            a.clone()
                                                        }
                                                    } else {
                                                        a.clone()
                                                    }
                                                }).collect::<Vec<_>>();
                                                let new_angle_bracketed = AngleBracketedGenericArguments {
                                                    colon2_token: None,
                                                    lt_token: Default::default(),
                                                    args: Punctuated::from_iter(new_args),
                                                    gt_token: Default::default(),
                                                };
                                                let new_path_arguments = PathArguments::AngleBracketed(new_angle_bracketed);
                                                let new_path_segment = PathSegment {
                                                    ident: ident.clone(),
                                                    arguments: new_path_arguments,
                                                };
                                                let new_segments = segments.iter().map(|s| {
                                                    if let PathSegment {
                                                        ident: ident_s,
                                                        arguments: _,
                                                    } = s {
                                                        if *ident_s == *ident {
                                                            new_path_segment.clone()
                                                        } else {
                                                            s.clone()
                                                        }
                                                    } else {
                                                        s.clone()
                                                    }
                                                }).collect::<Vec<_>>();
                                                let new_path = SynPath {
                                                    leading_colon: None,
                                                    segments: Punctuated::from_iter(new_segments),
                                                };
                                                let new_type_path = TypePath {
                                                    qself: None,
                                                    path: new_path,
                                                };
                                                let new_type = Type::Path(new_type_path);
                                                pat_type.ty = Box::new(new_type);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let syn::ReturnType::Type(_, _) = &func.sig.output {
                            for param in &mut func.sig.inputs {
                                if let FnArg::Typed(pat_type) = param {
                                    let ty = &*pat_type.ty;
                                    if let Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments,
                                        },
                                    }) = ty
                                    {
                                        for segment in segments {
                                            if let PathSegment {
                                                ident,
                                                arguments:
                                                    PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                                        colon2_token: None,
                                                        lt_token: _,
                                                        args,
                                                        gt_token: _,
                                                    }),
                                            } = segment
                                            {
                                                for arg in args {
                                                    if let GenericArgument::Lifetime(Lifetime {
                                                        apostrophe: _,
                                                        ident: ident_ref,
                                                    }) = arg
                                                    {
                                                        let new_ident = Ident::new("static", Span::call_site());
                                                        let new_lifetime = Lifetime {
                                                            apostrophe: Span::call_site(), 
                                                            ident: new_ident,
                                                        };
                                                        let new_arg = GenericArgument::Lifetime(new_lifetime);
                                                        let new_args = args.iter().map(|a| {
                                                            if let GenericArgument::Lifetime(lifetime) = a {
                                                                if *lifetime.ident == *ident_ref {
                                                                    new_arg.clone()
                                                                } else {
                                                                    a.clone()
                                                                }
                                                            } else {
                                                                a.clone()
                                                            }
                                                        }).collect::<Vec<_>>();
                                                        let new_angle_bracketed = AngleBracketedGenericArguments {
                                                            colon2_token: None,
                                                            lt_token: Default::default(),
                                                            args: Punctuated::from_iter(new_args),
                                                            gt_token: Default::default(),
                                                        };
                                                        let new_path_arguments = PathArguments::AngleBracketed(new_angle_bracketed);
                                                        let new_path_segment = PathSegment {
                                                            ident: ident.clone(),
                                                            arguments: new_path_arguments,
                                                        };
                                                        let new_segments = segments.iter().map(|s| {
                                                            if let PathSegment {
                                                                ident: ident_s,
                                                                arguments: _,
                                                            } = s {
                                                                if *ident_s == *ident {
                                                                    new_path_segment.clone()
                                                                } else {
                                                                    s.clone()
                                                                }
                                                            } else {
                                                                s.clone()
                                                            }
                                                        }).collect::<Vec<_>>();
                                                        let new_path = SynPath {
                                                            leading_colon: None,
                                                            segments: Punctuated::from_iter(new_segments),
                                                        };
                                                        let new_type_path = TypePath {
                                                            qself: None,
                                                            path: new_path,
                                                        };
                                                        let new_type = Type::Path(new_type_path);
                                                        pat_type.ty = Box::new(new_type);
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
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces lifetime parameters with the static lifetime in function and method signatures. This transformation tests the compiler's handling of const generic types with different lifetime parameters and may lead to ICEs or inference failures in the lifetime system."
    }
}