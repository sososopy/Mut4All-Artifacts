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

pub struct Replace_Static_Initializer_With_Inferred_Placeholder_186;

impl Mutator for Replace_Static_Initializer_With_Inferred_Placeholder_186 {
    fn name(&self) -> &str {
        "Replace_Static_Initializer_With_Inferred_Placeholder_186"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                let ty = &item_static.ty;
                if is_projection_from_dyn_trait_object(ty) {
                    let placeholder_expr: Expr = parse_quote! { _ };
                    item_static.expr = Box::new(placeholder_expr);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets static items whose type is a projection from a trait object through an associated type (e.g., `<dyn Trait as Mirror>::Assoc`). It replaces the initializer expression with a placeholder underscore `_`, forcing the compiler to infer the value while preserving the complex type annotation. This transformation stresses the compiler's layout computation and inference capabilities for trait object-associated types, potentially triggering ICEs or type resolution failures."
    }
}

fn is_projection_from_dyn_trait_object(ty: &Type) -> bool {
    match ty {
        Type::Path(TypePath { qself: None, path }) => {
            if path.segments.len() >= 2 {
                let last_segment = &path.segments.last().unwrap();
                if let PathArguments::AngleBracketed(args) = &last_segment.arguments {
                    if args.args.len() == 1 {
                        let first_arg = &args.args.first().unwrap();
                        if let GenericArgument::Type(Type::Path(TypePath { qself: Some(qself), path: inner_path })) = first_arg {
                            if qself.as_token.is_some() && inner_path.segments.len() == 1 {
                                let dyn_segment = &inner_path.segments.first().unwrap();
                                if dyn_segment.ident.to_string().starts_with("dyn") {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
            false
        }
        _ => false,
    }
}