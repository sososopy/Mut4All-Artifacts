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

pub struct InherentAssocTypeInHigherRankedBinder_419;

impl Mutator for InherentAssocTypeInHigherRankedBinder_419 {
    fn name(&self) -> &str {
        "InherentAssocTypeInHigherRankedBinder_419"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if let Some(qself) = &type_path.qself {
                                let original_type = *pat_type.ty.clone();
                                let new_type = create_new_type(original_type);
                                *pat_type.ty = new_type;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

fn create_new_type(original_type: Type) -> Type {
    let type_path = match original_type {
        Type::Path(type_path) => type_path,
        _ => unreachable!(),
    };
    let qself = type_path.qself.as_ref().unwrap();
    let qself_ty = qself.ty.clone();

    let lifetime = Lifetime::new("'a", Span::call_site());

    // Modify the qself's generic arguments to be a reference with the new lifetime
    let mut new_qself_ty = qself_ty;
    let type_path = new_qself_ty.as_path().unwrap();
    let last_segment = type_path.path.segments.last_mut().unwrap();
    if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
        let original_generic_arg = args.args[0].clone();
        let original_type = if let GenericArgument::Type(t) = &original_generic_arg {
            t
        } else {
            unreachable!()
        };
        let new_generic_arg = Type::Reference(syn::TypeReference {
            and_token: token::And::default(),
            lifetime: Some(lifetime.clone()),
            mutability: None,
            elem: Box::new(original_type.clone()),
        });
        args.args.clear();
        args.args.push(GenericArgument::Type(new_generic_arg));
    }

    // Create the associated type: Struct<&'a T>::Assoc
    let assoc_type = TypePath {
        qself: Some(QSelf {
            lt_token: token::Lt::default(),
            ty: Box::new(new_qself_ty),
            position: 0,
            as_token: None,
            gt_token: token::Gt::default(),
        }),
        path: {
            let mut path = type_path.path.clone();
            path.segments.push(PathSegment {
                ident: Ident::new("Assoc", Span::call_site()),
                arguments: PathArguments::None,
            });
            path
        },
    };

    // Create the higher-ranked binder: for<'a>
    let bound_lifetimes = BoundLifetimes {
        for_token: token::For::default(),
        lt_token: token::Lt::default(),
        lifetimes: Punctuated::from_iter(vec![GenericParam::Lifetime(LifetimeParam {
            attrs: vec![],
            lifetime: lifetime.clone(),
            colon_token: None,
            bounds: Punctuated::new(),
        })]),
        gt_token: token::Gt::default(),
    };

    // Create the outer function type: for<'a> fn(assoc_type)
    Type::BareFn(Box::new(syn::TypeBareFn {
        lifetimes: Some(bound_lifetimes),
        unsafety: None,
        constness: None,
        asyncness: None,
        abi: None,
        sig: FnSig {
            paren_token: Default::default(),
            inputs: Punctuated::from_iter(vec![FnArg::Typed(PatType {
                attrs: vec![],
                pat: parse_quote!(_),
                colon_token: Default::default(),
                ty: Box::new(syn::Type::Path(assoc_type)),
            })]),
            output: ReturnType::Default,
            ..Default::default()
        },
    }))
}